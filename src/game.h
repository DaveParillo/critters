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


using std::unordered_map;
using std::string;
using std::shared_ptr;

/**
 * The main Critter simulation controller.
 */
class game {
  
  public:
    /**
     * Create a simulator.
     * Before being used, at a minimum the set_view method must be called to initialize a UI.
     */
    game() = default;
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
     * @param item the type of Critter to create
     * @param num_items the number of items to create
     */
    void addItem(shared_ptr<critter> item, const int num_items);

  private:
    /** 
     * Process runtime keystrokes from users 
     */
    char command = 'x';
    /** 
     * Determine if debugging output is enabled, and if it is, at
     * what level of verbosity, or what type of debug statements to produce.
     */
    int  debug = 0;

    /**
     * The current move number.
     */
    unsigned long tick = 0;
    /**
     * A pointer to the main renderer of the simulation.
     * This object is a placeholder for adding other views (SDL, Swing)
     * at some future date.
     */
    unique_ptr<view> view = nullptr;
    /**
     * Represent each valid position within the game world.
     * As far as tiles are concerned, everything is a 'critter',
     * whether it is actually a critter, an obstacle or other non-player
     * entity, or a blank tile.
     */
    unordered_map<point,  shared_ptr<critter>> tiles;
    /**
     * Stores blank tiles.
     * @todo as this is used mostly during initiialization, there is probably away to 
     * refactor this out of the class and push it down as a local variable.
     */
    unordered_map<point,  shared_ptr<critter>> blanks;

    /**
     * Stores the information used to update scores.
     */
    std::map<string, shared_ptr<species>> players;

    /**
     * Represents the results between two Critters fighting.
     */
    enum class AttackResults {
      ATTACKER,     /*!< The attacker won */
      DEFENDER,     /*!< The defender won */
      DRAW          /*!< The fight was a tie - neither Critter won */
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
    bool  lone_species();

    /**
     * Update is the starting point for all movement and action initiated bya  Critter
     * each time step.
     * @param p the position where this critter currently resides
     * @param it a reference to the critter
     */
    void  update           (const point& p, const shared_ptr<critter>& it);
    /**
     * Determine whether a critter is allowed to move.
     * @param p the position where this critter currently resides (origin position)
     * @param it a reference to the critter
     * @param move_dir The direction the critter wants to move
     * @return true if it can move from its origin to its desired destination
     */
    bool  can_move         (const point& p, const shared_ptr<critter>& it, const direction& move_dir);
    /**
     * Move a critter.
     * Movement is according to instructions received by the critter.
     * Illegal moves are not allowed, but no movement is always an option.
     * @param p the position where this critter currently resides (origin position)
     * @param it a reference to the critter
     * @param move_dir The direction the critter wants to move
     */
    void  move             (const point& p, const shared_ptr<critter>& it, const direction& move_dir);

    /**
     * Move a critter from a source point to a destination.
     *
     * The desination must be either blank or contain food.
     *
     * @param src the position where this critter currently resides (origin position)
     * @param dest the destintation position
     */
    void  move             (const point& src, const point& dest);
    /**
     * Controller for all non-movement actions taken by a critter (fight, mate, etc).
     * @param src the position where this critter currently resides (origin position)
     * @param it a reference to the critter
     * @param dest the destintation position
     */
    void  take_action      (const point& src, shared_ptr<critter> it, const point& dest);
    /**
     * Controller what happens when a critter moves onto a tile containing food.
     * @param src the position where this critter currently resides (origin position)
     * @param it a reference to the critter
     * @param dest the destination postion
     */
    void  process_food     (const point& src, shared_ptr<critter> it, const point& dest);
    /**
     * Determine whether a critter is allowed to mate.
     * @param src_it a reference to the critter initiating the mating
     * @param dest_it a reference to the receiving critter 
     * @return true if src_it and dest_it can mate
     */
    bool  can_mate         (const shared_ptr<critter>& src_it, const shared_ptr<critter>& dest_it); 

    /**
     * Controller what happens when a critter moves onto a tile containing a mate.
     * @param src the position where this critter currently resides
     * @param src_it a reference to the critter
     * @param dest the position where the mate resides
     * @param dest_it a reference to the mate
     */
    void  process_mate     (const point& src,  shared_ptr<critter> src_it, 
                            const point& dest, shared_ptr<critter> dest_it); 
    /**
     * Determine whether a critter is allowed to fight.
     *
     * The opponent can be disabled and not in fighting shape.
     *
     * @param src the position where this critter currently resides
     * @param src_it a reference to the critter
     * @param dest the position where the opponent resides
     * @param dest_it a reference to the opponent
     * @return true if src_it can fight
     */
    bool  can_fight        (const point& src,  const shared_ptr<critter>& src_it, 
                            const point& dest, const shared_ptr<critter>& dest_it); 

    /**
     * Controller what happens when a critter moves onto a tile containing food.
     * @param src the position where this critter currently resides
     * @param src_it a reference to the critter
     * @param dest the position where the opponent resides
     * @param dest_it a reference to the opponent
     */
    void  process_fight    (const point& src,  shared_ptr<critter> src_it, 
                            const point& dest, shared_ptr<critter> dest_it); 

    /**
     * Determine the outcome of two critters that are fighting.
     * @param attacker a reference to the critter that initiated the fight
     * @param defender a reference to the opponent
     * @return the outcome, which could be a draw (no winner)
     */
    AttackResults fight_results    
      (shared_ptr<critter> attacker, shared_ptr<critter> defender);

    /**
     * Update scores and outcomes after a fight.
     * @param winner reference to the winner of the fight
     * @param loser reference to the loser of the fight
     */
    void update_kill_stats (shared_ptr<critter> winner, shared_ptr<critter> loser);

    /**
     * Get a random blank tile from the map of blanks.
     * @return a Point that is equal to EMPTY
     */
    point get_random_blank_tile();

    /**
     * Get all of the neighoring tiles that surround the indicated location.
     * @param p The location representing the center of the request
     * @return a map containing the contents of each of the surrounding 8 locations.
     */
    std::map<direction,  shared_ptr<critter>> get_neighbors(const point& p);


    /**
     * A special Critter defined by the simulator to occupy a blank tile.
     */
    class EMPTY : public critter {
      public:
        EMPTY() : critter("Empty") { }
        char glyph() const override { return char(32); }// ' ' 
        shared_ptr<critter> create()                      override {return std::make_shared<EMPTY>();}
    };

    /**
     * An unoccupied tile in the Critter world.
     * Moving onto a blank tile will not trigger any other action for the moving critter
     * during the current turn.
     */
    shared_ptr<critter> blank_tile = std::make_shared<game::EMPTY>();

};




#endif


