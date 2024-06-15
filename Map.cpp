#include <iostream>
#include "Map.h"

Map::Map() { // 생성자.
    sx = 0;
    sy = 0;
    s1_h = 45;
    s1_w = 21;
}

void Map::InitHome() {
    initscr(); // 처음 Main 화면 생성.
    keypad(stdscr, TRUE); // 특수키 입력 가능.
    curs_set(0); // cursor 안 보이게.
    noecho(); // 입력 값 출력 안 되게 하기.
    start_color();
    Home();
    refresh();
    endwin();
}

void Map::Start(int step) {
    mvprintw(2, 4, "Map %d", step + 1);
    refresh();
    int now = step;
    int next = now + 1;
    WINDOW *s1 = newwin(s1_h, s1_w, sy, sx); // Map 화면 생성.
    Score b(Goal[step][0], Goal[step][1], Goal[step][2], Goal[step][3]);

    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    attron(COLOR_PAIR(2));
    wbkgd(s1, COLOR_PAIR(2));

    keypad(s1, TRUE);
    nodelay(s1, TRUE); // 입력을 안 받아도 넘어가게 해 주는 함수.

    Snake s(10, 10, s1_h, s1_w); // 10, 10 위치에 뱀 생성.
    WINDOW *msg = newwin(15, 40, 15, 30);
    int k;
    if(s.Game(s1, b, now)){ // Map clear
        if(now == 3){ // mission all clear
            Map_msg(msg, 2);
            while(!((k=wgetch(msg)) == 'q')){
                flushinp();
                switch(k){
                    case '1':
                        delwin(msg);
                        delwin(s1);
                        clear();
                        Start(0);
                }
            }
            delwin(msg);
            delwin(s1);
            clear();
            Home();
        } else {
            Map_msg(msg, 1);
            while(!((k=wgetch(msg)) == 'q')){
                flushinp();
                switch(k){
                    case '1':
                        delwin(msg);
                        delwin(s1);
                        clear();
                        Start(next);
                }
            }
            delwin(msg);
            delwin(s1);
            clear();
            Home();
        }
    } else { // Map fail
        Map_msg(msg, 3);
        while(!((k=wgetch(msg)) == 'q')){
            flushinp();
            switch(k){
                case '1':
                    delwin(msg);
                    delwin(s1);
                    clear();
                    Start(0);
            }
        }
        delwin(msg);
        delwin(s1);
        clear();
        Home();
    }
}

void Map::Map_msg(WINDOW* clear, int condition) {
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    wmove(clear, 0, 0);
    wattron(clear, COLOR_PAIR(3));
    wborder(clear, '|','|','-','-','*','*','*','*');
    wbkgd(clear, COLOR_PAIR(3));
    if(condition == 1){
        mvwprintw(clear, 3, 4, " MISSION CLEAR!");
        mvwprintw(clear, 4, 4, " NEXT Map : 1");
        mvwprintw(clear, 5, 4, " RETURN TO MAIN : 'Q'");
    } else if(condition == 2){
        mvwprintw(clear, 3, 4, " ALL Clear Mission!");
        mvwprintw(clear, 4, 4, " RESTART GAME : 1");
        mvwprintw(clear, 5, 4, " RETURN TO MAIN : 'Q'");
    } else if (condition == 3){
        mvwprintw(clear, 3, 4, " MISSION FAIL!");
        mvwprintw(clear, 4, 4, " RESTART GAME : 1");
        mvwprintw(clear, 5, 4, " RETURN TO MAIN : 'Q'");
    }
    wrefresh(clear);
}

void Map::Home() {
    WINDOW *h = newwin(52, 140, 0, 0); // create main

    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(4));
    wbkgd(h, COLOR_PAIR(4));

    keypad(h, TRUE);
    nodelay(h, TRUE); // 입력을 안 받아도 넘어가게 해 주는 함수.

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
        "                                                                                                                                          ",
        "                                                                                                                                          "
    };
    for(int i=0; i<33; i++){
        for(int j=0;j<139; j++){
            mvwaddch(h, i+3, j+3, art[i][j]);
        }
    }

    refresh();
    int key;
    flushinp();
    while(!((key=wgetch(h)) == 'q')){
        flushinp();
        refresh();
        switch(key){ // ENTER입력 시 Map1 실행
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
    endwin();
    exit(0);
}
