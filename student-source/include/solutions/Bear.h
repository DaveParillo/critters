#pragma once

#include <Critter.h>
#include <Color.h>
#include <Direction.h>

/**
 * A Critter with the bear necessities.
 */
class Bear : public Critter {
  private:
    bool movedNorthLast; /**< indicates the direction of the last move made */

  public:
    Bear() : 
      Critter("Polar Bear"), 
      movedNorthLast(false)  { }

    /**
     * Inform the sim that this Tiger is a player and can take actions against other players.
     * @return always returns true
     */
    bool   is_player() const override { return true; }
    /**
     * return the bear color.
     * @return the color of this bear.
     */
    Color  color()     const override { return Color::CYAN; }

    /**
     * Decide whether to eat or not.
     * Each time a tiger eats, its initial hunger decreases by 1.
     * @return true of this tiger should eat.
     */
    bool eat() override;

    /**
     * Perform Bear-ish fighting actions.
     * @param opponent the critter unforunate enough to face a Bear
     * @return the attack chosen by this bear
     */
    Attack fight(std::string opponent) override;

    /**
     * Choose a move direction.
     * The critter mostly moves north and west.
     * @param neighbors the contents of the tile surrounding this bear
     * @return the direction this tiger want to move
     */
    Direction move(std::map<Direction, std::shared_ptr<Critter>> neighbors) override;

    /**
     * Make a new Bear.
     * @return a shared pointer to a new Bear object.
     */
    std::shared_ptr<Critter> create() override;
};


