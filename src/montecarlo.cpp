#include "../include/montecarlo.hpp"


namespace cleaner{
    montecarlo::montecarlo(world const& w, double epsilon, double gamma, int episodes) : w(w), epsilon(epsilon), gamma(gamma), episodes(episodes){
    }

    montecarlo::~montecarlo(){
    }

    void montecarlo::plots(){
      std::cout << this->getValueAt(0) << std::endl;
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
      action a;
      double r;
      this->episode.clear();
      int s, ss;

      for(s=0; s<this->w.getNumStates(); ++s){
        for(int a=0; a<action::END; ++a){
          this->pf[s][a] = -1;
        }
      }

      s = 0;
      double rd = rand() / ((double) RAND_MAX);
      for(int i=0; i<100; i++){
        if( rd > this->epsilon ) {
          a = greedy(s);
        }else {
          a = static_cast<action>(rand() % 7);
        }

        w.execute(s, a, ss, r);

        this->episode.push_back(std::make_tuple(s, a, r));

        if(this->pf[s][a] == -1){
          this->pf[s][a] = i;
        }

        s = ss;
      }
    }

    void montecarlo::backup(){
      int s, a;
      double old, cumul;

      for(s=0; s<this->w.getNumStates(); ++s){
        for(a=0; a<action::END; ++a){
          if( this->pf[s][a] > -1 ){
            old = QF(w.getState(s), action(a));
            cumul = this->getReturn(this->pf[s][a]);
            this->jf[s][a].second ++;
            this->jf[s][a].first += cumul;
            this->qf[s][a] = this->jf[s][a].first / this->jf[s][a].second;
          }
        }
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

        // Update phiResult depending on the current state and action
    void montecarlo::updatePhi(state* s, action a) {
      // Empty phi
      for(int i = 0; i<this->SIZE; i++) {
        phiResult[i] = 0;
      }

      // check caracteristics
      if(s->getBattery() == 0 && a != CHARGE && s->getBase()) {
        phiResult[0] = -1;
      }
      if(s->getBattery() < 2 && a == CHARGE && s->getBase()) {
        phiResult[1] = 0.5;
      }
      if(s->getGrid().at(s->getPose()) == false && a != CLEAN) {
        phiResult[2] = -0.5;
      }
      if(s->getGrid().at(s->getPose()) == true && a != LEFT && a != RIGHT && a != DOWN && a != UP) {
        phiResult[3] = -0.5;
      }
      if(a == WAIT) {
        phiResult[4] = -0.3;
      }
    }


    void montecarlo::updateTheta(int s, int ss, double d, int a) {
        for (int i = 0; i<this->SIZE; i++) {
          theta[i] = theta[i] + learning_rate * d * phiResult[i];
        }
    }

    double montecarlo::QF(state* s, action a) {
      updatePhi(s, action(a)); // calculate phi(s, a) and put the result in phiResult
      double scal = 0;
      for(int i = 0; i<this->SIZE; i++) {
        scal += this->theta[i] * this->phiResult[i];
      }
    }

    // display tab for debug theta or phiResult
    void montecarlo::displayTab(double *tab, int size, char* name) {
      printf("%s : ");
      for(int i = 0; i<size; i++) {
        printf(" %f ", tab[i]);
      }
      printf("\n");
    }

}