#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> getInputNums(std::string filename) {
  std::fstream input(filename);
  std::string line;
  std::vector<int> inputNums;

  while (getline(input, line)) {
    for (const char c : line) {
      inputNums.push_back(c - '0');
    }
  }

  return inputNums;
}

int solve(std::vector<int> input, int offset) {
  int result = 0;
  for (int i = 0; i < input.size(); ++i) {
    int j = (i + offset) % input.size();
    if (input[i] == input[j])
      result += input[i];
  }

  return result;
}

int main() {
  std::vector<int> input = getInputNums("./01_input.txt");

  std::cout << "Part 1: " << solve(input, 1) << std::endl;
  std::cout << "Part 2: " << solve(input, input.size() / 2) << std::endl;

  return 0;
}
