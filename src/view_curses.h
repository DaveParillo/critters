#ifndef MESA_CRITTERS_VIEW_CURSES_H
#define MESA_CRITTERS_VIEW_CURSES_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <ncurses.h>

#include "color.h"
#include "critter.h"
#include "point.h"
#include "species.h"
#include "view.h"

using std::map;
using std::unordered_map;
using std::string;
using std::shared_ptr;
using std::unique_ptr;


/**
 * Defines an ncurses representation of the Critters world.
 */
class view_curses : public view {
  public:
    /**
     * Init the ncurses environment.
     */
    view_curses() 
    {
      setup();
    }

    /**
     * Init the ncurses environment at a size.
     * @param height the desired height of the world
     * @param width the desired width of the world
     */
    view_curses(const int height, const int width) 
      : world_ht(height)
        , score_ht(10)
        , world_wd(width)
        , score_wd(0)
    {
      setup();
    }

    /**
     * Destroy the ncurses environment.
     */
    ~view_curses() {
      teardown();
    }

    /**
     * Render Critters and other objects on the screen.
     * @param p the location on the world screen
     * @param it an entity to draw.
     */
    void draw(const point& p, const shared_ptr<critter> it) const override;

    void redraw(const unordered_map<point,  shared_ptr<critter>> tiles) override {
      assert (tiles.size() > 0);
    }

    /**
     * @copydoc View::update_score()
     */
    void update_score(const map<string, shared_ptr<species>> players) override;
    /**
     * @copydoc View::update_time()
     */
    void update_time(const unsigned long tick) override;
    /**
     * @copydoc View::show_help()
     */
    void show_help() override;
    /**
     * @copydoc View::hide_help()
     */
    void hide_help() override;

    /**
     * @copydoc View::get_key()
     */
    char get_key() override;


    /**
     * @copydoc View::width()
     */
    int  height()  override { return world_ht; }
    /**
     * @copydoc View::width()
     */
    int  width()  override { return world_wd; }

    /**
     * Cleanup allocated ncurses windows and resources.
     */
    void teardown() override;

  private:
    WINDOW* world = nullptr;                   /**< nucurses window for the critter playing surface */
    WINDOW* score = nullptr;                   /**< nucurses window for the scores */
    WINDOW* help = nullptr;                    /**< nucurses window for the help dialog */
    int maxheight = 24;                        /**< maximum height of the console containing the game */
    int maxwidth = 72;                         /**< maximum width of the console containing the game */
    int world_ht = 0;                          /**< height of the world screen */
    int score_ht = 10;                         /**< height of the score screen */
    int world_wd = 0;                          /**< width of the world screen */
    int score_wd = 0;                          /**< width of the score screen */
    const std::string CLEAR = {65, ' '};       /**< a bunch of blank spaces used to clear a score line */

    /**
     * Initialize the view.
     */
    void setup();
    /**
     * Initialize the scoreboard.
     */
    void setup_score();
    /**
     * Initialize the playing surface.
     */
    void setup_world();

    /**
     * Initialize the colors used.
     */
    void setup_colors() const;
    /**
     * Set the color used for the next draw command.
     * Colors used are typically defined in terms of the critter color.
     * Use the color directly, reverse the color, etc.
     *
     * @param it the critter
     * @return the ncurses COLOR_PAIR index associated with the color used.
     *         All the color pairs are defined in setup_colors()
     */
    int  set_color(const shared_ptr<critter> it) const;
    /**
     * Apply any mods to a color based on the current state of a critter
     *
     * @param color the color to adjust
     * @param it the critter
     * @return the ncurses COLOR_PAIR index associated with the color used.
     */
    int  adjust_color(const int color, const shared_ptr<critter> it) const;

    /**
     * Reset the ncurses color environment back to its defaults.
     * @param color the color pair to unset.
     */
    void unset_color(const int color) const;
    

};



#endif


