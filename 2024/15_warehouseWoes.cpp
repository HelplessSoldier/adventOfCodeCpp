#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Vec2 {
  int x, y;
};

enum BoardSquareTypes { EMPTY, ROBOT, WALL, BOX };

typedef std::vector<std::vector<BoardSquareTypes>> Board;

struct WarehouseState {
  Board board;
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

    std::vector<BoardSquareTypes> currLine;
    for (const auto &c : line) {
      switch (c) {
      case '@':
        currLine.push_back(ROBOT);
        break;
      case '.':
        currLine.push_back(EMPTY);
        break;
      case '#':
        currLine.push_back(WALL);
        break;
      case 'O':
        currLine.push_back(BOX);
        break;
      default:
        assert(false && "Unexpected board char");
      }
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
      if (result.board[i][j] == ROBOT) {
        result.roboLoc = {j, i};
      }
    }
  }

  return result;
}

bool canMakeMove(const WarehouseState &state, int moveIndex) {
  Vec2 curr = state.roboLoc;
  while (state.board[curr.y][curr.x] != WALL) {
    curr.x += state.roboMoves[moveIndex].x;
    curr.y += state.roboMoves[moveIndex].y;
    if (state.board[curr.y][curr.x] == EMPTY) {
      return true;
    }
  }
  return false;
}

void makeMove(WarehouseState &state, int moveIndex) {
  Vec2 start = state.roboLoc;
  Vec2 end = start;
  while (true) {
    if (state.board[end.y][end.x] == EMPTY) {
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

  state.board[state.roboLoc.y][state.roboLoc.x] = EMPTY;

  state.roboLoc.x += state.roboMoves[moveIndex].x;
  state.roboLoc.y += state.roboMoves[moveIndex].y;
}

long boardScore(const WarehouseState &input) {
  long result = 0;
  for (int i = 0; i < input.board.size(); ++i) {
    for (int j = 0; j < input.board[0].size(); ++j) {
      if (input.board[i][j] == BOX) {
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
      char c;
      switch (val) {
      case WALL:
        std::cout << '#';
        break;
      case ROBOT:
        std::cout << '@';
        break;
      case EMPTY:
        std::cout << '.';
        break;
      case BOX:
        std::cout << 'O';
        break;
      }
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
  return boardScore(input);
}

enum BeegSquareTypes {
  E,   // empty
  R,   // robot
  W,   // wall
  B_L, // box_left
  B_R  // box_right
};

struct P2StateType {
  Vec2 roboLoc;
  std::vector<Vec2> roboMoves;
  std::vector<std::vector<BeegSquareTypes>> board;
};

P2StateType loadInputPart2(std::string filepath) {
  P2StateType result;
  std::fstream file(filepath);
  std::string line;
  while (true) {
    getline(file, line);
    if (line.empty())
      break;

    std::vector<BeegSquareTypes> curr;
    for (const auto &c : line) {
      switch (c) {
      case '.':
        curr.push_back(E);
        curr.push_back(E);
        break;
      case '#':
        curr.push_back(W);
        curr.push_back(W);
        break;
      case 'O':
        curr.push_back(B_L);
        curr.push_back(B_R);
        break;
      case '@':
        curr.push_back(R);
        curr.push_back(E);
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
      if (result.board[i][j] == R) {
        result.roboLoc = {j, i};
      }
    }
  }

  return result;
}

void logBoardP2(const P2StateType &state, int moveIndex) {

  std::cout << std::endl;

  if (state.roboMoves[moveIndex].x == 1 && state.roboMoves[moveIndex].y == 0) {
    std::cout << '>' << std::endl;
  } else if (state.roboMoves[moveIndex].x == -1 &&
             state.roboMoves[moveIndex].y == 0) {
    std::cout << '<' << std::endl;
  } else if (state.roboMoves[moveIndex].x == 0 &&
             state.roboMoves[moveIndex].y == 1) {
    std::cout << 'V' << std::endl;
  } else if (state.roboMoves[moveIndex].x == 0 &&
             state.roboMoves[moveIndex].y == -1) {
    std::cout << '^' << std::endl;
  }
  for (const auto &line : state.board) {
    for (const auto &val : line) {
      switch (val) {
      case R:
        std::cout << '@';
        break;
      case E:
        std::cout << '.';
        break;
      case B_L:
        std::cout << '[';
        break;
      case B_R:
        std::cout << ']';
        break;
      case W:
        std::cout << '#';
        break;
      }
    }
    std::cout << std::endl;
  }
}

long part2(P2StateType input) { return -1; }

int main() {
  WarehouseState testInput = loadInput("./15_testInput.txt");
  WarehouseState input = loadInput("./15_input.txt");
  P2StateType p2TestInput = loadInputPart2("./15_testInput.txt");
  P2StateType p2input = loadInputPart2("./15_input.txt");

  std::cout << "Test 1: " << part1(testInput) << std::endl;
  std::cout << "Part 1: " << part1(input) << std::endl;
  std::cout << "Test 2: " << part2(p2TestInput) << std::endl;
}
