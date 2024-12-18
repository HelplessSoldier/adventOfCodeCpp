#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

struct Vec2 {
  int x, y;

  bool operator<(const Vec2 &other) const {
    if (x < other.x)
      return true;
    if (x > other.x)
      return false;
    return y < other.y;
  }

  bool operator>(const Vec2 &other) const {
    if (x > other.x)
      return true;
    if (x < other.x)
      return false;
    return y > other.y;
  }
};

typedef std::vector<std::vector<int>> mat2i;

struct SearchState {
  Vec2 pos;
  int stepCount;

  bool operator<(const SearchState &other) const {
    if (pos < other.pos)
      return true;
    if (pos > other.pos)
      return false;
    return stepCount < other.stepCount;
  }
};

std::vector<Vec2> loadInput(std::string filepath) {
  std::vector<Vec2> result;
  std::fstream file(filepath);
  std::string line;
  while (getline(file, line)) {
    std::size_t commaPos = line.find(',');
    Vec2 curr;
    curr.x = std::stoi(line.substr(0, commaPos));
    curr.y = std::stoi(line.substr(commaPos + 1));
    result.push_back(curr);
  }
  return result;
}

void visualizeBoard(const std::map<Vec2, int> &corruptionTiming,
                    const Vec2 &pos, int boardWidth, int boardHeight,
                    int step) {
  for (int i = 0; i <= boardHeight; ++i) {
    for (int j = 0; j <= boardWidth; ++j) {
      Vec2 boardloc = {j, i};
      auto corruptionIt = corruptionTiming.find(boardloc);
      if (corruptionIt != corruptionTiming.end() &&
          corruptionIt->second < step) {
        std::cout << '#';
      } else if (pos.x == j && pos.y == i) {
        std::cout << 'X';
      } else {
        std::cout << '.';
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

int part1(const std::vector<Vec2> &input, int boardWidth, int boardHeight,
          int corruptionCount) {

  const std::vector<Vec2> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  std::map<Vec2, int> corruptionTiming;
  for (int i = 0; i < input.size(); ++i) {
    corruptionTiming[input[i]] = i;
  }

  for (auto &pair : corruptionTiming) {
    if (pair.second < corruptionCount) {
      pair.second = -1;
    } else {
      pair.second = INT_MAX;
    }
  }

  SearchState start = {{0, 0}, 0};
  Vec2 endPos = {boardWidth, boardHeight};

  std::queue<SearchState> queue;
  queue.push(start);

  std::set<SearchState> seen;

  while (!queue.empty()) {
    SearchState curr = queue.front();
    queue.pop();

    if (seen.find(curr) != seen.end())
      continue;

    seen.insert(curr);

    for (const Vec2 &dir : directions) {

      Vec2 newPos = {curr.pos.x + dir.x, curr.pos.y + dir.y};
      int newStepCount = curr.stepCount + 1;

      if (newPos.x == boardWidth && newPos.y == boardHeight) {
        return newStepCount;
      }

      if (newPos.x < 0 || newPos.y < 0 || newPos.x > boardWidth ||
          newPos.y > boardHeight) {
        continue;
      }

      auto corruptionIt = corruptionTiming.find(newPos);
      if (corruptionIt != corruptionTiming.end() &&
          corruptionIt->second < newStepCount) {
        continue;
      }

      queue.push({newPos, newStepCount});
    }
  }
  return -1;
}

bool canReachEnd(const mat2i &board) {
  const std::vector<Vec2> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

  Vec2 start = {0, 0};
  int boardWidth = board[0].size() - 1;
  int boardHeight = board.size() - 1;

  std::queue<Vec2> queue;
  queue.push(start);

  std::set<Vec2> seen;

  while (!queue.empty()) {
    Vec2 curr = queue.front();
    queue.pop();

    if (seen.find(curr) != seen.end())
      continue;

    seen.insert(curr);

    for (const Vec2 &dir : directions) {

      Vec2 newPos = {curr.x + dir.x, curr.y + dir.y};

      if (newPos.x < 0 || newPos.y < 0 || newPos.x > boardWidth ||
          newPos.y > boardHeight) {
        continue;
      }

      if (board[newPos.y][newPos.x])
        continue;

      if (newPos.x == boardWidth && newPos.y == boardHeight) {
        return true;
      }

      queue.push(newPos);
    }
  }
  return false;
}

Vec2 part2(std::vector<Vec2> input, int boardWidth, int boardHeight) {
  mat2i board(boardHeight + 1, std::vector<int>(boardWidth + 1, 0));
  for (int i = 0; i < input.size(); ++i) {
    board[input[i].y][input[i].x] = 1;
    if (!canReachEnd(board)) {
      return input[i];
    }
  }
  return {-1, -1};
}

int main() {
  std::vector<Vec2> testInput = loadInput("./18_testInput.txt");
  std::vector<Vec2> input = loadInput("./18_input.txt");

  std::cout << "Test 1: " << part1(testInput, 6, 6, 12) << std::endl;
  std::cout << "Part 1: " << part1(input, 70, 70, 1024) << std::endl;

  Vec2 test2Output = part2(testInput, 6, 6);
  std::cout << "Test 2: " << test2Output.x << "," << test2Output.y << std::endl;

  Vec2 part2Output = part2(input, 70, 70);
  std::cout << "Part 2: " << part2Output.x << "," << part2Output.y << std::endl;
}
