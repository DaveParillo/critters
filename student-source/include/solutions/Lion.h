#pragma once

#include <Critter.h>
#include <Color.h>
#include <Direction.h>


/**
 * A Critter with behaviors worthy of the King of the Jungle.
 */
class Lion : public Critter {
  private:
    bool foughtSinceLastEat; /**< flag to limit the amount of eating.  Ensure at least 1 fight between meals */
    int moveNum;             /**< counts how many times this lion has moved */

  public:
    Lion() : 
      Critter("Loopy Lion"), 
      foughtSinceLastEat(false), moveNum(0)  { }

    /**
     * Inform the simulator that this Critter is a player 
     * and can take actions against other players.
     * @return always returns true
     */
    bool   is_player() const override { return true; }
    /**
     * @return the color of this Lion.
     */
    Color  color()     const override { return Color::YELLOW; }

    /**
     * Decide whether to eat or not.
     * @return true only if the Lion has fought since the last time it ate.
     */
    bool eat() override;

    /**
     * Fight like the king of the jungle.
     * @param opponent the critter unforunate enough to face a Lion
     * @return the attack chosen by this Lion.
     * A Lion knows that a Bear will only return Critter::Attack::SCRATCH,
     * so it responds with Critter::Attack::ROAR, if the opponent
     * seems to be a Bear (the name contains a 'B').
     * @return Critter::Attack::POUNCE otherwise
     */
    Attack fight(std::string opponent) override;

    /**
     * Choose a move direction.
     * @param neighbors the contents of the tile surrounding this lion
     * @return the move direction. 
     * Lions move in a generally circular pattern, clock-wise.
     *
     * A Lion does not try to avoid obstacles,
     * so most Lion deaths are near Stones.
     *
     * They are creatures of the wide open savannah.
     */
    Direction move(std::map<Direction, std::shared_ptr<Critter>> neighbors) override;

    /**
     * Make a new Lion.
     * @return a shared pointer to a new Lion object.
     */
    std::shared_ptr<Critter> create() override;
};


