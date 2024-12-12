#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef std::vector<std::vector<int>> mat2i;

mat2i loadInput(std::string filepath) {
  mat2i result;
  std::fstream file(filepath);
  std::string line;
  while (getline(file, line)) {
    std::vector<int> currentLine;
    for (const char &square : line) {
      currentLine.push_back(square - 'A');
    }
    result.push_back(currentLine);
  }
  return result;
}

struct RegionInfo {
  int area, perimeter, vertices;
};

bool topLeftVertConcave(const mat2i &localAreaMap) {
  return localAreaMap[0][1] == 0 && localAreaMap[1][0] == 0;
};

bool topLeftVertConvex(const mat2i &localAreaMap) {
  return localAreaMap[0][0] == 0 && localAreaMap[0][1] == 1 &&
         localAreaMap[1][0] == 1;
};

int countVertices(const mat2i &input, int i, int j) {
  int result = 0;

  // using 0 as non-region spaces and 1 for the region
  mat2i localAreaMap(3, std::vector<int>(3, -1));

  int label = input[i][j];
  for (int di = -1; di <= 1; ++di) {
    for (int dj = -1; dj <= 1; ++dj) {

      int areaMapI = di + 1;
      int areaMapj = dj + 1;

      if (i + di < 0 || i + di >= input.size() || j + dj < 0 ||
          j + dj >= input[0].size()) {
        localAreaMap[areaMapI][areaMapj] = 0;
      } else if (input[i + di][j + dj] == label) {
        localAreaMap[areaMapI][areaMapj] = 1;
      } else {
        localAreaMap[areaMapI][areaMapj] = 0;
      }
    }
  }

  auto rotateMatrix = [&]() {
    // transpose then reverse row vectors
    mat2i temp(localAreaMap[0].size(),
               std::vector<int>(localAreaMap.size(), 0));
    for (int i = 0; i < localAreaMap.size(); ++i) {
      for (int j = 0; j < localAreaMap[0].size(); ++j) {
        temp[j][i] = localAreaMap[i][j];
      }
    }
    for (auto &row : temp) {
      std::reverse(row.begin(), row.end());
    }
    localAreaMap = temp;
  };

  for (int i = 0; i < 4; ++i) {
    rotateMatrix();
    if (topLeftVertConvex(localAreaMap) || topLeftVertConcave(localAreaMap))
      result++;
  }

  return result;
}

RegionInfo searchRegion(const mat2i &input, mat2i &seenMap, int i, int j) {

  const mat2i directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  RegionInfo result = {1, 0, 0};

  seenMap[i][j]++;

  result.vertices += countVertices(input, i, j);

  int curr = input[i][j];
  for (const std::vector<int> dir : directions) {
    int newI = i + dir[0];
    int newJ = j + dir[1];

    if (newI < 0 || newI >= input.size() || newJ < 0 ||
        newJ >= input[0].size()) { // oob counts as an edge
      result.perimeter++;
      continue;
    }

    int next = input[newI][newJ];

    if (curr != next) { // edge of region
      result.perimeter++;
      continue;
    }

    if (!seenMap[newI][newJ]) {
      RegionInfo nextResult = searchRegion(input, seenMap, newI, newJ);

      result.area += nextResult.area;
      result.perimeter += nextResult.perimeter;
      result.vertices += nextResult.vertices;
    }
  }
  return result;
}

std::pair<int, int> solve(const mat2i &input) {
  mat2i seenMap(input.size(), std::vector<int>(input[0].size(), 0));
  int p1Result = 0;
  int p2Result = 0;

  for (int i = 0; i < input.size(); ++i) {
    for (int j = 0; j < input[0].size(); ++j) {
      if (seenMap[i][j]) {
        continue;
      }

      RegionInfo region = searchRegion(input, seenMap, i, j);

      p1Result += region.area * region.perimeter;
      p2Result += region.area * region.vertices;
    }
  }

  return {p1Result, p2Result};
}

int main() {
  mat2i input = loadInput("./12_input.txt");
  mat2i testInput = loadInput("./12_testInput.txt");

  auto [p1TestOutput, p2TestOutput] = solve(testInput);
  auto [p1Output, p2Output] = solve(input);

  std::cout << "Test 1: " << p1TestOutput << std::endl;
  std::cout << "Part 1: " << p1Output << std::endl;
  std::cout << "Test 2: " << p2TestOutput << std::endl;
  std::cout << "Part 2: " << p2Output << std::endl;
}
