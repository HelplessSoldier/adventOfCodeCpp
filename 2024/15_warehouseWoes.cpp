#include <cassert>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

struct Vec2 {
  int x, y;
};

struct WarehouseState {
  std::vector<std::vector<char>> board;
  Vec2 roboLoc;
  std::vector<Vec2> roboMoves;
};

WarehouseState loadInput(std::string filepath) {
  WarehouseState result;
  std::fstream file(filepath);
  std::string line;

  // board
  while (true) {
    getline(file, line);

    if (line.empty())
      break;

    std::vector<char> currLine;
    for (const auto &c : line) {
      currLine.push_back(c);
    }
    result.board.push_back(currLine);
  }

  // moves
  while (getline(file, line)) {
    for (const auto &c : line) {
      switch (c) {
      case '^':
        result.roboMoves.push_back({0, -1});
        break;
      case 'v':
        result.roboMoves.push_back({0, 1});
        break;
      case '<':
        result.roboMoves.push_back({-1, 0});
        break;
      case '>':
        result.roboMoves.push_back({1, 0});
        break;
      default:
        assert(false && "Unexpected move char");
      }
    }
  }

  for (int i = 0; i < result.board.size(); ++i) {
    for (int j = 0; j < result.board[0].size(); ++j) {
      if (result.board[i][j] == '@') {
        result.roboLoc = {j, i};
      }
    }
  }

  return result;
}

bool canMakeMove(const WarehouseState &state, int moveIndex) {
  Vec2 curr = state.roboLoc;
  while (state.board[curr.y][curr.x] != '#') {
    curr.x += state.roboMoves[moveIndex].x;
    curr.y += state.roboMoves[moveIndex].y;
    if (state.board[curr.y][curr.x] == '.') {
      return true;
    }
  }
  return false;
}

void makeMove(WarehouseState &state, int moveIndex) {
  Vec2 start = state.roboLoc;
  Vec2 end = start;
  while (true) {
    if (state.board[end.y][end.x] == '.') {
      break;
    }
    end.y += state.roboMoves[moveIndex].y;
    end.x += state.roboMoves[moveIndex].x;
  }

  while (end.y != start.y || end.x != start.x) {
    Vec2 previousSquare = end;
    previousSquare.x -= state.roboMoves[moveIndex].x;
    previousSquare.y -= state.roboMoves[moveIndex].y;
    state.board[end.y][end.x] = state.board[previousSquare.y][previousSquare.x];
    end = previousSquare;
  }

  state.board[state.roboLoc.y][state.roboLoc.x] = '.';

  state.roboLoc.x += state.roboMoves[moveIndex].x;
  state.roboLoc.y += state.roboMoves[moveIndex].y;
}

long boardScore(const WarehouseState &input, bool part1) {
  long result = 0;

  char searchChar = part1 ? 'O' : '[';

  for (int i = 0; i < input.board.size(); ++i) {
    for (int j = 0; j < input.board[0].size(); ++j) {
      if (input.board[i][j] == searchChar) {
        result += 100 * i + j;
      }
    }
  }
  return result;
}

void logBoard(WarehouseState input, int moveIndex) {

  std::cout << std::endl;

  if (input.roboMoves[moveIndex].x == 1 && input.roboMoves[moveIndex].y == 0) {
    std::cout << '>' << std::endl;
  } else if (input.roboMoves[moveIndex].x == -1 &&
             input.roboMoves[moveIndex].y == 0) {
    std::cout << '<' << std::endl;
  } else if (input.roboMoves[moveIndex].x == 0 &&
             input.roboMoves[moveIndex].y == 1) {
    std::cout << 'V' << std::endl;
  } else if (input.roboMoves[moveIndex].x == 0 &&
             input.roboMoves[moveIndex].y == -1) {
    std::cout << '^' << std::endl;
  }

  for (auto line : input.board) {
    for (auto val : line) {
      std::cout << val;
    }
    std::cout << std::endl;
  }
}

long part1(WarehouseState input) {
  for (int i = 0; i < input.roboMoves.size(); ++i) {
    if (canMakeMove(input, i)) {
      makeMove(input, i);
    }
  }
  return boardScore(input, true);
}

WarehouseState loadInputPart2(std::string filepath) {
  WarehouseState result;
  std::fstream file(filepath);
  std::string line;
  while (true) {
    getline(file, line);
    if (line.empty())
      break;

    std::vector<char> curr;
    for (const auto &c : line) {
      switch (c) {
      case '.':
        curr.push_back('.');
        curr.push_back('.');
        break;
      case '#':
        curr.push_back('#');
        curr.push_back('#');
        break;
      case 'O':
        curr.push_back('[');
        curr.push_back(']');
        break;
      case '@':
        curr.push_back('@');
        curr.push_back('.');
        break;
      default:
        assert(false && "Unexpected char in loadInputPart2");
      }
    }
    result.board.push_back(curr);
  }

  while (getline(file, line)) {
    for (const auto &c : line) {
      switch (c) {
      case '^':
        result.roboMoves.push_back({0, -1});
        break;
      case 'v':
        result.roboMoves.push_back({0, 1});
        break;
      case '<':
        result.roboMoves.push_back({-1, 0});
        break;
      case '>':
        result.roboMoves.push_back({1, 0});
        break;
      default:
        assert(false && "Unexpected move char");
      }
    }
  }

  for (int i = 0; i < result.board.size(); ++i) {
    for (int j = 0; j < result.board[0].size(); ++j) {
      if (result.board[i][j] == '@') {
        result.roboLoc = {j, i};
      }
    }
  }

  return result;
}

bool canMakeMoveP2Vertical(const WarehouseState &input, int moveIndex,
                           std::vector<std::pair<Vec2, char>> &boxLocations) {

  Vec2 roboPos = input.roboLoc;
  Vec2 roboDel = input.roboMoves[moveIndex];

  assert(roboDel.x == 0);

  char immediateNextChar =
      input.board[roboPos.y + roboDel.y][roboPos.x + roboDel.x];

  if (immediateNextChar == '.') {
    return true;
  } else if (immediateNextChar == '#') {
    return false;
  }

  std::stack<Vec2> boxIndices;
  assert(immediateNextChar == '[' ||
         immediateNextChar == ']' &&
             "Unecpected char in canMakeMoveP2Vertical");

  boxIndices.push({roboPos.x, roboPos.y});

  while (true) {

    if (boxIndices.empty()) {
      return true;
    }

    Vec2 nextIndex = boxIndices.top();
    boxIndices.pop();
    nextIndex.y += roboDel.y;
    char nextChar = input.board[nextIndex.y][nextIndex.x];

    if (nextChar == ']') {
      boxLocations.push_back({nextIndex, ']'});
      boxLocations.push_back({{nextIndex.x - 1, nextIndex.y}, '['});
      boxIndices.push(nextIndex);
      boxIndices.push({nextIndex.x - 1, nextIndex.y});
    } else if (nextChar == '[') {
      boxLocations.push_back({nextIndex, '['});
      boxLocations.push_back({{nextIndex.x + 1, nextIndex.y}, ']'});
      boxIndices.push(nextIndex);
      boxIndices.push({nextIndex.x + 1, nextIndex.y});
    } else if (nextChar == '#') {
      return false;
    }
  }

  return false;
}

void makeMoveP2Vertical(WarehouseState &input, int moveIndex,
                        std::vector<std::pair<Vec2, char>> boxLocations) {
  int delY = input.roboMoves[moveIndex].y;

  std::vector<std::pair<Vec2, char>> newBoxLocations;

  for (const auto &boxLoc : boxLocations) {
    std::pair<Vec2, char> newBox = boxLoc;
    newBox.first.y += delY;
    newBoxLocations.push_back(newBox);
  }

  for (const auto &boxLoc : boxLocations) {
    input.board[boxLoc.first.y][boxLoc.first.x] = '.';
  }

  for (const auto &boxLoc : newBoxLocations) {
    input.board[boxLoc.first.y][boxLoc.first.x] = boxLoc.second;
  }

  input.board[input.roboLoc.y][input.roboLoc.x] = '.';
  input.roboLoc.y += delY;
  input.board[input.roboLoc.y][input.roboLoc.x] = '@';
}

long part2(WarehouseState input) {

  for (int i = 0; i < input.roboMoves.size(); ++i) {
    logBoard(input, 0);

    int delX = input.roboMoves[i].x;
    int delY = input.roboMoves[i].y;

    if (delY == 0) {
      // nothing changed for the horizontal movements. just use p1
      if (canMakeMove(input, i)) {
        makeMove(input, i);
      }
    } else {
      std::vector<std::pair<Vec2, char>> boxLocations;
      if (canMakeMoveP2Vertical(input, i, boxLocations)) {
        makeMoveP2Vertical(input, i, boxLocations);
      }
    }

    std::cin.get();
  }

  return boardScore(input, false);
}

int main() {
  WarehouseState testInput = loadInput("./15_testInput.txt");
  WarehouseState input = loadInput("./15_input.txt");
  WarehouseState p2TestInput = loadInputPart2("./15_testInput.txt");
  WarehouseState p2input = loadInputPart2("./15_input.txt");

  std::cout << "Test 1: " << part1(testInput) << std::endl;
  std::cout << "Part 1: " << part1(input) << std::endl;
  std::cout << "Test 2: " << part2(p2TestInput) << std::endl;
  std::cout << "Part 2: " << part2(p2input) << std::endl;
}
