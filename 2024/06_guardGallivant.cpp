#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

typedef std::vector<std::vector<int>> mat2i;

enum Heading { UP, RIGHT, DOWN, LEFT };

struct GuardState {
  int x, y;
  Heading heading;

  // needed to search for GuardStates in a std::set
  bool operator<(const GuardState &other) const {
    if (x != other.x)
      return x < other.x;
    if (y != other.y)
      return y < other.y;
    return heading < other.heading;
  }
};

std::string loadInput(std::string filepath) {
  std::fstream file(filepath);
  std::string line;
  std::string result;
  while (getline(file, line)) {
    result.append(line + '\n');
  }
  return result;
}

mat2i buildBoard(const std::string &input) {
  mat2i result;
  std::stringstream ss(input);
  std::string line;
  while (getline(ss, line)) {
    std::vector<int> currline;
    for (const char &c : line) {
      currline.push_back(c == '#' ? 1 : 0);
    }
    result.push_back(currline);
  }
  return result;
}

GuardState getGuardState(const std::string &input) {
  std::stringstream ss(input);
  std::string line;

  int guard_x = -1;
  int guard_y = 0;
  Heading guardHeading = UP;

  while (getline(ss, line)) {
    auto found = line.find('^');
    if (found != std::string::npos) {
      guard_x = found;
      break;
    }
    guard_y++;
  }

  return {guard_x, guard_y, guardHeading};
}

bool inBounds(const mat2i &board, int x, int y) {
  int height = board.size();
  int width = board[0].size();
  return (x < width && x >= 0 && y < height && y >= 0);
}

void guardTurn(GuardState &guardState, const mat2i &board, bool &running) {

  bool lookingForEmptySpace = true;
  while (lookingForEmptySpace) {

    std::vector<int> search = {0, 0}; // { delta x, delta y }
    Heading turnDirection;

    switch (guardState.heading) {
    case UP:
      turnDirection = RIGHT;
      search[1] = -1;
      break;
    case RIGHT:
      turnDirection = DOWN;
      search[0] = 1;
      break;
    case DOWN:
      turnDirection = LEFT;
      search[1] = 1;
      break;
    case LEFT:
      turnDirection = UP;
      search[0] = -1;
      break;
    }

    int search_x = guardState.x + search[0];
    int search_y = guardState.y + search[1];

    if (!inBounds(board, search_x, search_y)) {
      running = false;
      return;
    }

    if (board[search_y][search_x] == 1) {
      // hit an obstacle, turn right
      guardState.heading = turnDirection;
    } else {
      // can take a step forward
      lookingForEmptySpace = false;
    }
  }
}

void guardStep(GuardState &guardState, const mat2i &board, bool &running) {
  guardTurn(guardState, board, running);

  switch (guardState.heading) {
  case UP:
    guardState.y--;
    break;
  case RIGHT:
    guardState.x++;
    break;
  case DOWN:
    guardState.y++;
    break;
  case LEFT:
    guardState.x--;
    break;
  }
}

mat2i getVisited(const mat2i &board, GuardState guardState) {
  int boardWidth = board[0].size();
  int boardHeight = board.size();
  mat2i result(boardHeight, std::vector<int>(boardWidth, 0));

  bool running = true;
  while (running) {
    result[guardState.y][guardState.x]++;
    guardStep(guardState, board, running);
  }

  return result;
}

int part1(mat2i visitedCount) {
  int result = 0;
  for (const std::vector<int> &line : visitedCount) {
    for (const int &val : line) {
      if (val)
        result++;
    }
  }
  return result;
}

bool infiniteLoop(const mat2i &board, GuardState guardState) {
  // general idea is that if it returns to a prior state, it should keep
  // repeating that loop
  std::set<GuardState> seenStates;
  seenStates.insert(guardState);

  bool running = true;
  while (running) {
    guardStep(guardState, board, running);

    auto foundState = seenStates.find(guardState);
    if (foundState != seenStates.end()) {
      // inf loop found
      return true;
    }

    seenStates.insert(guardState);
  }
  return false;
}

int part2(const mat2i &board, const mat2i &visitedOnNormalPath,
          const GuardState initialState) {

  int result = 0;
  for (int y = 0; y < board.size(); ++y) {
    for (int x = 0; x < board[0].size(); ++x) {

      bool onGuard = (x == initialState.x && y == initialState.y);
      if (onGuard)
        continue;

      if (!visitedOnNormalPath[y][x])
        // no reason to check a location that the guard doesn't reach
        continue;

      mat2i alteredBoard = board;
      alteredBoard[y][x] = 1;

      if (infiniteLoop(alteredBoard, initialState))
        result++;
    }
  }

  return result;
}

int main() {
  std::string input = loadInput("./06_input.txt");

  std::string testInput = R"(....#.....
.........#
..........
..#.......
.......#..
..........
.#..^.....
........#.
#.........
......#...)";

  mat2i testBoard = buildBoard(testInput);
  mat2i board = buildBoard(input);

  GuardState testGuardState = getGuardState(testInput);
  GuardState guardState = getGuardState(input);

  mat2i testVisited = getVisited(testBoard, testGuardState);
  mat2i visited = getVisited(board, guardState);

  std::cout << "Test 1: " << part1(testVisited) << std::endl;
  std::cout << "Part 1: " << part1(visited) << std::endl;
  std::cout << "Test 2: " << part2(testBoard, testVisited, testGuardState)
            << std::endl;
  std::cout << "Part 2: " << part2(board, visited, guardState) << std::endl;
}
