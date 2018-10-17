#include "../include/qlearning.hpp"
#include <cmath>


namespace cleaner{
    qlearning::qlearning(world const& w, double epsilon, double learning_rate, double gamma, int episodes) : w(w){
      this->epsilon = epsilon;
      this->learning_rate = learning_rate;
      this->gamma = gamma;
      this->episodes = episodes;

      this->theta = new double[SIZE];
      this->phiResult = new double[SIZE];

    }

    qlearning::~qlearning(){}

    void qlearning::plots(){
      std::cout << this->getValueAt(0) << std::endl;
      points.push_back(std::make_pair(this->episode, this->getValueAt(0)));

      gp << "set xlabel 'Episodes'\n";
      gp << "set ylabel 'Mean Value'\n";
      gp << "plot '-' binary" << gp.binFmt1d(points, "record") << "with lines title 'Q-learning'\n";
      gp.sendBinary1d(points);
      gp.flush();
  }
    
    void qlearning::solve(){
      //TODO complete
      init();
      int ss = 0;
      int s = 0;
      int a;
      double r;

      for (int i = 0; i < episodes; i++){
        backup(s,a,ss,r);
        s = ss;
      }

      printf("----- test phi ----- \n");
      updatePhi(w.getState(0), action(4));
      printf("First three elements of phi result : %f, - %f - %f\n", phiResult[0], phiResult[1], phiResult[2]);
    }

    double qlearning::getValueAt(int s){
      double value = MIN;
      for(int a=0; a<action::END; ++a){
        value = std::max(value, QF(w.getState(s), action(a)));
      } return value;
    }

    int qlearning::greedy(int s){
      int agreedy;
      double value = MIN;
      double rd = rand() / ((double) RAND_MAX);

      if( rd > this->epsilon ) {
        for(int a=0; a<action::END; ++a){
          if( value < QF(w.getState(s), action(a))){
            agreedy = a;
            value = QF(w.getState(s), action(a));
          }
        }
      }

      else {
        agreedy = rand() % 7;
      }

      return agreedy;
    }
    
    void qlearning::backup(int s, int a, int ss, double r){
      double maxQt1;
      double d;
      for(int i = 0; i<100; i++) { // 100 est la taille d'un episode
        a = greedy(s);
        w.execute(s, action(a), ss, r);

        maxQt1 = getValueAt(ss);
        d = r + maxQt1 - QF(w.getState(s), action(a));
        
        updatePhi(w.getState(s), action(a));
        updateTheta(s, ss, d);

        s = ss;
      }
    }

    void qlearning::init(){
      for(int s=0; s<this->w.getNumStates(); ++s){
        this->qf.emplace(s,  std::unordered_map<int, double>());
        for(int a=0; a<action::END; ++a){
          this->qf.at(s).emplace(a, 0.0);
        }
      }
    }

    // Update phiResult depending on the current state and action
    void qlearning::updatePhi(state* s, action a) {
      // Empty phi
      for(int i = 0; i<this->SIZE; i++) {
        phiResult[i] = 0;
      }
      // check caracteristics
      if(s->getBattery() == 0 && a != CHARGE) {
        phiResult[0] = -5;
      }
      if(s->getBattery() < 100 && a == CHARGE && s->getBase()) {
        phiResult[1] = 5;
      }
      if(s->getGrid().at(s->getPose()) == false && a != CLEAN) {
        phiResult[2] = -5;
      }
      if(s->getGrid().at(s->getPose()) == true && a != LEFT && a != RIGHT && a != DOWN && a != UP) {
        phiResult[3] = -5;
      }
      if(a == WAIT) {
        phiResult[4] = -1;
      }
    }


    void qlearning::updateTheta(int s, int ss, double d) {
        // theta = theta + learning_rate * d + gradient(theta) * QF(s, a)
        for (int i = 0; i<this->SIZE; i++) {
          //
        }
    }

    double qlearning::QF(state* s, action a) {
      double scal = 0;
      for(int i = 0; i<this->SIZE; i++) {
        scal += this->theta[i] * this->phiResult[i];
      }
    }

    
}
