#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>

class SantaPath {
private:
  struct loc {
    int x;
    int y;
    bool operator<(const loc &other) const {
      if (x == other.x) {
        return y < other.y;
      }
      return x < other.x;
    }
  };

  std::set<loc> visited;
  loc currentLoc_p1 = {0, 0};
  int visitedCount = 1;

  std::set<loc> visitedP2;
  loc currentLocRobot_p2 = {0, 0};
  loc currentLocSanta_p2 = {0, 0};
  int visitedCountP2 = 1;

  void moveCurrentLoc(char direction, loc &currentLoc) {
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

  int countHouses(std::string input) {
    std::istringstream stream(input);
    visited.insert(currentLoc_p1);

    char currentChar;
    while (stream.get(currentChar)) {
      moveCurrentLoc(currentChar, currentLoc_p1);
      if (visited.insert(currentLoc_p1).second) {
        ++visitedCount;
      }
    }
    return visitedCount;
  }

  int countHousesP2(std::string input) {
    char currentChar;
    std::stringstream stream(input);
    std::string santaInput, robotInput;
    visitedP2.insert(currentLocRobot_p2);

    int charpos = 0;
    while (stream.get(currentChar)) {
      if (charpos % 2 == 0) {
        santaInput.push_back(currentChar);
      } else {
        robotInput.push_back(currentChar);
      }
      ++charpos;
    }

    std::stringstream santaStream(santaInput);
    while (santaStream.get(currentChar)) {
      moveCurrentLoc(currentChar, currentLocSanta_p2);
      if (visitedP2.insert(currentLocSanta_p2).second) {
        ++visitedCountP2;
      }
    }

    std::stringstream robotStream(robotInput);
    while (robotStream.get(currentChar)) {
      moveCurrentLoc(currentChar, currentLocRobot_p2);
      if (visitedP2.insert(currentLocRobot_p2).second) {
        ++visitedCountP2;
      }
    }

    return visitedCountP2;
  };

public:
  int getVisitedHousesCount(std::string filePath) {
    std::string input = getInput(filePath);
    return countHouses(input);
  }
  int getVisitedHousesCountP2(std::string filePath) {
    std::string input = getInput(filePath);
    return countHousesP2(input);
  }
};

int main() {
  auto start = std::chrono::high_resolution_clock::now();

  SantaPath s;
  std::cout << "Visited house count: "
            << s.getVisitedHousesCount("./03_input.txt") << std::endl;

  std::cout << "Part 2 visited houses count: "
            << s.getVisitedHousesCountP2("./03_input.txt") << std::endl;

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  std::cout << "Completed in: " << duration << "ms" << std::endl;
}
