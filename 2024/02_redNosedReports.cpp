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

bool isSafe(std::vector<int> line) {
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

bool isSafeWithDamping(std::vector<int> line) {
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

int part1(std::vector<std::vector<int>> input) {
  int result = 0;
  for (std::vector<int> line : input) {
    if (isSafe(line))
      result++;
  }
  return result;
}

int part2(std::vector<std::vector<int>> input) {
  int result = 0;
  for (std::vector<int> line : input) {
    if (isSafeWithDamping(line)) {
      result++;
    }
  }
  return result;
};

int main() {
  std::vector<std::vector<int>> input = loadInput("./02_input.txt");
  std::cout << "Part 1: " << part1(input) << std::endl;
  std::cout << "Part 2: " << part2(input) << std::endl;
}
