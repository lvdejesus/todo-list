#include "vector.h"
#include <ctype.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Mode {
  MODE_NORMAL,
  MODE_TYPE,
} Mode;

typedef struct Task {
  char *text;
} Task;

char *mode_text[] = {"NORMAL", "TYPE"};

void save_tasks(Vector *tasks) {
  FILE *fp = fopen("tasks.txt", "w");
  fprintf(fp, "%d\n", (int)tasks->size);
  for (int i = 0; i < tasks->size; i++) {
    Task *task = vector_get(tasks, i);
    fprintf(fp, "%s\n", task->text);
  }
  fclose(fp);
};

Vector *load_tasks() {
  FILE *fp = fopen("tasks.txt", "r");
  Vector *tasks = vector_new(4, sizeof(Task));

  if (fp == NULL) {
    return tasks;
  }

  int num_task;
  Task task;
  char buf[100];

  fscanf(fp, "%d", &num_task);
  for (int i = 0; i < num_task; i++) {
    fscanf(fp, " %[^\n]s", buf);
    char *text = malloc(sizeof(char) * (strlen(buf) + 1));
    strcpy(text, buf);
    task.text = text;
    vector_push(tasks, &task);
  }

  fclose(fp);

  return tasks;
};

int main() {
  initscr();
  cbreak();
  noecho();

  WINDOW *win = newwin(LINES, COLS, 0, 0);
  refresh();

  char ch;
  Mode mode = MODE_NORMAL;

  int line = 1, col = 3, num_task = 0;
  char buf[100];
  int i = 0;

  Vector *tasks = load_tasks();
  Task task;

  wbkgd(win, COLOR_PAIR(1));

  int line_l = 1;
  for (int i = 0; i < tasks->size; i++) {
    mvwprintw(win, line_l, 1, "%d", i + 1);
    Task *task = vector_get(tasks, i);
    int cols = COLS - 4;
    int last_breakable = 0;
    int skippable = 0;
    int start = 0;
    for (int j = 0; j < strlen(task->text); j++) {
      if (task->text[j] == ' ') { // space can be a point to break and can be
                                  // skipped (not like dashes)
        last_breakable = j;
        skippable = 1;
      }

      if (j - start + 1 > cols) {
        // hello world
        // last_break = -1, last_brekable = 5
        mvwaddnstr(win, line_l, 3, &task->text[start],
                   last_breakable - start + (1 - skippable));
        start = last_breakable + 1;
        j = start;
        line_l++;
      }
    }
    mvwaddstr(win, line_l, 3, &task->text[start]);
    line_l++;
  }

  do {
    if (mode == MODE_NORMAL) {
      switch (ch) {
      case 27:
        endwin();

        save_tasks(tasks);
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

        task.text = malloc(sizeof(strlen(buf) + 1));
        strcpy(task.text, buf);

        vector_push(tasks, &task);
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

    box(win, 0, 0);

    mvwprintw(win, 0, 1, "Tasks");
    mvwprintw(win, LINES - 1, 1, "%s", mode_text[mode]);
    // WINDOW *text_win = derwin(win, LINES - 2, COLS - 4, 0, 2);
    for (int i = 0; i < num_task; i++) {
      mvwprintw(win, i + 1, 1, "%d", i + 1);
    }
    // wrefresh(text_win);

    wmove(win, line, col);

    wrefresh(win);
  } while ((ch = getch()));

  return 0;
}
