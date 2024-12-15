#include <cassert>
#include <fstream>
#include <iostream>
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

long boardScoreP1(const WarehouseState &input) {
  long result = 0;
  for (int i = 0; i < input.board.size(); ++i) {
    for (int j = 0; j < input.board[0].size(); ++j) {
      if (input.board[i][j] == 'O') {
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
  return boardScoreP1(input);
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

bool canMakeMoveP2Vertical(const WarehouseState &input, int moveIndex) {
  // search all connected boxes until either find wall, or exhaust boxes
  std::cerr << "canMakeMoveP2Vertical not implemented" << std::endl;
  return false;
}

void makeMoveP2Vertical(WarehouseState &input, int moveIndex) {
  // add all connected boxes to some data structure, clone it,
  // move all of one structure by deltaY, delete unmoved structure
  // from the board, then replace the boxes from the moved structure.
  // finally move the robot and its location
  std::cerr << "makeMoveP2Vertical not implemented" << std::endl;
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
      if (canMakeMoveP2Vertical(input, i)) {
        makeMoveP2Vertical(input, i);
      }
    }
  }

  return -1;
}

int main() {
  WarehouseState testInput = loadInput("./15_testInput.txt");
  WarehouseState input = loadInput("./15_input.txt");
  WarehouseState p2TestInput = loadInputPart2("./15_testInput.txt");
  WarehouseState p2input = loadInputPart2("./15_input.txt");

  std::cout << "Test 1: " << part1(testInput) << std::endl;
  std::cout << "Part 1: " << part1(input) << std::endl;
  std::cout << "Test 2: " << part2(p2TestInput) << std::endl;
}
