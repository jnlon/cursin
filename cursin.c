#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define AMPLITUDE 1
#define PERIOD .35
#define DELAY 10000

//Color count
#define CC 5

void printwave(double AMP, double PER, double d, unsigned int *c) {

  for (double x=0.0;x<COLS+d;x+=1.0) {

    //Swap color
    attron(COLOR_PAIR(( (*c += 1) % CC) + 1));

    //Find Y 
    double ratio = (2.0*M_PI)/LINES;
    double y = sin(PER*x*ratio);
    y *= AMP; 
    y += LINES/2;

    //Print dot
    mvprintw((int)y, (int)x-d, ".");
  }
}

int getinput(double *AMP, double *PER, double *d, double CI ) {

  switch (getch()) {
    case KEY_UP:
      *AMP += 1;
      break;
    case KEY_DOWN:
      *AMP -= 1;
      break;
    case KEY_LEFT:
      if (*PER < 0.04)
        break;
      *PER -= .05;
      *d -= CI*(*PER);
      break;
    case KEY_RIGHT:
      *PER += .05;
      *d += CI*(*PER);
      break;
    case (int)'q':
      endwin();
      return 1;
  }
  return 0;
}

int main(int argc, char* argv[]) {

  //Init
  WINDOW* screen = initscr();
  start_color();
  nodelay(screen, 1);
  cbreak();
  noecho();
  curs_set(0);
  keypad(screen, TRUE);

  //Normal color
  init_pair(999, COLOR_WHITE, COLOR_BLACK); 

  //Fruity!
  init_pair(1, 1, COLOR_BLACK); //red
  init_pair(2, 2, COLOR_BLACK); //green
  init_pair(3, 3, COLOR_BLACK); //yellow
  init_pair(4, 4, COLOR_BLACK); //blue
  init_pair(5, 5, COLOR_BLACK); //magenta
  attron(A_BOLD);
  attron(A_STANDOUT);

  //Phase shift
  double d = 0.0;

  //Color swapper
  int c = 0;

  double AMP= (LINES/2)*AMPLITUDE; 
  double PER= PERIOD;
  double CI = ((2.0*M_PI)/LINES)/4;

  while (1) {

    usleep(DELAY);

    printwave(AMP, PER, d, &c);

    int quit = getinput(&AMP, &PER, &d, CI);
    if (quit == 1)
      break;

    //Print info in corner
    attron(COLOR_PAIR(999));
    mvprintw(1, 0, "AMP: %.2f", AMP);
    mvprintw(0, 0, "PERIOD %.2f", PER);

    refresh();
    clear();

    //scroll
    d += 1.0;

    //seamless loop
    if (d > COLS) 
      d -= CI; 
  }

  endwin();
  return 0; 
}
