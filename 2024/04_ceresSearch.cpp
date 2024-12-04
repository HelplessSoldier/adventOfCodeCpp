#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef std::vector<std::vector<int>> mat2i;

std::string loadData(std::string filepath) {
  std::fstream file(filepath);
  std::string line;
  std::string result;

  while (getline(file, line)) {
    result.append(line + '\n');
  }
  return result;
}

mat2i strToMat(const std::string &input) {
  mat2i result;
  std::vector<int> currLine;
  for (const char &c : input) {
    if (c == '\n' && !currLine.empty()) {
      result.push_back(currLine);
      currLine.clear();
    } else {
      currLine.push_back(c == 'X'   ? 0
                         : c == 'M' ? 1
                         : c == 'A' ? 2
                         : c == 'S' ? 3
                                    : -1);
    }
  }

  return result;
}

bool inbounds(int x, int y, int h, int w) {
  return (x >= 0 && x < h && y >= 0 && y < w);
}

int searchP1(const mat2i &matrix, int i, int j) {
  const int xmasLen = 4;
  mat2i deltas = {
      {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1},
  };

  int result = 0;

  int matHeight = matrix.size();
  int matWidth = matrix[0].size();

  for (const std::vector<int> &delta : deltas) {
    for (int distance = 0; distance < xmasLen; ++distance) {
      int x = i + delta[0] * distance;
      int y = j + delta[1] * distance;
      if (!inbounds(x, y, matHeight, matWidth) || matrix[x][y] != distance) {
        break;
      }
      if (distance == 3) {
        // got to S in XMAS
        result++;
      }
    }
  }

  return result;
}

int part1(const mat2i &matrix) {
  int result = 0;
  int width = matrix[0].size();
  int height = matrix.size();

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if (matrix[i][j] == 0) { // 'X'
        result += searchP1(matrix, i, j);
      }
    }
  }

  return result;
}

bool isValidX(const mat2i &matrix, int i, int j) {
  const std::vector<std::vector<int>> deltas = {{1, 1}, {1, -1}};

  int matHeight = matrix.size();
  int matWidth = matrix[0].size();

  int count = 0;
  for (std::vector<int> delta : deltas) {
    int x1 = i + delta[0];
    int y1 = j + delta[1];
    int x2 = i + -1 * delta[0];
    int y2 = j + -1 * delta[1];

    if (!inbounds(x1, y1, matHeight, matWidth) ||
        !inbounds(x2, y2, matHeight, matWidth)) {
      break;
    }

    if ((matrix[x1][y1] == 1 && matrix[x2][y2] == 3) ||
        (matrix[x1][y1] == 3 && matrix[x2][y2] == 1)) {
      ++count;
    }
  }
  return count == 2;
}

int part2(const mat2i &matrix) {
  int result = 0;

  int width = matrix[0].size();
  int height = matrix.size();

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if (matrix[i][j] == 2) { // 'A'
        result += isValidX(matrix, i, j);
      }
    }
  }

  return result;
}

int main() {
  std::string input = loadData("./04_input.txt");
  mat2i matrix = strToMat(input);

  std::cout << "Part 1: " << part1(matrix) << std::endl;
  std::cout << "Part 2: " << part2(matrix) << std::endl;
}
