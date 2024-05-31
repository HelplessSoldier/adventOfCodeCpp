// this could have been simplified using lambdas for each operation.

#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct pos {
  int x = 0;
  int y = 0;
};

enum operations { on, off, toggle, null };

struct command {
  operations op;
  pos pos1;
  pos pos2;
  friend std::ostream &operator<<(std::ostream &os, const command &cmd);
};

std::ostream &operator<<(std::ostream &os, const command &cmd) {
  const char *op_str = "";
  switch (cmd.op) {
  case on:
    op_str = "on";
    break;
  case off:
    op_str = "off";
    break;
  case toggle:
    op_str = "toggle";
    break;
  case null:
    op_str = "null";
    break;
  }

  os << "Operation: " << op_str << " ";
  os << "From (" << cmd.pos1.x << ", " << cmd.pos1.y << ") ";
  os << "to (" << cmd.pos2.x << ", " << cmd.pos2.y << ")";
  return os;
}

struct adjustedRange {
  int start_x = 0;
  int start_y = 0;
  int end_x = 0;
  int end_y = 0;
};

adjustedRange getRange(const command &comm) {
  adjustedRange range;
  range.start_x = std::min(comm.pos1.x, comm.pos2.x);
  range.end_x = std::max(comm.pos1.x, comm.pos2.x);
  range.start_y = std::min(comm.pos1.y, comm.pos2.y);
  range.end_y = std::max(comm.pos1.y, comm.pos2.y);
  return range;
}

void toggleSquareBits(std::vector<std::bitset<1000>> &lights,
                      const command &comm) {
  adjustedRange range = getRange(comm);
  for (int x = range.start_x; x <= range.end_x; ++x) {
    for (int y = range.start_y; y <= range.end_y; ++y) {
      lights[x][y].flip();
    }
  }
}

void turnOnSquareBits(std::vector<std::bitset<1000>> &lights,
                      const command &comm) {
  adjustedRange range = getRange(comm);
  for (int x = range.start_x; x <= range.end_x; ++x) {
    for (int y = range.start_y; y <= range.end_y; ++y) {
      lights[x][y] = 1;
    }
  }
}

void turnOffSquareBits(std::vector<std::bitset<1000>> &lights,
                       const command &comm) {
  adjustedRange range = getRange(comm);
  for (int x = range.start_x; x <= range.end_x; ++x) {
    for (int y = range.start_y; y <= range.end_y; ++y) {
      lights[x][y] = 0;
    }
  }
}

int countLitLights(std::vector<std::bitset<1000>> &lights) {
  int res = 0;
  for (auto layer : lights) {
    res += layer.count();
  }
  return res;
}

void parseNumbers(const std::string &line, int &num1, int &num2) {
  std::istringstream iss(line);
  std::string part;

  std::getline(iss, part, ',');
  num1 = std::stoi(part);

  std::getline(iss, part, ',');
  num2 = std::stoi(part);
}

command parseLine(const std::string &line) {
  std::stringstream stream(line);
  std::string curr;

  pos pos1, pos2;
  command comm = {null, pos1, pos2};

  // get operation
  stream >> curr;
  if (curr == "toggle") {
    comm.op = toggle;
  } else if (curr == "turn") {
    stream >> curr;
    if (curr == "on") {
      comm.op = on;
    } else if (curr == "off") {
      comm.op = off;
    }
  }

  // get point 1
  stream >> curr;
  int pos1_1, pos1_2;
  parseNumbers(curr, pos1_1, pos1_2);
  comm.pos1.x = pos1_1;
  comm.pos1.y = pos1_2;

  stream >> curr; // skip "Through"
  stream >> curr;
  int pos2_1, pos2_2;
  parseNumbers(curr, pos2_1, pos2_2);
  comm.pos2.x = pos2_1;
  comm.pos2.y = pos2_2;

  if (comm.op == null)
    throw new std::runtime_error("Failed to get operation");

  return comm;
}

void part1() {
  std::vector<std::bitset<1000>> lights(1000, std::bitset<1000>(0));

  std::ifstream stream("./06_input.txt");
  std::string line;
  while (std::getline(stream, line)) {
    command currentCommand = parseLine(line);
    switch (currentCommand.op) {
    case on:
      turnOnSquareBits(lights, currentCommand);
      break;
    case off:
      turnOffSquareBits(lights, currentCommand);
      break;
    case toggle:
      toggleSquareBits(lights, currentCommand);
      break;
    case null:
      std::cout << "How?" << std::endl;
      break;
    }
  }

  int count = countLitLights(lights);
  std::cout << "Part 1 answer is: " << count << std::endl;
}

void toggleSquareInts(std::vector<std::vector<int>> &lights,
                      const command &comm) {
  adjustedRange range = getRange(comm);
  for (int x = range.start_x; x <= range.end_x; ++x) {
    for (int y = range.start_y; y <= range.end_y; ++y) {
      lights[x][y] += 2;
    }
  }
}

void turnOnSquareInts(std::vector<std::vector<int>> &lights,
                      const command &comm) {
  adjustedRange range = getRange(comm);
  for (int x = range.start_x; x <= range.end_x; ++x) {
    for (int y = range.start_y; y <= range.end_y; ++y) {
      lights[x][y] += 1;
    }
  }
}

void turnOffSquareInts(std::vector<std::vector<int>> &lights,
                       const command &comm) {
  adjustedRange range = getRange(comm);
  for (int x = range.start_x; x <= range.end_x; ++x) {
    for (int y = range.start_y; y <= range.end_y; ++y) {
      if (lights[x][y] > 0)
        lights[x][y] -= 1;
    }
  }
}

int part2CountLights(std::vector<std::vector<int>> &lights) {
  int res = 0;
  for (auto row : lights) {
    for (auto lightVal : row) {
      res += lightVal;
    }
  }
  return res;
}

void part2() {
  std::vector<std::vector<int>> lights(1000, std::vector<int>(1000, 0));
  std::ifstream stream("./06_input.txt");
  std::string line;
  while (std::getline(stream, line)) {
    command currentCommand = parseLine(line);
    switch (currentCommand.op) {
    case on:
      turnOnSquareInts(lights, currentCommand);
      break;
    case off:
      turnOffSquareInts(lights, currentCommand);
      break;
    case toggle:
      toggleSquareInts(lights, currentCommand);
      break;
    case null:
      std::cout << "Also how?" << std::endl;
      break;
    }
  }
  int count = part2CountLights(lights);
  std::cout << "Part 2 answer is: " << count << std::endl;
}

int main() {
  part1();
  part2();
}
