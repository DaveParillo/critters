/*
 * olympian.h
 *
 * To make your own custom olympian, follow these steps.
 *
 * 1. Copy this file to a name based on the name of your critter
 *
 * 2. In this file, replace all occurrences of olympian with
 *    Feel free to be creative, 
 *    but recall class names and identifiers cannot contain spaces.
 *
 * 3. In add_players.cpp, add an #include for your critter and 
 *    add it to you the players vector.
 *
 */

#ifndef MESA_CRITTERS_OLYMPIAN_H
#define MESA_CRITTERS_OLYMPIAN_H

#include <iostream>
#include <memory>

#include "critter.h"
#include "direction.h"


/**
 * A stub for a future player.
 * In it's current state, this critter should be named 'Lunch'.
 */
class olympian : public critter {

  public:
    /**
     * Create a new critter named "Olympian"
     */
    olympian() : critter("Olympian") { }

    /**
     * Inform the sim this critter is a competitor.
     * @return true always.
     */
    bool is_player() const override { return true; }


    /**
     * Inform the sim of the color of this critter.
     * @return the color of this critter.
     */
    enum color  color()   const override { return color::RED; }



    /**
     * Make a new Olympian.
     * @return a shared pointer to a new Olympian object.
     */
    std::shared_ptr<critter> create() override {
      return std::make_shared<olympian>();
    }
};

#endif
