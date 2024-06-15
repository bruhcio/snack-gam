#ifndef MAP_H
#define MAP_H

#include <iostream>
#include "Snake.h"

class YJHMap {
public:
    int yjh_sx, yjh_sy; // Map 창 생성 위치
    int yjh_maph, yjh_mapw; // Map의 가로 세로 길이
    int yjh_goals[4][4] = {
        {8, 5, 2, 0}, // 예시: 스테이지 1 클리어 조건
        {12, 6, 3, 1}, // 예시: 스테이지 2 클리어 조건
        {14, 7, 4, 2}, // 예시: 스테이지 3 클리어 조건
        {16, 8, 5, 3}  // 예시: 스테이지 4 클리어 조건
    };

    YJHMap(); // 생성자
    void InitHomeScreen(); // 처음 시작 화면 (윈도우 생성)
    void StartLevel(int level); // 첫 번째 스테이지 실행
    void ShowMessage(WINDOW* window, int condition); // 메시지 창 표시
    void DisplayHomeScreen(); // 홈 화면 표시
};

#endif // MAP_H
