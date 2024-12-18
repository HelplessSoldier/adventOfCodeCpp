#include <cassert>
#include <climits>
#include <fstream>
#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <vector>

enum MazeTypes { EMPTY, WALL, START, END };

typedef std::vector<std::vector<std::vector<MazeTypes>>> Maze;

struct Vec2 {
  int x, y;

  bool operator<(const Vec2 &other) const {
    if (x < other.x)
      return true;
    if (x > other.x)
      return false;
    if (y < other.y)
      return true;
    return false;
  }
};

const std::vector<Vec2> directions2 = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

struct Vec3 {
  int x, y, z;
};

typedef std::vector<std::vector<std::vector<int>>> Mat3i;

const std::vector<Vec3> directions3 = {{1, 0, 0},  {-1, 0, 0}, {0, 1, 0},
                                       {0, -1, 0}, {0, 0, 1},  {0, 0, -1}};

struct InputType {
  Maze maze;
  Vec3 start;
  Vec2 end;
};

#define JUNCTIONLAYERCOUNT 999

void logMazeAt(const Maze &maze, int index) {
  for (auto row : maze[index]) {
    for (auto val : row) {
      if (val == WALL) {
        std::cout << '#';
      } else {
        std::cout << ' ';
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

InputType loadInput(std::string filepath) {
  InputType result;
  std::vector<std::vector<MazeTypes>> original;
  std::fstream file(filepath);
  std::string line;

  while (getline(file, line)) {
    std::vector<MazeTypes> oLine;
    for (const auto &c : line) {
      switch (c) {
      case '#':
        oLine.push_back(WALL);
        break;
      case 'S':
        oLine.push_back(START);
        break;
      case 'E':
        oLine.push_back(END);
        break;
      case '.':
        oLine.push_back(EMPTY);
        break;
      }
    }
    original.push_back(oLine);
  }

  for (int i = 0; i < original.size(); ++i) {
    for (int j = 0; j < original[0].size(); ++j) {
      if (original[i][j] == START) {
        result.start = {j, i, 0}; // start on the horizontal layer
      }
      if (original[i][j] == END) {
        result.end = {j, i};
      }
    }
  }

  std::vector<std::vector<MazeTypes>> junctionLayer(
      original.size(), std::vector<MazeTypes>(original[0].size(), WALL));

  for (int i = 1; i < original.size() - 1; ++i) {
    for (int j = 1; j < original[0].size() - 1; ++j) {
      if (original[i][j] == WALL)
        continue;

      int neighborCount = 0;
      for (const auto &dir : directions2) {
        auto check = original[i + dir.y][j + dir.x];
        if (check == EMPTY || check == START || check == END) {
          neighborCount++;
        }
      }
      if (neighborCount == 1) {
        junctionLayer[i][j] = EMPTY;
        continue;
      }

      for (int d = 0; d < directions2.size(); ++d) {
        Vec2 dir1 = directions2[d];
        Vec2 dir2 = directions2[(d + 1) % directions2.size()];

        if (original[i + dir1.y][j + dir1.x] != WALL &&
            original[i + dir2.y][j + dir2.x] != WALL) {
          junctionLayer[i][j] = EMPTY;
        }
      }
    }
  }

  std::vector<std::vector<MazeTypes>> verticalLayer(
      original.size(), std::vector<MazeTypes>(original[0].size(), WALL));

  std::vector<std::vector<MazeTypes>> horizontalLayer(
      original.size(), std::vector<MazeTypes>(original[0].size(), WALL));

  for (int i = 1; i < original.size() - 1; ++i) {
    for (int j = 1; j < original[0].size() - 1; ++j) {
      if (original[i][j] == WALL)
        continue;

      if (junctionLayer[i][j] == EMPTY) {
        verticalLayer[i][j] = EMPTY;
        horizontalLayer[i][j] = EMPTY;
      }
      if (original[i - 1][j] == EMPTY || original[i + 1][j] == EMPTY) {
        verticalLayer[i][j] = EMPTY;
      }
      if (original[i][j - 1] == EMPTY || original[i][j + 1] == EMPTY) {
        horizontalLayer[i][j] = EMPTY;
      }
    }
  }

  result.maze.push_back(horizontalLayer);
  for (int i = 0; i < JUNCTIONLAYERCOUNT; ++i) {
    result.maze.push_back(junctionLayer);
  }
  result.maze.push_back(verticalLayer);

  return result;
}

bool inBounds(const InputType &input, const Vec3 &loc) {
  return loc.z >= 0 && loc.z < input.maze.size() && loc.y >= 0 &&
         loc.y < input.maze[0].size() && loc.x >= 0 &&
         loc.x < input.maze[0][0].size();
}

Mat3i buildCostMap(const InputType &input) {

  Mat3i costMap(input.maze.size(),
                std::vector<std::vector<int>>(
                    input.maze[0].size(),
                    std::vector<int>(input.maze[0][0].size(), INT_MAX)));

  Vec3 first = input.start;
  std::stack<Vec3> stack;
  stack.push(first);

  costMap[first.z][first.y][first.x] = 0;

  while (!stack.empty()) {
    Vec3 curr = stack.top();
    stack.pop();

    for (const auto &dir : directions3) {

      Vec3 nextPos = {dir.x + curr.x, dir.y + curr.y, dir.z + curr.z};

      if (!inBounds(input, nextPos))
        continue;

      MazeTypes nextSquare = input.maze[nextPos.z][nextPos.y][nextPos.x];
      int currCost = costMap[curr.z][curr.y][curr.x];
      int nextCost = costMap[nextPos.z][nextPos.y][nextPos.x];

      if (nextSquare == EMPTY && nextCost > (currCost + 1)) {

        if (dir.z != 0 && (curr.z == 1 || curr.z == JUNCTIONLAYERCOUNT - 1)) {
          nextPos.z += dir.z;
          currCost++;
          for (int i = nextPos.z; i < costMap.size() && i >= 0; i += dir.z) {
            nextPos.z = i;
            costMap[nextPos.z][nextPos.y][nextPos.x] = currCost++;
          }
        }

        costMap[nextPos.z][nextPos.y][nextPos.x] = currCost + 1;
        stack.push(nextPos);
      }
    }
  }

  return costMap;
}

std::pair<int, int> minCostAtEnd(const Mat3i &costMap, const InputType &input) {
  int minSeen = INT_MAX;
  int minSeenIndex = -1;

  for (int i = 0; i < costMap.size(); ++i) {
    if (costMap[i][input.end.y][input.end.x] < minSeen) {
      minSeenIndex = i;
      minSeen = costMap[i][input.end.y][input.end.x];
    }
  }
  return {minSeen, minSeenIndex};
}

int countUniqueXY(Mat3i costMap, InputType input, int endIndexZ) {
  Vec3 start = {input.end.x, input.end.y, endIndexZ};
  std::stack<Vec3> stack;
  stack.push(start);

  std::set<Vec2> seenXY;

  while (!stack.empty()) {
    Vec3 curr = stack.top();
    stack.pop();
    int currCost = costMap[curr.z][curr.y][curr.x];

    if (currCost == 0) {
      seenXY.insert({curr.x, curr.y});
      continue;
    }

    for (const Vec3 &dir : directions3) {

      Vec3 newPos = {curr.x + dir.x, curr.y + dir.y, curr.z + dir.z};

      if (!inBounds(input, newPos)) {
        std::cout << "oob: " << newPos.x << " " << newPos.y << " " << newPos.z
                  << std::endl;
        continue;
      }

      std::cout << costMap[newPos.z][newPos.y][newPos.x] << " " << currCost;
      if (costMap[newPos.z][newPos.y][newPos.x] < currCost) {
        std::cout << " adding";
        seenXY.insert({newPos.x, newPos.y});
        stack.push(newPos);
      }
      std::cout << std::endl;
    }
  }

  return seenXY.size();
}

int main() {
  InputType testInput = loadInput("./16_testInput.txt");
  Mat3i testCostMap = buildCostMap(testInput);
  auto [testMinCost, testMinCostIndex] = minCostAtEnd(testCostMap, testInput);
  int uniqueXYPositions =
      countUniqueXY(testCostMap, testInput, testMinCostIndex);

  std::cout << "Test 1: " << testMinCost << std::endl;
  std::cout << "Test 2: " << uniqueXYPositions << std::endl;

  /*
  InputType input = loadInput("./16_input.txt");
  Mat3i costMap = buildCostMap(input);
  std::cout << "Part 2: " << minCostAtEnd(costMap, input) << std::endl;
  */
}
