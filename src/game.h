#ifndef MESA_CRITTERS_GAME_H
#define MESA_CRITTERS_GAME_H

#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "view.h"
#include "critter.h"
#include "direction.h"
#include "point.h"
#include "species.h"

/**
 * The main critter simulation controller.
 */
class game {
  
  public:
    /**
     * Create a rock-paper-scissors world simulator.
     * Before being used, at a minimum the set_view method must be called to initialize a UI.
     */
    game() = default;
    ~game() = default;
    /**
     * Start running the simulation.
     */
    void start();
    /**
     * Turns debug output on at the specified level.
     * Currently, the only level defined is 1.
     */
    void set_debug(int debug_level);

    /**
     * Initialize the view that will be used to render simulator output.
     */
    void set_view(std::unique_ptr<view> v);


    /**
     * Seeds the world with some number of Entities.
     * @param item the type of critter to create
     * @param num_items the number of items to create
     */
    void add_item(std::shared_ptr<critter> item, const int num_items);

  private:
    /** 
     * Process runtime keystrokes from users 
     */
    char command_ = 'x';
    /** 
     * Determine if debugging output is enabled, and if it is, at
     * what level of verbosity, or what type of debug statements to produce.
     */
    int  debug_ = 0;

    /**
     * The current move number.
     */
    unsigned long tick_ = 0;
    /**
     * A pointer to the main renderer of the simulation.
     * This object is a placeholder for adding other views (SDL, Swing)
     * at some future date.
     */
    std::unique_ptr<view> view_ = nullptr;
    /**
     * Represent each valid position within the game world.
     * As far as tiles are concerned, everything is a 'critter',
     * whether it is actually a critter, an obstacle or other non-player
     * entity, or a blank tile.
     */
    std::unordered_map<point,  std::shared_ptr<critter>> tiles_;
    /**
     * Stores blank tiles.
     * @todo as this is used mostly during initiialization, there is probably away to 
     * refactor this out of the class and push it down as a local variable.
     */
    std::unordered_map<point,  std::shared_ptr<critter>> blanks_;

    /**
     * Stores the information used to update scores.
     */
    std::map<std::string, std::shared_ptr<species>> players_;

    /**
     * Represents the results between two critters fighting.
     */
    enum class fight_results {
      ATTACKER,     /*!< The attacker won */
      DEFENDER,     /*!< The defender won */
      DRAW          /*!< The fight was a tie - neither critter won */
    };


    /**
     * Initialize all the tiles in the simulation.
     * By default, all tiles are initially empty.
     */
    void  init_tiles();
    /**
     * Update every tile in the simulation.
     * Calls update and modifies critters according to the rules of the game.
     */
    void  update_tiles();

    /**
     * Update is the starting point for all movement and action initiated by a critter
     * each time step.
     * @param t a reference to a game tile
     */
    void  update           (const std::pair<point,  std::shared_ptr<critter>>& t);

    /**
     * Move a critter from a source point to a destination.
     *
     * The destination must not be a stone.
     *
     * @param src the position where this critter currently resides (origin position)
     * @param dest the destintation position
     */
    void  move             (const point& src, const point& dest);

    /**
     * Controller for all non-movement actions taken by a critter (fight, mate, etc).
     * @param src the position where this critter currently resides (origin position)
     * @param dest the destintation position
     */
    void  take_action      (const point& src, const point& dest);

    /**
     * Controller what happens when a critter moves onto a tile containing food.
     * @param src the position where this critter currently resides (origin position)
     * @param dest the destination postion
     */
    void  process_food     (const point& src, const point& dest);

    /**
     * Controller what happens when a critter moves onto a tile containing a mate.
     * @param src the position where this critter currently resides
     * @param dest the position where the mate resides
     */
    void  process_mate     (const point& src, const point& dest);

    /**
     * Controller what happens when a critter moves onto a tile containing food.
     * @param src the position where this critter currently resides
     * @param dest the position where the opponent resides
     */
    void  process_fight    (const point& src, const point& dest);

    /**
     * Determine the outcome of two critters that are fighting.
     * @param attacker a reference to the critter that initiated the fight
     * @param defender a reference to the opponent
     * @return the outcome, which could be a draw (no winner)
     */
    fight_results get_fight_results (critter* attacker, critter* defender);

    /**
     * Update scores and outcomes after a fight.
     * @param winner reference to the winner of the fight
     * @param loser reference to the loser of the fight
     */
    void update_kill_stats (critter* winner, critter* loser);

    /**
     * Get all of the neighoring tiles that surround the indicated location.
     * @param p The location representing the center of the request
     * @return a map containing the contents of each of the surrounding 8 locations.
     */
    std::map<direction,  std::shared_ptr<critter>> get_neighbors(const point& p);


    /**
     * A special critter defined by the simulator to occupy a blank tile.
     */
    class EMPTY : public critter {
      public:
        EMPTY() : critter("Empty") { }
        char glyph() const override { return char(32); }// ' ' 
        std::shared_ptr<critter> create() override
          {
            return std::make_shared<EMPTY>();
          }
    };

    /**
     * An unoccupied tile in the critter world.
     * Moving onto a blank tile will not trigger any other action for the moving critter
     * during the current turn.
     */
    std::shared_ptr<critter> blank_tile = std::make_shared<game::EMPTY>();

};




#endif


