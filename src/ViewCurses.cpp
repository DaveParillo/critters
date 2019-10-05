#include <algorithm>
#include <string>
#include <vector>

#include "Color.h"
#include "ViewCurses.h"

void ViewCurses::setup() {
  // init ncurses
  initscr();
  nodelay(stdscr, true); // have getch not wait for user keypress
  keypad(stdscr, true);  // enable Fn-keys and keypad
  noecho();
  curs_set(0);
  getmaxyx(stdscr, maxheight, maxwidth);
  start_color();
  refresh();

  setup_colors();
  setup_world();
  setup_score();
}


void ViewCurses::hide_help() {
  delwin(help);
  touchwin(world);
  wrefresh(world);
  touchwin(score);
  wrefresh(score);
}
void ViewCurses::show_help() {
  auto ht = std::min (8, maxheight/2);
  help = newwin(ht, maxwidth/2, maxheight/4, maxwidth/4);
  wbkgd(help, COLOR_PAIR(1));
  box(help, 0,0);
  mvwprintw(help, 0, 2, " Commands Available ");

  mvwprintw(help, 2, 5, "p:  Play / pause simulation ");
  mvwprintw(help, 3, 5, "+:  Speed up simulation (can use =) ");
  mvwprintw(help, 4, 5, "-:  Slow down simulation ");
  mvwprintw(help, 5, 5, "h:  Show this screen ");
  mvwprintw(help, 6, 5, "q:  quit ");

  wrefresh(help);
}

char ViewCurses::get_key()  {
  return getch();
}

void ViewCurses::update_time(const unsigned long tick) {
  mvwprintw(score, score_ht-1, 9, "      ");
  mvwprintw(score, score_ht-1, 9, std::to_string(tick).c_str());
  wrefresh(score);
}

void ViewCurses::update_score(const map<string, shared_ptr<Species>> players) {
  std::vector<shared_ptr<Species>> dudes;
  for (auto& p: players) {
    dudes.push_back(p.second);
  }
  std::sort(dudes.begin(), dudes.end(), 
      [](shared_ptr<Species> a, shared_ptr<Species> b) {
        return b->score() < a->score();   
      });

  auto i = 1;
  for (auto& d: dudes) {
    //mvwprintw(score, i, 2,  CLEAR.c_str());  // not working, unexpected
    mvwprintw(score, i, 2,  "                ");
    mvwprintw(score, i, 15, "        ");
    mvwprintw(score, i, 23, "        ");
    mvwprintw(score, i, 30, "        ");
    mvwprintw(score, i, 38, "        ");
    mvwprintw(score, i, 49, "        ");
    mvwprintw(score, i, 59, "        ");
    mvwprintw(score, i, 2,  d->name().c_str());
    mvwprintw(score, i, 16, std::to_string(d->alive()).c_str());
    mvwprintw(score, i, 24, std::to_string(d->dead()).c_str());
    mvwprintw(score, i, 31, std::to_string(d->kills()).c_str());
    mvwprintw(score, i, 39, std::to_string(d->feedings()).c_str());
    mvwprintw(score, i, 50, std::to_string(d->starved()).c_str());
    mvwprintw(score, i, 60, std::to_string(d->score()).c_str());
    i++;
  }
  wrefresh(score);
}

// each cell in ncurses is shaded using a 'color pair'
// a foreground and a background color assigned to an int value
void ViewCurses::setup_colors() const {
  init_pair(1,COLOR_WHITE, COLOR_BLUE);   // used for score area

  // 'normal' colors
  init_pair(2,COLOR_WHITE, COLOR_BLACK);
  init_pair(3,COLOR_RED, COLOR_BLACK);
  init_pair(4,COLOR_GREEN, COLOR_BLACK);
  init_pair(5,COLOR_BLUE, COLOR_BLACK);
  init_pair(6,COLOR_YELLOW, COLOR_BLACK);
  init_pair(7,COLOR_MAGENTA, COLOR_BLACK);
  init_pair(8,COLOR_CYAN, COLOR_BLACK);
  // 'sleeping' colors
  init_pair(12,  COLOR_BLACK, COLOR_WHITE);
  init_pair(13, COLOR_BLACK, COLOR_RED);
  init_pair(14, COLOR_BLACK, COLOR_GREEN);
  init_pair(15, COLOR_BLACK, COLOR_BLUE);
  init_pair(16, COLOR_BLACK, COLOR_YELLOW);
  init_pair(17, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(18, COLOR_BLACK, COLOR_CYAN);
  // 'mating' colors
  init_pair(22,COLOR_WHITE, COLOR_RED);
  init_pair(23,COLOR_MAGENTA, COLOR_RED);
  init_pair(24,COLOR_GREEN, COLOR_RED);
  init_pair(25,COLOR_BLUE, COLOR_RED);
  init_pair(26,COLOR_YELLOW, COLOR_RED);
  init_pair(27,COLOR_MAGENTA, COLOR_RED);
  init_pair(28,COLOR_CYAN, COLOR_RED);
}

int ViewCurses::adjust_color(const int color, const shared_ptr<Critter> it) const {
  auto c = color;
  if (it->is_asleep()) c += 10;
  if (it->is_mating()) c += 20;

  return c;
}

int ViewCurses::set_color(const shared_ptr<Critter> it) const {
  int color;
  switch (it->color()) {
    case Color::WHITE:
      color = 2;
      break;
    case Color::RED:
      color = 3;
      break;
    case Color::GREEN:
      color = 4;
      break;
    case Color::BLUE:
      color = 5;
      break;
    case Color::YELLOW:
      color = 6;
      break;
    case Color::MAGENTA:
      color = 7;
      break;
    case Color::CYAN:
      color = 8;
      break;
    default:
      color = 2;
      break;

  }
  color = adjust_color(color, it);
  wattron(world, COLOR_PAIR(color));
  return color;
}

void ViewCurses::unset_color(const int color) const {
  wattroff(world, COLOR_PAIR(color));
}




void ViewCurses::setup_score() {
  if (score_wd == 0) {
    score = newwin(score_ht, maxwidth, 0, 0);
  } else {
    score = newwin(score_ht, score_wd, 0, 0);
  }
  wbkgd(score, COLOR_PAIR(1));
  getmaxyx(score, score_ht, score_wd);
  box(score, 0,0);
  mvwprintw(score, 0, 2, " Scores ");
  mvwprintw(score, 0, 15, " Alive ");
  mvwprintw(score, 0, 23, " Dead ");
  mvwprintw(score, 0, 30, " Kills ");
  mvwprintw(score, 0, 38, " Feedings ");
  mvwprintw(score, 0, 49, " Starved ");
  mvwprintw(score, 0, 59, " Score ");

  mvwprintw(score, score_ht-1, 2, " Move: ");

  mvwprintw(score, score_ht-1, 15, " Normal:   ");
  mvwprintw(score, score_ht-1, 28, " Asleep:   ");
  mvwprintw(score, score_ht-1, 41, " Mating:   ");
  wattron(score, COLOR_PAIR(2));
  mvwprintw(score, score_ht-1, 24, "X");
  wattroff(score, COLOR_PAIR(2));
  wattron(score, COLOR_PAIR(12));
  mvwprintw(score, score_ht-1, 37, "X");
  wattroff(score, COLOR_PAIR(12));
  wattron(score, COLOR_PAIR(22));
  mvwprintw(score, score_ht-1, 50, "X");
  wattroff(score, COLOR_PAIR(22));
  wrefresh(score);

}

void ViewCurses::setup_world() {
  if (world_wd == 0) {
    world = newwin(maxheight - score_ht, maxwidth, score_ht, 0);
  } else {
    world = newwin(world_ht, world_wd, score_ht, 0);
  }
  wbkgd(world, COLOR_PAIR(2));
  getmaxyx(world, world_ht, world_wd);
  wrefresh(world);
}


void ViewCurses::teardown() {
  nodelay(stdscr, false);
  getch();
  delwin(world);
  delwin(score);
  endwin();
}

void ViewCurses::draw(const Point& p, const std::shared_ptr<Critter>  it) const {
  wmove(world, p.y(), p.x());
  auto c = set_color(it);
  waddch(world,it->glyph());
  unset_color(c);
  wrefresh(world);

}



