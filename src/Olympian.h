/*
 * Olympian.h
 *
 *  Created on: Apr 22, 2014
 *      Author: Tasha
 * To rename your Olympian, follow these steps.  We'll assume
 * you're changing the name from Olympian to MyCritter.
 * Anywhere you see MyCritter in the instructions, use the name
 * of your critter.
 *
 * 1. Change the name of this file to MyCritter.h.  Close the file
 *    then right-click on it in the project tree and choose rename.
 *
 * 2. In this file, replace all occurrences of 
 *    OLYMPIAN_H with MY_CRITTER_H
 *
 * 3. In this file, replace all occurrences of Olympian with MyCritter.
 *
 * 4. In main.cpp, change line 6 to say #include "MyCritter.h"
 *
 * 5. In main.cpp, in lines 104, replace Olympian with MyCritter.
 */

#ifndef OLYMPIAN_H
#define OLYMPIAN_H
#include <iostream>
#include <memory>

#include "Direction.h"
#include "Critter.h"

/**
 * A stub for a future player.
 * In it's current state, this critter should be named 'Lunch'.
 */
class Olympian : public Critter {

  public:
    /**
     * Create a new critter named "Olympian"
     */
    Olympian() : Critter("Olympian") { }

    /**
     * Inform the sim this critter is a competitor.
     * @return true always.
     */
    bool is_player() const override { return true; }


    /**
     * Inform the sim of the color of this critter.
     * @return the color of this critter.
     */
    Color  color()   const override { return Color::RED; }



    /**
     * Make a new Olympian.
     * @return a shared pointer to a new Olympian object.
     */
    std::shared_ptr<Critter> create() override {
      return std::make_shared<Olympian>();
    }
};



#endif
