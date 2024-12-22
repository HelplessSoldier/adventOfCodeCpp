#include <algorithm>
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

struct Vec2 {
  int x, y;

  Vec2 operator+(const Vec2 &other) const { return {x + other.x, y + other.y}; }
  Vec2 operator-(const Vec2 &other) const { return {x - other.x, y - other.y}; }
  bool operator<(const Vec2 &other) const {
    if (x < other.x)
      return true;
    if (x > other.x)
      return false;
    return y < other.y;
  }
};

std::ostream &operator<<(std::ostream &os, const Vec2 &vec) {
  os << "[" << vec.x << ", " << vec.y << "]";
  return os;
}

const Vec2 forbiddenKeypad = {0, 3};
const Vec2 forbiddenArrows = {0, 0};

std::vector<std::string> loadInput(std::string filepath) {
  std::fstream file(filepath);
  std::string line;

  std::vector<std::string> codes;

  while (getline(file, line)) {
    codes.push_back(line);
  }
  return codes;
}

std::string robotStep(const std::string &input,
                      const std::map<char, Vec2> &arrowPosMap) {
  std::string result;

  Vec2 currentPos = {2, 0};

  for (const char &c : input) {

    auto arrowIt = arrowPosMap.find(c);
    if (arrowIt == arrowPosMap.end()) {
      throw std::logic_error("Unexpected item in bagging area");
    }

    Vec2 target = arrowIt->second;
    Vec2 delta = target - currentPos;

    while (delta.x != 0 || delta.y != 0) {
      if (delta.x < 0) {
        result.push_back('<');
        delta.x++;
      } else if (delta.x > 0) {
        result.push_back('>');
        delta.x--;
      }

      if (delta.y < 0) {
        result.push_back('^');
        delta.y++;
      } else if (delta.y > 0) {
        result.push_back('v');
        delta.y--;
      }
    }

    currentPos = target;
    result.push_back('A');
  }

  return result;
}

void findLowestRecursive(std::string inputs,
                         const std::map<char, Vec2> &arrowPosMap, int numRobots,
                         long long &minSeen) {

  if (numRobots == 0) {
    long long inputsLength = inputs.size();
    minSeen = std::min(minSeen, inputsLength);
    return;
  }

  std::string step = robotStep(inputs, arrowPosMap);
  findLowestRecursive(step, arrowPosMap, numRobots - 1, minSeen);
}

long long findLowestLength(Vec2 start, Vec2 end,
                           std::map<char, Vec2> arrowPosMap, int numRobots) {

  Vec2 delta = end - start;

  std::string initialInputs;
  while (delta.x != 0 || delta.y != 0) {
    if (delta.x != 0) {
      initialInputs.push_back(delta.x < 0 ? '<' : '>');
      delta.x += delta.x < 0 ? 1 : -1;
    } else {
      initialInputs.push_back(delta.y < 0 ? '^' : 'v');
      delta.y += delta.y < 0 ? 1 : -1;
    }
  }

  long long llongMax = LLONG_MAX;
  long long minSeen;
  findLowestRecursive(initialInputs, arrowPosMap, numRobots, minSeen);

  return minSeen;
}

int numericPartOfCode(const std::string &code) {
  int result = 0;
  bool inLeadingZeros = true;
  for (char c : code) {
    if (c != '0')
      inLeadingZeros = false;

    if (inLeadingZeros)
      continue;

    if (c == 'A')
      continue;

    result = result * 10 + static_cast<int>(c - '0');
  }
  return result;
}

long long solve(std::vector<std::string> input, int numRobots) {
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

  std::map<char, Vec2> keyPosMap;
  keyPosMap['0'] = {1, 3};
  keyPosMap['1'] = {0, 2};
  keyPosMap['2'] = {1, 2};
  keyPosMap['3'] = {2, 2};
  keyPosMap['4'] = {0, 1};
  keyPosMap['5'] = {1, 1};
  keyPosMap['6'] = {2, 1};
  keyPosMap['7'] = {0, 0};
  keyPosMap['8'] = {1, 0};
  keyPosMap['9'] = {2, 0};
  keyPosMap['A'] = {2, 3};

  /* x-right y-down
      +---+---+
      | ^ | A | 0
  +---+---+---+
  | < | v | > | 1
  +---+---+---+
    0   1   2
  */

  std::map<char, Vec2> arrowPosMap;
  arrowPosMap['^'] = {1, 0};
  arrowPosMap['>'] = {2, 1};
  arrowPosMap['v'] = {1, 1};
  arrowPosMap['<'] = {0, 1};
  arrowPosMap['A'] = {2, 0};

  long long result = 0;
  for (std::string code : input) {

    int codeNum = numericPartOfCode(code);
    long long currLength = 0;
    char lastChar = code[0];

    for (int i = 1; i < code.size(); ++i) {

      Vec2 target = keyPosMap[code[i]];
      Vec2 start = keyPosMap[lastChar];
      Vec2 delta = target - start;

      std::cout << "Finding " << lastChar << start << " to " << code[i]
                << target << std::endl;

      currLength += findLowestLength(start, target, arrowPosMap, numRobots);
      currLength++; // account for the final A

      std::cout << currLength << std::endl;

      lastChar = code[i];
    }

    result += codeNum * currLength;
  }

  return result;
}

int main() {
  std::vector<std::string> testInput = loadInput("./21_testInput.txt");

  long long testResult = solve(testInput, 25);
  std::cout << "Test 1: " << testResult << std::endl;
}
