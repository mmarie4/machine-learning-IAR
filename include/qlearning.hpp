#pragma once

#include <math.h>
#include <tuple>
#include <vector>
#include <unordered_map>

#include "world.hpp"
#include "../include/gnuplot-iostream.h"

//!
//! \file     qlearning.hpp
//! \author   Jilles S. Dibangoye
//! \brief    qlearning class
//! \version  1.0
//! \date     15 Octobre 2017
//!
//! This class provides the qlearning's robot cleaner public interface.
//!

//! \namespace  cleaner
//!
//! Namespace grouping all tools required for the robot cleaner project.
namespace cleaner{
  class qlearning{
  protected:
    world w;
    Gnuplot gp;
    int episode = 0, episodes;
    double MIN = -100000, MAX = 100000;
    double gamma, epsilon, learning_rate;
    std::vector<std::pair<double, double>> points;
    std::unordered_map<int, std::unordered_map<int, double>> qf;

    void backup(int /*current state*/, int /*action*/, int /*next state*/, double /*reward*/);
    void plots();
    void init();

    // caracteristics for large scale algo
    int SIZE = 5;
    double* theta;
    double* phiResult;
    void updatePhi(state* s, action a);
    void updateTheta(int s, int ss, double d, int a);
    void displayTab(double *theta, int size, char* name);
    double QF(state* s, action a);
    double delta;


  public:
    ~qlearning();
    qlearning(world const&, double, double, double, int);
    void solve();
    int greedy(int);
    double getValueAt(int);
  };
}
