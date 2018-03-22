// 8puzzle.cpp
// Compile with: g++ 8puzzle.cpp -o 8puzzle

#include "8puzzle.h"

typename std::vector<Vertex>::const_iterator Graph::cbegin(Vertex v) const {
    adjacents.clear();

    for (int y = 0; y <= 4; y++) {
        for (int x = 0; x <= 4; x++) {
            auto moves = getMoves(v, x, y);
            for (auto &move : moves) {
                adjacents.push_back(doMove(v, move));
            }
        }
    }
//    }

    return adjacents.cbegin();
}

typename std::vector<Vertex>::const_iterator Graph::cend() const {
    return adjacents.cend();
}

Vertex doMove(const Vertex &v, const Move &m) {
    Vertex n = v;

    n[m.current.y][m.current.x] = 0;
    n[m.middle.y][m.middle.x] = 0;
    n[m.target.y][m.target.x] = 1;

    return n;
}

std::ostream &operator<<(std::ostream &os, const Vertex &state) {
    std::string spacer = "        ";
    for (int y = 0; y <= 4; y++) {
        std::cout << spacer;
        for (int x = 0; x <= y; x++) {
            if (state[y][x] == 1) {
                std::cout << "O   ";
            } else if (state[y][x] == 0) {
                std::cout << ".   ";
            }
        }
        os << std::endl;
        spacer.erase(0, 2);
    }
    return os;
}

Path dfs(const Graph &graph, const Vertex &start, std::function<bool(const Vertex &vertex)> goalTest) {
    std::stack<Path> queue;
    std::set<Vertex> visited;
    Path path;

    queue.push(path);
    while (!queue.empty()) {
        path = queue.top();
        queue.pop();

        Vertex last;
        if (path.size() == 0) {
            last = start;
        } else {
            last = path.back();
        }
        if (goalTest(last))
            return path; // path is a vector of Vertices

        if (visited.find(last) == visited.end()) {
            visited.insert(last);
            for (auto it = graph.cbegin(last); it != graph.cend(); it++) { // extend path with new Vertex
                Path n = path;
                n.push_back(*it);
                queue.push(n);
            }
        }
    }
    return Path();
}

Path bfs(const Graph &graph, const Vertex &start, std::function<bool(const Vertex &vertex)> goalTest) {
    std::deque<Path> queue;
    std::set<Vertex> visited;
    Path path;

    queue.push_back(path);
    while (!queue.empty()) {
        path = queue.front();
        queue.pop_front();

        Vertex last;
        if (path.size() == 0) {
            last = start;
        } else {
            last = path.back();
        }
        if (goalTest(last))
            return path; // path is a vector of Vertices

        for (auto it = graph.cbegin(last); it != graph.cend(); it++) { // extend path with new Vertex
            if (visited.find(*it) == visited.end()) {
                Path n = path;
                n.push_back(*it);
                queue.push_back(n);
                visited.insert(*it);
            }
        }
    }

    return Path(); // return empty path
}

int main() {
    Graph graph;

    Vertex start = {{
                            {{0, -1, -1, -1, -1}},
                            {{1, 1, -1, -1, -1}},
                            {{1, 1, 1, -1, -1}},
                            {{1, 1, 1, 1, -1}},
                            {{1, 1, 1, 1, 1}},
                    }};


    Path path = bfs(graph, start, [&](Vertex v) { return isDone(v); });


    std::cout << "turn 0" << std::endl;
    std::cout << start << std::endl;
    int turn = 1;
    for (auto it = path.cbegin(); it != path.cend(); it++) {
        std::cout << "turn " << turn << std::endl;
        std::cout << *it << std::endl;
        turn++;
    }

    return 0;
}

std::vector<Move> getMoves(Vertex &v, int x, int y) {
    std::vector<Move> moves;
    if (checkPoint(v, x, y) != 1)
        return moves;

    // left up
    if (checkPoint(v, x - 2, y - 2) == 0 && checkPoint(v, x - 1, y - 1) == 1) {
        moves.emplace_back(Point(x, y), Point(x - 1, y - 1), Point(x - 2, y - 2));
    }
    // right up
    if (checkPoint(v, x, y - 2) == 0 && checkPoint(v, x, y - 1) == 1) {
        moves.emplace_back(Point(x, y), Point(x, y - 1), Point(x, y - 2));
    }
    // left down
    if (checkPoint(v, x, y + 2) == 0 && checkPoint(v, x, y + 1) == 1) {
        moves.emplace_back(Point(x, y), Point(x, y + 1), Point(x, y + 2));
    }
    // right down
    if (checkPoint(v, x + 2, y + 2) == 0 && checkPoint(v, x + 1, y + 1) == 1) {
        moves.emplace_back(Point(x, y), Point(x + 1, y + 1), Point(x + 2, y + 2));
    }
    // left
    if (checkPoint(v, x - 2, y) == 0 && checkPoint(v, x - 1, y) == 1) {
        moves.emplace_back(Point(x, y), Point(x - 1, y), Point(x - 2, y));
    }
    // right
    if (checkPoint(v, x + 2, y) == 0 && checkPoint(v, x + 1, y) == 1) {
        moves.emplace_back(Point(x, y), Point(x + 1, y), Point(x + 2, y));
    }

    return moves;
}

int checkPoint(Vertex &v, int x, int y) {
    if (0 > x || 0 > y || x > 4 || y > 4)
        return -1;
    return v[y][x];
}

bool isDone(Vertex &v) {
    int total = 0;
    for (int y = 0; y <= 4; y++) {
        for (int x = 0; x <= y; x++) {
                  total+=v[y][x];
        }
    }
    return total == 1;
}