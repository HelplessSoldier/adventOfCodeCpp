#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool isNiceStringP1(const std::string &input) {
  std::string vowels = "aeiou";
  std::array<std::string, 4> naughtyPairs = {"ab", "cd", "pq", "xy"};

  int doubledCount = 0;
  int vowelCount = 0;
  for (int i = 0; i < input.size(); ++i) {
    // check for vowel
    for (char vowel : vowels) {
      if (input[i] == vowel) {
        ++vowelCount;
        break;
      }
    }

    if (i >= input.size() - 1)
      break;
    std::string checkstr = input.substr(i, 2);

    // check for naughty pairs
    for (std::string naughty : naughtyPairs) {
      if (checkstr == naughty)
        return false;
    }

    // check for doubled
    if (checkstr[0] == checkstr[1])
      ++doubledCount;
  }

  if (vowelCount >= 3 && doubledCount >= 1) {
    return true;
  } else {
    return false;
  }
}

bool hasDoubledPair(const std::string &input) {
  std::vector<std::string> segments;

  for (int i = 0; i < input.size() - 1; ++i) {
    segments.push_back(input.substr(i, 2));
  }

  for (int i = 0; i < segments.size(); ++i) {
    for (int j = i + 2; j < segments.size(); ++j) {
      if (segments[i] == segments[j])
        return true;
    }
  }

  return false;
}

bool hasSamwiched(const std::string &input) {
  for (int i = 0; i < input.size() - 2; ++i) {
    if (input[i] == input[i + 2])
      return true;
  }
  return false;
}

bool isNiceStringP2(const std::string &input) {
  return hasDoubledPair(input) && hasSamwiched(input);
}

int main() {
  std::ifstream stream("./05_input.txt");
  std::string line;

  int niceCountP1 = 0;
  int niceCountP2 = 0;

  while (std::getline(stream, line)) {
    if (isNiceStringP1(line))
      ++niceCountP1;

    if (isNiceStringP2(line))
      ++niceCountP2;
  }

  std::cout << "Part 1 answer: " << niceCountP1 << std::endl;
  std::cout << "Part 2 answer: " << niceCountP2 << std::endl;

  return 0;
}
