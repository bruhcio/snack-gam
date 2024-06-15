#include <iostream>
#include "Map.h"

Map::Map() { // 생성자
    sx = 0;
    sy = 0;
    s1_h = 45;
    s1_w = 21;
}

void Map::InitHome() {
    initscr(); // 처음 Main 화면 생성
    keypad(stdscr, TRUE); // 특수키 입력 가능
    curs_set(0); // 커서 안 보이게 설정
    noecho(); // 입력 값 화면에 표시 안 되게 설정
    start_color(); // 색상 모드 시작
    Home(); // 홈 화면 호출
    refresh(); // 화면 갱신
    endwin(); // ncurses 모드 종료
}

void Map::Start(int step) {
    mvprintw(2, 4, "Map %d", step + 1); // 맵 단계 출력
    refresh(); // 화면 갱신
    int now = step;
    int next = now + 1;
    WINDOW *s1 = newwin(s1_h, s1_w, sy, sx); // Map 화면 생성
    Score b(Goal[step][0], Goal[step][1], Goal[step][2], Goal[step][3]);

    init_pair(2, COLOR_BLACK, COLOR_WHITE); // 색상 쌍 설정
    attron(COLOR_PAIR(2)); // 색상 적용
    wbkgd(s1, COLOR_PAIR(2)); // 배경 색상 설정

    keypad(s1, TRUE); // 특수키 입력 가능
    nodelay(s1, TRUE); // 입력을 안 받아도 넘어가게 설정

    Snake s(10, 10, s1_h, s1_w); // 10, 10 위치에 뱀 생성
    WINDOW *msg = newwin(15, 40, 15, 30);
    int k;
    if(s.Game(s1, b, now)) { // 맵 클리어
        if(now == 3) { // 모든 미션 클리어
            Map_msg(msg, 2);
            while(!((k=wgetch(msg)) == 'q')) {
                flushinp();
                switch(k) {
                    case '1':
                        delwin(msg);
                        delwin(s1);
                        clear();
                        Start(0); // 첫 번째 맵으로 재시작
                }
            }
            delwin(msg);
            delwin(s1);
            clear();
            Home(); // 홈 화면으로 돌아가기
        } else {
            Map_msg(msg, 1);
            while(!((k=wgetch(msg)) == 'q')) {
                flushinp();
                switch(k) {
                    case '1':
                        delwin(msg);
                        delwin(s1);
                        clear();
                        Start(next); // 다음 맵으로 이동
                }
            }
            delwin(msg);
            delwin(s1);
            clear();
            Home(); // 홈 화면으로 돌아가기
        }
    } else { // 맵 실패
        Map_msg(msg, 3);
        while(!((k=wgetch(msg)) == 'q')) {
            flushinp();
            switch(k) {
                case '1':
                    delwin(msg);
                    delwin(s1);
                    clear();
                    Start(0); // 첫 번째 맵으로 재시작
            }
        }
        delwin(msg);
        delwin(s1);
        clear();
        Home(); // 홈 화면으로 돌아가기
    }
}

void Map::Map_msg(WINDOW* clear, int condition) {
    init_pair(3, COLOR_BLACK, COLOR_WHITE); // 색상 쌍 설정
    wmove(clear, 0, 0); // 커서 위치 설정
    wattron(clear, COLOR_PAIR(3)); // 색상 적용
    wborder(clear, '|','|','-','-','*','*','*','*'); // 테두리 설정
    wbkgd(clear, COLOR_PAIR(3)); // 배경 색상 설정
    if(condition == 1) {
        mvwprintw(clear, 3, 4, " MISSION CLEAR!");
        mvwprintw(clear, 4, 4, " NEXT Map : 1");
        mvwprintw(clear, 5, 4, " RETURN TO MAIN : 'Q'");
    } else if(condition == 2) {
        mvwprintw(clear, 3, 4, " ALL Clear Mission!");
        mvwprintw(clear, 4, 4, " RESTART GAME : 1");
        mvwprintw(clear, 5, 4, " RETURN TO MAIN : 'Q'");
    } else if (condition == 3) {
        mvwprintw(clear, 3, 4, " MISSION FAIL!");
        mvwprintw(clear, 4, 4, " RESTART GAME : 1");
        mvwprintw(clear, 5, 4, " RETURN TO MAIN : 'Q'");
    }
    wrefresh(clear); // 창 갱신
}

void Map::Home() {
    WINDOW *h = newwin(52, 140, 0, 0); // 메인 화면 생성

    init_pair(4, COLOR_GREEN, COLOR_BLACK); // 색상 쌍 설정
    attron(COLOR_PAIR(4)); // 색상 적용
    wbkgd(h, COLOR_PAIR(4)); // 배경 색상 설정

    keypad(h, TRUE); // 특수키 입력 가능
    nodelay(h, TRUE); // 입력을 안 받아도 넘어가게 설정

    char art[33][139] =
    {
        "                                                                                                                                         ",
        "                                                  Final Project - Snakegame                                                               ",
        "                                                                                                                                          ",
        "                                                  Press 'Enter' : Start the Game                                                          ",
        "                                                  Press 'Q' to End the Game                                                               ",
        "                                                  1. Start : Map - 1                                                                      ",
        "                                                  2. Start : Map - 2                                                                      ",
        "                                                  3. Start : Map - 3                                                                      ",
        "                                                  4. Start : Map - 4                                                                      ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          ",
        "                                                                                                                                          "
    };
    for(int i = 0; i < 33; i++) {
        for(int j = 0; j < 139; j++) {
            mvwaddch(h, i + 3, j + 3, art[i][j]);
        }
    }

    refresh(); // 화면 갱신
    int key;
    flushinp();
    while(!((key = wgetch(h)) == 'q')) {
        flushinp();
        refresh();
        switch(key) { // ENTER 입력 시 Map1 실행
            case 10:
                delwin(h);
                clear();
                Start(0);
                break;
            case '1':
                delwin(h);
                clear();
                Start(0);
                break;
            case '2':
                delwin(h);
                clear();
                Start(1);
                break;
            case '3':
                delwin(h);
                clear();
                Start(2);
                break;
            case '4':
                delwin(h);
                clear();
                Start(3);
                break;
        }
    }
    endwin(); // ncurses 모드 종료
    exit(0); // 프로그램 종료
}
