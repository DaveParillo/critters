#ifndef MESA_CRITTERS_STONE_H
#define MESA_CRITTERS_STONE_H

#include <memory>

#include "direction.h"
#include "critter.h"

/**
 * A Stone is an obstacle in the game.
 * Critters that move into a stone are temporarily stunned.
 * Stunned critters are unable to take any action and can't move
 * until they have recovered.
 *
 * Additionally, they are vulnerable to attack.
 * A stunned Critter immediately loses a fight against any other Critter
 * that attacks it while it is stunned.
 */
class stone : public critter {

  public:
    stone() : critter("Stone") { }

    /**
     * Rather than the default 'S', a Stone is represented as a '#'.
     * @return the symbol for a Stone.
     */
    char glyph() const override{ 
      return char(35); // #
    }


    /**
     * Make a new Stone.
     * @return a shared pointer to a new Stone object.
     */
    std::shared_ptr<critter> create() override {
      return std::make_shared<stone>();
    }
};

#endif
