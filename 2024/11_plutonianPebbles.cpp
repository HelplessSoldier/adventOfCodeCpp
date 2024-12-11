#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<long> loadInput(std::string filepath) {
  std::fstream file(filepath);
  std::string line;
  getline(file, line);
  file.close();

  std::stringstream ss(line);
  std::vector<long> result;

  int curr;
  while (ss >> curr) {
    result.push_back(curr);
  }

  return result;
}

int digitCount(const long &value) {
  int digits = 0;
  long temp = value;
  while (temp > 0) {
    digits++;
    temp /= 10;
  }
  return digits;
}

bool evenDigits(const long &value) { return digitCount(value) % 2 == 0; }

std::vector<long> splitEven(const long &value) {
  int halfWidth = digitCount(value) / 2;
  long rightVal = 0;
  long leftVal = value;
  long place = 1;
  for (int i = 0; i < halfWidth; ++i) {
    int digit = leftVal % 10;
    leftVal /= 10;
    rightVal += place * digit;
    place *= 10;
  }

  return {leftVal, rightVal};
}

// yoinked from
// https://www.geeksforgeeks.org/how-to-create-an-unordered_map-of-pairs-in-c/
struct longPairHash {
  size_t operator()(const std::pair<long, long> &pair) const {
    size_t hash1 = std::hash<long>{}(pair.first);
    size_t hash2 = std::hash<long>{}(pair.second);
    return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
  }
};

typedef std::unordered_map<std::pair<long, int>, long, longPairHash> memo;

long pebbleCount(const std::vector<long> &input, const int numBlinks,
                 memo &memoMap) {
  if (numBlinks == 0) {
    return input.size();
  }

  long result = 0;
  for (const long &value : input) {
    if (memoMap.find({value, numBlinks}) != memoMap.end()) {
      // dejavu
      result += memoMap[{value, numBlinks}];
      continue;
    }
    if (value == 0) {
      long count = pebbleCount({1}, numBlinks - 1, memoMap);
      memoMap[{value, numBlinks}] = count;
      result += count;
    } else if (evenDigits(value)) {
      long count = pebbleCount(splitEven(value), numBlinks - 1, memoMap);
      memoMap[{value, numBlinks}] = count;
      result += count;
    } else {
      long count = pebbleCount({value * 2024}, numBlinks - 1, memoMap);
      memoMap[{value, numBlinks}] = count;
      result += count;
    }
  }
  return result;
}

long solve(const std::vector<long> &input, const int numBlinks) {
  memo memoMap;
  return pebbleCount(input, numBlinks, memoMap);
}

int main() {
  auto start = std::chrono::high_resolution_clock::now();

  std::vector<long> testInput = {125, 17};
  std::vector<long> input = loadInput("./11_input.txt");

  std::cout << "Test 1: " << solve(testInput, /*numBlinks=*/25) << std::endl;
  std::cout << "Part 1: " << solve(input, /*numBlinks=*/25) << std::endl;
  std::cout << "Part 2: " << solve(input, /*numBlinks=*/75) << std::endl;

  auto end = std::chrono::high_resolution_clock::now();
  auto time =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Completed in: " << time.count() << "ms" << std::endl; // ~60ms
}
