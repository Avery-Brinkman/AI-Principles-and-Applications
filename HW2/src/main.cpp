#include <iostream>

#include "GameBoard.h"

using namespace GAME_NS;

int main() {
  std::vector<Queen> queens = {{1, 7}, {2, 4}, {3, 8}, {4, 1}, {4, 6},
                               {5, 5}, {6, 2}, {7, 2}, {8, 3}};
  GameBoard board(9, 4, queens);

  return 0;
}
