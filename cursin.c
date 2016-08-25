#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define DEFAULT_AMPLITUDE 1
#define DEFAULT_PERIOD .35
#define DELAY 15000

//Color count
#define NUM_COLORS 6

typedef unsigned short ushort;

ushort printwave(double amplitude, double period,
    short phase_shift, ushort color_index) {

  double x;
  for (x=0.0; x < (COLS+phase_shift) ; x += 1.0) {

    // Swap color
    color_index += 1;
    attron(COLOR_PAIR((color_index % NUM_COLORS)+1));

    // Find Y 
    double ratio = (2.0*M_PI)/LINES;
    double y = sin(period*x*ratio);
    y *= amplitude; 
    y += LINES/2;

    // Print cell
    mvprintw((int)(y), (int)(x-phase_shift), " ");

  }

  return color_index;

}

int main(int argc, char* argv[]) {

  // Curses init
  WINDOW* screen = initscr();
  start_color();
  nodelay(screen, 1);
  cbreak();
  noecho();
  curs_set(0);
  keypad(screen, TRUE);

  // Color pairs
  init_pair(1, COLOR_RED, COLOR_BLACK); 
  init_pair(2, COLOR_GREEN, COLOR_BLACK); 
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  init_pair(7, COLOR_WHITE, COLOR_BLACK);
  attron(A_BOLD);
  attron(A_STANDOUT);

  // Wave attributes
  ushort color_index = 0;
  short phase_shift = 0;
  double amplitude = (LINES/2)*DEFAULT_AMPLITUDE; 
  double period = DEFAULT_PERIOD;

  while (1) {

    erase();

    color_index = printwave(amplitude, period, phase_shift, color_index);

    attron(COLOR_PAIR(7));
    mvprintw(0, 0, "PERIOD %.2f", period);
    mvprintw(1, 0, "AMP: %.2f", amplitude);
    mvprintw(2, 0, "PHASE: %d", phase_shift);

    refresh();

    switch (getch()) {
      case KEY_UP: amplitude += 1.0; break;
      case KEY_DOWN: amplitude -= 1.0; break;
      case KEY_LEFT: period -= .05; break;
      case KEY_RIGHT: period += .05; break;
      case (int)'q':
        endwin();
        return 0;
      default: break;
    }

    usleep(DELAY);

    phase_shift += 1; 

    if (phase_shift*period >= LINES)
      phase_shift = 0;

  }

  endwin();
  return 0; 
}
