#include "../include/montecarlo.hpp"


namespace cleaner{
    montecarlo::montecarlo(world const& w, double epsilon, double gamma, int episodes) : w(w) {
      this->epsilon = epsilon;
      this->gamma = gamma;
      this->episodes = episodes;
    }

    montecarlo::~montecarlo(){}

    void montecarlo::plots(){
      std::cout << this->getValueAt(0) << std::endl;
      points.push_back(std::make_pair(this->cepisode, this->getValueAt(0)));

      gp << "set xlabel 'Episodes'\n";
      gp << "set ylabel 'Mean Value'\n";
      gp << "plot '-' binary" << gp.binFmt1d(points, "record") << "with lines title 'Monte Carlo'\n";
      gp.sendBinary1d(points);
      gp.flush();
  }

    void montecarlo::solve(){
      this->init();

      do{
        this->setEpisode();
        this->backup();
        this->plots();
      }while( ++this->cepisode < this->episodes );
    }

    double montecarlo::getValueAt(int s){
      double value = MIN;
      for(int a=0; a<action::END; ++a){
        value = std::max(value, this->qf[s][a]);
      } return value;
    }

    //<! prescribes the greedy action
    action montecarlo::greedy(int s){
      action agreedy;
      double value = MIN;
      for(int a=0; a<action::END; ++a){
        if( value < this->qf[s][a] ){
          agreedy = static_cast<action>(a);
          value = this->qf[s][a];
        }
      } return agreedy;
    }

    double montecarlo::getReturn(int pose){
      double r = 0;
      for(int i=pose; i<100; i++){
        r += pow( this->gamma, i-pose ) * std::get<2>(this->episode[i]);
      }

      return r;
    }


    void montecarlo::setEpisode(){
      ep[0].st = w.getState(cursor);
      ep[0].act = policy;
      ep[0].reward = w.reward(w.getState(cursor), policy);
      ep[1].st = w.getState(cursor+1);
      ep[1].act = policy;
      ep[1].reward = w.reward(w.getState(cursor+1), policy);
      ep[2].st = w.getState(cursor+2);
      ep[2].act = policy;
      ep[2].reward = w.reward(w.getState(cursor+2), policy);
      cursor+=3;
    }

    void montecarlo::backup(){
      int G1 = 0;
      int G2 = 0;
      int G3 = 0;
      // fin the one, two or three different states in the episode
      state* state1 = ep[0].st;
      state* state2;
      state* state3;
      if (ep[0].st != ep[1].st) {
        state2 = ep[1].st;
        if (ep[0].st != ep[2].st) {
          state3 = ep[2].st;
        } else {
          state3 = NULL;
        }
      } else if (ep[0].st != ep[2].st) {
        state2 = ep[2].st;
        state3 = NULL;
      }
      // calcul de G
      for(int i = 0; i<sizeof(ep); i++) {
        // calcul des différents G
        if(ep[i].st == state1) {
            G1+= ep[i].reward;
        } else if(ep[i].st == state2) {
            G2 += ep[i].reward;
        }  else if(ep[i].st == state3) {
            G3 += ep[i].reward;
        }
       // append G to returns
       // change policy
       ep[i].act = greedy(ep[i].st);
      }
    }

    void montecarlo::init(){
      for(int s=0; s<this->w.getNumStates(); ++s){
        this->pf.emplace(s,  std::unordered_map<int, int>());
        this->qf.emplace(s,  std::unordered_map<int, double>());
        this->jf.emplace(s,  std::unordered_map<int, std::pair<double, int>>());
        for(int a=0; a<action::END; ++a){
          this->pf.at(s).emplace(a, -1);
          this->qf.at(s).emplace(a, 0.0);
          this->jf.at(s).emplace(a, std::pair<double, int>(0.0, 0));
        }
      }
    }

}
