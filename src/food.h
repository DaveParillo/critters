#ifndef MESA_CRITTERS_FOOD_H
#define MESA_CRITTERS_FOOD_H

#include <memory>

#include "critter.h"
#include "direction.h"

/**
 * Food is consumed by player critters in the game.
 * Critters that move into food are fed for a fixed number of turns.
 */
class food : public critter {

  public:
    food() : critter("Food") { }

    
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
    std::shared_ptr<critter> create() override {
      return std::make_shared<food>();
    }
};

#endif
