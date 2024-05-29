#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string getInput(std::string filepath) {
  std::ifstream file(filepath);
  std::string firstLine;

  if (file.is_open()) {
    std::getline(file, firstLine);
    file.close();
  }

  return firstLine;
}

int findFloor(std::string input) {
  std::stringstream stream(input);

  int floor = 0;
  char current;
  while (stream.get(current)) {
    if (current == '(') {
      ++floor;
    } else if (current == ')') {
      --floor;
    } else {
      std::cerr << "Invalid input character: < " << current << " >"
                << std::endl;
      std::terminate();
    }
  }
  return floor;
}

int main() {

  std::string test1("(())");
  std::string test2("(((");

  std::cout << "Test1 should be 0: " << findFloor(test1) << std::endl;
  std::cout << "Test2 should be 3: " << findFloor(test2) << std::endl;

  std::string input = getInput("./01_input.txt");
  std::cout << "Floor found at: " << findFloor(input) << std::endl;

  return 0;
}
