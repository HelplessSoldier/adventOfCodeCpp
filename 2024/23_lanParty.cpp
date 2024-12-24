#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

struct ComputerNode {
  std::string name;
  std::vector<std::shared_ptr<ComputerNode>> connected;
};

typedef std::map<std::string, std::shared_ptr<ComputerNode>> CompMap;

CompMap buildNetwork(std::string filepath) {

  CompMap computerMap;

  std::fstream file(filepath);
  std::string line;

  while (getline(file, line)) {

    std::string computer1;
    std::string computer2;

    std::stringstream ss(line);
    getline(ss, computer1, '-');
    getline(ss, computer2);

    if (!computerMap.count(computer1)) {
      computerMap[computer1] =
          std::make_shared<ComputerNode>(ComputerNode{computer1, {}});
    }

    if (!computerMap.count(computer2)) {
      computerMap[computer2] =
          std::make_shared<ComputerNode>(ComputerNode{computer2, {}});
    }

    computerMap[computer1]->connected.push_back(computerMap[computer2]);
    computerMap[computer2]->connected.push_back(computerMap[computer1]);
  }

  return computerMap;
}

std::vector<std::vector<std::string>> groupsOf3Computers(CompMap computerMap,
                                                         char startsWith) {
  std::vector<std::vector<std::string>> result;

  for (auto [startName, startNode] : computerMap) {
    if (startName[0] != startsWith)
      continue;

    //                                       path
    std::queue<std::pair<ComputerNode, std::vector<std::string>>> searching;
    searching.push({*startNode, {startName}});

    while (!searching.empty()) {

      auto [node, path] = searching.front();
      searching.pop();

      if (path.size() > 3)
        continue;

      for (auto next : node.connected) {
        if (next->name == startName && path.size() == 3) {
          result.push_back(path);
        }

        std::vector<std::string> nextPath = path;
        nextPath.push_back(next->name);
        searching.push({*next, nextPath});
      }
    }
  }

  return result;
}

void filterDuplicates(std::vector<std::vector<std::string>> &groups) {
  for (std::vector<std::string> &group : groups)
    std::sort(group.begin(), group.end());
  std::sort(groups.begin(), groups.end());
  groups.erase(std::unique(groups.begin(), groups.end()), groups.end());
}

int part1(const CompMap &conputerMap) {
  std::vector<std::vector<std::string>> groups =
      groupsOf3Computers(conputerMap, 't');
  filterDuplicates(groups);
  return groups.size();
}

bool connectedToAllOthers(
    const std::shared_ptr<ComputerNode> &node,
    const std::vector<std::shared_ptr<ComputerNode>> &clique) {

  for (auto cliqueNode : clique) {
    bool found = false;
    for (auto connectedNode : cliqueNode->connected) {
      if (connectedNode == node)
        found = true;
    }
    if (!found) {
      return false;
    }
  }
  return true;
}

void traverseCliqueRecursive(
    std::vector<std::shared_ptr<ComputerNode>> &potentialClique,
    std::vector<std::shared_ptr<ComputerNode>> &candidates,
    std::vector<std::shared_ptr<ComputerNode>> &bestClique) {

  if (candidates.empty()) {
    if (potentialClique.size() > bestClique.size()) {
      bestClique = potentialClique;
    }
    return;
  }

  for (int i = 0; i < candidates.size(); ++i) {
    std::shared_ptr<ComputerNode> candidate = candidates[i];

    if (connectedToAllOthers(candidate, potentialClique)) {
      potentialClique.push_back(candidate);

      std::vector<std::shared_ptr<ComputerNode>> newCandidates;

      for (int j = i + 1; j < candidates.size(); ++j) {
        std::shared_ptr<ComputerNode> nextCandidate = candidates[j];
        newCandidates.push_back(nextCandidate);
      }

      traverseCliqueRecursive(potentialClique, newCandidates, bestClique);

      potentialClique.pop_back();
    }
  }
}

std::pair<int, std::vector<std::string>>
traverseClique(const std::shared_ptr<ComputerNode> &start) {

  std::vector<std::shared_ptr<ComputerNode>> potentialClique = {start};
  std::vector<std::shared_ptr<ComputerNode>> candidates = start->connected;
  std::vector<std::shared_ptr<ComputerNode>> bestClique;

  traverseCliqueRecursive(potentialClique, candidates, bestClique);

  std::vector<std::string> cliqueNames;
  for (const auto &node : bestClique) {
    cliqueNames.push_back(node->name);
  }

  return {bestClique.size(), cliqueNames};
}

std::string part2(const CompMap &computerMap) {

  int maxCliqueSize = -1;
  std::vector<std::string> maxCliqueNames;

  for (auto [startName, startNode] : computerMap) {

    auto [cliqueSize, cliqueMembers] = traverseClique(startNode);

    if (cliqueSize > maxCliqueSize) {
      maxCliqueSize = cliqueSize;
      maxCliqueNames = cliqueMembers;
    }
  }

  std::sort(maxCliqueNames.begin(), maxCliqueNames.end());
  std::string result;
  for (int i = 0; i < maxCliqueNames.size(); ++i) {
    result += maxCliqueNames[i];
    if (i != maxCliqueNames.size() - 1)
      result += ',';
  }
  return result;
}

int main() {
  CompMap testComputers = buildNetwork("./23_testInput.txt");
  CompMap testComputers2 = buildNetwork("./23_testInput2.txt");
  CompMap computers = buildNetwork("./23_input.txt");

  int test1Result = part1(testComputers);
  std::cout << "Test 1: " << test1Result << std::endl;

  int part1Result = part1(computers);
  std::cout << "Part 1: " << part1Result << std::endl;

  std::string test2Result = part2(testComputers2);
  std::cout << "Test 2: " << test2Result << std::endl;

  std::string part2Result = part2(computers);
  std::cout << "Part 2: " << part2Result << std::endl;
}
