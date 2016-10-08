#ifndef CRITTER_H
#define CRITTER_H

#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <string>

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include "Color.h"
#include "Direction.h"

/**
 * Base class for a specific instance of a game object.  
 * Could be a Critter, food or an obstacle.
 *
 * Critters that are 'players' battle it out for dominance in the Critters world.
 * Critters can move in any of the 8 cardinal directions each time step (tick).
 *
 * @todo non-player or inanimate objects should probably not be 'critters'
 */
class Critter {
  private:
    std::string _name;            /**< Name of this critter */
    Color _color;                 /**< Color of this critter */
    char _glyph;                  /**< Symbol displayed for a critter */
    bool _updated;                /**< Has this critter already been updated? */

    bool _awake;                  /**< Is this critter awake? */
    bool _mating;                 /**< Is this critter mating? */
    bool _has_mated;              /**< Has this critter mated once already? */
    int  _baby_timer;             /**< Controls how long new critters are in their newborn state */

    unsigned int _food_remaining; /**< Food reserves remaining before this critter sleeps */
    unsigned int _wait_time;      /**< Time remaining before this critter can take any action */

    static constexpr unsigned int 
      MAX_FOOD = 500;             /**< the maximum amount of food a critter can consume
                                       without being a glutton and having to sleep some of it off. */

    Critter() {};                 /**< No arg constructor doesn't make sense. Must have a name. */


  public:
    /**
     * Create a new Critter.
     * The symbol for each critter is by default the first character of its name, 
     * but this can be overridden with the glyph() function. 
     * @param name the name of this Critter.
     */
    Critter(const std::string name) : 
      _name(name), _color(Color::WHITE), 
      _awake(true), _mating(false), _has_mated(false), _baby_timer(20),
      _wait_time(0) {

        _glyph = std::toupper(name.at(0));
        _food_remaining = MAX_FOOD / 2;
      }

    /**
     * Defines attack options for Critters.
     * Exactly one of these is returned by the fight method.
     */
    enum class Attack {
      ROAR,         /*!< Roar beats Scratch */
      POUNCE,       /*!< Pounce beats Roar */
      SCRATCH,      /*!< Scratch beats Pounce */
      FORFEIT       /*!< any other attack beats Forfeit */
    };

    /**
     * Return the name of this Critter.
     * @return the name of this Critter.
     */
    std::string name() const {
      return _name;
    }
    /**
     * Return the symbol of this Critter.
     * This character is the symbol drawn for each Critter in the game.
     * @return the symbol of this Critter.
     */
    virtual char glyph() const { 
      if (is_baby()) return std::tolower(_glyph);
      return _glyph; 
    }

    /**
     * Informs the Simulator that this Critter is a 'player'.
     * Only players appear on the scoreboard, can accumulate points and win.
     * @return true if this Critter is a player in the Simulator.
     * @return false if this Critter is a non-player entity.
     */
    virtual bool is_player() const{ 
      return false; 
    }
    /**
     * Check if this critter has been updated already.
     * @return true if this critter has been updated.
     */
    bool updated() { 
      return _updated; 
    }
    /**
     * Change the update state of this critter.
     * @param value the new state
     */
    void set_update(bool value) {
      _updated = value;
    }


    /**
     * Create a new Critter.
     * @return a shared pointer to a new critter object.
     */
    virtual std::shared_ptr<Critter> create() = 0;

    /**
     * Informs the Simulator of any movement a Critter wants to take during a turn.
     * Direction::CENTER indicates no movement occurs.
     *
     * The Simulator will call move exactly once each turn.
     *
     * Override both move and fight, unless your critter is named 'Lunch'.
     *
     * @param neighbors A map of what is in the neighoring tiles around this critter.
     * @return the Direction this Critter should move this turn
     */
    virtual Direction move(std::map<Direction, std::shared_ptr<Critter>> neighbors) {
      assert (neighbors.size() > 0);
      return Direction::CENTER;
    }
    /**
     * Informs the Simulator of the Critter color.
     *
     * The Sim may occasionally override this color to indicate 
     * information about the Critter state, but will preserve this color as
     * the 'primary color' associated with this Critter.
     *
     * @return the Color of this Critter
     */
    virtual Color color() const {
      return _color;
    }

    /**
     * Informs the Simulator of how to respond during a fight.
     * Attack::FORFEIT always results in the death of this Critter.
     *
     * The Simulator will call fight exactly once each turn.
     *
     * @param opponent the name of the opponent.
     *        For default critters, the opponent names will *always* match the species name,
     *        which is the name of the critters class.
     * @return the Attack this Critter should make this turn
     */
    virtual Attack fight(const std::string opponent) {
      if (opponent.length() == 0) return Attack::FORFEIT;
      return Attack::FORFEIT;
    }


    /**
     * Informs the Simulator that the critter want to eat the food it just moved onto.
     *
     * Since there is a cost associated with eating, a critter is not obligated to eat.
     *
     * The Simulator will call eat exactly once each turn.
     *
     * Eating gains the ability to make more critters to fight.
     * @return true to inform the sim that this critter should eat the food
     * it just moved onto.
     * @return false to leave the food alone.
     */
    virtual bool eat() {
      return false;
    }

    // The following methods are called by the simulator to inform you of
    // specific events that happen. You may override these methods in your
    // Torero class if you want to be informed of these events but it is
    // not required. 
    //
    /**
     * Used by the simulator to inform this Critter it just won a fight.
     */
    virtual void won()    const {}
    /**
     * Used by the simulator to inform this Critter it just lost a fight.
     */
    virtual void lost()   const {}
    /**
     * Used by the simulator to inform this Critter the fight was a draw (no winner).
     */
    virtual void draw()   const {}
    /**
     * Used by the simulator to inform this Critter is has been put to sleep.
     * A sleeping Critter can take no actions.
     * It can't move until it wakes up and immediately loses all fights.
     */
    virtual void sleep()  const {}

    /* Accessor methods */
    /**
     * Returns the amount of time until the Critter sleeps due to extreme hunger.
     * @return the food reserves remaining
     */
    unsigned int food_remaining() const { return _food_remaining; }
    /**
     * Returns the amount of time until the Critter can take action,
     * either due to sleeping, mating, or waiting off some other penalty.
     * @return the wait time remaining
     */
    unsigned int wait_remaining() const { return _wait_time; }

    /**
     * Returns the awake state of this Critter.
     * @return true if this Critter is awake
     */
    bool is_awake() const { return _awake; }
    /**
     * Returns the sleep state of this Critter.
     * @return true if this Critter is not awake
     */
    bool is_asleep() const { return !_awake; }
    /**
     * Returns the mating state of this Critter.
     * @return true if this Critter is actively mating or recovering from mating.
     */
    bool is_mating() const { return _mating; }
    /**
     * Check if this critter has already mated.
     * A critter can only initiate mating once in its lifetime.
     * A critter can be the recipient of mating requests an unlimited number of times.
     * @return true if this critter is a parent.
     */
    bool is_parent() const { 
      return _has_mated; 
    }
    /**
     * Check if this critter is a newborn critter.
     * A newborn can't attack for 10 turns.
     * @return true if this critter is a baby.
     */
    bool is_baby() const { 
      return _baby_timer > 0; 
    }

    /**
     * Called by the Simulator sta start the process of two Critters of the same
     * species to mate.
     * @param rest the number of turns the critter will rest after mating.
     */
    void start_mating(unsigned int rest);

    /**
     * Called by the Simulator when the return value from Critter::eat == true.
     * If the amount of food remaining for a critter exceeds MAX_FOOD, the critter
     * is _stuffed_ and needs to sleep it off until the value drops below MAX_FOOD.
     */
    void eat_food();

    /**
     * Force this Critter to sleep the indicated number of turns.
     * @param num_turns the number of turns the critter will sleep.
     */
    void sleep(unsigned int num_turns);

    /**
     * Update Critter state variables that need modifying every time step:
     *  - wait time
     *  - checking to see if the awake or mating states can change
     */
    void tick();

    /**
     * Insert formatted Critter state information into an output stream.
     * @todo this should probably not be a friend - it only accesses public methods
     * @param os reference to an output stream
     * @param it reference to a Critter to print
     * @return the modified output stream
     */
    friend std::ostream& operator<<(std::ostream &os, const Critter& it) {
      os << "Name: " << it.name() << std::endl;
      os << "\tColor: " << ColorNames[it.color()] << std::endl;
      os << "\tIs awake?: " << it.is_awake() << std::endl;
      os << "\tIs mating?: " << it.is_mating() << std::endl;
      os << "\tIs player?: " << it.is_player() << std::endl;
      return os;
    }

};


#endif
