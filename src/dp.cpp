#include "../include/dp.hpp"


namespace cleaner{
    dp::dp(world const& w, double epsilon, double gamma) : w(w), epsilon(epsilon), gamma(gamma){}

    void dp::solve(){
      //TODO complete
    }

    double dp::getValueAt(int s) const{
      return this->vf.at(s);
    }

    void dp::backup(){
      //TODO complete
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
