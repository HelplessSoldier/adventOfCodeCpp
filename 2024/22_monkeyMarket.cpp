#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <sys/types.h>
#include <vector>

std::vector<long> loadInput(std::string filepath) {
  std::vector<long> result;
  std::fstream file(filepath);
  std::string line;
  while (getline(file, line)) {
    result.push_back(std::stoi(line));
  }
  return result;
}

void mix(long input, long &secret) { secret = input ^ secret; }

void prune(long &secret) { secret = secret % 16777216; }

void monkeyHash(long &secret) {
  long step1 = secret * 64;
  mix(step1, secret);
  prune(secret);

  long step2 = secret / 32;
  mix(step2, secret);
  prune(secret);

  long step3 = secret * 2048;
  mix(step3, secret);
  prune(secret);
}

long part1(std::vector<long> input) {
  long result = 0;

  for (int i = 0; i < input.size(); ++i) {
    long secret = input[i];
    for (int j = 0; j < 2000; ++j) {
      monkeyHash(secret);
    }
    result += secret;
  }

  return result;
}

long part2(std::vector<long> input) {

  std::map<std::vector<int>, long> sequenceToPriceMap;

  for (int buyerIndex = 0; buyerIndex < input.size(); ++buyerIndex) {

    long secret = input[buyerIndex];

    std::deque<int> priceSeq(5);
    for (int i = 0; i < priceSeq.size(); ++i) {
      priceSeq[i] = secret % 10;
      monkeyHash(secret);
    }

    std::set<std::vector<int>> seenSequences;
    for (int i = 0; i < 2000 - priceSeq.size() + 1; ++i) {

      std::vector<int> deltas = {
          priceSeq[1] - priceSeq[0], priceSeq[2] - priceSeq[1],
          priceSeq[3] - priceSeq[2], priceSeq[4] - priceSeq[3]};

      if (seenSequences.find(deltas) == seenSequences.end()) {
        sequenceToPriceMap[deltas] += priceSeq[4];
        seenSequences.insert(deltas);
      }

      priceSeq.push_back(secret % 10);
      priceSeq.pop_front();

      monkeyHash(secret);
    }
  }

  long maxPrice = 0;
  for (auto pair : sequenceToPriceMap) {
    maxPrice = std::max(maxPrice, pair.second);
  }

  return maxPrice;
}

int main() {
  std::vector<long> testInput = loadInput("./22_testInput.txt");
  std::vector<long> testInput2 = loadInput("./22_testInput2.txt");
  std::vector<long> input = loadInput("./22_input.txt");

  long test1Result = part1(testInput);
  long part1Result = part1(input);
  long test2Result = part2(testInput2);
  long part2Result = part2(input);

  std::cout << "Test 1: " << test1Result << std::endl;
  std::cout << "Part 1: " << part1Result << std::endl;
  std::cout << "Test 2: " << test2Result << std::endl;
  std::cout << "Part 2: " << part2Result << std::endl;
}
