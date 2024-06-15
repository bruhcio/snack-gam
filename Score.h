#ifndef SCORE_H
#define SCORE_H

#include <ncurses.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

class YJHScore {
public:
    int yjh_mbl; // 기본 미션: 최소 몸길이
    int yjh_gt; // 성장 미션: 목표 성장 아이템 수
    int yjh_pt; // 독 미션: 목표 독 아이템 수
    int yjh_gatet; // 게이트 미션: 목표 게이트 통과 수
    WINDOW *yjh_scorewin = newwin(15, 50, 5, 100); // 점수 창
    WINDOW *yjh_missionwin = newwin(15, 50, 26, 100); // 미션 창

    YJHScore(int body, int growth, int poison, int gate);
    void YJHScoreBoard(int body, int max_body, int growth, int poison, int gate, int elapsed_time, int big_growth); // 점수판 함수
    bool YJHMissionBoard(int body, int growth, int poison, int gate); // 미션판 함수
    void YJHEndGame(); // 게임 오버 함수
};

#endif // SCORE_H
