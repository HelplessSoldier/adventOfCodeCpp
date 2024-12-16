#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

typedef std::vector<std::vector<char>> mat2c;
typedef std::vector<std::vector<int>> mat2i;

struct Vec2 {
  int x, y;
  bool operator<(const Vec2 &other) const {
    return (x < other.x) || (x == other.x && y < other.y);
  }
  bool operator==(const Vec2 &other) const {
    return x == other.x && y == other.y;
  }
};

struct State {
  Vec2 pos, vel;
};

mat2c loadInput(std::string filepath) {
  mat2c result;
  std::fstream file(filepath);
  std::string line;
  while (getline(file, line)) {
    std::vector<char> curr;
    for (const char &c : line) {
      curr.push_back(c);
    }
    result.push_back(curr);
  }
  return result;
}

Vec2 matmul(const mat2i &A, const Vec2 &b) {
  Vec2 result;
  result.x = A[0][0] * b.x + A[0][1] * b.y;
  result.y = A[1][0] * b.x + A[1][1] * b.y;
  return result;
}

long part1(const mat2c &input) {
  const int turnCost = 1000;
  const int stepCost = 1;

  mat2i costMap(input.size(), std::vector<int>(input[0].size(), INT_MAX));
  std::stack<State> states;

  Vec2 endLoc;

  for (int i = 0; i < input.size(); ++i) {
    for (int j = 0; j < input[0].size(); ++j) {
      if (input[i][j] == 'S') {
        states.push({{j, i}, {1, 0}});
        costMap[i][j] = 0;
      }
      if (input[i][j] == 'E') {
        endLoc.x = j;
        endLoc.y = i;
      }
    }
  }

  mat2i leftTurnMatrix = {{0, -1}, {1, 0}};
  mat2i rightTurnMatrix = {{0, 1}, {-1, 0}};

  while (!states.empty()) {
    State curr = states.top();
    states.pop();

    int currLocCost = costMap[curr.pos.y][curr.pos.x];

    Vec2 leftDelta = matmul(leftTurnMatrix, curr.vel);
    Vec2 rightDelta = matmul(rightTurnMatrix, curr.vel);

    Vec2 forwardPos = {curr.pos.x + curr.vel.x, curr.pos.y + curr.vel.y};
    Vec2 leftPos = {curr.pos.x + leftDelta.x, curr.pos.y + leftDelta.y};
    Vec2 rightPos = {curr.pos.x + rightDelta.x, curr.pos.y + rightDelta.y};

    char forwardChar = input[forwardPos.y][forwardPos.x];
    char leftChar = input[leftPos.y][leftPos.x];
    char rightChar = input[rightPos.y][rightPos.x];

    if (forwardChar == '.' || forwardChar == 'E') {
      if (costMap[forwardPos.y][forwardPos.x] > currLocCost + stepCost) {
        costMap[forwardPos.y][forwardPos.x] = currLocCost + stepCost;
        State forwardState = {forwardPos, curr.vel};
        states.push(forwardState);
      }
    }

    if (leftChar == '.' || leftChar == 'E') {
      if (costMap[leftPos.y][leftPos.x] > currLocCost + turnCost + stepCost) {
        costMap[leftPos.y][leftPos.x] = currLocCost + turnCost + stepCost;
        State leftState = {leftPos, leftDelta};
        states.push(leftState);
      }
    }

    if (rightChar == '.' || rightChar == 'E') {
      if (costMap[rightPos.y][rightPos.x] > currLocCost + turnCost + stepCost) {
        costMap[rightPos.y][rightPos.x] = currLocCost + turnCost + stepCost;
        State rightState = {rightPos, rightDelta};
        states.push(rightState);
      }
    }
  }

  return costMap[endLoc.y][endLoc.x];
}

int main() {
  mat2c testInput = loadInput("./16_testInput.txt");
  mat2c input = loadInput("./16_input.txt");

  std::cout << "Test 1: " << part1(testInput) << std::endl;
  std::cout << "Part 1: " << part1(input) << std::endl;
}
