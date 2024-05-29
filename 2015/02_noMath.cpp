#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

struct dims {
  int w;
  int l;
  int h;
};

int presentSurfaceArea(const dims &dimensions) {
  dims d = dimensions;
  return 2 * d.l * d.w + 2 * d.w * d.h + 2 * d.h * d.l;
}

int areaOfSmallestPresentSide(const dims &dimensions) {
  dims d = dimensions;
  int side1 = d.w * d.l;
  int side2 = d.l * d.h;
  int side3 = d.h * d.w;
  return std::min(std::min(side1, side2), side3);
}

int paperRequired(const dims &dimensions) {
  return presentSurfaceArea(dimensions) + areaOfSmallestPresentSide(dimensions);
}

dims getDimensions(std::string line) {
  dims res;
  std::replace(line.begin(), line.end(), 'x', ' ');
  std::istringstream iss(line);
  iss >> res.w >> res.l >> res.h;
  return res;
};

int getTotalWrappingPaper(std::string filepath) {
  std::ifstream stream(filepath);
  if (!stream) {
    throw std::runtime_error("Stream failed to open in getTotalWrappingPaper");
  }
  int total = 0;
  std::string line;
  while (std::getline(stream, line)) {
    dims current = getDimensions(line);
    total += paperRequired(current);
  }
  return total;
}

int smallestPerimeter(const dims &dimensions) {
  dims d = dimensions;
  int per1 = d.w + d.w + d.h + d.h;
  int per2 = d.h + d.h + d.l + d.l;
  int per3 = d.l + d.l + d.w + d.w;
  return std::min(std::min(per1, per2), per3);
}

int bowLength(const dims &dimensions) {
  dims d = dimensions;
  return d.h * d.w * d.l;
}

int ribbonRequired(const dims &dimensions) {
  return smallestPerimeter(dimensions) + bowLength(dimensions);
}

int getTotalRibbon(std::string filepath) {
  std::ifstream stream(filepath);
  if (!stream) {
    throw std::runtime_error("Stream failed to open in getTotalRibbon");
  }
  int total = 0;
  std::string line;
  while (std::getline(stream, line)) {
    dims current = getDimensions(line);
    total += ribbonRequired(current);
  }
  return total;
}

int main() {
  int areaReq = getTotalWrappingPaper("./02_input.txt");
  std::cout << "Area required: " << areaReq << std::endl;

  int ribbonReq = getTotalRibbon("./02_input.txt");
  std::cout << "Ribbon required: " << ribbonReq << std::endl;

  return 0;
}
