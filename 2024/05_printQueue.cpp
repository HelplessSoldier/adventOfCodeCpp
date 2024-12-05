#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::tuple<std::vector<std::vector<int>>,
           std::unordered_map<int, std::set<int>>>
loadData(std::string filepath) {
  std::string line;
  std::fstream file(filepath);

  std::unordered_map<int, std::set<int>> orderRulesMap;
  while (getline(file, line)) {
    if (line.empty())
      break;

    std::stringstream ss(line);
    std::string substr;
    std::vector<std::string> currLine;
    while (getline(ss, substr, '|')) {
      currLine.push_back(substr);
    }

    orderRulesMap[std::stoi(currLine[0])].insert(std::stoi(currLine[1]));
  }

  std::vector<std::vector<int>> printOrders;
  while (getline(file, line)) {
    std::stringstream ss(line);
    std::string substr;
    std::vector<int> currLine;
    while (getline(ss, substr, ',')) {
      currLine.push_back(std::stoi(substr));
    }
    printOrders.push_back(currLine);
  }

  return {printOrders, orderRulesMap};
}

bool isValidLineP1(
    const std::vector<int> &line,
    const std::unordered_map<int, std::set<int>> &orderRulesMap) {
  std::unordered_set<int> seen;
  for (const int value : line) {
    seen.insert(value);
    auto it = orderRulesMap.find(value);
    if (it == orderRulesMap.end()) {
      // no rule for this value
      continue;
    }

    const std::set<int> &cannotPrecede = it->second;
    for (const int &val : cannotPrecede) {
      if (seen.find(val) != seen.end()) {
        // a value preceded this one that broke the ordering rules
        return false;
      }
    }
  }

  return true;
}

std::tuple<std::vector<std::vector<int>>, std::vector<std::vector<int>>>
separateIntoValidAndInvalid(
    const std::vector<std::vector<int>> &printOrders,
    const std::unordered_map<int, std::set<int>> &orderRulesMap) {

  std::vector<std::vector<int>> valids, invalids;
  for (const std::vector<int> &line : printOrders) {
    if (isValidLineP1(line, orderRulesMap)) {
      valids.push_back(line);
    } else {
      invalids.push_back(line);
    }
  }
  return {valids, invalids};
}

std::vector<int>
fixLine(const std::vector<int> &line,
        const std::unordered_map<int, std::set<int>> &orderRulesMap) {

  std::vector<int> fixedLine(line);

  while (!isValidLineP1(fixedLine, orderRulesMap)) {

    std::unordered_map<int, int> valueToIdx;
    for (int i = 0; i < fixedLine.size(); ++i) {
      valueToIdx[fixedLine[i]] = i;
    }

    for (int i = 0; i < fixedLine.size(); ++i) {
      auto it = orderRulesMap.find(fixedLine[i]);
      if (it != orderRulesMap.end()) {
        const std::set<int> &cannotPrecede = it->second;
        for (const int &val : cannotPrecede) {
          if (valueToIdx.find(val) != valueToIdx.end()) {
            int otherIdx = valueToIdx[val];
            if (otherIdx < i) {
              std::swap(fixedLine[otherIdx], fixedLine[i]);
              valueToIdx[fixedLine[otherIdx]] = otherIdx;
              valueToIdx[fixedLine[i]] = i;
            }
          }
        }
      }
    }
  }
  return fixedLine;
}

int part1(const std::vector<std::vector<int>> &validLines) {
  int result = 0;
  for (const std::vector<int> &line : validLines) {
    int middle = line.size() / 2;
    result += line[middle];
  }
  return result;
}

int part2(const std::vector<std::vector<int>> &invalidLines,
          const std::unordered_map<int, std::set<int>> &orderRulesMap) {
  int result = 0;

  for (const std::vector<int> &line : invalidLines) {
    std::vector fixed = fixLine(line, orderRulesMap);
    int middle = fixed.size() / 2;
    result += fixed[middle];
  }

  return result;
}

int main() {
  auto [printOrders, orderRulesMap] = loadData("./05_input.txt");

  auto [validLines, invalidLines] =
      separateIntoValidAndInvalid(printOrders, orderRulesMap);

  std::cout << "Part 1: " << part1(validLines) << std::endl;

  std::cout << "Part 2: " << part2(invalidLines, orderRulesMap) << " ";
}
