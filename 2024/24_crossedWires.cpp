#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

enum GateType { AND, OR, XOR };

struct Wire;

struct Gate {
  std::shared_ptr<Wire> parentLeft, parentRight, child;
  GateType type;
  int value;
};

struct Wire {
  std::string name;
  std::shared_ptr<Gate> parent, child;
  int value;

  Wire(const std::string &name, int value = -1,
       std::shared_ptr<Gate> parent = nullptr,
       std::shared_ptr<Gate> child = nullptr)
      : name(name), parent(parent), child(child), value(value) {}
};

typedef std::set<std::shared_ptr<Gate>> GateSet;
typedef std::set<std::shared_ptr<Wire>> WireSet;

GateType strToGate(const std::string &str) {
  if (str == "OR") {
    return OR;
  } else if (str == "XOR") {
    return XOR;
  } else if (str == "AND") {
    return AND;
  } else {
    throw std::logic_error("wat");
  }
}

std::pair<GateSet, WireSet> buildGraph(std::string filepath) {
  GateSet gates;
  WireSet wires;

  std::fstream file(filepath);
  std::string line;

  bool inFirstChunk = true;
  while (getline(file, line)) {
    if (line.empty()) {
      inFirstChunk = false;
      continue;
    }

    if (inFirstChunk) {

      std::stringstream ss(line);

      std::string name;
      std::string value;

      std::getline(ss, name, ':');
      std::getline(ss, value);

      bool val = value[1] - '0';

      std::shared_ptr<Wire> newWire = std::make_shared<Wire>(name, val);
      wires.insert(newWire);

    } else {

      std::stringstream ss(line);
      std::string parentLeft, parentRight, child, arrow, operation;

      std::getline(ss, parentLeft, ' ');
      std::getline(ss, operation, ' ');
      std::getline(ss, parentRight, ' ');
      std::getline(ss, arrow, ' ');
      std::getline(ss, child, ' ');

      std::shared_ptr<Gate> newGate = std::make_shared<Gate>();

      auto leftParentIt = std::find_if(wires.begin(), wires.end(),
                                       [&](const std::shared_ptr<Wire> &t) {
                                         return t->name == parentLeft;
                                       });

      auto rightParentIt = std::find_if(wires.begin(), wires.end(),
                                        [&](const std::shared_ptr<Wire> &t) {
                                          return t->name == parentRight;
                                        });

      auto childIt = std::find_if(
          wires.begin(), wires.end(),
          [&](const std::shared_ptr<Wire> &t) { return t->name == child; });

      if (leftParentIt != wires.end()) {
        newGate->parentLeft = *leftParentIt;
      } else {
        newGate->parentLeft = std::make_shared<Wire>(parentLeft);
        wires.insert(newGate->parentLeft);
      }

      if (rightParentIt != wires.end()) {
        newGate->parentRight = *rightParentIt;
      } else {
        newGate->parentRight = std::make_shared<Wire>(parentRight);
        wires.insert(newGate->parentRight);
      }

      if (childIt != wires.end()) {
        newGate->child = *childIt;
      } else {
        newGate->child = std::make_shared<Wire>(child);
        wires.insert(newGate->child);
      }

      newGate->type = strToGate(operation);

      gates.insert(newGate);
    }
  }

  return {gates, wires};
}

void propogateOperations(GateSet gates, WireSet wires) {
  bool allSet = false;
  while (!allSet) {

    bool madeChange = false;

    for (auto &gate : gates) {
      // already been set
      if (gate->child->value != -1)
        continue;

      // can set
      if (gate->parentLeft->value != -1 && gate->parentRight->value != -1) {
        switch (gate->type) {
        case AND:
          gate->child->value =
              gate->parentLeft->value && gate->parentRight->value;
          break;
        case OR:
          gate->child->value =
              gate->parentLeft->value || gate->parentRight->value;
          break;
        case XOR:
          gate->child->value =
              gate->parentLeft->value ^ gate->parentRight->value;
          break;
        }
        madeChange = true;
      }
    }

    if (!madeChange)
      allSet = true;
  }
}

long getOutput(WireSet wires, char leader) {
  std::vector<std::shared_ptr<Wire>> outWires;
  for (auto wire : wires) {
    if (wire->name[0] == leader) {
      outWires.push_back(wire);
    }
  }

  std::sort(outWires.begin(), outWires.end(),
            [&](const std::shared_ptr<Wire> &a,
                const std::shared_ptr<Wire> &b) { return a->name > b->name; });

  long result = 0;
  for (auto wire : outWires) {
    result = (result << 1) | wire->value;
  }

  return result;
}

int main() {
  auto [testGates, testWires] = buildGraph("./24_testInput.txt");
  propogateOperations(testGates, testWires);
  long testResult = getOutput(testWires, 'z');
  std::cout << "Test 1: " << testResult << std::endl;

  auto [gates, wires] = buildGraph("./24_input.txt");
  propogateOperations(gates, wires);
  long result1 = getOutput(wires, 'z');
  std::cout << "Part 1: " << result1 << std::endl;
}
