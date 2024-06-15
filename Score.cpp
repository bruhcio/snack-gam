#include "Score.h"

Score::Score(int b, int g, int p, int d) {
    basic_mis = b;
    growth_mis = g;
    poison_mis = p;
    gate_mis = d;
}

void Score::ScoreBoard(int body, int max_body, int grow, int poison, int gate, int elapsed_time) {
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    wmove(score, 0, 0);
    wattron(score, COLOR_PAIR(3));
    wborder(score, '|','|','-','-','*','*','*','*');
    wbkgd(score, COLOR_PAIR(3));
    mvwprintw(score, 2, 3, " SCORE BOARD");
    mvwprintw(score, 3, 3, " B : %d / %d ", body, max_body);
    mvwprintw(score, 4, 3, " + : %d", grow );
    mvwprintw(score, 5, 3, " - : %d", poison );
    mvwprintw(score, 6, 3, " O : %d ", gate);
    mvwprintw(score, 7, 3, " Time : %d s", elapsed_time);
    wrefresh(score);
}

bool Score::MissionBoard(int max_body, int grow, int poison, int gate) {
    bool clear = true;
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    wmove(mis, 0, 0);
    wattron(mis, COLOR_PAIR(3));
    wborder(mis, '|','|','-','-','*','*','*','*');
    wbkgd(mis, COLOR_PAIR(3));
    mvwprintw(mis, 2, 3, " MISSION");

    if(max_body >= basic_mis)
        mvwprintw(mis, 3, 3, " B : %d  (Clear!)", basic_mis );
    else {
        mvwprintw(mis, 3, 3, " B : %d ( X )", basic_mis );
        clear = false;
    }

    if(grow >= growth_mis)
        mvwprintw(mis, 4, 3, " + : %d  (Clear!)", growth_mis );
    else {
        mvwprintw(mis, 4, 3, " + : %d ( X )", growth_mis );
        clear = false;
    }

    if(poison >= poison_mis)
        mvwprintw(mis, 5, 3, " - : %d  (Clear!)", poison_mis );
    else {
        mvwprintw(mis, 5, 3, " - : %d ( X )", poison_mis );
        clear = false;
    }

    if(gate >= gate_mis)
        mvwprintw(mis, 6, 3, " O : %d  (Clear!)", gate_mis );
    else {
        mvwprintw(mis, 6, 3, " O : %d ( X )", gate_mis );
        clear = false;
    }

    wrefresh(mis);
    return clear;
}

void Score::gameover() {
    delwin(score);
    delwin(mis);
}
