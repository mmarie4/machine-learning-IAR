/*=============================================================================
  Copyright (c) 2017-2018 Jilles Steeve Dibangoye
==============================================================================*/
#pragma once

#include "types.hpp"

//!
//! \file     action.hpp
//! \author   Jilles S. Dibangoye
//! \brief    state class
//! \version  1.0
//! \date     15 Octobre 2017
//!
//! This class provides the action's robot cleaner public interface.
//!

//! \namespace  cleaner
//!
//! Namespace grouping all tools required for the robot cleaner project.
namespace cleaner{
  enum action {LEFT=0, RIGHT, UP, DOWN, CHARGE, WAIT, CLEAN, END};
}
