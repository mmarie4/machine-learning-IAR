/*=============================================================================
  Copyright (c) 2017-2018 Jilles Steeve Dibangoye
==============================================================================*/
#pragma once

#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <cmath>

#include "types.hpp"
#include "state.hpp"
#include "action.hpp"

//!
//! \file     World.hpp
//! \author   Jilles S. Dibangoye
//! \brief    World class
//! \version  1.0
//! \date     15 Octobre 2017
//!
//! This class provides the world's robot cleaner public interface.
//!

//! \namespace  cleaner
//!
//! Namespace grouping all tools required for the robot cleaner project.
namespace cleaner{
  class world{
  protected:
    double proba = 0.1;

    // width + height of the grid
    size width, height;

    // battery levels
    size cbattery;

    // set of all states
    std::vector<state*> states;

    // iterators
    bool *cbegin, *cend, *current;

    // generate the set of states
    void populate();

    // generate variations
    std::vector<bool> init(size);
    std::vector<bool> next(size);

    // comparator
    bool compare(std::vector<bool>const&, std::vector<bool>const&) const;
    bool compare(std::vector<bool>const&, std::vector<bool>const&, int) const;

    // check whether or not the model is sound
    bool sound(double) const;

  public:
      ~world();
      world(size, size, size);

      // returns the number of states
      int getNumStates() const;

      // returns the state associated with the index
      state* getState(int) const;

      // returns the initial state
      state* getStartState() const;

      // returns the reward for a pair of state and action
      double reward(state* const, action) const;

      // returns the set of all states
      std::vector<state*>const& getStates() const;

      // returns the probability of a triplet
      double probability(state* const, action, state* const)  const;

      // returns the tuple next state and reward given current state and action
      void execute(int, action, int&, double&);

      /*!
      * \fn std::ostream& operator<<(std::ostream&, const world&)
      * \brief print the world
      * \param std::ostream&
      * \param const world& the world to be printed
      * \return std::ostream&
      *
      * This method should produce an output of this form:
      */
      friend std::ostream& operator<<(std::ostream& os, const world& model){
        os << "\033[1m\033[32m ######################## <world> \033[0m" <<  std::endl;
        os << "\033[1m\033[32m ######################## \t<preamble> \033[0m" << std::endl;
        os << "\033[1m\033[32m ######################## \t\t<states>" << model.states.size() << "</states> \033[0m" << std::endl;
        os << "\033[1m\033[32m ######################## \t\t<sound> " << model.sound(0.001) << "</sound> \033[0m" << std::endl;
        os << "\033[1m\033[32m ######################## \t\t<actions> " << action::END - action::LEFT << "</actions> \033[0m" << std::endl;
        os << "\033[1m\033[32m ######################## \t</preamble> \033[0m" << std::endl;
        os << "\033[1m\033[32m ######################## \t<param> \033[0m" << std::endl;

        // int a; double prob;
        // os << "\033[1m\033[32m ######################## \t\t<reward> \033[0m" << std::endl;
        // for(a=0; a<action::END; ++a) for(auto s : model.states) {
        //   os << "\033[1m\033[32m ######################## \t\t\t<reward state=\"" << *s << "\"  action=\"" << a << "\"> \033[0m" << std::endl;
        //   os << "\033[1m\033[32m" << model.reward(s, static_cast<action>(a)) << "</reward> \033[0m" << std::endl;
        // } os << "\033[1m\033[32m ######################## \t\t</reward> \033[0m" << std::endl;
        //
        // os << "\033[1m\033[32m ######################## \t\t<dynamics> \033[0m" << std::endl;
        // for(a=0; a<action::END; ++a) for(auto s : model.states) for(auto ss : model.states) if( (prob=model.probability(s, static_cast<action>(a), ss)) > 0 ) {
        //   os << "\033[1m\033[32m ######################## \t\t\t<dynamics state=\""<< *s << "\" action=\""<< a << "\" next-state=\""<< *ss << "\"> \033[0m";
        //   os << "\033[1m\033[32m" << prob  << "</dynamics-entry> \033[0m" << std::endl;
        // } os << "\033[1m\033[32m ######################## \t\t</dynamics> \033[0m" << std::endl;
        //
        // os << "\033[1m\033[32m ######################## \t</param> \033[0m" << std::endl;
        // os << "\033[1m\033[32m ######################## </world> \033[0m" << std::endl;

        return os;
      }
  };
}
