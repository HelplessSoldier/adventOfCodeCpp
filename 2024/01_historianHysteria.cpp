#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

std::tuple<std::vector<int>, std::vector<int>> getInputs(std::string filepath) {
  std::fstream file(filepath);

  std::vector<int> leftNums, rightNums;
  int left, right;
  while (file >> left >> right) {
    leftNums.push_back(left);
    rightNums.push_back(right);
  }
  file.close();

  return {leftNums, rightNums};
}

int part1(std::vector<int> leftNums, std::vector<int> rightNums) {
  std::sort(leftNums.begin(), leftNums.end());
  std::sort(rightNums.begin(), rightNums.end());

  int sum = 0;
  for (int i = 0; i < leftNums.size(); ++i) {
    sum += std::abs(leftNums[i] - rightNums[i]);
  }

  return sum;
}

int part2(std::vector<int> leftNums, std::vector<int> rightNums) {
  std::map<int, int> rightCount;
  for (int i = 0; i < rightNums.size(); ++i) {
    ++rightCount[rightNums[i]];
  }

  int similarityScore = 0;
  for (int i = 0; i < leftNums.size(); ++i) {
    int left = leftNums[i];
    similarityScore += left * rightCount[left];
  }

  return similarityScore;
}

int main() {
  std::vector<int> leftNums, rightNums;
  std::tie(leftNums, rightNums) = getInputs("./01_input.txt");

  std::cout << "Part1: " << part1(leftNums, rightNums) << std::endl;
  std::cout << "Part2: " << part2(leftNums, rightNums) << std::endl;
}
