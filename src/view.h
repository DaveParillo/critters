#ifndef MESA_CRITTERS_VIEW_H
#define MESA_CRITTERS_VIEW_H

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "critter.h"
#include "point.h"
#include "species.h"

/**
 * Interface for all renderers of a Critters world.
 */
class view {
  private:
    int height_;   /**< height of the view occupied by the Critter world */
    int width_;    /**< width  of the view occupied by the Critter world */
  public:
    /**
     * Init the viewable game world. 
     */
    view() = default;
    /**
     * Init the game with a specified height and width
     * @param height of the space allocated for the Critter world
     * @param width of the space allocated for the Critter world
     */
    view(int height, int width) : height_(height), width_(width) {};
    /**
     * Destroy View resources.
     */
    virtual ~view() = default;
    /**
     * Render a Critter at its defined location.
     * @param p the Point in the Critter world to be drawn.
     * @param it the Critter to draw
     */
    virtual void draw(const point& p, const std::shared_ptr<critter> it) const = 0;

    virtual void redraw(const std::unordered_map<point,  std::shared_ptr<critter>> tiles) = 0;

    /**
     * Update the the scores for all the Critters that are competing.
     * @param players the Critters whose scores will be updated.
     *        It is expectd that all of the critters will always be updated.
     */
    virtual void update_score(const std::map<std::string, std::shared_ptr<species>>& players) = 0;

    /**
     * Update the # of moves counter in the view.
     * @param tick the current time
     */
    virtual void update_time(const int64_t tick) = 0;
    /**
     * Display runtime help information.
     */
    virtual void show_help() = 0;
    /**
     * Hide the runtime help information.
     */
    virtual void hide_help() = 0;

    /**
     * Get keyboard commands from the user.
     * @return the character pressed
     */
    virtual char get_key() = 0;

    /**
     * Return the height of the view occupied by the Critter world.
     * @return the height of the view occupied by the Critter world.
     */
    virtual int height() {return height_;}
    /**
     * Return the width of the view occupied by the Critter world.
     * @return the width of the view occupied by the Critter world.
     */
    virtual int width()  {return width_;}

    /**
     * Cleanup allocated view windows and resources.
     */
    virtual void teardown() = 0;
};

#endif

