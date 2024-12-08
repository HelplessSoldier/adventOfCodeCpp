#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef std::vector<std::vector<int>> mat2i;

struct Antenna {
  char label;
  int x, y;
};

struct InputData {
  std::vector<Antenna> antennas;
  int boardWidth, boardHeight;
};

InputData parseInputFromStream(std::istream &in) {
  std::string line;
  int y_pos = 0;
  InputData result;

  while (std::getline(in, line)) {
    result.boardWidth = static_cast<int>(line.size());
    for (int x_pos = 0; x_pos < static_cast<int>(line.size()); ++x_pos) {
      if (line[x_pos] != '.') {
        result.antennas.push_back({line[x_pos], x_pos, y_pos});
      }
    }
    y_pos++;
  }

  result.boardHeight = y_pos;
  return result;
}

InputData loadInput(const std::string &filepath) {
  std::ifstream file(filepath);
  return parseInputFromStream(file);
}

InputData getTestInput() {
  std::string testStr =
      R"(............
........0...
.....0......
.......0....
....0.......
......A.....
............
............
........A...
.........A..
............
............)";

  std::stringstream ss(testStr);
  return parseInputFromStream(ss);
}

void sortInput(InputData &input) {
  std::sort(
      input.antennas.begin(), input.antennas.end(),
      [](const Antenna &a, const Antenna &b) { return a.label < b.label; });
}

std::vector<std::vector<Antenna>> splitAntennasByGroup(const InputData &input) {
  std::vector<std::vector<Antenna>> result;
  std::vector<Antenna> group;
  char currentLabel = input.antennas[0].label;
  for (const Antenna &antenna : input.antennas) {
    if (currentLabel != antenna.label) {
      result.push_back(group);
      group.clear();
      group.shrink_to_fit();
      currentLabel = antenna.label;
    }
    group.push_back(antenna);
  }

  result.push_back(group);

  return result;
};

bool inBounds(int x, int y, const mat2i &arr) {
  return (x < arr[0].size() && x >= 0 && y < arr.size() && y >= 0);
}

void findAntinodes(const std::vector<Antenna> &group, mat2i &nodeMap,
                   bool part1) {

  /*    [y-down, x-right]
   *
   *            # antiNode1
   *                 delx
   *                a---
   *                   | dely
   *                   b
   *
   *                       # antiNode2
   */

  for (int index = 0; index < group.size() - 1; ++index) {
    for (int jndex = index + 1; jndex < group.size(); ++jndex) {

      Antenna a = group[index];
      Antenna b = group[jndex];

      int delx = b.x - a.x;
      int dely = b.y - a.y;

      int antiNode1_x = a.x - delx;
      int antiNode1_y = a.y - dely;

      int antiNode2_x = b.x + delx;
      int antiNode2_y = b.y + dely;

      bool searching1 = true;
      bool searching2 = true;

      if (inBounds(antiNode1_x, antiNode1_y, nodeMap)) {
        nodeMap[antiNode1_y][antiNode1_x]++;
      } else {
        searching1 = false;
      }

      if (inBounds(antiNode2_x, antiNode2_y, nodeMap)) {
        nodeMap[antiNode2_y][antiNode2_x]++;
      } else {
        searching2 = false;
      }

      if (!part1) {
        // include the antennas
        nodeMap[a.y][a.x]++;
        nodeMap[b.y][b.x]++;

        // part2, keep stepping antiNode positions until both oob
        while (searching1 || searching2) {
          antiNode1_x -= delx;
          antiNode1_y -= dely;

          antiNode2_x += delx;
          antiNode2_y += dely;

          if (inBounds(antiNode1_x, antiNode1_y, nodeMap)) {
            nodeMap[antiNode1_y][antiNode1_x]++;
          } else {
            searching1 = false;
          }

          if (inBounds(antiNode2_x, antiNode2_y, nodeMap)) {
            nodeMap[antiNode2_y][antiNode2_x]++;
          } else {
            searching2 = false;
          }
        }
      }
    }
  }
}

int solve(InputData input, bool part1) {
  sortInput(input);
  mat2i nodeMap(input.boardHeight, std::vector<int>(input.boardWidth, 0));
  std::vector<std::vector<Antenna>> groupedAntennas =
      splitAntennasByGroup(input);

  for (const std::vector<Antenna> &group : groupedAntennas) {
    findAntinodes(group, nodeMap, part1);
  }

  int result = 0;
  for (const std::vector<int> &line : nodeMap) {
    for (const int &val : line) {
      if (val != 0) {
        result++;
      }
    }
  }

  return result;
}

int main() {
  auto start = std::chrono::high_resolution_clock::now();

  InputData input = loadInput("./08_input.txt");
  InputData testInput = getTestInput();

  std::cout << "Test 1: " << solve(testInput, /*part1=*/true) << std::endl;
  std::cout << "Part 1: " << solve(input, /*part1=*/true) << std::endl;
  std::cout << "Test 2: " << solve(testInput, /*part1=*/false) << std::endl;
  std::cout << "Part 2: " << solve(input, /*part1=*/false) << std::endl;

  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Completed in: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                     start)
                   .count()
            << "us" << std::endl; // 1850us
}
