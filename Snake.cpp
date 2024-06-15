#include "Snake.h"

Snake::Snake(int x, int y, int height, int width)
    : set_x(x), set_y(y), h(height), w(width), direction(2), max_len(min_len) {
    body.push_back({ x, y });
    for (int i = 1; i < min_len; i++) {
        body.push_back({ x, y - i });
    }
    for (int i = 0; i < 3; i++) {
        last_item_time[i] = 0; // Initialize last item generation times
    }
    gate_time = 0; // Initialize gate time
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
        if (body.size() > max_len) max_len = body.size(); // Update max length

        time_t current_time = time(NULL);
        
        // Check if it's time to generate new items
        for (int i = 0; i < 3; i++) {
            if (difftime(current_time, last_item_time[i]) >= item_spawn_interval[i]) {
                SpawnItem(map_number, i);
                last_item_time[i] = current_time; // Reset item generation timer
            }
        }

        DelItem(map_number); // Check and delete expired items

        // Check if it's time to generate gates
        if (!gates_generated && (difftime(current_time, start_time) >= 30.0 || body.size() > 10)) {
            GenerateGate(map_number);
            gate_time = current_time; // Record gate generation time
            gates_generated = true;
        }

        // Check if it's time to delete gates
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

        usleep(200000); // control speed
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

    // Check collision with wall
    if (map[map_number][y][x] == '1' || map[map_number][y][x] == '9') return true;

    // Check collision with immune wall
    if (map[map_number][y][x] == '2') return false;

    // Check collision with itself
    for (int i = 1; i < body.size(); i++) {
        if (body[i].first == y && body[i].second == x) return true;
    }

    // Check collision with growth item
    if (map[map_number][y][x] == '5') {
        growth++;
        body.push_back({ body.back().first, body.back().second });
    }

    // Check collision with poison item
    if (map[map_number][y][x] == '6') {
        poison++;
        if (body.size() <= 3) return true;
        body.pop_back();
    }

    // Check collision with big growth item
    if (map[map_number][y][x] == '8') {
        for (int i = 0; i < 5; ++i) {
            body.push_back({ body.back().first, body.back().second });
        }
    }

    // Check collision with gate
    if (map[map_number][y][x] == '7') {
        int gate_idx = (gate[0][0] == y && gate[0][1] == x) ? 1 : 0;
        y = gate[gate_idx][0];
        x = gate[gate_idx][1];

        // Determine new direction based on gate exit position
        if (x == 0) direction = 1; // left wall, move right
        else if (x == w - 1) direction = 3; // right wall, move left
        else if (y == 0) direction = 2; // top wall, move down
        else if (y == h - 1) direction = 0; // bottom wall, move up
        else {
            // internal gate, follow the direction
            switch (direction) {
            case 0: // up
                if (map[map_number][y - 1][x] != '1' && map[map_number][y - 1][x] != '2') y -= 1;
                else if (map[map_number][y][x + 1] != '1' && map[map_number][y][x + 1] != '2') x += 1;
                else if (map[map_number][y][x - 1] != '1' && map[map_number][y][x - 1] != '2') x -= 1;
                else y += 1;
                break;
            case 1: // right
                if (map[map_number][y][x + 1] != '1' && map[map_number][y][x + 1] != '2') x += 1;
                else if (map[map_number][y + 1][x] != '1' && map[map_number][y + 1][x] != '2') y += 1;
                else if (map[map_number][y - 1][x] != '1' && map[map_number][y - 1][x] != '2') y -= 1;
                else x -= 1;
                break;
            case 2: // down
                if (map[map_number][y + 1][x] != '1' && map[map_number][y + 1][x] != '2') y += 1;
                else if (map[map_number][y][x + 1] != '1' && map[map_number][y][x + 1] != '2') x += 1;
                else if (map[map_number][y][x - 1] != '1' && map[map_number][y][x - 1] != '2') x -= 1;
                else y -= 1;
                break;
            case 3: // left
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
    gate_time = time(NULL); // 기록 게이트 생성 시간
}

void Snake::DelGate(int map_number) {
    for (int i = 0; i < 2; i++) {
        map[map_number][gate[i][0]][gate[i][1]] = '1'; // 게이트를 원래 벽으로 변경
    }
}

char Snake::MapChar(int map_num, int y, int x) {
    switch (map[map_num][y][x]) {
        case '9': return '*';  // Border
        case '1': return '#';  // Wall
        case '2': return '+';  // Immune Wall
        case '0': return ' ';  // Empty space
        case '5': return 'G';  // Growth item
        case '6': return 'P';  // Poison item
        case '7': return 'O';  // Gate
        case '8': return 'B';  // Big growth item
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
