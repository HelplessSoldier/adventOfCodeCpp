#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string getInput(std::string filepath) {
  std::ifstream file(filepath);
  std::string firstLine;
  if (file) {
    std::getline(file, firstLine);
  }
  return firstLine;
}

int findFloor(std::string input) {
  std::stringstream stream(input);

  int charPos = 1;
  int floor = 0;
  bool basementSeen = false;
  char current;

  std::for_each(input.begin(), input.end(), [&](char current) {
    switch (current) {
    case '(':
      ++floor;
      break;
    case ')':
      --floor;
      break;
    default:
      std::cerr << "Invalid input character: < " << current << " >"
                << std::endl;
      std::terminate();
    }

    if (floor < 0 && !basementSeen) {
      std::cout << "First touched basement at charPos: " << charPos
                << std::endl;
      basementSeen = true;
    }
    ++charPos;
  });
  return floor;
}

int main() {
  std::string input = getInput("./01_input.txt");
  std::cout << "Floor found at: " << findFloor(input) << std::endl;

  return 0;
}
