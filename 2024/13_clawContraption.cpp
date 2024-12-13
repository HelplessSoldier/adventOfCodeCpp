#include <climits>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

struct Vec2 {
  long x, y;
};

struct InputBlock {
  Vec2 A, B;
  long prizeX, prizeY;
};

std::vector<InputBlock> loadInput(std::string filepath) {
  std::vector<InputBlock> result;

  std::fstream file(filepath);
  std::string line;

  const InputBlock empty = {{-1, -1}, {-1, -1}, -1, -1};
  InputBlock current = empty;
  long lineOffset = 1;
  while (getline(file, line)) {
    if (line.empty()) {
      lineOffset = 0;
      result.push_back(current);
      current = empty;
    }

    std::stringstream ss(line);
    if (lineOffset == 1) {
      // button A deltas
      std::string word;
      for (long i = 0; i < 3; ++i) {
        ss >> word;
      }
      current.A.x = std::stoi(word.substr(1, word.size() - 2));

      ss >> word;
      current.A.y = std::stoi(word.substr(1, word.size() - 1));

    } else if (lineOffset == 2) {
      // button B deltas
      std::string word;
      for (long i = 0; i < 3; ++i) {
        ss >> word;
      }
      current.B.x = std::stoi(word.substr(1, word.size() - 2));

      ss >> word;
      current.B.y = std::stoi(word.substr(1, word.size() - 1));

    } else if (lineOffset == 3) {
      // prize location
      std::string word;
      ss >> word >> word;
      current.prizeX = std::stoi(word.substr(2, word.size() - 1));

      ss >> word;
      current.prizeY = std::stoi(word.substr(2, word.size() - 1));
    }

    ++lineOffset;
  }

  return result;
}

long stepCost(const InputBlock &block) {
  const long bPrice = 1;
  const long aPrice = 3;

  // winnable check:
  // for x * a + y * b = c to be possible, c % gcd(a, b) must be 0
  if (block.prizeX % std::gcd(block.A.x, block.B.x) != 0 ||
      block.prizeY % std::gcd(block.A.y, block.B.y) != 0) {
    return 0;
  }

  long bPress = 0; // y
  long aPress = 0; // x

  // x * a + y * b = c where a, b, c are vec2's and x = aPress, y = bPress;
  // solve for x: x = (c - yb) / a
  // split longo x and y components
  // x = (c_x - yb_x) / a_x  eq 1.
  // x = (c_y - yb_y) / a_y  eq 2.

  // x * a + y * b >= c; set x = 0; solve -> y >= c / b
  long yMax_x = (block.prizeX / block.B.x) + 10; // + 10 for safety
  long yMax_y = (block.prizeY / block.B.y) + 10; // + 10 for safety
  long yMax = std::max(yMax_x, yMax_y);

  for (long y = 0; y < yMax; ++y) {

    // int division inaccuracy guard
    if ((block.prizeX - y * block.B.x) % block.A.x != 0 ||
        (block.prizeY - y * block.B.y) % block.A.y != 0) {
      continue;
    }

    long x1 = (block.prizeX - y * block.B.x) / block.A.x;
    long x2 = (block.prizeY - y * block.B.y) / block.A.y;

    if (x1 == x2) {
      bPress = y;
      aPress = x1;
      break;
    }
  }

  return bPress * bPrice + aPress * aPrice;
}

long part1(const std::vector<InputBlock> &input) {
  long result = 0;
  for (const InputBlock &block : input) {
    result += stepCost(block);
  }
  return result;
}

long part2(const std::vector<InputBlock> &input) {
  long result = 0;
  std::vector<InputBlock> newInput;
  for (const InputBlock &block : input) {
    InputBlock bigBoy = block;
    bigBoy.prizeY += 10000000000000;
    bigBoy.prizeX += 10000000000000;
    newInput.push_back(bigBoy);
  }
  return part1(newInput);
}

int main() {
  std::vector<InputBlock> testInput = loadInput("./13_testInput.txt");
  std::vector<InputBlock> input = loadInput("./13_input.txt");

  std::cout << "Test 1: " << part1(testInput) << std::endl;
  std::cout << "Part 1: " << part1(input) << std::endl;
  std::cout << "Test 2: " << part2(testInput) << std::endl;
  std::cout << "Part 2: " << part2(input) << std::endl;
}
