#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::vector<int>> loadInput(std::string filepath) {
  std::fstream file(filepath);
  std::string line;
  std::vector<std::vector<int>> data;

  while (getline(file, line)) {
    std::stringstream ss(line);
    std::vector<int> currLine;

    int value;
    while (ss >> value) {
      currLine.push_back(value);
    }

    data.push_back(currLine);
  }

  return data;
}

bool isSafe(const std::vector<int> &line) {
  int direction = 0; // -1 for decreasing, 1 for increasing

  for (int i = 0; i < line.size() - 1; ++i) {
    int left = line[i];
    int right = line[i + 1];

    if (!direction)
      direction = left < right ? 1 : -1;

    int currDirection = left < right ? 1 : -1;
    if (currDirection != direction)
      return false;

    int delta = std::abs(left - right);
    if (delta > 3 || delta < 1)
      return false;
  }
  return true;
}

bool isSafeWithDamping(const std::vector<int> &line) {
  if (isSafe(line))
    return true;

  for (int i = 0; i < line.size(); ++i) {
    std::vector<int> cutVec(line.begin(), line.end());
    cutVec.erase(cutVec.begin() + i);

    if (isSafe(cutVec))
      return true;
  }
  return false;
}

int part1(const std::vector<std::vector<int>> &input) {
  int result = 0;
  for (const std::vector<int> &line : input) {
    if (isSafe(line))
      result++;
  }
  return result;
}

int part2(const std::vector<std::vector<int>> &input) {
  int result = 0;
  for (const std::vector<int> &line : input) {
    if (isSafeWithDamping(line))
      result++;
  }
  return result;
};

int duration(std::chrono::high_resolution_clock::time_point &start,
             std::chrono::high_resolution_clock::time_point &end) {
  return std::chrono::duration_cast<std::chrono::microseconds>(end - start)
      .count();
}

int main() {
  using clock = std::chrono::high_resolution_clock;

  std::vector<std::vector<int>> input = loadInput("./02_input.txt");

  auto startP1 = clock::now();
  std::cout << "Part 1: " << part1(input) << std::endl;
  auto endP1 = clock::now();
  std::cout << duration(startP1, endP1) << "µs" << std::endl; // 55us-ish

  auto startP2 = clock::now();
  std::cout << "Part 2: " << part2(input) << std::endl;
  auto endP2 = clock::now();
  std::cout << duration(startP2, endP2) << "µs" << std::endl; // 1200us-ish
}
