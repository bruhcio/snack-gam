#include "Snake.h"

YJHSnake::YJHSnake(int x, int y, int height, int width)
    : yjh_x(x), yjh_y(y), yjh_height(height), yjh_width(width), yjh_dir(2), yjh_maxlen(yjh_minlen) {
    yjh_body.push_back({ x, y });
    for (int i = 1; i < yjh_minlen; i++) {
        yjh_body.push_back({ x, y - i });
    }
    for (int i = 0; i < 3; i++) {
        yjh_lastitemtime[i] = 0; // 아이템 마지막 생성 시간 초기화
    }
    yjh_gatetime = 0; // 게이트 시간 초기화
}

bool YJHSnake::Game(WINDOW* window, YJHScore& score, int map_number) {
    int growth_count = 0, poison_count = 0, big_growth_count = 0;
    int gate_uses = 0; // 게이트 통과 횟수 변수 선언 및 초기화
    time_t start_time = time(NULL);
    bool gates_generated = false;

    while (1) {
        int input = wgetch(window);
        switch (input) {
        case KEY_UP:
            if (yjh_dir != 2) yjh_dir = 0;
            break;
        case KEY_DOWN:
            if (yjh_dir != 0) yjh_dir = 2;
            break;
        case KEY_LEFT:
            if (yjh_dir != 1) yjh_dir = 3;
            break;
        case KEY_RIGHT:
            if (yjh_dir != 3) yjh_dir = 1;
            break;
        case 'q':
            return false;
        default:
            break;
        }

        Move(yjh_dir);
        if (CheckCollision(growth_count, poison_count, map_number, gate_uses, big_growth_count)) return false; // 게이트 통과 횟수를 참조로 전달
        if (yjh_body.size() > yjh_maxlen) yjh_maxlen = yjh_body.size(); // 최대 길이 업데이트

        time_t current_time = time(NULL);

        // 새로운 아이템을 생성할 시간인지 확인
        for (int i = 0; i < 3; i++) {
            if (difftime(current_time, yjh_lastitemtime[i]) >= yjh_itemspawnint[i]) {
                SpawnItem(map_number, i);
                yjh_lastitemtime[i] = current_time; // 아이템 생성 시간 초기화
            }
        }

        RemoveItem(map_number); // 만료된 아이템 삭제

        // 게이트를 생성할 시간인지 확인
        if (!gates_generated && (difftime(current_time, start_time) >= 10.0 || yjh_body.size() >= 10)) {
            CreateGate(map_number);
            yjh_gatetime = current_time; // 게이트 생성 시간 기록
            gates_generated = true;
        }

        // 게이트를 삭제할 시간인지 확인
        if (gates_generated && difftime(current_time, yjh_gatetime) >= 5.0) {
            RemoveGate(map_number);
            gates_generated = false;
        }

        Display(window, map_number);

        int elapsed_time = difftime(current_time, start_time); // 경과 시간 계산
        score.YJHScoreBoard(yjh_body.size(), yjh_maxlen, growth_count, poison_count, gate_uses, elapsed_time, big_growth_count); // 경과 시간과 함께 업데이트
        if (score.YJHMissionBoard(yjh_maxlen, growth_count, poison_count, gate_uses)) {
            return true;
        }

        usleep(200000); // 속도 조절
    }
}

void YJHSnake::Move(int direction) {
    int y = yjh_body[0].first;
    int x = yjh_body[0].second;

    switch (direction) {
    case 0:
        y -= 1;
        break;
    case 1:
        x += 1;
        break;
    case 2:
        y += 1;
        break;
    case 3:
        x -= 1;
        break;
    }

    yjh_body.insert(yjh_body.begin(), { y, x });
    yjh_body.pop_back();
}

bool YJHSnake::CheckCollision(int& growth, int& poison, int map_number, int& gate_uses, int& big_growth) {
    int y = yjh_body[0].first;
    int x = yjh_body[0].second;

    // 벽과 충돌 확인
    if (yjh_map[map_number][y][x] == '1' || yjh_map[map_number][y][x] == '9') return true;

    // 면역 벽과 충돌 확인
    if (yjh_map[map_number][y][x] == '2') return false;

    // 자기 자신과 충돌 확인
    for (int i = 1; i < yjh_body.size(); i++) {
        if (yjh_body[i].first == y && yjh_body[i].second == x) return true;
    }

    // 성장 아이템과 충돌 확인
    if (yjh_map[map_number][y][x] == '5') {
        growth++;
        yjh_body.push_back({ yjh_body.back().first, yjh_body.back().second });
    }

    // 독 아이템과 충돌 확인
    if (yjh_map[map_number][y][x] == '6') {
        poison++;
        if (yjh_body.size() <= 3) return true;
        yjh_body.pop_back();
    }

    // 큰 성장 아이템과 충돌 확인
    if (yjh_map[map_number][y][x] == '8') {
        big_growth++; // 큰 성장 아이템 수 증가
        growth++; // 성장 아이템 수 증가
        for (int i = 0; i < 5; ++i) {
            yjh_body.push_back({ yjh_body.back().first, yjh_body.back().second });
        }
    }

    // 게이트와 충돌 확인
    if (yjh_map[map_number][y][x] == '7') {
        int gate_idx = (yjh_gatepos[0][0] == y && yjh_gatepos[0][1] == x) ? 1 : 0;
        y = yjh_gatepos[gate_idx][0];
        x = yjh_gatepos[gate_idx][1];

        // 게이트 통과 횟수 증가
        gate_uses++;

        // 게이트 통과 후 새로운 방향 결정
        if (x == 0) yjh_dir = 1; // 왼쪽 벽, 오른쪽으로 이동
        else if (x == yjh_width - 1) yjh_dir = 3; // 오른쪽 벽, 왼쪽으로 이동
        else if (y == 0) yjh_dir = 2; // 위쪽 벽, 아래로 이동
        else if (y == yjh_height - 1) yjh_dir = 0; // 아래쪽 벽, 위로 이동
        else {
            // 내부 게이트, 방향을 따라 이동
            switch (yjh_dir) {
            case 0: // 위
                if (yjh_map[map_number][y - 1][x] != '1' && yjh_map[map_number][y - 1][x] != '2') y -= 1;
                else if (yjh_map[map_number][y][x + 1] != '1' && yjh_map[map_number][y][x + 1] != '2') x += 1;
                else if (yjh_map[map_number][y][x - 1] != '1' && yjh_map[map_number][y][x - 1] != '2') x -= 1;
                else y += 1;
                break;
            case 1: // 오른쪽
                if (yjh_map[map_number][y][x + 1] != '1' && yjh_map[map_number][y][x + 1] != '2') x += 1;
                else if (yjh_map[map_number][y + 1][x] != '1' && yjh_map[map_number][y + 1][x] != '2') y += 1;
                else if (yjh_map[map_number][y - 1][x] != '1' && yjh_map[map_number][y - 1][x] != '2') y -= 1;
                else x -= 1;
                break;
            case 2: // 아래
                if (yjh_map[map_number][y + 1][x] != '1' && yjh_map[map_number][y + 1][x] != '2') y += 1;
                else if (yjh_map[map_number][y][x + 1] != '1' && yjh_map[map_number][y + 1][x + 1] != '2') x += 1;
                else if (yjh_map[map_number][y][x - 1] != '1' && yjh_map[map_number][y + 1][x - 1] != '2') x -= 1;
                else y -= 1;
                break;
            case 3: // 왼쪽
                if (yjh_map[map_number][y][x - 1] != '1' && yjh_map[map_number][y][x - 1] != '2') x -= 1;
                else if (yjh_map[map_number][y + 1][x] != '1' && yjh_map[map_number][y + 1][x] != '2') y += 1;
                else if (yjh_map[map_number][y - 1][x] != '1' && yjh_map[map_number][y - 1][x] != '2') y -= 1;
                else x += 1;
                break;
            }
        }

        yjh_body.insert(yjh_body.begin(), { y, x });
        yjh_body.pop_back();
        return false;
    }

    return false;
}

void YJHSnake::SpawnItem(int map_number, int item_type) {
    srand((unsigned int)time(0));
    int y, x;
    do {
        y = (rand() % (yjh_height - 2)) + 1;
        x = (rand() % (yjh_width - 2)) + 1;
    } while (!IsValidItemPosition(map_number, y, x));
    
    yjh_itempos[item_type][0] = y;
    yjh_itempos[item_type][1] = x;
    yjh_itempos[item_type][3] = item_type;
    yjh_itempos[item_type][2] = time(0);

    yjh_map[map_number][y][x] = yjh_itemshape[item_type];
}

void YJHSnake::RemoveItem(int map_number) {
    time_t current_time = time(0);
    for (int i = 0; i < 3; i++) {
        if (difftime(current_time, yjh_itempos[i][2]) > yjh_itemdur) {
            yjh_map[map_number][yjh_itempos[i][0]][yjh_itempos[i][1]] = '0';
        }
    }
}

bool YJHSnake::IsValidItemPosition(int map_number, int y, int x) {
    return yjh_map[map_number][y][x] == '0';
}

void YJHSnake::CreateGate(int map_number) {
    srand(time(NULL));
    int gate_count = 0;
    while (gate_count < 2) {
        int y = rand() % yjh_height;
        int x = rand() % yjh_width;
        if (yjh_map[map_number][y][x] == '1') {
            yjh_map[map_number][y][x] = '7';
            yjh_gatepos[gate_count][0] = y;
            yjh_gatepos[gate_count][1] = x;
            gate_count++;
        }
    }
    yjh_gatetime = time(NULL); // 게이트 생성 시간 기록
}

void YJHSnake::RemoveGate(int map_number) {
    for (int i = 0; i < 2; i++) {
        yjh_map[map_number][yjh_gatepos[i][0]][yjh_gatepos[i][1]] = '1'; // 게이트를 원래 벽으로 변경
    }
}

char YJHSnake::GetMapChar(int map_number, int y, int x) {
    switch (yjh_map[map_number][y][x]) {
        case '9': return '*';  // 경계
        case '1': return '#';  // 벽
        case '2': return '+';  // 면역 벽
        case '0': return ' ';  // 빈 공간
        case '5': return 'G';  // 성장 아이템
        case '6': return 'P';  // 독 아이템
        case '7': return 'O';  // 게이트
        case '8': return 'B';  // 큰 성장 아이템
        default: return ' ';
    }
}

void YJHSnake::Display(WINDOW* window, int map_number) {
    werase(window);
    for (int i = 0; i < yjh_height; i++) {
        for (int j = 0; j < yjh_width; j++) {
            char ch = GetMapChar(map_number, i, j);
            mvwaddch(window, i, j, ch);
        }
    }
    // 머리 부분을 'H'로 표시하고, 몸통 부분을 '3'으로 표시
    mvwaddch(window, yjh_body[0].first, yjh_body[0].second, 'H');
    for (int i = 1; i < yjh_body.size(); i++) {
        mvwaddch(window, yjh_body[i].first, yjh_body[i].second, '3');
    }
    wrefresh(window);
}
