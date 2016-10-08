#ifndef FOOD_H
#define FOOD_H

#include <memory>

#include "Direction.h"
#include "Critter.h"

/**
 * Food is consumed by player critters in the game.
 * Critters that move into food are fed for a fixed number of turns.
 */
class Food : public Critter {

  public:
    Food() : Critter("Food") { }

    
    /**
     * Rather than the default 'F', food is represented as a period: '.'.
     * @return the symbol for Food.
     */
    char glyph() const override { 
      return char(46);
    }

    
    /**
     * Make more Food.
     * @return a shared pointer to a new Food object.
     */
    std::shared_ptr<Critter> create() override {
      return std::make_shared<Food>();
    }
};

#endif
