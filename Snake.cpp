#include "Snake.h"

Snake::Snake(int x, int y, int height, int width)
    : set_x(x), set_y(y), h(height), w(width), direction(2), max_len(min_len) {
    body.push_back({ x, y });
    for (int i = 1; i < min_len; i++) {
        body.push_back({ x, y - i });
    }
    for (int i = 0; i < 3; i++) {
        last_item_time[i] = 0; // 아이템 마지막 생성 시간 초기화
    }
    gate_time = 0; // 게이트 시간 초기화
}

bool Snake::Game(WINDOW* s1, Score& b, int map_number) {
    int growth = 0, poison = 0, gate_use = 0;
    time_t start_time = time(NULL);
    bool gates_generated = false;

    while (1) {
        int input = wgetch(s1);
        switch (input) {
        case KEY_UP:
            if (direction != 2) direction = 0;
            break;
        case KEY_DOWN:
            if (direction != 0) direction = 2;
            break;
        case KEY_LEFT:
            if (direction != 1) direction = 3;
            break;
        case KEY_RIGHT:
            if (direction != 3) direction = 1;
            break;
        case 'q':
            return false;
        default:
            break;
        }

        Move(direction);
        if (Collision(growth, poison, map_number)) return false;
        if (body.size() > max_len) max_len = body.size(); // 최대 길이 업데이트

        time_t current_time = time(NULL);

        // 새로운 아이템을 생성할 시간인지 확인
        for (int i = 0; i < 3; i++) {
            if (difftime(current_time, last_item_time[i]) >= item_spawn_interval[i]) {
                SpawnItem(map_number, i);
                last_item_time[i] = current_time; // 아이템 생성 시간 초기화
            }
        }

        DelItem(map_number); // 만료된 아이템 삭제

        // 게이트를 생성할 시간인지 확인
        if (!gates_generated && (difftime(current_time, start_time) >= 30.0 || body.size() > 10)) {
            GenerateGate(map_number);
            gate_time = current_time; // 게이트 생성 시간 기록
            gates_generated = true;
        }

        // 게이트를 삭제할 시간인지 확인
        if (gates_generated && difftime(current_time, gate_time) >= 5.0) {
            DelGate(map_number);
            gates_generated = false;
        }

        Show(s1, map_number);

        int elapsed_time = difftime(current_time, start_time); // 경과 시간 계산
        b.ScoreBoard(body.size(), max_len, growth, poison, gate_use, elapsed_time); // 경과 시간 전달
        if (b.MissionBoard(max_len, growth, poison, gate_use)) {
            return true;
        }

        usleep(200000); // 속도 조절
    }
}

void Snake::Move(int direction) {
    int y = body[0].first;
    int x = body[0].second;

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

    body.insert(body.begin(), { y, x });
    body.pop_back();
}

bool Snake::Collision(int& growth, int& poison, int map_number) {
    int y = body[0].first;
    int x = body[0].second;

    // 벽과 충돌 확인
    if (map[map_number][y][x] == '1' || map[map_number][y][x] == '9') return true;

    // 면역 벽과 충돌 확인
    if (map[map_number][y][x] == '2') return false;

    // 자기 자신과 충돌 확인
    for (int i = 1; i < body.size(); i++) {
        if (body[i].first == y && body[i].second == x) return true;
    }

    // 성장 아이템과 충돌 확인
    if (map[map_number][y][x] == '5') {
        growth++;
        body.push_back({ body.back().first, body.back().second });
    }

    // 독 아이템과 충돌 확인
    if (map[map_number][y][x] == '6') {
        poison++;
        if (body.size() <= 3) return true;
        body.pop_back();
    }

    // 큰 성장 아이템과 충돌 확인
    if (map[map_number][y][x] == '8') {
        for (int i = 0; i < 5; ++i) {
            body.push_back({ body.back().first, body.back().second });
        }
    }

    // 게이트와 충돌 확인
    if (map[map_number][y][x] == '7') {
        int gate_idx = (gate[0][0] == y && gate[0][1] == x) ? 1 : 0;
        y = gate[gate_idx][0];
        x = gate[gate_idx][1];

        // 게이트 출구 위치에 따른 새로운 방향 결정
        if (x == 0) direction = 1; // 왼쪽 벽, 오른쪽으로 이동
        else if (x == w - 1) direction = 3; // 오른쪽 벽, 왼쪽으로 이동
        else if (y == 0) direction = 2; // 위쪽 벽, 아래로 이동
        else if (y == h - 1) direction = 0; // 아래쪽 벽, 위로 이동
        else {
            // 내부 게이트, 방향을 따라 이동
            switch (direction) {
            case 0: // 위
                if (map[map_number][y - 1][x] != '1' && map[map_number][y - 1][x] != '2') y -= 1;
                else if (map[map_number][y][x + 1] != '1' && map[map_number][y][x + 1] != '2') x += 1;
                else if (map[map_number][y][x - 1] != '1' && map[map_number][y][x - 1] != '2') x -= 1;
                else y += 1;
                break;
            case 1: // 오른쪽
                if (map[map_number][y][x + 1] != '1' && map[map_number][y][x + 1] != '2') x += 1;
                else if (map[map_number][y + 1][x] != '1' && map[map_number][y + 1][x] != '2') y += 1;
                else if (map[map_number][y - 1][x] != '1' && map[map_number][y - 1][x] != '2') y -= 1;
                else x -= 1;
                break;
            case 2: // 아래
                if (map[map_number][y + 1][x] != '1' && map[map_number][y + 1][x] != '2') y += 1;
                else if (map[map_number][y][x + 1] != '1' && map[map_number][y][x + 1] != '2') x += 1;
                else if (map[map_number][y][x - 1] != '1' && map[map_number][y][x - 1] != '2') x -= 1;
                else y -= 1;
                break;
            case 3: // 왼쪽
                if (map[map_number][y][x - 1] != '1' && map[map_number][y][x - 1] != '2') x -= 1;
                else if (map[map_number][y + 1][x] != '1' && map[map_number][y + 1][x] != '2') y += 1;
                else if (map[map_number][y - 1][x] != '1' && map[map_number][y - 1][x] != '2') y -= 1;
                else x += 1;
                break;
            }
        }

        body.insert(body.begin(), { y, x });
        body.pop_back();
        return false;
    }

    return false;
}

void Snake::SpawnItem(int map_number, int item_type) {
    srand((unsigned int)time(0));
    int y, x;
    do {
        y = (rand() % (h - 2)) + 1;
        x = (rand() % (w - 2)) + 1;
    } while (!UnableItem(map_number, y, x));
    
    item_pos[item_type][0] = y;
    item_pos[item_type][1] = x;
    item_pos[item_type][3] = item_type;
    item_pos[item_type][2] = time(0);

    map[map_number][y][x] = item_shape[item_type];
}

void Snake::DelItem(int map_number) {
    time_t current_time = time(0);
    for (int i = 0; i < 3; i++) {
        if (difftime(current_time, item_pos[i][2]) > item_duration) {
            map[map_number][item_pos[i][0]][item_pos[i][1]] = '0';
        }
    }
}

bool Snake::UnableItem(int map_number, int p1, int p2) {
    return map[map_number][p1][p2] == '0';
}

void Snake::GenerateGate(int map_number) {
    srand(time(NULL));
    int gate_count = 0;
    while (gate_count < 2) {
        int y = rand() % h;
        int x = rand() % w;
        if (map[map_number][y][x] == '1') {
            map[map_number][y][x] = '7';
            gate[gate_count][0] = y;
            gate[gate_count][1] = x;
            gate_count++;
        }
    }
    gate_time = time(NULL); // 게이트 생성 시간 기록
}

void Snake::DelGate(int map_number) {
    for (int i = 0; i < 2; i++) {
        map[map_number][gate[i][0]][gate[i][1]] = '1'; // 게이트를 원래 벽으로 변경
    }
}

char Snake::MapChar(int map_num, int y, int x) {
    switch (map[map_num][y][x]) {
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

void Snake::Show(WINDOW* s1, int map_num) {
    werase(s1);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            char ch = MapChar(map_num, i, j);
            mvwaddch(s1, i, j, ch);
        }
    }
    // 머리 부분을 'H'로 표시하고, 몸통 부분을 '3'으로 표시
    mvwaddch(s1, body[0].first, body[0].second, 'H');
    for (int i = 1; i < body.size(); i++) {
        mvwaddch(s1, body[i].first, body[i].second, '3');
    }
    wrefresh(s1);
}
