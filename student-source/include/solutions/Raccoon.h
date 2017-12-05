#pragma once
#include <Critter.h>
#include <Color.h>
#include <Direction.h>

#include <map>
#include <memory>

/**
 * A random Raccoon.
 *
 * The Raccoon strategy is to act completely randomly.
 *
 * This strategy is simple and works very well for fighting, but not much else.
 * A Critter can be _too_ random: not stopping to look around creates missed opportunities.
 *
 * Also, success depends on using a good random number algorithm.
 * It is a given that Random Raccoon does not use rand().
 */
class Raccoon: public Critter
{
  public:
    Raccoon() :
      Critter("Random Raccoon"),
      distance_{random_distance()},
      direction_{random_direction()}
    {}
    ~Raccoon() = default;

    /**
     * Inform the simulator that this Raccoon is a player 
     * and can take actions against other players.
     * @return always returns true
     */
    bool is_player() const override { return true; }

    /**
     * @return the color of this Raccoon.
     */
    Color color() const override { return Color::RED; }

    /**
     * Choose a move direction.
     * @param neighbors the contents of the tile surrounding this Raccoon.
     * @return the move direction. Randomly selected, of course.
     */
    Direction move(std::map<Direction, std::shared_ptr<Critter>> neighbors) override;

    /**
     * Fight randomly.
     *
     * @param opponent the critter unforunate enough to face this Raccoon.
     * @return the attack chosen by this Raccoon.
     */
    Attack fight(const std::string name) override;

    /**
     * Decide whether to eat or not.
     * @return true 1/2 the time.
     */
    bool eat() override;

    /**
     * Create a new raccoon
     */
    std::shared_ptr<Critter> create() override {
      return std::make_shared<Raccoon>();
    }

  private:
    size_t    distance_;  /**< Random distance to move */
    Direction direction_; /**< Random direction to move */

    /** 
     * Pick a random direction
     */
    Direction random_direction();

    /** 
     * Pick a random distance
     */
    size_t random_distance();

};

