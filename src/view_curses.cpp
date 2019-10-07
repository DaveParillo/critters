
#include <algorithm>
#include <string>
#include <map>
#include <vector>

#include <ncurses.h>
#include "color.h"
#include "view_curses.h"

using std::shared_ptr;

void view_curses::setup() {
  // init ncurses
  initscr();
  nodelay(stdscr, true); // have getch not wait for user keypress
  keypad(stdscr, true);  // enable Fn-keys and keypad
  noecho();
  curs_set(0);
  getmaxyx(stdscr, maxheight_, maxwidth_);
  start_color();
  refresh();

  setup_colors();
  setup_world();
  setup_score();
}


void view_curses::hide_help() {
  delwin(help_);
  touchwin(world_);
  wrefresh(world_);
  touchwin(score_);
  wrefresh(score_);
}
void view_curses::show_help() {
  auto ht = std::min (8, maxheight_/2);
  help_ = newwin(ht, maxwidth_/2, maxheight_/4, maxwidth_/4);
  wbkgd(help_, COLOR_PAIR(1));
  box(help_, 0,0);
  mvwprintw(help_, 0, 2, " Commands Available ");

  mvwprintw(help_, 2, 5, "p:  Play / pause simulation ");
  mvwprintw(help_, 3, 5, "+:  Speed up simulation (can use =) ");
  mvwprintw(help_, 4, 5, "-:  Slow down simulation ");
  mvwprintw(help_, 5, 5, "h:  Show this screen ");
  mvwprintw(help_, 6, 5, "q:  quit ");

  wrefresh(help_);
}

char view_curses::get_key()  {
  return getch();
}

void view_curses::update_time(const unsigned long tick) {
  mvwprintw(score_, score_ht_-1, 9, "      ");
  mvwprintw(score_, score_ht_-1, 9, std::to_string(tick).c_str());
  wrefresh(score_);
}

void view_curses::update_score(const std::map<std::string, shared_ptr<species>> players) {
  std::vector<shared_ptr<species>> dudes;
  for (auto& p: players) {
    dudes.push_back(p.second);
  }
  std::sort(dudes.begin(), dudes.end(), 
      [](shared_ptr<species> a, shared_ptr<species> b) {
        return b->score() < a->score();   
      });

  auto i = 1;
  for (auto& d: dudes) {
    //mvwprintw(score, i, 2,  CLEAR.c_str());  // not working, unexpected
    mvwprintw(score_, i, 2,  "                ");
    mvwprintw(score_, i, 15, "        ");
    mvwprintw(score_, i, 23, "        ");
    mvwprintw(score_, i, 30, "        ");
    mvwprintw(score_, i, 38, "        ");
    mvwprintw(score_, i, 49, "        ");
    mvwprintw(score_, i, 59, "        ");
    mvwprintw(score_, i, 2,  d->name().c_str());
    mvwprintw(score_, i, 16, std::to_string(d->alive()).c_str());
    mvwprintw(score_, i, 24, std::to_string(d->dead()).c_str());
    mvwprintw(score_, i, 31, std::to_string(d->kills()).c_str());
    mvwprintw(score_, i, 39, std::to_string(d->feedings()).c_str());
    mvwprintw(score_, i, 50, std::to_string(d->starved()).c_str());
    mvwprintw(score_, i, 60, std::to_string(d->score()).c_str());
    i++;
  }
  wrefresh(score_);
}

// each cell in ncurses is shaded using a 'color pair'
// a foreground and a background color assigned to an int value
void view_curses::setup_colors() const {
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

int view_curses::adjust_color(const int color, const critter& it) const {
  auto c = color;
  if (it.is_asleep()) c += 10;
  if (it.is_mating()) c += 20;

  return c;
}

int view_curses::set_color(const critter& it) const {
  int color;
  switch (it.color()) {
    case color::WHITE:
      color = 2;
      break;
    case color::RED:
      color = 3;
      break;
    case color::GREEN:
      color = 4;
      break;
    case color::BLUE:
      color = 5;
      break;
    case color::YELLOW:
      color = 6;
      break;
    case color::MAGENTA:
      color = 7;
      break;
    case color::CYAN:
      color = 8;
      break;
    default:
      color = 2;
      break;

  }
  color = adjust_color(color, it);
  wattron(world_, COLOR_PAIR(color));
  return color;
}

void view_curses::unset_color(const int color) const {
  wattroff(world_, COLOR_PAIR(color));
}




void view_curses::setup_score() {
  if (score_wd_ == 0) {
    score_ = newwin(score_ht_, maxwidth_, 0, 0);
  } else {
    score_ = newwin(score_ht_, score_wd_, 0, 0);
  }
  wbkgd(score_, COLOR_PAIR(1));
  getmaxyx(score_, score_ht_, score_wd_);
  box(score_, 0,0);
  mvwprintw(score_, 0, 2, " Scores ");
  mvwprintw(score_, 0, 15, " Alive ");
  mvwprintw(score_, 0, 23, " Dead ");
  mvwprintw(score_, 0, 30, " Kills ");
  mvwprintw(score_, 0, 38, " Feedings ");
  mvwprintw(score_, 0, 49, " Starved ");
  mvwprintw(score_, 0, 59, " Score ");

  mvwprintw(score_, score_ht_-1, 2, " Move: ");

  mvwprintw(score_, score_ht_-1, 15, " Normal:   ");
  mvwprintw(score_, score_ht_-1, 28, " Asleep:   ");
  mvwprintw(score_, score_ht_-1, 41, " Mating:   ");
  wattron(score_, COLOR_PAIR(2));
  mvwprintw(score_, score_ht_-1, 24, "X");
  wattroff(score_, COLOR_PAIR(2));
  wattron(score_, COLOR_PAIR(12));
  mvwprintw(score_, score_ht_-1, 37, "X");
  wattroff(score_, COLOR_PAIR(12));
  wattron(score_, COLOR_PAIR(22));
  mvwprintw(score_, score_ht_-1, 50, "X");
  wattroff(score_, COLOR_PAIR(22));
  wrefresh(score_);

}

void view_curses::setup_world() {
  if (world_wd_ == 0) {
    world_ = newwin(maxheight_ - score_ht_, maxwidth_, score_ht_, 0);
  } else {
    world_ = newwin(world_ht_, world_wd_, score_ht_, 0);
  }
  wbkgd(world_, COLOR_PAIR(2));
  getmaxyx(world_, world_ht_, world_wd_);
  wrefresh(world_);
}


void view_curses::teardown() {
  nodelay(stdscr, false);
  getch();
  delwin(world_);
  delwin(score_);
  endwin();
}

void view_curses::draw(const point& p, const critter&  it) const {
  wmove(world_, p.y, p.x);
  auto c = set_color(it);
  waddch(world_,it.glyph());
  unset_color(c);
  wrefresh(world_);

}



