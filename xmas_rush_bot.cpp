//C++ CODE TO PLAY CODINGAME XMAS_RUSH

#include <bits/stdc++.h>
using namespace std;

const int TURN_PUSH = 0, TURN_MOVE = 1;
const int MY_ID = 0, HIS_ID = 1;

// up, right, down, left
const int d_row[4] = {-1, 0, 1, 0};
const int d_col[4] = {0, 1, 0, -1};
const string dir_name[4] = {"UP", "RIGHT", "DOWN", "LEFT"};

bool is_vertical(int dir) {
    return dir == 0 || dir == 2;
}

bool is_horizontal(int dir) {
    return !is_vertical(dir);
}

int inv_dir(int dir) {
    return dir ^ 2;
}

const int N = 7;
string grid[N][N];

void read_grid() {
    for(int row = 0; row < N; ++row) {
        for(int col = 0; col < N; ++col) {
            cin >> grid[row][col];
        }
    }
}

bool inside(int row, int col) {
    return 0 <= row && row < N && 0 <= col && col < N;
}

bool can_go(int row, int col, int dir) {
    assert(0 <= dir && dir < 4);
    int r2 = row + d_row[dir];
    int c2 = col + d_col[dir];
    return grid[row][col][dir] == '1'
        && inside(r2, c2) && grid[r2][c2][inv_dir(dir)] == '1';
}

void read(pair<int, int>& p) {
    cin >> p.first >> p.second;
}

struct Item {
    string name;
    int row, col;
    int player;
    void read() {
        cin >> name >> col >> row >> player;
    }
};

struct Player {
    int cntCards;
    int row, col;

    void read() {
        cin >> cntCards;
        cin >> col >> row;
        string this_cell;
        cin >> this_cell;
    }

    bool can_go(int dir) const {
        return ::can_go(row, col, dir);
    }

    void go(int dir) {
        assert(can_go(dir));
        row += d_row[dir];
        col += d_col[dir];
    }
};

void print_moves(vector<int> dirs) {
    assert(!dirs.empty());
    cout << "MOVE";
    for (int dir : dirs) {
        cout << " " << dir_name[dir];
    }
    cout << endl;
}

void print_move(int dir) {
    print_moves({dir});
}

int distance(pair<int,int> a, pair<int,int> b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

// BFS function previously outside a function, now fixed
int bfs(pair<int, int> a, pair<int, int> b) {
    vector<pair<int,int>> q;
    q.push_back(a);
    vector<vector<bool>> visited(N, vector<bool>(N));
    vector<vector<int>> dist(N, vector<int>(N, 1e9));
    visited[a.first][a.second] = true;
    dist[a.first][a.second] = 0;
    for (int i = 0; i < (int) q.size(); ++i) {
        int row = q[i].first;
        int col = q[i].second;
        for (int dir = 0; dir < 4; ++dir) {
            if (can_go(row, col, dir)) {
                int r2 = row + d_row[dir];
                int c2 = col + d_col[dir];
                if (!visited[r2][c2]) {
                    q.emplace_back(r2,c2);
                    visited[r2][c2] = true;
                    dist[r2][c2] = dist[row][col] + 1;
                }
            }
        }
    }
    return dist[b.first][b.second];
}

void component(pair<int,int> a, pair<int,int> goal) {
    vector<pair<int,int>> q;
    q.push_back(a);
    vector<vector<bool>> visited(N, vector<bool>(N));
    vector<vector<pair<int,int>>> previous_cell(N, vector<pair<int,int>>(N));
    visited[a.first][a.second] = true;
    for(int i = 0; i < (int) q.size(); ++i) {
        int row = q[i].first;
        int col = q[i].second;
        for(int dir = 0; dir < 4; ++dir) {
            if(can_go(row, col, dir)) {
                int r2 = row + d_row[dir];
                int c2 = col + d_col[dir];
                if(!visited[r2][c2]) {
                    previous_cell[r2][c2] = {row, col};
                    visited[r2][c2] = true;
                    q.emplace_back(r2, c2);
                }
            }
        }
    }
    pair<int,int> best = a;
    for(pair<int,int> cell : q) {
        if(distance(cell, goal) < distance(best, goal)) {
            best = cell;
        }
    }
    vector<pair<int,int>> path;
    while(best != a) {
        path.push_back(best);
        best = previous_cell[best.first][best.second];
    }
    path.push_back(a);
    reverse(path.begin(), path.end());
    if((int) path.size() == 1) {
        cout << "PASS" << endl;
        return;
    }

    cout << "MOVE";
    for(int i = 0; i < (int) path.size() - 1; ++i) {
        // we move from path[i] to path[i+1]
        int row = path[i].first;
        int col = path[i].second;
        for(int dir = 0; dir < 4; ++dir) {
            int r2 = row + d_row[dir];
            int c2 = col + d_col[dir];
            if(make_pair(r2, c2) == path[i+1]) {
                cout << " " << dir_name[dir];
            }
        }
    }
    cout << endl;
}

int main() {
    srand(time(0)); // random seed

    // game loop
    while (1) {
        int turnType;
        cin >> turnType; cin.ignore();
        read_grid();
        Player me, he;
        me.read();
        he.read();
        int numItems;
        cin >> numItems; cin.ignore();
        vector<Item> items(numItems);
        for(Item& item : items) {
            item.read();
        }
        int numQuests;
        cin >> numQuests; cin.ignore();
        pair<int,int> goal(-1, -1);
        for (int i = 0; i < numQuests; i++) {
            string questItemName;
            int questPlayerId;
            cin >> questItemName >> questPlayerId; cin.ignore();
            if(questPlayerId == MY_ID) {
                for(Item item : items) {
                    if(item.name == questItemName && item.player == MY_ID) {
                        goal = make_pair(item.row, item.col);
                    }
                }
            }
        }
        // assert(goal != make_pair(-1, -1));
        if(turnType == TURN_MOVE) {
            if(goal.first >= 0) {
                component({me.row, me.col}, goal);
            } else {
                vector<int> moves;
                for(int rep = 0; rep < 20; ++rep) {
                    vector<int> order{0, 1, 2, 3};
                    random_shuffle(order.begin(), order.end());
                    for(int dir : order) {
                        int r2 = me.row + d_row[dir];
                        int c2 = me.col + d_col[dir];
                        if(!me.can_go(dir)) {
                            continue;
                        }
                        if(goal.first < 0 || distance(goal, {r2, c2}) < distance(goal, {me.row, me.col})) {
                            moves.push_back(dir);
                            me.go(dir);
                            break;
                        }
                    }
                }
                if(moves.empty()) {
                    cerr << "can't move at all" << endl;
                    cout << "PASS" << endl;
                } else {
                    print_moves(moves);
                }
            }
        } else {
            if(true) { // if(goal.first < 0 || me.row == goal.first || me.col ==
                int dir = rand() % 4;
                // TODO: sometimes push the goal
                int which = me.row;
                if(is_vertical(dir)) {
                    which = me.col;
                }
                cout << "PUSH " << which << " " << dir_name[dir] << endl;
            } else {
                if(me.row < goal.first) {
                    cout << "PUSH " << me.col << " DOWN" << endl;
                } else if(me.row > goal.first) {
                    cout << "PUSH " << me.col << " UP" << endl;
                } else if(me.col < goal.second) {
                    cout << "PUSH " << me.row << " RIGHT" << endl;
                } else if(me.col > goal.second) {
                    cout << "PUSH " << me.row << " LEFT" << endl;
                } else {
                    assert(false);
                }
            }
        }

     
    }
}

// go wrap-around
// if you are stuck, do something random
// make plans and remember history between turns
// you can push your goal outside and then push it next to you to acquire it
// min-max tree
// alpha-beta pruning
