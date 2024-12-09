#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

std::string loadInput(std::string filepath) {
  std::fstream file(filepath);
  std::string line;
  std::string result;
  getline(file, line);
  result.append(line);
  return result;
}

std::vector<long> buildFileStructure(const std::string &input) {
  std::vector<long> result;

  size_t id = 0;
  unsigned char curr;

  for (int i = 0; i < input.size(); ++i) {
    curr = input[i] - '0';

    if (i % 2 == 0) {
      // file
      for (int j = 0; j < curr; ++j) {
        result.push_back(id);
      }
      id++;
    } else {
      // empty
      for (int j = 0; j < curr; ++j) {
        result.push_back(-1);
      }
    }
  }
  return result;
}

void p1ShiftFiles(std::vector<long> &fileStructure) {
  long *right = &fileStructure[fileStructure.size() - 1];
  long *left = &fileStructure[0];

  while (left < right) {
    if (*left == -1 && *right != -1) {
      *left = *right;
      *right = -1;
    }
    while (*left != -1)
      ++left;
    while (*right == -1)
      --right;
  }
}

int findEmpty(std::vector<long> &fileStructure, int fileLen) {
  for (int i = 0; i < fileStructure.size(); ++i) {
    if (fileStructure[i] == -1) {
      int emptyLen = 1;
      int emptyStart = i;
      while (fileStructure[++i] == -1) {
        emptyLen++;
      }
      if (emptyLen >= fileLen)
        return emptyStart;
    }
  }
  return -1;
}

//      index, length
std::pair<int, int> findFile(const std::vector<long> &fileStructure, int id) {
  for (int i = fileStructure.size() - 1; i >= 0; --i) {
    if (fileStructure[i] == id) {
      for (int j = 0; j <= i; j++) {
        if (fileStructure[i - j] != id) {
          return {i - j + 1, j};
        }
      }
    }
  }
  return {-1, -1};
}

void p2ShiftFiles(std::vector<long> &fileStructure) {
  int fileIndex = fileStructure.size() - 1;
  while (fileStructure[--fileIndex] == -1)
    ;
  int id = fileStructure[fileIndex] + 1;

  while (id-- >= 0) {
    std::pair<int, int> file = findFile(fileStructure, id);

    if (file.first == -1 || file.second == -1)
      // didn't find the file
      continue;

    int empty = findEmpty(fileStructure, file.second);

    if (empty == -1)
      // didn't find a large enough space
      continue;

    if (file.first < empty)
      // wants to move the file right
      continue;

    for (int i = 0; i < file.second; ++i) {
      fileStructure[empty + i] = id;
      fileStructure[file.first + i] = -1;
    }
  }
}

long long checkSum(const std::vector<long> &fileStructure) {
  long long result = 0;
  for (int i = 0; i < fileStructure.size(); ++i) {
    if (fileStructure[i] == -1)
      continue;
    result += i * fileStructure[i];
  }
  return result;
}

long long part1(std::vector<long> fileStructure) {
  p1ShiftFiles(fileStructure);
  return checkSum(fileStructure);
}

long long part2(std::vector<long> fileStructure) {
  p2ShiftFiles(fileStructure);
  return checkSum(fileStructure);
}

int main() {
  std::string input = loadInput("./09_input.txt");
  std::string testInput = "2333133121414131402";

  std::vector<long> testFileStructure = buildFileStructure(testInput);
  std::vector<long> fileStructure = buildFileStructure(input);

  std::cout << "Test 1: " << part1(testFileStructure) << std::endl;
  std::cout << "Part 1: " << part1(fileStructure) << std::endl;
  std::cout << "Test 2: " << part2(testFileStructure) << std::endl;
  std::cout << "Part 2: " << part2(fileStructure) << std::endl;
}
