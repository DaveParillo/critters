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

    std::shared_ptr<Critter> create() override {
      return std::make_shared<Duck>();
    }

    // -------------------------------------------------------------------------
    // Encapsulate fight result data
    // Use to predict most successful attacks against known enemy types
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

      size_t operator[](size_t i) { 
        return data[i];
      }

     // @return Attack predicted to have most success
      Attack best_attack() const;

      // @param attack Attack data to increment
      void won(Attack attack) {
        ++wins;
        if (attack > Attack::FORFEIT || attack < Attack::ROAR) return;
        ++data.at(size_t(attack));
      }

      // @param attack Attack data to decrement
      void lost(Attack attack) {
        ++losses;
        if (attack > Attack::FORFEIT || attack < Attack::ROAR) return;
        auto &n = data.at(size_t(attack));
        if (n > 1) --n;
      }
    };

    using fightmap = std::map<std::string, FightResults>;

  private:
    size_t               distance_;
    Direction            direction_;
    std::string          opponent_;
    Attack               attack_used_;

    static fightmap      fight_results_;

    struct Priority {
      Direction direction;
      size_t weight;

      Priority() = default;
      Priority(Direction dir, size_t wt)
        : direction{dir}, weight{wt}
      {}
    };
    
    using neighbor = std::pair<const Direction, std::shared_ptr<Critter>>;

    void evaluate_neighbor(const neighbor &n, std::vector<Priority>* weighted_dirs);

    bool is_hungry() const;


    bool is_starving() const;

    /** 
     * Pick a random direction, but prioritize diagonals:
     * NORTH_EAST: 1, SOUTH_EAST: 3,
     * SOUTH_WEST: 5, NORTH_WEST: 7
     */
    Direction random_direction();

    size_t random_distance();

    /// Get attack
    // @param data Fight data map
    // @param name Opponent critter name
    // @return Suggested attack (weighted or randomized)
    Attack best_attack(const fightmap& data, std::string name);


};
