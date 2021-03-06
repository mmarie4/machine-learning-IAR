#include <time.h>

#include "../include/dp.hpp"
#include "../include/world.hpp"
#include "../include/qlearning.hpp"
#include "../include/montecarlo.hpp"

int main(int argc, char** argv){
  int wi, he, ba, dc;

  if(argc == 5 ) {
    wi = atoi(argv[1]);
    he = atoi(argv[2]);
    ba = atoi(argv[3]);
    dc = atoi(argv[4]);
  } else {
    wi = 10;
    he = 10;
    ba = 3;
    dc = 3;
  }

  srand (time(NULL));
  cleaner::world w(wi, he, ba, dc);
  std::cout << w << std::endl;
/*
  printf("\n------ Dynamic programming -------\n");
  cleaner::dp dp_solver(w, 0.001, 0.99);
  dp_solver.solve();
  std::cout << "dp_solver("<< *w.getState(0) << ") = " << dp_solver.getValueAt(0) << std::endl;
  */

 
  printf("\n------ Monte Carlo -------\n");
  cleaner::montecarlo mc_solver(w, 0.1, 0.99, 0.1, 3000);
  mc_solver.solve();
  std::cout << "mc_solver("<< *w.getState(0) << ") = " << mc_solver.getValueAt(0) << std::endl;
  

  /*
  printf("\n------ Q-Learning -------\n");
  cleaner::qlearning q_solver(w, 0.1, 0.1, 0.99, 1000); //(w, 0.1, 0.1, 0.99, 1000) : default
  q_solver.solve();
  std::cout << "q_solver("<< *w.getState(0) << ") = " << q_solver.getValueAt(0) << std::endl;
  */

  return 0;
}
