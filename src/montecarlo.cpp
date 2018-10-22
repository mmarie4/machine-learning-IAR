#include "../include/montecarlo.hpp"


namespace cleaner{
    montecarlo::montecarlo(world const& w, double epsilon, double gamma, double learning_rate, int episodes) : w(w), epsilon(epsilon), gamma(gamma), learning_rate(learning_rate), episodes(episodes){
    }

    montecarlo::~montecarlo(){
    }

    void montecarlo::plots(){
      //printf("getValueAt(0) = %f\n", getValueAt(0));
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
        value = std::max(value, QF(w.getState(s), action(a)));
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
      double d;

      for(s=0; s<this->w.getNumStates(); ++s){
        if(w.dirty_cells_2_entries[s] != 0) {
          printf("w.dirty_cells_2_entries[s] = %d pour s = %d and size of dirty_cells = %d\n", w.dirty_cells_2_entries[s], s, w.dirty_cells_2_entries.size());
        }
        for(a=0; a<action::END; ++a){
          if( this->pf[s][a] > -1 ){
            old = QF(w.getState(s), action(a));
            cumul = this->getReturn(this->pf[s][a]);
            this->jf[s][a].second ++;
            this->jf[s][a].first += cumul;

            if(action(a) != LEFT && action(a) != RIGHT && action(a) != UP && action(a) != DOWN) {
              //printf("============================================================================================> NOT MOVING\n");
            }
            
            // mettre a jour theta
            d = cumul - old;
            updateTheta(s, d, a);
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
      if(w.dirty_cells_2_entries[s->getPose()]>=0 && a != CLEAN) {
        phiResult[2] = -0.5;
      }
      if(w.dirty_cells_2_entries[s->getPose()]<0 && a != LEFT && a != RIGHT && a != DOWN && a != UP) {
        phiResult[3] = -0.5;
      }
      if(a == WAIT) {
        phiResult[4] = -0.3;
      }
    }


    void montecarlo::updateTheta(int s, double d, int a) {
        for (int i = 0; i<this->SIZE; i++) {
          theta[i] = theta[i] + this->learning_rate * d * phiResult[i];
        }
    }

    double montecarlo::QF(state* s, action a) {
      updatePhi(s, a); // calculate phi(s, a) and put the result in phiResult
      double scal = 0;
      for(int i = 0; i<this->SIZE; i++) {
        scal += this->theta[i] * this->phiResult[i];
      }
      return scal;
    }

    // display tab for debug theta or phiResult
    void montecarlo::displayTab(double *tab, int size, char name[]) {
      for(int i = 0; i<size; i++) {
        printf(" %f ", tab[i]);
      }
      printf("\n");
    }


}