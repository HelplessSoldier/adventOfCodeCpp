#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
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

long part1(WarehouseState input, bool visualize) {

  for (int i = 0; i < input.roboMoves.size(); ++i) {
    if (visualize)
      logBoard(input, i);

    if (canMakeMove(input, i)) {
      makeMove(input, i);
    }

    if (visualize) {
      // std::cin.get();
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
  }
  return boardScore(input);
}

int main() {
  WarehouseState testInput = loadInput("./15_testInput.txt");
  WarehouseState input = loadInput("./15_input.txt");

  std::cout << "Test 1: " << part1(testInput, /*visualize=*/true) << std::endl;
  std::cout << "Part 1: " << part1(input, /*visualize=*/false) << std::endl;
}
