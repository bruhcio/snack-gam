#include <iostream>
#include "Map.h"

YJHMap::YJHMap() { // 생성자
    yjh_sx = 0;
    yjh_sy = 0;
    yjh_maph = 45;
    yjh_mapw = 21;
}

void YJHMap::InitHomeScreen() {
    initscr(); // 처음 Main 화면 생성
    keypad(stdscr, TRUE); // 특수키 입력 가능
    curs_set(0); // 커서 안 보이게 설정
    noecho(); // 입력 값 화면에 표시 안 되게 설정
    start_color(); // 색상 모드 시작
    DisplayHomeScreen(); // 홈 화면 호출
    refresh(); // 화면 갱신
    endwin(); // ncurses 모드 종료
}

void YJHMap::StartLevel(int level) {
    int yjh_level_h = 21;
    int yjh_level_w = 45;
    mvprintw(2, 4, "Map %d", level + 1);
    refresh();
    int yjh_curr_level = level;
    int yjh_next_level = yjh_curr_level + 1;
    WINDOW *yjh_win = newwin(yjh_level_h, yjh_level_w, yjh_sy, yjh_sx); // Map 화면 생성
    YJHScore yjh_score(yjh_goals[level][0], yjh_goals[level][1], yjh_goals[level][2], yjh_goals[level][3]);

    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    attron(COLOR_PAIR(2));
    wbkgd(yjh_win, COLOR_PAIR(2));

    keypad(yjh_win, TRUE);
    nodelay(yjh_win, TRUE); // 입력을 안 받아도 넘어가게 해 주는 함수

    YJHSnake yjh_snake(10, 10, yjh_level_h, yjh_level_w); // 10, 10 위치에 뱀 생성
    WINDOW *msg_window = newwin(15, 40, 15, 30);
    int key_input;
    if(yjh_snake.Game(yjh_win, yjh_score, yjh_curr_level)) { // Map clear
        if(yjh_curr_level == 3) { // mission all clear
            ShowMessage(msg_window, 2);
            while(!((key_input = wgetch(msg_window)) == 'q')) {
                flushinp();
                switch(key_input) {
                    case '1':
                        delwin(msg_window);
                        delwin(yjh_win);
                        clear();
                        StartLevel(0); // 첫 번째 맵으로 재시작
                }
            }
            delwin(msg_window);
            delwin(yjh_win);
            clear();
            DisplayHomeScreen(); // 홈 화면으로 돌아가기
        } else {
            ShowMessage(msg_window, 1);
            while(!((key_input = wgetch(msg_window)) == 'q')) {
                flushinp();
                switch(key_input) {
                    case '1':
                        delwin(msg_window);
                        delwin(yjh_win);
                        clear();
                        StartLevel(yjh_next_level); // 다음 맵으로 이동
                }
            }
            delwin(msg_window);
            delwin(yjh_win);
            clear();
            DisplayHomeScreen(); // 홈 화면으로 돌아가기
        }
    } else { // Map fail
        ShowMessage(msg_window, 3);
        while(!((key_input = wgetch(msg_window)) == 'q')) {
            flushinp();
            switch(key_input) {
                case '1':
                    delwin(msg_window);
                    delwin(yjh_win);
                    clear();
                    StartLevel(0); // 첫 번째 맵으로 재시작
            }
        }
        delwin(msg_window);
        delwin(yjh_win);
        clear();
        DisplayHomeScreen(); // 홈 화면으로 돌아가기
    }
}

void YJHMap::ShowMessage(WINDOW* window, int condition) {
    init_pair(3, COLOR_BLACK, COLOR_WHITE); // 색상 쌍 설정
    wmove(window, 0, 0); // 커서 위치 설정
    wattron(window, COLOR_PAIR(3)); // 색상 적용
    wborder(window, '|','|','-','-','*','*','*','*'); // 테두리 설정
    wbkgd(window, COLOR_PAIR(3)); // 배경 색상 설정
    if(condition == 1) {
        mvwprintw(window, 3, 4, " MISSION CLEAR!");
        mvwprintw(window, 4, 4, " NEXT Map : 1");
        mvwprintw(window, 5, 4, " RETURN TO MAIN : 'Q'");
    } else if(condition == 2) {
        mvwprintw(window, 3, 4, " ALL Clear Mission!");
        mvwprintw(window, 4, 4, " RESTART GAME : 1");
        mvwprintw(window, 5, 4, " RETURN TO MAIN : 'Q'");
    } else if (condition == 3) {
        mvwprintw(window, 3, 4, " MISSION FAIL!");
        mvwprintw(window, 4, 4, " RESTART GAME : 1");
        mvwprintw(window, 5, 4, " RETURN TO MAIN : 'Q'");
    }
    wrefresh(window); // 창 갱신
}

void YJHMap::DisplayHomeScreen() {
    WINDOW *yjh_homewin = newwin(52, 140, 0, 0); // 메인 화면 생성

    init_pair(4, COLOR_GREEN, COLOR_BLACK); // 색상 쌍 설정
    attron(COLOR_PAIR(4)); // 색상 적용
    wbkgd(yjh_homewin, COLOR_PAIR(4)); // 배경 색상 설정

    keypad(yjh_homewin, TRUE); // 특수키 입력 가능
    nodelay(yjh_homewin, TRUE); // 입력을 안 받아도 넘어가게 설정

    char yjh_home_art[33][139] =
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
        "                                                                                                                                          ",
        "                                                                                                                                          "
    };
    for(int i = 0; i < 33; i++) {
        for(int j = 0; j < 139; j++) {
            mvwaddch(yjh_homewin, i + 3, j + 3, yjh_home_art[i][j]);
        }
    }

    refresh(); // 화면 갱신
    int key;
    flushinp();
    while(!((key = wgetch(yjh_homewin)) == 'q')) {
        flushinp();
        refresh();
        switch(key) { // ENTER 입력 시 Map1 실행
            case 10:
                delwin(yjh_homewin);
                clear();
                StartLevel(0);
                break;
            case '1':
                delwin(yjh_homewin);
                clear();
                StartLevel(0);
                break;
            case '2':
                delwin(yjh_homewin);
                clear();
                StartLevel(1);
                break;
            case '3':
                delwin(yjh_homewin);
                clear();
                StartLevel(2);
                break;
            case '4':
                delwin(yjh_homewin);
                clear();
                StartLevel(3);
                break;
        }
    }
    endwin(); // ncurses 모드 종료
    exit(0); // 프로그램 종료
}
