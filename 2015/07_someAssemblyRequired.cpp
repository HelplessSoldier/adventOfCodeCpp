#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct line {
  std::vector<std::string> inputWires;
  std::string outputWire;
  std::vector<int> numbers;
  std::string operation;
};

std::vector<std::string> splitSubstrings(const std::string &line,
                                         std::string delimiter) {
  std::vector<std::string> result;
  size_t right = 0;
  size_t left = 0;
  std::string substring;
  while ((right = line.find(delimiter, left)) != std::string::npos) {
    result.push_back(line.substr(left, right - left));
    left = right + delimiter.length();
  }
  result.push_back(line.substr(left));
  return result;
}

bool fitsRange(std::string str, char low, char high) {
  for (char c : str) {
    if (!(c >= low && c <= high))
      return false;
  }
  return true;
}

bool isWire(std::string str) { return fitsRange(str, 'a', 'z'); }
bool isNumber(std::string str) { return fitsRange(str, '0', '9'); }
bool isOperation(std::string str) { return fitsRange(str, 'A', 'Z'); }
bool isArrow(std::string str) { return str == "->"; }

line parseLine(std::string currentLine) {
  line result;

  std::vector<std::string> substrings = splitSubstrings(currentLine, " ");
  bool arrowSeen = false;

  for (std::string substr : substrings) {
    if (isWire(substr)) {
      if (arrowSeen) {
        assert(result.outputWire.size() == 0);
        result.outputWire = substr;
      } else {
        result.inputWires.push_back(substr);
      }
    } else if (isNumber(substr)) {
      assert(!arrowSeen);
      result.numbers.push_back(std::stoi(substr));
    } else if (isOperation(substr)) {
      assert(result.operation.size() == 0);
      assert(!arrowSeen);
      result.operation = substr;
    } else if (isArrow(substr)) {
      assert(!arrowSeen);
      arrowSeen = true;
    }
  }

  return result;
}

struct node {
  int value = -1;
  std::vector<std::shared_ptr<node>> parents;
  std::shared_ptr<node> child;
  std::string operation;
  std::string childWire;
  std::vector<std::string> parentWires;

  node(const std::string &wireName) : childWire(wireName){};
};

std::shared_ptr<node>
getOrCreateNode(std::unordered_map<std::string, std::shared_ptr<node>> &wiremap,
                const std::string &wirename) {
  if (wiremap.find(wirename) == wiremap.end()) {
    wiremap[wirename] = std::make_shared<node>(wirename);
  }
  return wiremap[wirename];
}

void buildGraph(std::vector<node> &nodes, const std::vector<line> &lines) {
  assert(!lines.empty());
  assert(nodes.empty());

  std::unordered_map<std::string, std::shared_ptr<node>> wireMap;
  for (const line &l : lines) {
    std::shared_ptr<node> outputNode = getOrCreateNode(wireMap, l.outputWire);

    if (l.operation.empty()) {
      outputNode->operation = "ASSIGN";
    } else {
      outputNode->operation = l.operation;
    }

    for (const std::string &inputWire : l.inputWires) {
      std::shared_ptr<node> parentNode = getOrCreateNode(wireMap, inputWire);
      outputNode->parents.push_back(parentNode);
    }

    for (int number : l.numbers) {
      std::shared_ptr<node> literalNode = std::make_shared<node>("literal");
      literalNode->value = number;
      outputNode->parents.push_back(literalNode);
    }
  }

  for (const auto &pair : wireMap) {
    nodes.push_back(*pair.second);
  }
}

void propogateValues(std::vector<node> &nodes) {}

int main() {
  std::ifstream file("07_input.txt");
  // std::ifstream file("07_testInput.txt");

  std::string currentLine;

  if (!file.is_open()) {
    std::cerr << "Could not open file" << std::endl;
    return 1;
  }

  std::vector<line> lines;
  while (std::getline(file, currentLine)) {
    lines.push_back(parseLine(currentLine));
  }
  file.close();

  std::vector<node> nodes;
  buildGraph(nodes, lines);
  propogateValues(nodes);
}
