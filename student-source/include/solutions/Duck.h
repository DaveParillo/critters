#pragma once
#include <Critter.h>
#include <Color.h>
#include <Direction.h>

#include <map>
#include <memory>
#include <vector>
#include <utility>

using std::size_t;
using Attack = Critter::Attack;

/*
 * An extremely dangerous Duck.
 *
 * The Duck is the only default competitor that attempts to use optional parts of the Critter interface.
 *
 * It uses won() and lost() to keep track of its own performance, so that it can
 * attempt to pick better attacks in the future.
 *
 * For each move, it priortizes the possible moves it might make and picks the highest 'priority' move.
 * Generally, an attack is top priority.
 */
class Duck: public Critter
{
  public:
    Duck() :
      Critter("Danger Duck"),
      distance_{random_distance()},
      direction_{random_direction()}
    {}
    ~Duck() = default;

    /**
     * Inform the simulator that this Duck is a player 
     * and can take actions against other players.
     * @return always returns true
     */
    bool is_player() const override { return true; }

    /**
     * @return the color of this Duck.
     */
    Color color() const override { return Color::RED; }

    /**
     * Choose a move direction.
     * @param neighbors the contents of the tile surrounding this Duck
     * @return the move direction. 
     *
     * Ducks always evaluate their surroundings and move
     * based on the current situation.
     *
     * A Duck will always attack the weakest opponent in it's
     * immediate area, if it has more than one to choose from,
     * but won't back down from a random encounter either.
     *
     * If they don't have anything else to so, they will prefer
     * a random 'Duck' Diagonal path.
     */
    Direction move(std::map<Direction, std::shared_ptr<Critter>> neighbors) override;

    /**
     * Fight like no duck you ever saw.
     *
     * @param opponent the critter unforunate enough to face this Duck.
     * @return the attack chosen by this Duck.
     *
     * Ducks use the Critter::won and Critter::Lost functions to keep a record
     * of what attacks work against which opponents.
     * The attack chosen is based on past performance against opponents of this species.
     *
     * All Ducks know the results of all fights made by all Ducks.
     * Yes, these are psychic ducks.
     */
    Attack fight(const std::string opponent) override;

    /**
     * Decide whether to eat or not.
     * @return true only if this Duck::is_hungry
     */
    bool eat() override {
      return is_hungry();
    }

    /**
     * Gather stats on a fight this Duck just won
     */
    void won() const override;

    /**
     * Gather stats on a fight this Duck just lost
     */
    void lost() const override;

    /**
     * Allow the simulator to make a new Duck.
     */
    std::shared_ptr<Critter> create() override {
      return std::make_shared<Duck>();
    }

    /**
     * Store fight results.
     * Used to predict most successful attacks 
     * against previously encountered critters.
     *
     * Works best against critters with non-random attacks.
     *
     * Works reliably enough against critters that depend on rand()
     * to almost always win.
     */
    struct FightResults
    {
      // 0: ROAR,    1: POUNCE
      // 2: SCRATCH, 3: FORFEIT
      std::array<size_t, 4> data;

      size_t wins = 0, losses = 0;

      FightResults():
        data{{ 1, 1, 1, 0}}
      {}

      FightResults(size_t r, size_t p, size_t s, size_t f):
        data{{r, p, s, f}}
      {}

      size_t operator[](const size_t i) const { 
        return data[i];
      }

      /** 
       * @return Attack current best guess at what attack to use.
       */
      Attack best_attack() const;

      /** 
       * Record our glorious victory.
       * Increment the data array with the attack used.
       */
      void won(const Attack& attack);

      /** 
       * Record our shameful defeat.
       * Increment the data array with the attack used.
       */
      void lost(const Attack& attack);
    };

  private:
    size_t           distance_;      /**< Random distance to move */
    Direction        direction_;     /**< Random direction to move */
    std::string      opponent_;      /**< current opponent */
    Attack           attack_used_;   /**< attack used against the current opponent */

    static std::map<std::string, FightResults>  
                     fight_results_; /**< tally of wins and losses for each critter */

    /**
     * Used to assign a weight to each neighboring cell.
     * The direction with the highest weight wins.
     */
    struct Priority {
      Direction direction;
      size_t weight;

      Priority() = default;
      Priority(Direction dir, size_t wt)
        : direction{dir}, weight{wt}
      {}
    };
    
    /**
     * Evaluate a single neighbor (Direction, Critter pair), assign it
     * a weight and update weighted_dirs.
     */
    void evaluate_neighbor(const std::pair<const Direction, std::shared_ptr<Critter>>& n, 
                           std::vector<Priority>* weighted_dirs);

    /**
     * Returns true if this Duck is moderately hungry.
     */
    bool is_hungry() const;


    /**
     * Returns true if this Duck is desperately hungry.
     */
    bool is_starving() const;

    /** 
     * Pick a random direction, but prioritize diagonals:
     * NORTH_EAST: 1, SOUTH_EAST: 3,
     * SOUTH_WEST: 5, NORTH_WEST: 7
     */
    Direction random_direction();

    size_t random_distance();

    /** Get the best attack possible against the provided opponent.
     * @param data results from previous fights
     * @param name opponent name
     * @return Suggested attack 
     *
     * If we've encountered this opponent before, use the FightResults map
     * otherwise return a random Attack.
     *
     * @todo
     *  This doesn't work well against opponents that make truly random attacks
     *  Should be able to detect this and use only random attacks against them.
     */
    Attack best_attack(const std::map<std::string, 
                       FightResults>& data, 
                       const std::string& name);

};

