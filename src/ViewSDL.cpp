
#include <cassert>

#include "sdl_clean.h"
#include "Color.h"
#include "ViewSDL.h"

ViewSDL::ViewSDL() :
  maxheight(640), maxwidth(640), world_ht(0), score_ht(10 * FONT_SIZE), world_wd(maxwidth), score_wd(maxwidth)
{
  setup();
}

ViewSDL::ViewSDL(const int height, const int width) :
  maxheight(640), maxwidth(640), world_ht(height), score_ht(10*FONT_SIZE), world_wd(width), score_wd(maxwidth)
{
  setup();
}


ViewSDL::~ViewSDL() {
  teardown();
}

void ViewSDL::teardown() {
  sdl_clean(renderer, world);
  sdl_clean(help, help_renderer);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
}

void ViewSDL::setup() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    exit(-1);
  }

  world = SDL_CreateWindow("C++ Critters", SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           maxwidth, maxheight, SDL_WINDOW_SHOWN);
  setup_world();
  setup_score();

  assert (world != nullptr);
}

void ViewSDL::setup_score() {
  int x, y;
  SDL_GetWindowPosition (world, &x, &y);
  score = SDL_CreateWindow("Score", x, y, score_wd, score_ht, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS );

  score_renderer = SDL_CreateRenderer(score, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(score_renderer, 25,25,112, 255);
  //SDL_SetRenderDrawColor(score_renderer, 65,105,225, 255); // royal blue
  SDL_RenderClear(score_renderer);
  title_score();
  SDL_RenderPresent(score_renderer);
}

void ViewSDL::setup_world() {

  if (world_ht == 0) {
    world_ht = maxheight - score_ht;
  }
  //renderer = SDL_CreateRenderer(world, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  renderer = SDL_CreateRenderer(world, -1, SDL_RENDERER_ACCELERATED);
  assert (renderer != nullptr);

  //SDL_SetRenderDrawColor(renderer, 152,251,152, 255);  // world background from original critters
  SDL_SetRenderDrawColor(renderer, 40,40,40, 255);
  SDL_RenderClear(renderer);

  assert (TTF_Init() == 0);

  //Open the font
  font = TTF_OpenFont("VeraMoBd.ttf", FONT_SIZE);
  assert (font != nullptr);

}


void ViewSDL::redraw(const unordered_map<Point,  shared_ptr<Critter>> tiles) {
  SDL_RenderClear(renderer);

  SDL_Color color;
  for (auto t: tiles) {
    color = get_color(t.second->color());
    SDL_Texture *image = renderText(std::string(1, t.second->glyph()), color, renderer);
    renderTexture(image, renderer, TILE_SIZE*t.first.x()+FONT_SIZE/2, TILE_SIZE*t.first.y() + score_ht);
  }
  SDL_RenderPresent(renderer);
}

void ViewSDL::title_score () {
  SDL_SetRenderDrawColor(score_renderer, 255,255,255, 255);
  SDL_RenderDrawRect(score_renderer, nullptr);
  SDL_SetRenderDrawColor(score_renderer, 25,25,112, 255); // dark blue
  //SDL_SetRenderDrawColor(score_renderer, 65,105,225, 255); // royal blue

  auto ypos = FONT_SIZE/2;
  SDL_Color color = { 255, 255, 255, 255 };
  SDL_Texture *title1 = renderText("Scores", color, score_renderer);
  renderTexture(title1, score_renderer, 2*FONT_SIZE, ypos);
  SDL_Texture *title2 = renderText("Alive", color, score_renderer);
  renderTexture(title2, score_renderer, 10*FONT_SIZE, ypos);
  SDL_Texture *title3 = renderText("Dead", color, score_renderer);
  renderTexture(title3, score_renderer, 15*FONT_SIZE, ypos);
  SDL_Texture *title4 = renderText("Kills", color, score_renderer);
  renderTexture(title4, score_renderer, 20*FONT_SIZE, ypos);
  SDL_Texture *title5 = renderText("Feedings", color, score_renderer);
  renderTexture(title5, score_renderer, 25*FONT_SIZE, ypos);
  SDL_Texture *title6 = renderText("Starved", color, score_renderer);
  renderTexture(title6, score_renderer, 32*FONT_SIZE, ypos);
  SDL_Texture *title7 = renderText("Score", color, score_renderer);
  renderTexture(title7, score_renderer, 40*FONT_SIZE, ypos);

  SDL_Texture *title8 = renderText("Move: ", color, score_renderer);
  renderTexture(title8, score_renderer, 2*FONT_SIZE, score_ht - 1.5*FONT_SIZE);

}

void ViewSDL::update_score(const map<string, shared_ptr<Species>> players) {
  SDL_RenderClear(score_renderer);

  title_score();
  auto i = 1;
  SDL_Color color = { 255, 255, 255, 255 };
  auto ypos = FONT_SIZE/2;
  auto offset = 0;

  for (auto& p: players) {
    offset = ypos + (i*FONT_SIZE);
    SDL_Texture* name = renderText(p.second->name(), color, score_renderer);
    renderTexture(name, score_renderer, FONT_SIZE, offset);

    // the spaces here are a hack to deal with values = 0
    SDL_Texture* alive = renderText(" " + std::to_string(p.second->alive()), color, score_renderer);
    renderTexture(alive, score_renderer, 10*FONT_SIZE, offset);
    SDL_Texture* dead = renderText(" " + std::to_string(p.second->dead()), color, score_renderer);
    renderTexture(dead, score_renderer, 15*FONT_SIZE, offset);
    SDL_Texture* kills = renderText(" " + std::to_string(p.second->kills()), color, score_renderer);
    renderTexture(kills, score_renderer, 20*FONT_SIZE, offset);
    SDL_Texture* feedings = renderText(" " + std::to_string(p.second->feedings()), color, score_renderer);
    renderTexture(feedings, score_renderer, 25*FONT_SIZE, offset);
    SDL_Texture* starved = renderText(" " + std::to_string(p.second->starved()), color, score_renderer);
    renderTexture(starved, score_renderer, 32*FONT_SIZE, offset);
    SDL_Texture* score = renderText(" " + std::to_string(p.second->score()), color, score_renderer);
    renderTexture(score, score_renderer, 40*FONT_SIZE, offset);

    i++;
  }
  
  SDL_Texture* move = renderText(std::to_string(ticks), color, score_renderer);
  renderTexture(move, score_renderer, 6*FONT_SIZE, score_ht - 1.5*FONT_SIZE);

  SDL_RenderPresent(score_renderer);

}


SDL_Color ViewSDL::get_color(const Color c) const {
  SDL_Color color;
  switch (c) {
    case Color::WHITE:
      color = { 255, 255, 255, 255 };
      break;
    case Color::RED:
      color = { 255, 0, 0, 255 };
      break;
    case Color::GREEN:
      color = { 0, 255, 0, 255 };
      break;
    case Color::BLUE:
      color = { 0, 0, 255, 255 };
      break;
    case Color::YELLOW:
      color = { 255, 255, 0, 255 };
      break;
    case Color::MAGENTA:
      color = { 255, 0, 255, 255 };
      break;
    case Color::CYAN:
      color = { 0, 255, 255, 255 };
      break;
    default:
      color = { 0, 0, 0, 255 };
      break;

  }
  return color;


}

void ViewSDL::update_time(const unsigned long tick) {
  ticks = tick;
}


char ViewSDL::get_key() {
  int x, y;
  SDL_GetWindowPosition (world, &x, &y);
  SDL_SetWindowPosition (score, x, y);
  SDL_RaiseWindow(score);

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      teardown();
      return 'q';
    }
    if (event.type == SDL_KEYDOWN) {
      return event.key.keysym.sym;
    }
  }
  return '~';
}

SDL_Texture* ViewSDL::renderText(const std::string& message, SDL_Color color, SDL_Renderer* renderer)
{
  std::string key = message + std::to_string(color.r << 16 | color.g << 8 | color.b);
  if (glyphs.find(key) == glyphs.end()) {
    SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
    assert (surf != nullptr);

    auto v = SDL_CreateTextureFromSurface(renderer, surf);
    glyphs.insert(std::map<std::string, SDL_Texture*>::value_type(key, v));

    SDL_FreeSurface(surf);
  }

  return glyphs.at(key);
}

void ViewSDL::renderTexture(SDL_Texture* tex, SDL_Renderer* ren, SDL_Rect dst, SDL_Rect* clip) const
{
  SDL_RenderCopy(ren, tex, clip, &dst);
}

void ViewSDL::renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, SDL_Rect* clip) const
{
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  if (clip != nullptr){
    dst.w = clip->w;
    dst.h = clip->h;
  }
  else {
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
  }
  renderTexture(tex, ren, dst, clip);
}


void ViewSDL::hide_help() {
  SDL_HideWindow(help);
}

void ViewSDL::show_help() {

  if (help == nullptr) {
    help = SDL_CreateWindow("C++ Critters Help",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        450, 200, SDL_WINDOW_SHOWN);
    help_renderer = SDL_CreateRenderer(help, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(help_renderer, 255,255,255, 255);
    SDL_RenderClear(help_renderer);

    SDL_Color color = { 0, 0, 0, 255 };
    SDL_Texture *line1 = renderText("Commands Available: ", color, help_renderer);
    renderTexture(line1, help_renderer, 10, 10);
    SDL_Texture *line2 = renderText("p:  Play / pause simulation ", color, help_renderer);
    renderTexture(line2, help_renderer, 50, FONT_SIZE *3);
    SDL_Texture *line3 = renderText("+:  Speed up simulation (can use =) ", color, help_renderer);
    renderTexture(line3, help_renderer, 50, FONT_SIZE *5);
    SDL_Texture *line4 = renderText("-:  Slow down simulation ", color, help_renderer);
    renderTexture(line4, help_renderer, 50, FONT_SIZE *7);
    SDL_Texture *line5 = renderText("h:  Show / hide this screen ", color, help_renderer);
    renderTexture(line5, help_renderer, 50, FONT_SIZE *9);
    SDL_Texture *line6 = renderText("q:  quit", color, help_renderer);
    renderTexture(line6, help_renderer, 50, FONT_SIZE *11);


    SDL_RenderPresent(help_renderer);
  } else {
    SDL_ShowWindow(help);
  }

}




