#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
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

std::tuple<Vec2, Vec2, mat2i, std::map<Vec2, std::vector<Vec2>>>
buildCostMap(const mat2c &input) {

  const int turnCost = 1000;
  const int stepCost = 1;

  mat2i costMap(input.size(), std::vector<int>(input[0].size(), INT_MAX));
  std::stack<State> states;

  std::map<Vec2, std::vector<Vec2>> predecessors;

  Vec2 endLoc, startLoc;
  for (int i = 0; i < input.size(); ++i) {
    for (int j = 0; j < input[0].size(); ++j) {
      if (input[i][j] == 'S') {
        states.push({{j, i}, {1, 0}});
        costMap[i][j] = 0;
        startLoc.x = j;
        startLoc.y = i;
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
        predecessors[forwardPos] = {curr.pos};
      } else if (costMap[forwardPos.y][forwardPos.x] ==
                 currLocCost + stepCost) {
        predecessors[forwardPos].push_back(curr.pos);
      }
    }

    if (leftChar == '.' || leftChar == 'E') {
      if (costMap[leftPos.y][leftPos.x] > currLocCost + turnCost + stepCost) {
        costMap[leftPos.y][leftPos.x] = currLocCost + turnCost + stepCost;
        State leftState = {leftPos, leftDelta};
        states.push(leftState);
        predecessors[leftPos] = {curr.pos};
      } else if (costMap[leftPos.y][leftPos.x] ==
                 currLocCost + turnCost + stepCost) {
        predecessors[leftPos].push_back(curr.pos);
      }
    }

    if (rightChar == '.' || rightChar == 'E') {
      if (costMap[rightPos.y][rightPos.x] > currLocCost + turnCost + stepCost) {
        costMap[rightPos.y][rightPos.x] = currLocCost + turnCost + stepCost;
        State rightState = {rightPos, rightDelta};
        states.push(rightState);
        predecessors[rightPos] = {curr.pos};
      } else if (costMap[rightPos.y][rightPos.x] ==
                 currLocCost + turnCost + stepCost) {
        predecessors[rightPos].push_back(curr.pos);
      }
    }
  }

  return {startLoc, endLoc, costMap, predecessors};
}

int sumValidPathLengths(const Vec2 &startLoc, const Vec2 &endLoc,
                        const std::map<Vec2, std::vector<Vec2>> &predecessors) {

  std::set<Vec2> visited;
  std::stack<Vec2> stack;

  stack.push(endLoc);

  while (!stack.empty()) {
    Vec2 curr = stack.top();
    stack.pop();

    visited.insert(curr);

    if (predecessors.find(curr) != predecessors.end()) {
      for (const Vec2 &prev : predecessors.at(curr)) {
        stack.push(prev);
      }
    }
  }

  int sum = 0;
  for (const Vec2 &pos : visited) {
    sum += 1;
  }

  return sum;
}

void solve(const mat2c &input, std::string label) {
  auto [startLoc, endLoc, costMap, predecessors] = buildCostMap(input);
  std::cout << label << " 1: " << costMap[endLoc.y][endLoc.x] << std::endl;

  int pathSquaresCount = sumValidPathLengths(startLoc, endLoc, predecessors);
  std::cout << label << " 2: " << pathSquaresCount << std::endl;
}

int main() {
  mat2c testInput = loadInput("./16_testInput.txt");
  mat2c input = loadInput("./16_input.txt");

  solve(testInput, "Test");
  // solve(input, "Part");
}
