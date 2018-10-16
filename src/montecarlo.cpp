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
      // set the first element of the episode
      std::tuple<int, int, int> firstTuple = std::make_tuple(0, greedy(0), w.reward(w.getStartState(), greedy(0)));
      episode.push_back(firstTuple);
      
      int newState;
      double newReward;

      // Fill the vector with tuples
      int EPISODE_SIZE = 100;
      printf("w.getNumState() = %d\n", w.getNumStates());
      for(int i = 0; i<EPISODE_SIZE; i++) {
        printf("before execute (i=%d)\n", i);
        w.execute(i, greedy(i), newState, newReward);
        printf("before creating tuple (i=%d)\n", i);
        std::tuple<int, int, int> newTuple = std::make_tuple(newState, greedy(newState), newReward);
        printf("tuple created (i=%d)\n", i);
        episode.push_back(newTuple);
        printf("Tuple added to episode (i=%d)\n", i);
      }

    }      

void montecarlo::backup(){
      for(int i = 0 ; i < episode.size(); i++){
        std::tuple<int, int, int> curs = episode.at(i);
        int a = std::get<1>(curs);
        int s = std::get<0>(curs);
        if(pf[s][a] == -1){
          pf[s][a] = 0;                    //first occurence found
          jf[s][a].first += getReturn(i);   //Sum the rewards from the index of first occurence
          jf[s][a].second += 1;             //Count the rewards from the first occurence for the mean
          qf[s][a] = jf[s][a].first / jf[s][a].second;  //Do the mean and update Q value
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

}
