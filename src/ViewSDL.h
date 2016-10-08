#ifndef VIEWSDL_H
#define VIEWSDL_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Color.h"
#include "Critter.h"
#include "Point.h"
#include "Species.h"
#include "View.h"

using std::map;
using std::unordered_map;
using std::string;
using std::shared_ptr;
using std::unique_ptr;


/**
 * Defines an SDL2 based representation of the Critters world.
 */
class ViewSDL : public View {
  public:
    /**
     * Init the graphics environment.
     */
    ViewSDL();
    /**
     * Init the SDL environment at a size.
     * @param height the desired height of the world
     * @param width the desired width of the world
     */
    ViewSDL(int height, int width);
    /**
     * Destroy the graphics environment.
     */
    ~ViewSDL();

    /**
     * @copydoc View::redraw
     */
    void redraw(const unordered_map<Point,  shared_ptr<Critter>> tiles) override;
    void draw(const Point& p, const shared_ptr<Critter> it) const override {
      assert (p.x() >= 0);
      assert(it != nullptr);
    }

    /**
     * @copydoc View::update_score()
     */
    void update_score(const map<string, shared_ptr<Species>> players) override;
    /**
     * @copydoc View::update_time()
     */
    void update_time(const unsigned long tick) override;
    /**
     * @copydoc View::show_help()
     * @todo the second time help is shown, no text is rendered
     * @todo is help is shown while play is in motion, all of the empty tile fonts are rendered incorrectly
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
    int  height()  override { return world_ht / TILE_SIZE; }
    /**
     * @copydoc View::width()
     */
    int  width()  override { return world_wd / TILE_SIZE; }

  private:
    SDL_Window* world;                            /**< SDL main window for the critters world */
    SDL_Renderer* renderer;                       /**< Renderer for the main window */
    SDL_Window* score;                            /**< SDL window for the score window */
    SDL_Renderer* score_renderer;                 /**< Renderer for the score window */
    SDL_Window* help;                             /**< SDL window for the help dialog */
    SDL_Renderer* help_renderer;                  /**< Renderer for the help window */
    int maxheight;                                /**< Maximum height of the window containing the game */
    int maxwidth;                                 /**< Maximum width of the window containing the game */
    int world_ht;                                 /**< Height of the world subwindow */
    int score_ht;                                 /**< Height of the score subwindow */
    int world_wd;                                 /**< Width of the world subwindow */
    int score_wd;                                 /**< Width of the score subwindow */
    unsigned long ticks;                          /**< Total number of moves completed by critters so far */
    SDL_Event event;                              /**< SDL event object - used to handle keyboard events. */
    TTF_Font* font;                               /**< Font used to render critter glyphs */
    map<const std::string, SDL_Texture*> glyphs;  /**< Cached text */


    static constexpr unsigned int TILE_SIZE = 20;
    static constexpr unsigned int FONT_SIZE = 14;


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
     * Convert a critter color defintion into an SDL_Color
     */
    SDL_Color get_color(const Color c) const;

    /**
     * Cleanup allocated SDL windows and resources.
     */
    void teardown();
    /**
     * Redraw headings in the score window.
     */
    void title_score();


    /**
     * Render the message we want to display to a texture for drawing.
     *
     * The text renderer draws from a map of SDL_Textures built up as
     * textures are created.  Since many of the textures are the same, but
     * simply rendered at different locations, this saves memory over time.
     *
     * @param message The text to display
     * @param color The text color
     * @param renderer The renderer to load the texture in
     * @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
     */
    SDL_Texture* renderText(const std::string &message,
                            SDL_Color color,
                            SDL_Renderer *renderer) ;

    /**
     * Draw an SDL_Texture to an SDL_Renderer at some destination rect
     * taking a clip of the texture if desired
     * @param tex The source texture we want to draw
     * @param ren The renderer we want to draw to
     * @param dst The destination rectangle to render the texture to
     * @param clip The sub-section of the texture to draw (clipping rect)
     *		default of nullptr draws the entire texture
     */
    void renderTexture(SDL_Texture *tex,
                       SDL_Renderer *ren,
                       SDL_Rect dst,
                       SDL_Rect *clip = nullptr) const;


    /**
     * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
     * the texture's width and height and taking a clip of the texture if desired
     * If a clip is passed, the clip's width and height will be used instead of
     *	the texture's
     * @param tex The source texture we want to draw
     * @param ren The renderer we want to draw to
     * @param x The x coordinate to draw to
     * @param y The y coordinate to draw to
     * @param clip The sub-section of the texture to draw (clipping rect)
     *		default of nullptr draws the entire texture
     */
    void renderTexture(SDL_Texture *tex,
                       SDL_Renderer *ren,
                       int x, int y,
                       SDL_Rect *clip = nullptr) const;




};



#endif


