#include <algorithm>
#include <iostream>
#include <ranges>

#include "GameBoard.h"

using namespace GAME_NS;

GameBoard::GameBoard(int numQueens, int range, const std::vector<Queen>& queens)
    : m_numQueens{numQueens}, m_range{range}, m_queens{queens} {

  // Find out how many conflicts we start with
  m_conflicts = getConflicts(m_queens);
}

int GameBoard::getConflicts(const std::vector<Queen>& queens) const {
  int conflicts = 0;
  // Compare each queen with each other queen.
  for (auto first = queens.begin(); first < queens.end(); first++) {
    // Don't need to compare queens before it, which skips duplicate counts
    for (auto second = first + 1; second < queens.end(); second++) {
      // Check if the two queens are in conflict
      if (inConflict(*first, *second))
        conflicts++;
    }
  }
  return conflicts;
}

bool GameBoard::inConflict(const Queen& first, const Queen& second) const {
  // Get the slope between the two to check for diagonals
  double slope;
  float dy = ((float)second.row - (float)first.row);
  if (float dx = ((float)second.col - (float)first.col); dx == 0.0)
    slope = INT_MAX;
  else
    slope = abs(dy / dx);

  // If they are in the same row, the same column, or same diagonal, check if they are too close
  return (((first.row == second.row) || (first.col == second.col) || (slope == 1)) &&
          (distance(first, second) <= m_range));
}

int GameBoard::distance(const Queen& first, const Queen& second) const {
  // Simple distance calculation
  return (int)sqrt(pow(second.row - first.row, 2) + pow(second.col - first.col, 2));
}

BoardDescription GameBoard::horizontal() {
  BoardDescription best = {m_conflicts, m_queens};
  for (int q = 0; q < m_numQueens; q++) {
    for (int x = -1 * m_range; x < m_range; x++) {
      if (x == 0)
        continue;

      std::vector<Queen> newQueens = m_queens;
      newQueens[q].col += x;
      if ((newQueens[q].col <= 0) || (newQueens[q].col > 8))
        continue;

      m_examined++;
      int newConflicts = getConflicts(newQueens);
      if (newConflicts < best.conflicts) {
        best.conflicts = newConflicts;
        best.queens = newQueens;
      }
    }
  }

  return best;
}

BoardDescription GameBoard::vertical() {
  BoardDescription best = {m_conflicts, m_queens};
  for (int q = 0; q < m_numQueens; q++) {
    for (int y = -1 * m_range; y < m_range; y++) {
      if (y == 0)
        continue;

      std::vector<Queen> newQueens = m_queens;
      newQueens[q].row += y;
      if ((newQueens[q].row <= 0) || (newQueens[q].row > 8))
        continue;

      m_examined++;
      int newConflicts = getConflicts(newQueens);
      if (newConflicts < best.conflicts) {
        best.conflicts = newConflicts;
        best.queens = newQueens;
      }
    }
  }

  return best;
}

BoardDescription GameBoard::diagonalPos() {
  BoardDescription best = {m_conflicts, m_queens};
  for (int q = 0; q < m_numQueens; q++) {
    for (int d = -1 * m_range; d < m_range; d++) {
      if (d == 0)
        continue;

      std::vector<Queen> newQueens = m_queens;
      newQueens[q].row += d;
      newQueens[q].col -= d;
      if ((newQueens[q].row <= 0) || (newQueens[q].row > 8) || (newQueens[q].col <= 0) ||
          (newQueens[q].col > 8))
        continue;

      m_examined++;
      int newConflicts = getConflicts(newQueens);
      if (newConflicts < best.conflicts) {
        best.conflicts = newConflicts;
        best.queens = newQueens;
      }
    }
  }

  return best;
}

BoardDescription GameBoard::diagonalNeg() {
  BoardDescription best = {m_conflicts, m_queens};
  for (int q = 0; q < m_numQueens; q++) {
    for (int d = -1 * m_range; d < m_range; d++) {
      if (d == 0)
        continue;

      std::vector<Queen> newQueens = m_queens;
      newQueens[q].row -= d;
      newQueens[q].col += d;
      if ((newQueens[q].row <= 0) || (newQueens[q].row > 8) || (newQueens[q].col <= 0) ||
          (newQueens[q].col > 8))
        continue;

      m_examined++;
      int newConflicts = getConflicts(newQueens);
      if (newConflicts < best.conflicts) {
        best.conflicts = newConflicts;
        best.queens = newQueens;
      }
    }
  }

  return best;
}

void GameBoard::improve() {
  // Vector of the best horizontal, vertical, and diagonal moves
  std::vector<BoardDescription> options = {horizontal(), vertical(), diagonalPos(), diagonalNeg()};
  // Define how to compare each option
  auto board_compare = [](const BoardDescription& a, const BoardDescription& b) {
    return (a.conflicts < b.conflicts);
  };

  // Get the best move
  auto bestMove = std::ranges::min_element(options, board_compare);
}