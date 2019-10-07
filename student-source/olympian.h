#ifndef MESA_CRITTERS_STUDENT_SOURCE_OLYMPIAN_H
#define MESA_CRITTERS_STUDENT_SOURCE_OLYMPIAN_H
/*
 * olympian.h
 *
 * To make your own custom competitor, follow these steps.
 *
 * 1. Copy this file to a name based on the name of your critter
 *
 *    Consider both a .h and .cpp for your critter
 *
 *    For any files added or removed, ensure they are all listed
 *    in CMakeLists.txt.
 *
 * 2. In this file, replace all occurrences of olympian with 
 *    Feel free to be creative, 
 *    but recall class names and identifiers cannot contain spaces.
 *
 * 3. In add_players.cpp, add an #include for your critter and 
 *    add it to the players vector.
 *
 */

#include <memory>

#include <direction.h>
#include <critter.h>

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
     *
     * If you return false, no creature can attack you,
     * but you don't get a score either.
     *
     * @return true always.
     */
    bool is_player() const override { return true; }

    /**
     * Inform the sim of the color of this critter.
     * @return the color of this critter.
     * @see the Color enum for a list of available colors.
     */
    enum color  color()   const override { return color::MAGENTA; }

    /**
     * Make a new Olympian.
     * @return a shared pointer to a new Olympian.
     */
    std::shared_ptr<critter> create() override {
      return std::make_shared<olympian>();
    }
};

#endif

