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
    int basic_mis; // 기본 미션
    int growth_mis; // 성장 미션
    int poison_mis; // 독 미션
    int gate_mis; // 게이트 미션
    WINDOW *score = newwin(15, 50, 5, 100); // 점수 창
    WINDOW *mis = newwin(15, 50, 26, 100); // 미션 창
    Score(int b, int g, int p, int d);
    void ScoreBoard(int body, int body_max, int grow, int poison, int gate, int elapsed_time); // 점수판 함수
    bool MissionBoard(int body, int grow, int poison, int gate); // 미션판 함수
    void gameover(); // 게임 오버 함수
};

#endif // SCORE_H
