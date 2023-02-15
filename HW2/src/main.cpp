#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "GameBoard.h"

using namespace GAME_NS;

std::vector<std::string> parse(std::string& str, std::string delim) {
  size_t pos = 0;
  std::string token;
  std::vector<std::string> out = {};
  while ((pos = str.find(delim)) != std::string::npos) {
    token = str.substr(0, pos);
    out.push_back(token);
    str.erase(0, pos + delim.length());
  }
  out.push_back(str);
  return out;
}

int main() {
  std::string numberOfQueens, range, positionsLine;
  std::vector<GameBoard> inputs = {};

  std::ifstream inputFile("------------PATH/TO/FILE-------------");

  if (inputFile.is_open()) {
    while (!inputFile.eof()) {
      std::vector<Queen> queens = {};

      // Get values from file
      std::getline(inputFile, numberOfQueens);
      std::getline(inputFile, range);
      std::getline(inputFile, positionsLine);

      std::vector<std::string> positionsList = parse(positionsLine, " ");
      for (std::string positionString : positionsList) {
        std::vector<std::string> position = parse(positionString, ",");
        queens.push_back({ std::stoi(position[0]), std::stoi(position[1]) });
      }

      inputs.push_back({ std::stoi(numberOfQueens), std::stoi(range), queens });
    }
  }
  inputFile.close();

  for (GAME_NS::GameBoard board : inputs) {
    board.solve();
    std::cout << std::endl << "===============================================" << std::endl;
  }

  return 0;
}
