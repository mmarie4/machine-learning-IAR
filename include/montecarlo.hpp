#pragma once

#include <math.h>
#include <tuple>
#include <vector>
#include <unordered_map>

#include "world.hpp"
#include "../include/gnuplot-iostream.h"

//!
//! \file     montecarlo.hpp
//! \author   Jilles S. Dibangoye
//! \brief    montecarlo class
//! \version  1.0
//! \date     15 Octobre 2017
//!
//! This class provides the montecarlo's robot cleaner public interface.
//!

//! \namespace  cleaner
//!
//! Namespace grouping all tools required for the robot cleaner project.
namespace cleaner{
  class montecarlo{
  protected:
    world w;
    Gnuplot gp;
    std::vector<std::pair<double, double>> points;

    double gamma, epsilon;
    int cepisode = 0, episodes;
    
    double MIN = -100000, MAX = 100000;
    std::vector<std::tuple<int, int, int>> episode;

    // backup the position of a pair of state and action in the current episode
    std::unordered_map<int, std::unordered_map<int, int>> pf;

    // q-value function
    std::unordered_map<int, std::unordered_map<int, double>> qf;

    // track the mean of any pair of state and action
    std::unordered_map<int, std::unordered_map<int, std::pair<double, int>>> jf;

    double getReturn(int i);
    void setEpisode();
    void backup();
    void plots();
    void init();

  public:
    ~montecarlo();
    montecarlo(world const&, double, double, int);
    void solve();
    action greedy(int);
    double getValueAt(int);
  };

}
