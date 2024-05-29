#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>

class houses {
private:
  struct loc {
    int x = 0;
    int y = 0;
    bool operator<(const loc &other) const {
      if (x == other.x) {
        return y < other.y;
      }
      return x < other.x;
    }
  };

  std::set<loc> visited;
  loc currentLoc;
  int visitedCount = 1;

  void moveCurrentLoc(char direction, loc &currentloc) {
    switch (direction) {
    case '<':
      --currentLoc.y;
      break;
    case '>':
      ++currentLoc.y;
      break;
    case '^':
      ++currentLoc.x;
      break;
    case 'v':
      --currentLoc.x;
      break;
    default:
      throw new std::runtime_error("Unexpected character in input file");
    }
  }

  std::string getInput(std::string filePath) {
    std::ifstream file(filePath);
    if (!file) {
      throw new std::runtime_error("No open file");
    }
    std::string input;
    std::getline(file, input);
    return input;
  }

  int countHouses(std::string filePath) {
    std::string input = getInput(filePath);
    std::istringstream stream(input);
    visited.insert(currentLoc);

    char currentChar;
    while (stream.get(currentChar)) {
      moveCurrentLoc(currentChar, currentLoc);
      if (visited.insert(currentLoc).second) {
        ++visitedCount;
      }
    }
    return visitedCount;
  }

public:
  int getVisitedHousesCount(std::string filePath) {
    return countHouses(filePath);
  }
};

int main() {
  houses h;
  std::cout << "Visited house count: "
            << h.getVisitedHousesCount("./03_input.txt") << std::endl;
}
