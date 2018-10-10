/*=============================================================================
  Copyright (c) 2017-2018 Jilles Steeve Dibangoye
==============================================================================*/
#pragma once

#include <cmath>
#include<limits>
#include <unordered_map>

#include "world.hpp"

//!
//! \file     dp.hpp
//! \author   Jilles S. Dibangoye
//! \brief    dp class
//! \version  1.0
//! \date     15 Octobre 2017
//!
//! This class provides the dp's robot cleaner public interface.
//!

//! \namespace  cleaner
//!
//! Namespace grouping all tools required for the robot cleaner project.
namespace cleaner{
  class dp{
  protected:
    world w;
    double epsilon, gamma;
    double MIN = -100000, MAX = 100000;
    std::unordered_map<int, double> vf, vf_copy;

    double distance();
    void backup();
    void copy();
    void init();

  public:
    dp(world const&, double, double);
    void solve();
    double getValueAt(int) const;
  };
}
