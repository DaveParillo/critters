#ifndef VIEW_H
#define VIEW_H

#include <iostream>
#include <memory>
#include <unordered_map>

#include "Critter.h"
#include "Point.h"
#include "Species.h"


using std::map;
using std::unordered_map;
using std::string;
using std::shared_ptr;
using std::unique_ptr;


/**
 * Interface for all renderers of a Critters world.
 */
class View {
  private:
    int _height;   /**< height of the view occupied by the Critter world */
    int _width;    /**< width  of the view occupied by the Critter world */
  public:
    /**
     * Init the viewable game world. 
     */
    View(){};
    /**
     * Init the game with a specified height and width
     * @param height of the space allocated for the Critter world
     * @param width of the space allocated for the Critter world
     */
    View(int height, int width) : _height(height), _width(width) {};
    /**
     * Destroy View resources.
     */
    virtual ~View() {};
    /**
     * Render a Critter at its defined location.
     * @param p the Point in the Critter world to be drawn.
     * @param it the Critter to draw
     */
    virtual void draw(const Point& p, const shared_ptr<Critter> it) const = 0;

    virtual void redraw(const unordered_map<Point,  shared_ptr<Critter>> tiles) = 0;

    /**
     * Update the the scores for all the Critters that are competing.
     * @param players the Critters whose scores will be updated.
     *        It is expectd that all of the critters will always be updated.
     */
    virtual void update_score(const map<string, shared_ptr<Species>> players) = 0;

    /**
     * Update the # of moves counter in the view.
     * @param tick the current time
     */
    virtual void update_time(const unsigned long tick) = 0;
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
    virtual int height() {return _height;}
    /**
     * Return the width of the view occupied by the Critter world.
     * @return the width of the view occupied by the Critter world.
     */
    virtual int width()  {return _width;}

    /**
     * Cleanup allocated view windows and resources.
     */
    virtual void teardown() = 0;
};



#endif


