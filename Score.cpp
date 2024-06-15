#include "Score.h"

YJHScore::YJHScore(int body, int growth, int poison, int gate) {
    yjh_mbl = body;
    yjh_gt = growth;
    yjh_pt = poison;
    yjh_gatet = gate;
}

void YJHScore::YJHScoreBoard(int body, int max_body, int growth, int poison, int gate, int elapsed_time, int big_growth) {
    init_pair(3, COLOR_BLACK, COLOR_WHITE); // 색상 쌍 설정
    wmove(yjh_scorewin, 0, 0); // 커서 위치 설정
    wattron(yjh_scorewin, COLOR_PAIR(3)); // 색상 적용
    wborder(yjh_scorewin, '|','|','-','-','*','*','*','*'); // 테두리 설정
    wbkgd(yjh_scorewin, COLOR_PAIR(3)); // 배경 색상 설정
    mvwprintw(yjh_scorewin, 2, 3, " 스코어 보드");
    mvwprintw(yjh_scorewin, 3, 3, " B : %d / %d ", body, max_body);
    mvwprintw(yjh_scorewin, 4, 3, " + : %d", growth );
    mvwprintw(yjh_scorewin, 5, 3, " - : %d", poison );
    mvwprintw(yjh_scorewin, 6, 3, " O : %d ", gate);
    mvwprintw(yjh_scorewin, 7, 3, " BB : %d ", big_growth); // Big Growth 아이템 표시 - 한 번 먹으면 5개 늘어나는 거임
    mvwprintw(yjh_scorewin, 8, 3, " Time : %d s", elapsed_time);
    wrefresh(yjh_scorewin); // 창 갱신
}

bool YJHScore::YJHMissionBoard(int max_body, int growth, int poison, int gate) {
    bool yjh_allclear = true;
    init_pair(3, COLOR_BLACK, COLOR_WHITE); // 색상 쌍 설정
    wmove(yjh_missionwin, 0, 0); // 커서 위치 설정
    wattron(yjh_missionwin, COLOR_PAIR(3)); // 색상 적용
    wborder(yjh_missionwin, '|','|','-','-','*','*','*','*'); // 테두리 설정
    wbkgd(yjh_missionwin, COLOR_PAIR(3)); // 배경 색상 설정
    mvwprintw(yjh_missionwin, 2, 3, " 미션");

    if(max_body >= yjh_mbl)
        mvwprintw(yjh_missionwin, 3, 3, " B : %d  (Clear!)", yjh_mbl );
    else {
        mvwprintw(yjh_missionwin, 3, 3, " B : %d ( X )", yjh_mbl );
        yjh_allclear = false;
    }

    if(growth >= yjh_gt)
        mvwprintw(yjh_missionwin, 4, 3, " + : %d  (Clear!)", yjh_gt );
    else {
        mvwprintw(yjh_missionwin, 4, 3, " + : %d ( X )", yjh_gt );
        yjh_allclear = false;
    }

    if(poison >= yjh_pt)
        mvwprintw(yjh_missionwin, 5, 3, " - : %d  (Clear!)", yjh_pt );
    else {
        mvwprintw(yjh_missionwin, 5, 3, " - : %d ( X )", yjh_pt );
        yjh_allclear = false;
    }

    if(gate >= yjh_gatet)
        mvwprintw(yjh_missionwin, 6, 3, " O : %d  (Clear!)", yjh_gatet );
    else {
        mvwprintw(yjh_missionwin, 6, 3, " O : %d ( X )", yjh_gatet );
        yjh_allclear = false;
    }

    wrefresh(yjh_missionwin); // 창 갱신
    return yjh_allclear;
}

void YJHScore::YJHEndGame() {
    delwin(yjh_scorewin); // score 창 삭제
    delwin(yjh_missionwin); // mis 창 삭제
}
