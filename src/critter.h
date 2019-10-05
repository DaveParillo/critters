#ifndef MESA_CRITTERS_CRITTER_H
#define MESA_CRITTERS_CRITTER_H

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iosfwd>
#include <map>
#include <memory>
#include <string>

#include "color.h"
#include "direction.h"

/**
 * Base class for a specific instance of a game object.  
 * Could be a critter, food or an obstacle.
 *
 * Critters that are 'players' battle it out for dominance in the critters world.
 * Critters can move in any of the 8 cardinal directions each time step (tick).
 *
 * @todo non-player or inanimate objects should probably not be 'critters'
 */
class critter {
  private:
    std::string name_;            /**< Name of this critter */
    color color_;                 /**< Color of this critter */
    char glyph_;                  /**< Symbol displayed for a critter */
    bool updated_;                /**< Has this critter already been updated? */

    bool awake_;                  /**< Is this critter awake? */
    bool mating_;                 /**< Is this critter mating? */
    bool has_mated_;              /**< Has this critter mated once already? */
    int  baby_timer_;             /**< Controls how long new critters are in their newborn state */

    unsigned int food_remaining_; /**< Food reserves remaining before this critter sleeps */
    unsigned int wait_time_;      /**< Time remaining before this critter can take any action */

    static constexpr unsigned int 
      MAX_FOOD = 500;             /**< the maximum amount of food a critter can consume
                                       without being a glutton and having to sleep some of it off. */


  public:
    /** Default constructor doesn't make sense.
     * A critter must have a name.
     */
    critter() = delete;
    /**
     * Create a new critter.
     * The symbol for each critter is by default the first character of its name, 
     * but this can be overridden with the glyph() function. 
     * @param name the name of this critter.
     */
    explicit critter(const std::string& name) 
      : name_(name)
        , color_(color::WHITE)
        , glyph_('x')
        , updated_(false)
        , awake_(true)
        , mating_(false)
        , has_mated_(false)
        , baby_timer_(20)
        , wait_time_(0) 
    {
      glyph_ = std::toupper(name.at(0));
      food_remaining_ = MAX_FOOD / 2;
    }

    /**
     * Destroy critter resources.
     */
    virtual ~critter() = default;

    /**
     * Defines attack options for critters.
     * Exactly one of these is returned by the fight method.
     */
    enum class attack {
      ROAR,         /*!< Roar beats Scratch */
      POUNCE,       /*!< Pounce beats Roar */
      SCRATCH,      /*!< Scratch beats Pounce */
      FORFEIT       /*!< any other attack beats Forfeit */
    };

    /**
     * Array of attacks to make looping on attack types more convenient.
     */
    static const std::array<critter::attack, 4> attacks;

    /**
     * Return the name of this critter.
     * @return the name of this critter.
     */
    std::string name() const {
      return name_;
    }
    /**
     * Return the symbol of this critter.
     * This character is the symbol drawn for each critter in the game.
     * @return the symbol of this critter.
     */
    virtual char glyph() const { 
      if (is_baby()) return std::tolower(glyph_);
      return glyph_; 
    }

    /**
     * Informs the simulator that this critter is a 'player'.
     * Only players appear on the scoreboard, can accumulate points and win.
     * @return true if this critter is a player in the Simulator.
     * @return false if this critter is a non-player entity.
     */
    virtual bool is_player() const{ 
      return false; 
    }
    /**
     * Check if this critter has been updated already.
     * @return true if this critter has been updated.
     */
    bool updated() { 
      return updated_; 
    }
    /**
     * Change the update state of this critter.
     * @param value the new state
     */
    void set_update(bool value) {
      updated_ = value;
    }


    /**
     * Create a new Critter.
     * @return a shared pointer to a new critter object.
     */
    virtual std::shared_ptr<critter> create() = 0;

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
    virtual direction move(std::map<direction, std::shared_ptr<critter>> neighbors) {
      assert (neighbors.size() > 0);
      return direction::CENTER;
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
    virtual color color() const {
      return color_;
    }

    /**
     * Informs the Simulator of how to respond during a fight.
     * attack::FORFEIT always results in the death of this Critter.
     *
     * The Simulator will call fight exactly once each turn.
     *
     * @param opponent the name of the opponent.
     *        For default critters, the opponent names will *always* match the species name,
     *        which is the name of the critters class.
     * @return the attack this critter should make this turn
     */
    virtual attack fight(const std::string& opponent) {
      if (opponent.length() == 0) return attack::FORFEIT;
      return attack::FORFEIT;
    }


    /**
     * Informs the simulator that the critter want to eat the food it just moved onto.
     *
     * Since there is a cost associated with eating, a critter is not obligated to eat.
     *
     * The simulator will call eat exactly once each turn.
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
     * Used by the simulator to inform this critter it just won a fight.
     */
    virtual void won()    const {}
    /**
     * Used by the simulator to inform this critter it just lost a fight.
     */
    virtual void lost()   const {}
    /**
     * Used by the simulator to inform this critter the fight was a draw (no winner).
     */
    virtual void draw()   const {}
    /**
     * Used by the simulator to inform this critter it has been put to sleep.
     * A sleeping Critter can take no actions.
     * It can't move until it wakes up and immediately loses all fights.
     */
    virtual void sleep()  const {}

    //
    // Functions used by the simulator, not players.
    //

    /**
     * Returns the amount of time until the critter sleeps due to extreme hunger.
     * @return the food reserves remaining
     */
    unsigned int food_remaining() const { return food_remaining_; }
    /**
     * Returns the amount of time until the critter can take action,
     * either due to sleeping, mating, or waiting off some other penalty.
     * @return the wait time remaining
     */
    unsigned int wait_remaining() const { return wait_time_; }

    /**
     * Returns the awake state of this critter.
     * @return true if this critter is awake
     */
    bool is_awake() const { return awake_; }
    /**
     * Returns the sleep state of this critter.
     * @return true if this critter is not awake
     */
    bool is_asleep() const { return !awake_; }
    /**
     * Returns the mating state of this critter.
     * @return true if this critter is actively mating or recovering from mating.
     */
    bool is_mating() const { return mating_; }
    /**
     * Check if this critter has already mated.
     * A critter can only initiate mating once in its lifetime.
     * A critter can be the recipient of mating requests an unlimited number of times.
     * @return true if this critter is a parent.
     */
    bool is_parent() const { 
      return has_mated_; 
    }
    /**
     * Check if this critter is a newborn critter.
     * A newborn can't attack for 10 turns.
     * @return true if this critter is a baby.
     */
    bool is_baby() const { 
      return baby_timer_ > 0; 
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


};

/**
 * Insert formatted Critter state information into an output stream.
 * @param os reference to an output stream
 * @param it reference to a Critter to stream
 * @return the modified output stream
 */
std::ostream& operator<<(std::ostream &os, const critter& it);

/**
 * Insert attack enum class values into an output stream.
 * @param os reference to an output stream
 * @param rhs reference to a Attack to stream
 * @return the modified output stream
 */
std::ostream& operator<<(std::ostream& os, const critter::attack& rhs);

#endif
