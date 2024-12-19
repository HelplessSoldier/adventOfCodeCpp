#include <cassert>
#include <chrono>
#include <climits>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct InputType {
  std::vector<std::string> towels;
  std::vector<std::string> patterns;
};

InputType loadInput(std::string filepath) {
  InputType result;
  std::fstream file(filepath);
  std::string line;
  // towels
  getline(file, line);
  std::stringstream towels(line);
  std::string towel;
  while (std::getline(towels, towel, ',')) {
    towel.erase(0, towel.find_first_not_of(' '));
    result.towels.push_back(towel);
  }

  // empty
  getline(file, line);

  // patterns
  while (getline(file, line)) {
    result.patterns.push_back(line);
  }

  return result;
}

bool canMakePattern(const std::string &pattern,
                    const std::vector<std::string> &towels,
                    std::set<std::string> &seen, std::string curr = "") {

  if (seen.find(curr) != seen.end())
    return false;
  seen.insert(curr);

  if (curr == pattern)
    return true;

  if (curr.size() > pattern.size())
    return false;

  for (int i = 0; i < curr.size(); ++i) {
    if (curr[i] != pattern[i]) {
      return false;
    }
  }

  int nextIndex = curr.size();
  for (const std::string &towel : towels) {
    if (nextIndex + towel.size() > pattern.size() ||
        pattern.compare(nextIndex, towel.size(), towel) != 0) {
      continue;
    }

    std::string newStr = curr + towel;
    if (canMakePattern(pattern, towels, seen, newStr)) {
      return true;
    }
  }

  return false;
}

long long countPatterns(const std::string &pattern,
                        const std::vector<std::string> &towels,
                        std::unordered_map<std::string, long long> &memo,
                        std::string curr = "") {

  if (memo.find(curr) != memo.end())
    return memo[curr];

  if (curr == pattern)
    return memo[curr] = 1;

  if (curr.size() > pattern.size())
    return memo[curr] = 0;

  for (int i = 0; i < curr.size(); ++i) {
    if (curr[i] != pattern[i]) {
      return memo[curr] = 0;
    }
  }

  long long count = 0;
  int nextIndex = curr.size();
  for (const std::string &towel : towels) {
    if (nextIndex + towel.size() > pattern.size() ||
        pattern.compare(nextIndex, towel.size(), towel) != 0) {
      continue;
    }

    std::string newStr = curr + towel;
    count += countPatterns(pattern, towels, memo, newStr);
  }
  return memo[curr] = count;
}

std::pair<int, long long> solve(InputType input) {
  int result1 = 0;
  long long result2 = 0;
  for (const std::string &pattern : input.patterns) {
    std::set<std::string> seen;
    std::unordered_map<std::string, long long> memo;
    if (canMakePattern(pattern, input.towels, seen)) {
      result1++;
      result2 += countPatterns(pattern, input.towels, memo);
    }
  }
  return {result1, result2};
}

int main() {
  auto start = std::chrono::high_resolution_clock::now();

  InputType testInput = loadInput("./19_testInput.txt");
  std::pair<int, long long> testResults = solve(testInput);
  std::cout << "Test 1: " << testResults.first << std::endl;
  std::cout << "Test 2: " << testResults.second << std::endl;

  InputType input = loadInput("./19_input.txt");
  std::pair<int, long long> results = solve(input);
  std::cout << "Part 1: " << results.first << std::endl;
  std::cout << "Part 2: " << results.second << std::endl;

  auto end = std::chrono::high_resolution_clock::now();
  auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();
  std::cout << "Completed in " << time << "ms" << std::endl;
}
