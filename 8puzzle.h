// 8puzzle.h

#ifndef _8PUZZLE_H
#define _8PUZZLE_H

#include <array>
#include <vector>
#include <iostream>
#include <functional>
#include <stack>
#include <set>

struct Point{
    int x{},y{};
    Point (int,int);
    Point();
};
Point::Point(int a,int b) {
    x = a;
    y = b;
}
Point::Point() = default;

struct Move {
    public:Move(Point, Point, Point);
    Point current, target, middle;
};
Move::Move(Point a,Point b,Point c){
    current = a;
    middle = b;
    target = c;
}
using State = std::array<std::array<int,5>,5>;

using Vertex = State;
using Path = std::vector<Vertex>;

class Graph
{
public:
    typename std::vector<Vertex>::const_iterator cbegin(Vertex v) const;
    typename std::vector<Vertex>::const_iterator cend() const;
private:
    mutable std::vector<Vertex> adjacents;
};

std::ostream &operator<<(std::ostream &os, const Vertex &state);

Vertex doMove(const Vertex &v, const Move &m);
Path dfs(const Graph &graph, const Vertex &start, std::function<bool(const Vertex &vertex)> goalTest);
Path bfs(const Graph &graph, const Vertex &start, std::function<bool(const Vertex &vertex)> goalTest);
std::vector<Move> getMoves(Vertex &v, int x, int y);
int checkPoint(Vertex &v, int x, int y);
bool isDone(Vertex &v);
#endif // _8PUZZLE_H

