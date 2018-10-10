#include "../include/qlearning.hpp"


namespace cleaner{
    qlearning::qlearning(world const& w, double epsilon, double learning_rate, double gamma, int episodes) : w(w){
      this->epsilon = epsilon;
      this->learning_rate = learning_rate;
      this->gamma = gamma;
      this->episodes = episodes;
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
    }

    double qlearning::getValueAt(int s){
      double value = MIN;
      for(int a=0; a<action::END; ++a){
        value = std::max(value, this->qf[s][a]);
      } return value;
    }

    int qlearning::greedy(int s){
      int agreedy;
      double value = MIN;
      double rd = rand() / ((double) RAND_MAX);

      if( rd > this->epsilon ) {
        for(int a=0; a<action::END; ++a){
          if( value < this->qf[s][a] ){
            agreedy = a;
            value = this->qf[s][a];
          }
        }
      }

      else {
        agreedy = rand() % 7;
      }

      return agreedy;
    }

    void qlearning::backup(int s, int a, int ss, double r){
      //TODO complete
    }

    void qlearning::init(){
      for(int s=0; s<this->w.getNumStates(); ++s){
        this->qf.emplace(s,  std::unordered_map<int, double>());
        for(int a=0; a<action::END; ++a){
          this->qf.at(s).emplace(a, 0.0);
        }
      }
    }
}
