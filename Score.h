#ifndef SCORE_H
#define SCORE_H

#include <ncurses.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

class Score {
public:
    int basic_mis;
    int growth_mis;
    int poison_mis;
    int gate_mis;
    WINDOW *score = newwin(15, 50, 5, 100);
    WINDOW *mis = newwin(15, 50, 26, 100);
    Score(int b, int g, int p, int d);
    void ScoreBoard(int body, int body_max, int grow, int poison, int gate, int elapsed_time); // 함수 시그니처 수정
    bool MissionBoard(int body, int grow, int poison, int gate);
    void gameover();
};

#endif // SCORE_H
