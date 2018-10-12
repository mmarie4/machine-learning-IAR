#include "../include/dp.hpp"


namespace cleaner{
    dp::dp(world const& w, double epsilon, double gamma) : w(w), epsilon(epsilon), gamma(gamma){}

    void dp::solve(){
      // processus itératif qui va itérativement appeler backup pour atteindre la bonne valeur de Vf
      // il faut aussi actualiser vf_copy
      // condition d'arret : distance entre vf et l'etape d'après < epsilon
      double error = 1;
      init();
      while(error > epsilon) {
        copy();
        backup();
        error = distance();
      }
      // on donne le vf final au robot
    }

    double dp::getValueAt(int s) const{
      return this->vf.at(s);
    }


    // Backup : calcul d'une itération
    // Il faut definir une policy à la con au début (quelles actions associées à un état)
    // Calculer Vt
    // Puis il faut améliorer la policy (voir les slides)
    // --> on peut faire les deux dans une seule boucle
    void dp::backup(){
      for(int s=0; s<this->w.getNumStates(); ++s){
        this->vf[s] = MIN;
        for(int a=0; a<action::END; a++){
          double value = w.reward(w.getState(s), static_cast<action>(a));
          for(int ss=0; ss<this->w.getNumStates(); ++ss)
            value += this->gamma * w.probability(w.getState(s), static_cast<action>(a), w.getState(ss)) * this->vf_copy[ss];
          this->vf[s] = std::max(this->vf[s], value);
        }
      }
    }

    void dp::init(){
      for(int s=0; s<this->w.getNumStates(); ++s){
        this->vf.emplace(s, 0.0);
      }
    }

    void dp::copy(){
      for(int s=0; s<this->w.getNumStates(); ++s){
        this->vf_copy[s] = this->vf[s];
      }
    }

    double dp::distance(){
      double error = 0;
      for(int s=0; s<this->w.getNumStates(); ++s){
        error = std::max( error, std::abs(this->vf[s] - this->vf_copy[s]) );
      }

      //TODO
      std::cout << error << std::endl;
      return error;
    }
}
