#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::string loadData(std::string filepath) {
  std::fstream file(filepath);
  std::string line, result;

  while (getline(file, line)) {
    result.append(line);
  }

  return result;
}

std::vector<std::pair<int, int>> inputNums(const std::string &input,
                                           bool respectConditionals) {
  std::regex mulPattern(R"(mul\((\d+),(\d+)\))");
  std::regex doPattern(R"(do\(\))");
  std::regex dontPattern(R"(don't\(\))");

  bool enabledInputs = true;
  std::vector<std::pair<int, int>> result;

  for (int i = 0; i < input.size(); ++i) {
    // do and don't
    if (respectConditionals && input[i] == 'd') {
      int conditionalEnd = i;
      while (conditionalEnd < input.size() && input[conditionalEnd] != ')') {
        ++conditionalEnd;
      }

      std::string substring = input.substr(i, conditionalEnd - i + 1);
      if (std::regex_match(substring, doPattern)) {
        enabledInputs = true;
      } else if (std::regex_match(substring, dontPattern)) {
        enabledInputs = false;
      }
    }

    // mul patterns
    if (enabledInputs && input[i] == 'm') {

      int nextClosingBrace = i;

      while (nextClosingBrace < input.size() &&
             input[nextClosingBrace] != ')') {
        ++nextClosingBrace;
      }

      if (nextClosingBrace < input.size() && input[nextClosingBrace] == ')') {
        std::string substring = input.substr(i, nextClosingBrace - i + 1);
        std::smatch matches;
        if (std::regex_match(substring, matches, mulPattern)) {
          int left = std::stoi(matches[1].str());
          int right = std::stoi(matches[2].str());

          result.push_back({left, right});
        }
      }
    }
  }
  return result;
}

int solve(const std::string &input, bool respectConditionals) {
  std::vector<std::pair<int, int>> numPairs =
      inputNums(input, respectConditionals);

  int result = 0;
  for (const std::pair<int, int> &nums : numPairs) {
    result += nums.first * nums.second;
  }
  return result;
}

int main() {
  std::string input = loadData("./03_input.txt");
  std::cout << "Part 1: " << solve(input, false) << std::endl;
  std::cout << "Part 2: " << solve(input, true) << std::endl;
}
