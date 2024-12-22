// incomplete solution, fails of test 1, and too slow for part 2

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sys/types.h>
#include <vector>

typedef std::vector<std::vector<int>> Codes_t;

enum DirectionLabel { UP, DOWN, LEFT, RIGHT, A };

struct Vec2 {
  int x, y;

  bool operator==(const Vec2 &other) const {
    return x == other.x && y == other.y;
  }

  bool operator!=(const Vec2 &other) const {
    return x != other.x || y != other.y;
  }

  bool operator<(const Vec2 &other) const {
    if (x < other.x)
      return true;
    if (x > other.x)
      return false;
    return y < other.y;
  }
};

typedef std::vector<u_int8_t> Sequence;

Codes_t loadInput(std::string filepath) {
  Codes_t result;

  std::fstream file(filepath);
  std::string line;

  while (getline(file, line)) {
    std::vector<int> code;
    for (int i = 0; i < line.size(); ++i) {
      if (line[i] == 'A') {
        code.push_back(10);
      } else {
        code.push_back(line[i] - '0');
      }
    }
    result.push_back(code);
  }
  return result;
}

void logSequence(const Sequence &seq) {
  for (const u_int8_t &label : seq) {
    switch (label) {
    case UP:
      std::cout << '^';
      break;
    case DOWN:
      std::cout << 'v';
      break;
    case LEFT:
      std::cout << '<';
      break;
    case RIGHT:
      std::cout << '>';
      break;
    case A:
      std::cout << 'A';
      break;
    }
  }
  std::cout << std::endl;
}

Sequence traverseKeypad(Vec2 &currpos, const Vec2 &emptyKey,
                        const Vec2 &target) {
  Sequence result;

  Vec2 del{target.x - currpos.x, target.y - currpos.y};

  DirectionLabel horizontalLabel = (del.x < 0) ? LEFT : RIGHT;
  DirectionLabel verticalLabel = (del.y < 0) ? UP : DOWN;

  bool emptyKeyOnRow = false;
  {
    Vec2 temp = currpos;
    int steps = std::abs(del.x);
    for (int i = 0; i < steps; ++i) {
      temp.x += (del.x > 0) ? 1 : -1;
      if (temp == emptyKey) {
        emptyKeyOnRow = true;
        break;
      }
    }
  }

  bool emptyKeyOnCol = false;
  {
    Vec2 temp = currpos;
    int steps = std::abs(del.y);
    for (int i = 0; i < steps; ++i) {
      temp.y += (del.y > 0) ? 1 : -1;
      if (temp == emptyKey) {
        emptyKeyOnCol = true;
        break;
      }
    }
  }

  auto moveVert = [&]() {
    Vec2 temp = del;
    while (temp.y != 0) {
      result.push_back(verticalLabel);
      temp.y += (temp.y > 0) ? -1 : 1;
    }
    currpos.y += del.y;
  };

  auto moveHori = [&]() {
    Vec2 temp = del;
    while (temp.x != 0) {
      result.push_back(horizontalLabel);
      temp.x += (temp.x > 0) ? -1 : 1;
    }
    currpos.x += del.x;
  };

  if (emptyKeyOnRow) {
    moveVert();
    moveHori();
  } else if (emptyKeyOnCol) {
    moveHori();
    moveVert();
  } else {
    moveHori();
    moveVert();
  }

  std::sort(result.begin(), result.end());

  result.push_back(A);
  return result;
}

class NumericalKeypad {
  /*  x-right y-down
      0   1   2
    +---+---+---+
    | 7 | 8 | 9 | 0
    +---+---+---+
    | 4 | 5 | 6 | 1
    +---+---+---+
    | 1 | 2 | 3 | 2
    +---+---+---+
        | 0 | A | 3
        +---+---+
    */

public:
  NumericalKeypad() : currpos({2, 3}), emptyKey({0, 3}) {
    keyPosMap[0] = {1, 3};
    keyPosMap[1] = {0, 2};
    keyPosMap[2] = {1, 2};
    keyPosMap[3] = {2, 2};
    keyPosMap[4] = {0, 1};
    keyPosMap[5] = {1, 1};
    keyPosMap[6] = {2, 1};
    keyPosMap[7] = {0, 0};
    keyPosMap[8] = {1, 0};
    keyPosMap[9] = {2, 0};
    keyPosMap[10] = {2, 3}; // A
  }

  Sequence sequence(const std::vector<int> &code) {
    Sequence result;
    for (const int &val : code) {
      Vec2 target = keyPosMap[val];
      Sequence subSeq = traverseKeypad(currpos, emptyKey, target);
      result.insert(result.end(), subSeq.begin(), subSeq.end());
    }
    return result;
  }

private:
  Vec2 currpos, emptyKey;
  std::map<int, Vec2> keyPosMap;
};

class DirectionalKeypad {
  /* x-right y-down
      +---+---+
      | ^ | A | 0
  +---+---+---+
  | < | v | > | 1
  +---+---+---+
    0   1   2
  */

public:
  DirectionalKeypad() : currpos({2, 0}), emptyKey({0, 0}) {
    keyPosMap[UP] = {1, 0};
    keyPosMap[RIGHT] = {2, 1};
    keyPosMap[DOWN] = {1, 1};
    keyPosMap[LEFT] = {0, 1};
    keyPosMap[A] = {2, 0};
  }

  Sequence sequence(const Sequence &code) {

    /*
    static std::map<Sequence, Sequence> memo;
    if (memo.find(code) != memo.end()) {
      return memo[code];
    }
    */

    Sequence result;

    for (const u_int8_t &val : code) {
      Vec2 target = keyPosMap[static_cast<DirectionLabel>(val)];
      Sequence subSeq = traverseKeypad(currpos, emptyKey, target);
      result.insert(result.end(), subSeq.begin(), subSeq.end());
    }

    // memo[code] = result;

    return result;
  }

private:
  Vec2 currpos, emptyKey;
  std::map<DirectionLabel, Vec2> keyPosMap;
};

Sequence shortestSequence(const std::vector<int> &code, int numRobots) {

  NumericalKeypad keypad;
  Sequence seq = keypad.sequence(code);

  DirectionalKeypad robot;
  for (int i = 0; i < numRobots; ++i) {
    seq = robot.sequence(seq);
  }

  return seq;
}

int numericPartOfCode(const std::vector<int> &code) {
  int result = 0;
  bool inLeadingZeros = true;
  for (int num : code) {
    if (num != 0)
      inLeadingZeros = false;

    if (inLeadingZeros)
      continue;

    if (num == 10) // A
      continue;

    result = result * 10 + num;
  }
  return result;
}

long long solve(const Codes_t &input, int numRobots) {
  long long result = 0;

  NumericalKeypad keypad;
  DirectionalKeypad robot;

  for (int i = 0; i < input.size(); ++i) {
    std::vector<int> code = input[i];
    Sequence initialSeq = keypad.sequence(code);
    Sequence next = shortestSequence(code, numRobots);
    int num = numericPartOfCode(code);
    result += next.size() * num;
  }

  return result;
}

int main() {
  // A -> 10
  Codes_t testInput = loadInput("./21_testInput.txt");
  Codes_t input = loadInput("./21_input.txt");

  auto start = std::chrono::high_resolution_clock::now();

  /*
    for (int i = 1; i <= 25; ++i) {

      auto now = std::chrono::high_resolution_clock::now();
      auto t =
          std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

      std::cout << i << " " << solve(input, i) << " total time: " << t << "s"
                << std::endl;
    }
    */

  // std::cout << "Test 1: " << solve(testInput, 2) << std::endl;
  std::cout << "Part 1: " << solve(input, 2) << std::endl;
  std::cout << "Part 2: " << solve(input, 25) << std::endl;
  // first incorrect: 12198179905576
}
