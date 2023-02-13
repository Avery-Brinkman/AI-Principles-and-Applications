#include "GameBoard.h"
#include <iostream>

using namespace GAME_NS;

GameBoard::GameBoard(int numQueens, int range, const std::vector<Queen>& queens) :
  m_numQueens{ numQueens }, m_range{ range }, m_queens{ queens } {

  m_conflicts = 0;
  for (auto first = m_queens.begin(); first < m_queens.end(); first++) {
    for (auto second = first + 1; second < m_queens.end(); second++) {
      if (inConflict(*first, *second)) m_conflicts++;
    }
  }

  std::cout << m_conflicts << std::endl;
}

bool GameBoard::inConflict(const Queen& first, const Queen& second) const {
  float dy = ((float)second.row - (float)first.row);
  float dx = ((float)second.col - (float)first.col);

  double slope;
  if (dx == 0.0) slope = INT_MAX;
  else slope = abs(dy / dx);

  return (((first.row == second.row) || (first.col == second.col) || (slope == 1)) && (distance(first, second) <= m_range));
}

int GameBoard::distance(const Queen& first, const Queen& second) const {
  return (int)sqrt(pow(second.row - first.row, 2) + pow(second.col - first.col, 2));
}