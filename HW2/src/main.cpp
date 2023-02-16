#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "GameBoard.h"

using namespace GAME_NS;

// Takes a string and splits it into a vector, where delim is the seperator
std::vector<std::string> parse(std::string& str, const std::string_view delim) {
  std::vector<std::string> returnList = {};

  size_t pos = 0;
  std::string token;

  // Find the given character in the string
  while ((pos = str.find(delim)) != std::string::npos) {
    // Split the string
    token = str.substr(0, pos);
    // Add to the list
    returnList.push_back(token);
    // Erase that part of the string
    str.erase(0, pos + delim.length());
  }
  // Add the last bit of the string
  returnList.push_back(str);
  str.erase(0, str.length());

  return returnList;
}

int main() {
  std::vector<GameBoard> inputs = {};

  // Open file
  // Adjust path accordingly
  std::ifstream inputFile("../../TestPatterns.txt");

  // Make sure it opened
  if (inputFile.is_open()) {
    // Read to end
    while (!inputFile.eof()) {
      // Local vars
      std::string numberOfQueens;
      std::string range;
      std::string positionsLine;
      std::vector<Queen> queens = {};

      // Get values from file
      std::getline(inputFile, numberOfQueens);
      std::getline(inputFile, range);
      std::getline(inputFile, positionsLine);

      // Split the line of positions into vector of positions
      // Then split each pos into row and col
      for (std::vector<std::string> positionsList = parse(positionsLine, " ");
           std::string positionString : positionsList) {
        std::vector<std::string> position = parse(positionString, ",");
        // Convert row and col to numbers, make a queen, and add it to the list of queens
        queens.push_back({std::stoi(position[0]), std::stoi(position[1])});
      }

      // Convert number and range to numbers, create a gameboard, and add it to the list
      inputs.emplace_back(std::stoi(numberOfQueens), std::stoi(range), queens);
    }
  } else {
    std::cout << "Could not open file!" << std::endl;
  }
  inputFile.close();

  // For each game board, solve it
  for (GAME_NS::GameBoard board : inputs) {
    board.solve();
    std::cout << std::endl << "===============================================" << std::endl;
  }

  // Show results
  system("pause");
  return 0;
}
