#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct InputLine {
  long target;
  std::vector<long> inputs;
};

enum Operations { ADD, MUL, CAT };

std::vector<InputLine> loadInput(std::string filepath) {
  std::fstream file(filepath);
  std::string line;
  std::vector<InputLine> result;
  while (getline(file, line)) {
    InputLine current;
    auto it = line.find(':');
    line.erase(it, 1);

    std::stringstream ss(line);
    ss >> current.target;

    long input;
    while (ss >> input) {
      current.inputs.push_back(input);
    }
    result.push_back(current);
  }
  return result;
}

long concatInts(long a, long b) {
  int aDigitCount = 0;
  long temp = b;
  while (temp > 0) {
    aDigitCount++;
    temp /= 10;
  }

  for (int i = 0; i < aDigitCount; ++i) {
    a *= 10;
  }
  a += b;

  return a;
}

long evaluate(const std::vector<long> &inputs,
              const std::vector<Operations> &operations) {

  long result = inputs[0];
  for (int i = 1; i < inputs.size(); ++i) {
    long num = inputs[i];
    Operations currOp = operations[i - 1];
    switch (currOp) {
    case ADD:
      result += num;
      break;
    case MUL:
      result *= num;
      break;
    case CAT:
      result = concatInts(result, num);
      break;
    }
  }

  return result;
}

void p1PermuteOperations(std::vector<Operations> &operations) {
  for (int i = operations.size() - 1; i >= 0; --i) {
    if (operations[i] == ADD) {
      operations[i] = MUL;
      return;
    } else {
      operations[i] = ADD;
    }
  }
}

void p2PermuteOperations(std::vector<Operations> &operations) {
  for (int i = operations.size() - 1; i >= 0; --i) {
    if (operations[i] == ADD) {
      operations[i] = MUL;
      return;
    } else if (operations[i] == MUL) {
      operations[i] = CAT;
    } else {
      operations[i] = ADD;
    }
  }
}

bool p1Valid(const InputLine &input) {
  int numOperations = input.inputs.size() - 1;

  if (input.inputs.size() == 1 && input.inputs[0] == input.target) {
    return true;
  }

  std::vector<Operations> operations(numOperations, ADD);
  long maxPermutations = std::pow(2, numOperations);

  for (int i = 0; i < maxPermutations; ++i) {
    if (evaluate(input.inputs, operations) == input.target) {
      return true;
    }
    p1PermuteOperations(operations);
  }

  return false;
}

long long part1(const std::vector<InputLine> &inputs) {
  long long result = 0;
  for (const InputLine &input : inputs) {
    if (p1Valid(input)) {
      result += input.target;
    }
  }
  return result;
}

bool p2Valid(const InputLine &input) {
  if (p1Valid(input)) {
    return true;
  }

  int numOps = input.inputs.size() - 1;
  std::vector<Operations> ops(numOps, ADD);
  int maxPermutations = std::pow(3, numOps);

  for (int i = 0; i < maxPermutations; ++i) {
    long evaluated = evaluate(input.inputs, ops);
    if (evaluated == input.target) {
      return true;
    }
    p2PermuteOperations(ops);
  }

  return false;
}

long long part2(const std::vector<InputLine> &inputs) {
  long long result = 0;
  for (const InputLine &input : inputs) {
    if (p2Valid(input)) {
      result += input.target;
    }
  }
  return result;
}

int main() {
  std::vector<InputLine> inputs = loadInput("./07_input.txt");
  // clang-format off
  std::vector<InputLine> testInput = {
    {190, {10, 19}},
    {3267, {81, 40, 27}},
    {83, {17, 5}},
    {156, {15, 6}},
    {7290, {6, 8, 6, 15}},
    {161011, {16, 10, 13}},
    {192, {17, 8, 14}},
    {21037, {9, 7, 18, 13}},
    {292, {11, 6, 16, 20}}};
  // clang-format on

  std::cout << "Test 1: " << part1(testInput) << std::endl;
  std::cout << "Part 1: " << part1(inputs) << std::endl;
  std::cout << "Test 2: " << part2(testInput) << std::endl;
  std::cout << "Part 2: " << part2(inputs) << std::endl;
}
