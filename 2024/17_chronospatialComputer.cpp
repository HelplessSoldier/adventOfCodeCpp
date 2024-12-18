/*
 * this is incomplete for part 2, currently attempts to brute force.
 */

#include <cassert>
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define OP_ADV 0
#define OP_BXL 1
#define OP_BST 2
#define OP_JNZ 3
#define OP_BXC 4
#define OP_OUT 5
#define OP_BDV 6
#define OP_CDV 7

class Computer {
public:
  Computer(std::string filepath) : ic(0), ra(0), rb(0), rc(0), output({}) {
    std::fstream file(filepath);
    std::string line;

    // register A
    getline(file, line);
    line.erase(0, 12);
    ra = std::stoi(line);

    // register B
    getline(file, line);
    line.erase(0, 12);
    rb = stoi(line);

    // register C
    getline(file, line);
    line.erase(0, 12);
    rc = stoi(line);

    // program
    getline(file, line);
    getline(file, line);
    line.erase(0, 9);
    for (int i = 0; i < line.size(); i += 2) {
      flatProgram.push_back(static_cast<int>(line[i] - '0'));
    }
    for (int i = 0; i < flatProgram.size(); i += 2) {
      std::vector<int> pair = {flatProgram[i], flatProgram[i + 1]};
      program.push_back(pair);
    }
  }

  std::string getOutput() {
    if (output.empty()) {
      return "";
    }

    std::string result;
    for (const int &val : output) {
      result.push_back(static_cast<char>(val + '0'));
      result.push_back(',');
    }
    result.erase(result.size() - 1);
    return result;
  }

  void run() {
    while (true) {
      if (ic < 0 || ic >= program.size())
        break;

      performOp(program[ic++]);
    }
  }

  long findOriginalRegisterA() {
    long checking = 0;
    while (true) {
      assert(checking != LONG_MAX && "overflow");

      ra = checking;
      rb = 0;
      rc = 0;
      ic = 0;

      run();
      if (outputAndProgramTheSame())
        return checking;

      ++checking;
      output.clear();
    }
  }

private:
  long ic, ra, rb, rc;
  std::vector<int> flatProgram;
  std::vector<std::vector<int>> program;
  std::vector<int> output;
  const int BITMASK3 = 0x7;

  bool outputAndProgramTheSame() {
    if (output.size() != flatProgram.size()) {
      return false;
    }
    for (int i = 0; i < output.size(); ++i) {
      if (output[i] != flatProgram[i]) {
        return false;
      }
    }
    return true;
  }

  int combo(int operand) {
    if (operand >= 0 && operand <= 3) {
      return operand;
    } else if (operand == 4) {
      return ra;
    } else if (operand == 5) {
      return rb;
    } else if (operand == 6) {
      return rc;
    } else if (operand == 7) {
      assert(false && "Hit reserved operand in combo");
    } else {
      assert(false && "Wtf");
    }
  }

  // 0 division ra / 2^combo -> ra
  void adv(int operand) { ra = ra / std::pow(2, operand); }

  // 1 bitwise xor of rb and literal operand -> rb
  void bxl(int operand) { rb = rb ^ operand; }

  // 2 combo % 8 -> rb
  void bst(int operand) { rb = combo(operand) & BITMASK3; }

  // 3 if ra == 0, nothing. else set literal -> ic
  void jnz(int operand) { ic = ra == 0 ? ic : operand; }

  // 4 bitwise xor of rb and rc -> rb
  void bxc(int operand) { rb = rb ^ rc; }

  // 5 combo % 8 -> output (csv)
  void out(int operand) { output.push_back(combo(operand) & BITMASK3); }

  // 6 division ra / 2^combo -> rb
  void bdv(int operand) { rb = ra / std::pow(2, combo(operand)); }

  // 7 division ra / 2^combo -> rc
  void cdv(int operand) { rc = ra / std::pow(2, combo(operand)); }

  void performOp(std::vector<int> instruction) {
    int operation = instruction[0];
    int operand = instruction[1];

    switch (operation) {
    case OP_ADV:
      adv(operand);
      break;
    case OP_BXL:
      bxl(operand);
      break;
    case OP_BST:
      bst(operand);
      break;
    case OP_JNZ:
      jnz(operand);
      break;
    case OP_BXC:
      bxc(operand);
      break;
    case OP_OUT:
      out(operand);
      break;
    case OP_BDV:
      bdv(operand);
      break;
    case OP_CDV:
      cdv(operand);
      break;
    default:
      assert(false && "Unknown op code");
    }
  }
};

int main() {
  /*
  Computer testComputer("./17_testInput.txt");
  testComputer.run();
  std::cout << "Test 1: " << testComputer.getOutput() << std::endl;

  Computer testComputer2("./17_testInput2.txt");
  std::cout << "Test 2: " << testComputer2.findOriginalRegisterA() << std::endl;
  */

  Computer computer("./17_input.txt");
  computer.run();
  std::cout << "Part 1: " << computer.getOutput() << std::endl;
  std::cout << "Part 2: " << computer.findOriginalRegisterA() << std::endl;
}
