#ifndef MAP_H
#define MAP_H

#include <iostream>
#include "Snake.h"

class Map {
public:
    int sx, sy; // Map 창 생성 위치
    int s1_h, s1_w; // Map1의 가로 세로 길이
    int Goal[4][4] = {
        {10, 5, 2, 2}, // 예시: 스테이지 1 클리어 조건
        {12, 6, 3, 3}, // 예시: 스테이지 2 클리어 조건
        {14, 7, 4, 4}, // 예시: 스테이지 3 클리어 조건
        {16, 8, 5, 5}  // 예시: 스테이지 4 클리어 조건
    };

    Map(); // 생성자
    void InitHome(); // 처음 시작 화면 (윈도우 생성)
    void Start(int step); // 첫 번째 스테이지 실행
    void Map_msg(WINDOW* clear, int condition);
    void Home();
};

#endif // MAP_H
