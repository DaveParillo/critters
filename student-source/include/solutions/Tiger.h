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
    int remainingFeedings;   /**< the amount of hunger remaining for this tiger */
    int numMovesInCurrDir;   /**< limits the randomness of moves */
    Direction currDir;       /**< the direction this tiger is currently moving */

  public:
    Tiger();

    /**
     * Create a hungry tiger.
     * @param hunger determines how hungry this tiger is.
     *        Tiger will eat ehn they encounter food if this value is > 0
     */
    Tiger(unsigned int hunger);

    /**
     * Inform the sim that this Tiger is a player and can take actions against other players.
     * @return always returns true
     */
    bool is_player() const override { return true; }

    /**
     * Decide whether to eat or not.
     * Each time a tiger eats, its initial hunger decreases by 1.
     * @return true of this tiger should eat.
     */
    bool eat() override;

    /**
     * Return the symbol for a Tiger.
     * Rather than the default 'T', a Tiger is represented as numberindicating how hungry it is.
     * @return the Tiger symbol
     */
    char glyph() const override;

    /**
     * Perform Tiger-ish fighting actions.
     * @param opponent the critter unforunate enough to face a Tiger
     * @return the attack chosen by this tiger
     */
    Attack fight(std::string opponent) override;

    /**
     * Choose a move direction.
     * @param neighbors the contents of the tile surrounding this tiger
     * @return the direction this tiger want to move
     */
    Direction move(std::map<Direction, std::shared_ptr<Critter>> neighbors) override;

    
    /**
     * Make a new Tiger.
     * @return a shared pointer to a new Tiger object.
     */
    std::shared_ptr<Critter> create() override;
};


