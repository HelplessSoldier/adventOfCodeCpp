#include <cassert>
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Vec2 {
  long long x, y;
};

struct InputBlock {
  Vec2 a, b, p;
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
      current.a.x = std::stoi(word.substr(1, word.size() - 2));

      ss >> word;
      current.a.y = std::stoi(word.substr(1, word.size() - 1));

    } else if (lineOffset == 2) {
      // button B deltas
      std::string word;
      for (long i = 0; i < 3; ++i) {
        ss >> word;
      }
      current.b.x = std::stoi(word.substr(1, word.size() - 2));

      ss >> word;
      current.b.y = std::stoi(word.substr(1, word.size() - 1));

    } else if (lineOffset == 3) {
      // prize location
      std::string word;
      ss >> word >> word;
      current.p.x = std::stoi(word.substr(2, word.size() - 1));

      ss >> word;
      current.p.y = std::stoi(word.substr(2, word.size() - 1));
    }

    ++lineOffset;
  }
  result.push_back(current);

  return result;
}

long long cost(const InputBlock &in) {
  const long long aCost = 3;
  const long long bCost = 1;

  // na + mb = p
  // n = (p_x - mb_x) / a_x = (p_y - m * b_y) / a_y
  // m = (a_y * p_x - a_x * p_y) / (a_y * b_x - a_x * b_y) = m_num / m_denom

  long long m_num = in.a.y * in.p.x - in.a.x * in.p.y;
  long long m_denom = in.a.y * in.b.x - in.a.x * in.b.y;
  long long m = m_num / m_denom;

  long long n_num = (in.p.x - m * in.b.x);
  long long n = n_num / in.a.x;

  bool correct =
      n * in.a.x + m * in.b.x == in.p.x && n * in.a.y + m * in.b.y == in.p.y;

  if (correct)
    return n * aCost + m * bCost;

  return 0;
}

long long solve(std::vector<InputBlock> input, bool part2) {
  long long result = 0;

  if (part2) {
    for (InputBlock &block : input) {
      block.p.y += 10000000000000;
      block.p.x += 10000000000000;
    }
  }

  for (const InputBlock &block : input) {
    result += cost(block);
  }

  return result;
}

int main() {
  std::vector<InputBlock> testInput = loadInput("./13_testInput.txt");
  std::vector<InputBlock> input = loadInput("./13_input.txt");

  std::cout << "Test 1: " << solve(testInput, false) << std::endl;
  std::cout << "Part 1: " << solve(input, false) << std::endl;
  std::cout << "Test 2: " << solve(testInput, true) << std::endl;
  std::cout << "Part 2: " << solve(input, true) << "\n" << std::endl;
}
