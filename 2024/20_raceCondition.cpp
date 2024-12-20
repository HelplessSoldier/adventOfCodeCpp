#include <algorithm>
#include <bitset>
#include <cassert>
#include <climits>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

typedef std::vector<std::vector<int>> Mat2i;

struct Vec2 {
  int x, y;

  bool operator<(const Vec2 &other) const {
    if (x < other.x)
      return true;
    if (x > other.x)
      return false;
    return y < other.y;
  }

  bool operator==(const Vec2 &other) const {
    return x == other.x && y == other.y;
  }
};

struct Vec2Hash {
  size_t operator()(const Vec2 &vec) const {
    return std::hash<int>()(vec.x) ^ (std::hash<int>()(vec.y) << 1);
  }
};

struct Input_t {
  Vec2 start, end;
  Mat2i maze;
};

const std::vector<Vec2> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

Input_t loadInput(std::string filepath) {
  Input_t result;
  std::fstream file(filepath);
  std::string line;
  int rowIndex = 0;
  while (getline(file, line)) {
    std::vector<int> row;
    for (int i = 0; i < line.size(); ++i) {
      switch (line[i]) {
      case '.':
        row.push_back(0);
        break;
      case '#':
        row.push_back(1);
        break;
      case 'S':
        row.push_back(0);
        result.start = {i, rowIndex};
        break;
      case 'E':
        row.push_back(0);
        result.end = {i, rowIndex};
        break;
      default:
        assert(false);
      }
    }
    result.maze.push_back(row);
    rowIndex++;
  }
  return result;
}

bool inBounds(const Mat2i &board, const Vec2 &pos) {
  return pos.x >= 0 && pos.y >= 0 && pos.x < board[0].size() &&
         pos.y < board.size();
}

std::map<Vec2, int> getCheatEndLocations(const Mat2i &maze, const Vec2 &origin,
                                         int cheatLimit) {

  //      origin         delx + dely <= cheatLimit
  //        1
  //  dely  2
  //        3 4 5 6
  //          delx

  std::map<Vec2, int> result;

  for (int delx = -cheatLimit; delx <= cheatLimit; delx++) {
    for (int dely = -cheatLimit; dely <= cheatLimit; dely++) {
      if (std::abs(delx) + std::abs(dely) > cheatLimit)
        continue;

      Vec2 curr = {origin.x + delx, origin.y + dely};

      bool oob = !inBounds(maze, curr);
      if (oob)
        continue;

      bool wall = maze[curr.y][curr.x] == 1;
      bool start = curr.x == origin.x && curr.y == origin.y;
      if (wall || start)
        continue;

      result[curr] = std::abs(delx) + std::abs(dely);
    }
  }

  return result;
}

// input is 141x141 = 19881 locations
struct State {
  Vec2 pos;
  bool cheated;
  int stepCount;
  std::bitset<19881> visited;
};

int vec2ToVisited(const Vec2 &vec, const int &boardWidth) {
  return vec.x + vec.y * boardWidth;
}

std::map<int, int> pathCountsByLength(Input_t input, int cheatLimit,
                                      int &normalPathCount) {

  // stepCount, paths
  std::map<int, int> countedPathsBySteps;

  std::stack<State> stack;
  State first = {
      /*pos=*/input.start,
      /*cheated=*/false,
      /*stepCount=*/0,
  };
  stack.push(first);

  //                start, end
  std::set<std::pair<Vec2, Vec2>> seenCheats;

  while (!stack.empty()) {
    State curr = stack.top();
    stack.pop();

    if (!curr.cheated) {

      auto cheatLocsAndSteps =
          getCheatEndLocations(input.maze, curr.pos, cheatLimit);

      for (const auto &cheat : cheatLocsAndSteps) {

        std::pair<Vec2, Vec2> checkCheat = {curr.pos, cheat.first};
        if (seenCheats.find(checkCheat) != seenCheats.end()) {
          continue;
        }
        seenCheats.insert({curr.pos, cheat.first});

        State cheatState = {/*pos=*/cheat.first,
                            /*cheated=*/true,
                            /*stepCount=*/cheat.second + curr.stepCount,
                            /*visited=*/curr.visited};

        cheatState.visited.set(
            vec2ToVisited(cheatState.pos, input.maze[0].size()));

        bool atEnd =
            cheat.first.x == input.end.x && cheat.first.y == input.end.y;

        if (atEnd) {
          countedPathsBySteps[cheatState.stepCount]++;
          continue;
        }

        if (cheatState.stepCount > normalPathCount)
          continue;

        stack.push(cheatState);
      }
    }

    for (const Vec2 &dir : directions) {
      Vec2 newLoc = {curr.pos.x + dir.x, curr.pos.y + dir.y};

      bool oob = !inBounds(input.maze, newLoc);
      bool wall = input.maze[newLoc.y][newLoc.x] == 1;

      bool seen =
          curr.visited.test(vec2ToVisited(newLoc, input.maze[0].size()));

      if (oob || wall || seen)
        continue;

      State newState = {/*pos=*/newLoc,
                        /*cheated=*/curr.cheated,
                        /*stepCount=*/curr.stepCount + 1,
                        /*visited=*/curr.visited};

      newState.visited.set(vec2ToVisited(newLoc, input.maze[0].size()));

      bool atEnd =
          newState.pos.x == input.end.x && newState.pos.y == input.end.y;

      if (atEnd) {
        if (!newState.cheated) {
          normalPathCount = std::min(normalPathCount, newState.stepCount);
        }
        countedPathsBySteps[newState.stepCount]++;
        continue;
      }

      if (newState.stepCount > normalPathCount)
        continue;

      stack.push(newState);
    }
  }

  return countedPathsBySteps;
}

int solve(Input_t input, int cheatLimit, int minTimeSave) {
  int normalPathCount = INT_MAX;

  // stepCount, paths
  std::map<int, int> counts =
      pathCountsByLength(input, cheatLimit, normalPathCount);

  int result = 0;
  for (auto count : counts) {

    if (count.first >= normalPathCount)
      continue;

    int savedTime = normalPathCount - count.first;

    if (savedTime >= minTimeSave) {
      result += count.second;
    }
  }

  return result;
}

int main() {
  Input_t testInput = loadInput("./20_testInput.txt");
  Input_t input = loadInput("./20_input.txt");

  std::cout << "Test 1: " << solve(testInput, 2, 0) << std::endl;
  std::cout << "Part 1: " << solve(input, 2, 100) << std::endl;
  std::cout << "Test 2: " << solve(testInput, 20, 0) << std::endl;
  std::cout << "Part 2: " << solve(input, 20, 100) << std::endl;
}
