#pragma once

#include <iostream>
#include <memory>


#include <Critter.h>
#include <Color.h>
#include <Direction.h>

/**
 * A Critter with tiger-ish behaviors.
 */
class Tiger : public Critter {
  private:
    unsigned int remainingFeedings;   /**< the amount of hunger remaining for this tiger */
    unsigned int numMovesInCurrDir;   /**< limits the randomness of moves */
    Direction currDir;                /**< the direction this tiger is currently moving */

    
  public:
    Tiger();

    /**
     * Create a hungry tiger.
     * @param hunger determines how hungry this tiger is.
     *        Tiger will eat when they encounter food if this value is > 0
     */
    Tiger(unsigned int hunger);

    /**
     * Inform the Simulator that this Tiger is a player 
     * and can take actions against other players.
     * @return always returns true
     */
    bool is_player() const override { return true; }

    /**
     * Decide whether to eat or not.
     * Each time a tiger eats, its initial hunger decreases by 1.
     * @return true if it's remaining feedings > 0.
     */
    bool eat() override;

    /**
     * Return the symbol for a Tiger.
     * Rather than the default 'T', 
     * a Tiger is represented as number indicating how hungry it is.
     * @return the Tiger 'num feedings' value
     */
    char glyph() const override;

    /**
     * Perform Tiger-ish fighting actions.
     * @param opponent the critter unforunate enough to face a Tiger
     * @return Critter::Attack::SCRATCH, if the Tiger is still hungry
     * @return Critter::Attack::POUNCE otherwise 
     */
    Attack fight(std::string opponent) override;

    /**
     * Choose a move direction.
     *
     * Tigers only ever move NORTH, SOUTH, EAST, or WEST;
     * never diagonally.
     * 
     * They also move randomly, but do not change direction each turn.
     * A Tiger moves for 3 turns in the same direction, 
     * then chooses a new direction, which might be the same as the current direction.
     *
     * Tigers make no attempt to avoid obstacles.
     *
     * @param neighbors the contents of the tile surrounding this Tiger
     * @return the direction this Tiger wants to move
     */
    Direction move(std::map<Direction, std::shared_ptr<Critter>> neighbors) override;

    
    /**
     * Make a new Tiger.
     * @return a shared pointer to a new Tiger object.
     */
    std::shared_ptr<Critter> create() override;
};


