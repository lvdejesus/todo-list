#include <ctype.h>
#include <curses.h>

typedef enum Mode {
  MODE_NORMAL,
  MODE_TYPE,
} Mode;

typedef struct Task {
  char *text;
} Task;

char *mode_text[] = {"NORMAL", "TYPE"};

int main() {
  initscr();
  cbreak();
  noecho();

  WINDOW *win = newwin(LINES, COLS, 0, 0);
  refresh();

  char ch;
  int quit = 0;
  Mode mode = MODE_NORMAL;

  int line = 1, col = 3, num_task = 0;
  char buf[100];
  int i = 0;

  do {
    if (mode == MODE_NORMAL) {
      switch (ch) {
      case 27:
        endwin();
        return 0;
      case 'n':
        mode = MODE_TYPE;

        num_task++;
      }
    } else if (mode == MODE_TYPE) {
      if (ch == 27) {
        mode = MODE_NORMAL;

      } else if (ch == '\n') {
        mode = MODE_NORMAL;

        buf[i] = '\0';
        i = 0;

      } else if ((ch == '\b') || (ch == 127)) {
        col--;

        buf[i] = ch;
        i--;

        mvwaddch(win, line, col, ' ');
      } else if (!iscntrl(ch)) {
        mvwaddch(win, line, col, ch);
        buf[i] = ch;
        i++;
        col++;
      }
    }

    if (quit) {
      break;
    }

    box(win, 0, 0);
    mvwprintw(win, 0, 1, "Tasks");
    mvwprintw(win, LINES - 1, 1, "%s", mode_text[mode]);
    for (int i = 0; i < num_task; i++) {
      mvwprintw(win, i + 1, 1, "%d", i + 1);
    }

    wmove(win, line, col);

    wrefresh(win);
  } while ((ch = getch()));

  return 0;
}
