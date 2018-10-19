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
        printf("getValueAt(0) = %f\n", getValueAt(0));
        a = greedy(s);
        w.execute(s, action(a), ss, r);

        maxQt1 = getValueAt(ss);
        d = r + maxQt1 - QF(w.getState(s), action(a));
        
        updateTheta(s, ss, d, a);

        s = ss;
        //displayTab(this->theta, this->SIZE, "Theta\0");
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


    void qlearning::updateTheta(int s, int ss, double d, int a) {
        for (int i = 0; i<this->SIZE; i++) {
          theta[i] = theta[i] + learning_rate * d * phiResult[i];
        }
    }

    double qlearning::QF(state* s, action a) {
      updatePhi(s, action(a)); // calculate phi(s, a) and put the result in phiResult
      double scal = 0;
      for(int i = 0; i<this->SIZE; i++) {
        scal += this->theta[i] * this->phiResult[i];
      }
    }

    // display tab for debug theta or phiResult
    void qlearning::displayTab(double *tab, int size, char* name) {
      printf("%s : ");
      for(int i = 0; i<size; i++) {
        printf(" %f ", tab[i]);
      }
      printf("\n");
    }
    
}
