#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
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
  std::shared_ptr<Wire> originalParentLeft, originalParentRight, originalChild;
  GateType type;
  int value, originalValue;
};

struct Wire {
  std::string name;
  std::shared_ptr<Gate> parent, child;
  std::shared_ptr<Gate> originalParent, originalChild;
  int value, originalValue;

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

std::string gateToStr(GateType type) {
  switch (type) {
  case AND:
    return "AND";
  case OR:
    return "OR";
  case XOR:
    return "XOR";
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

  for (auto &gate : gates) {
    gate->originalValue = gate->value;
    gate->originalParentLeft = gate->parentLeft;
    gate->originalParentRight = gate->parentRight;
    gate->originalChild = gate->child;
  }

  for (auto &wire : wires) {
    wire->originalValue = wire->value;
    wire->originalParent = wire->parent;
    wire->originalChild = wire->child;
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

long part1(std::string filepath) {
  auto [gates, wires] = buildGraph(filepath);
  propogateOperations(gates, wires);
  return getOutput(wires, 'z');
}

void generateDotFile(const GateSet &gates, const WireSet &wires,
                     const std::string &filename) {
  std::ofstream out(filename);

  out << "digraph Circuit {\n";

  std::map<std::shared_ptr<Gate>, int> gateIDs;
  std::map<std::shared_ptr<Wire>, int> wireIDs;

  int gateCounter = 0;
  for (auto &g : gates) {
    gateIDs[g] = gateCounter++;
  }

  int wireCounter = 0;
  for (auto &w : wires) {
    wireIDs[w] = wireCounter++;
  }

  for (auto &g : gates) {
    int id = gateIDs[g];
    std::string fillColor;
    if (g->type == XOR) {
      fillColor = "red";
    } else if (g->type == AND) {
      fillColor = "green";
    } else {
      fillColor = "lightBlue";
    }

    out << "  gate" << id
        << " [shape=rectangle, style=filled, fillcolor=" << fillColor << ", "
        << "label=\"" << gateToStr(g->type) << "\\nGate" << id << "\"];\n";
  }

  for (auto &w : wires) {
    int id = wireIDs[w];
    out << "  wire" << id
        << " [shape=ellipse, style=filled, fillcolor=lightgray, " << "label=\""
        << w->name << "\"];\n";
  }

  for (auto &g : gates) {
    int gateId = gateIDs[g];

    if (g->parentLeft) {
      int leftWireId = wireIDs[g->parentLeft];
      out << "  wire" << leftWireId << " -> gate" << gateId << " [label=\""
          << g->parentLeft->name << "\"];\n";
    }

    if (g->parentRight) {
      int rightWireId = wireIDs[g->parentRight];
      out << "  wire" << rightWireId << " -> gate" << gateId << " [label=\""
          << g->parentRight->name << "\"];\n";
    }

    if (g->child) {
      int childWireId = wireIDs[g->child];
      out << "  gate" << gateId << " -> wire" << childWireId << " [label=\""
          << g->child->name << "\"];\n";
    }
  }

  out << "}\n";
  out.close();
}

std::string part2(std::string filepath) {
  auto [gates, wires] = buildGraph(filepath);

  generateDotFile(gates, wires, "24_viz.dot");

  return "unimpl";
}

int main() {
  std::cout << "Test 1: " << part1("./24_testInput.txt") << std::endl;
  std::cout << "Part 1: " << part1("./24_input.txt") << std::endl;

  /* error bits from earlier attempt
   z07
   z11
   z18
   z35
      */

  // just manually found these from the viz...
  // z35 cfk  fixes z35
  // cbj qjj  fixes z11
  // gmt z07  fixes z07
  // dmn z18  fixes z17

  std::vector<std::string> swaps = {"z35", "cfk", "cbj", "qjj",
                                    "gmt", "z07", "dmn", "z18"};

  std::sort(swaps.begin(), swaps.end());
  for (auto swap : swaps) {
    std::cout << swap << ",";
  }
  std::cout << std::endl;
}
