#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Vec2 {
  long x, y;
};

struct Robot {
  Vec2 position, velocity;
};

std::vector<Robot> loadInput(std::string filepath) {

  std::vector<Robot> result;

  std::fstream file(filepath);
  std::string line;

  while (getline(file, line)) {
    Robot currentBot = {{-1, -1}, {-1, -1}};

    std::stringstream ss(line);
    std::string posStr;
    ss >> posStr;
    std::string velStr;
    ss >> velStr;

    posStr.erase(posStr.begin(), posStr.begin() + 2);
    velStr.erase(velStr.begin(), velStr.begin() + 2);

    int posDelim = posStr.find(',');
    int velDelim = velStr.find(',');

    posStr[posDelim] = ' ';
    velStr[velDelim] = ' ';

    std::stringstream posStream(posStr);
    std::stringstream velStream(velStr);

    posStream >> currentBot.position.x;
    posStream >> currentBot.position.y;

    velStream >> currentBot.velocity.x;
    velStream >> currentBot.velocity.y;

    result.push_back(currentBot);
  }

  return result;
}

long part1(std::vector<Robot> bots, bool test) {
  int boardWidth = test ? 11 : 101;
  int boardHeight = test ? 7 : 103;
  int seconds = 100;

  for (int i = 0; i < seconds; ++i) {
    for (auto &bot : bots) {
      bot.position.x = (bot.position.x + bot.velocity.x) % boardWidth;
      bot.position.y = (bot.position.y + bot.velocity.y) % boardHeight;

      if (bot.position.x < 0) {
        bot.position.x = boardWidth + bot.position.x;
      }
      if (bot.position.y < 0) {
        bot.position.y = boardHeight + bot.position.y;
      }
    }
  }

  int halfWidth = boardWidth / 2;
  int halfHeight = boardHeight / 2;

  // quadrants
  int tl = 0;
  int tr = 0;
  int bl = 0;
  int br = 0;

  for (const Robot &bot : bots) {
    if (bot.position.x > halfWidth) {
      if (bot.position.y > halfHeight) {
        tr++;
      } else if (bot.position.y < halfHeight) {
        br++;
      }
    } else if (bot.position.x < halfWidth) {
      if (bot.position.y > halfHeight) {
        tl++;
      } else if (bot.position.y < halfHeight) {
        bl++;
      }
    }
  }

  return tl * tr * bl * br;
}

void visualizeBoard(const std::vector<Robot> &bots, int second, int boardHeight,
                    int boardWidth) {

  std::vector<std::string> board(boardHeight, std::string(boardWidth, '.'));

  for (const auto &bot : bots) {
    int x = bot.position.x;
    int y = bot.position.y;

    if (x >= 0 && x < boardWidth && y >= 0 && y < boardHeight) {
      board[y][x] = 'R';
    }
  }

  for (int row = 0; row < boardHeight; ++row) {
    std::cout << board[row] << "\n";
  }

  std::cout << "\n";
  std::cout << "Second " << second + 1 << std::endl;
}

bool possibleTree(std::vector<Robot> &bots, int boardWidth, int boardHeight) {
  for (const auto &bot : bots) {
    int yPos = bot.position.y;
    int xPos = bot.position.x;
    bool verticalLine = true;
    for (int i = 0; i < 5; ++i) {
      if (std::find_if(bots.begin(), bots.end(), [&](Robot bot) {
            return bot.position.y == yPos + i && bot.position.x == xPos + i;
          }) == bots.end()) {
        verticalLine = false;
      }
    }
    if (verticalLine)
      return true;
  }
  return false;
}

void part2(std::vector<Robot> bots) {

  int boardWidth = 101;
  int boardHeight = 103;

  int second = 0;

  while (true) {
    for (auto &bot : bots) {
      bot.position.x = (bot.position.x + bot.velocity.x) % boardWidth;
      bot.position.y = (bot.position.y + bot.velocity.y) % boardHeight;

      if (bot.position.x < 0) {
        bot.position.x = boardWidth + bot.position.x;
      }
      if (bot.position.y < 0) {
        bot.position.y = boardHeight + bot.position.y;
      }
    }
    if (possibleTree(bots, boardWidth, boardHeight)) {
      visualizeBoard(bots, second, boardHeight, boardWidth);
      std::cin.get();
    }
    second++;
  }
}

int main() {
  std::vector<Robot> testInput = loadInput("./14_testInput.txt");
  std::vector<Robot> input = loadInput("./14_input.txt");

  std::cout << "Test 1: " << part1(testInput, true) << std::endl;
  std::cout << "Part 1: " << part1(input, false) << std::endl;

  part2(input); // found it at 8149
}
