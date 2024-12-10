#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef std::vector<std::vector<int>> mat2i;

mat2i loadInput(std::string filepath) {
  std::fstream file(filepath);
  std::string line;
  mat2i result;
  while (getline(file, line)) {
    std::vector<int> row;
    for (int i = 0; i < line.size(); ++i) {
      row.push_back(line[i] - '0');
    }
    result.push_back(row);
  }
  file.close();
  return result;
}

bool inBounds(const mat2i &input, int x, int y) {
  return (x >= 0 && x < input[0].size() && y >= 0 && y < input.size());
}

int trailheadScore(const mat2i &input, mat2i &seenMap, int x, int y,
                   bool part1) {
  int result = 0;
  if (input[y][x] == 9) {
    if (part1) {
      if (seenMap[y][x] == 0) {
        result = 1;
      }
      seenMap[y][x]++;
      return result;
    } else { // part 2
      return 1;
    }
  }

  int currentHeight = input[y][x];
  const mat2i directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

  for (const std::vector<int> &dir : directions) {
    if (!inBounds(input, x + dir[0], y + dir[1]))
      continue;
    if (input[y + dir[1]][x + dir[0]] == currentHeight + 1) {
      result += trailheadScore(input, seenMap, x + dir[0], y + dir[1], part1);
    }
  }
  return result;
}

void resetSeenMap(mat2i &seenMap) {
  for (int i = 0; i < seenMap.size(); ++i) {
    for (int j = 0; j < seenMap[0].size(); ++j) {
      seenMap[i][j] = 0;
    }
  }
}

int solve(const mat2i &input, bool part1) {
  int result = 0;
  mat2i seenMap(input.size(), std::vector<int>(input[0].size(), 0));
  for (int y = 0; y < input.size(); ++y) {
    for (int x = 0; x < input.size(); ++x) {
      if (input[y][x] == 0) {
        result += trailheadScore(input, seenMap, x, y, part1);
        resetSeenMap(seenMap);
      }
    }
  }
  return result;
}

int main() {
  mat2i input = loadInput("./10_input.txt");
  // clang-format off
  mat2i testInput = {
    {8,9,0,1,0,1,2,3},
    {7,8,1,2,1,8,7,4},
    {8,7,4,3,0,9,6,5},
    {9,6,5,4,9,8,7,4},
    {4,5,6,7,8,9,0,3},
    {3,2,0,1,9,0,1,2},
    {0,1,3,2,9,8,0,1},
    {1,0,4,5,6,7,3,2}};
  // clang-format on

  std::cout << "Test 1: " << solve(testInput, /*part1=*/true) << std::endl;
  std::cout << "Part 1: " << solve(input, /*part1=*/true) << std::endl;
  std::cout << "Test 2: " << solve(testInput, /*part1=*/false) << std::endl;
  std::cout << "Part 2: " << solve(input, /*part1=*/false) << std::endl;
}
