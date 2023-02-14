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

void GameBoard::improve() {
  // Vector of the best horizontal, vertical, and diagonal moves
  // std::vector<BoardDescription> options = {horizontal(), vertical(), diagonalPos(),
  // diagonalNeg()};
  // Define how to compare each option
  // auto board_compare = [](const BoardDescription& a, const BoardDescription& b) {
  //  return (a.conflicts < b.conflicts);
  //};

  BoardDescription best = {m_conflicts, m_queens};

  // Look at each queen
  for (int q = 0; q < m_numQueens; q++) {
    // Look at each postition on the board
    for (int r = 1; r <= 8; r++) {
      for (int c = 1; c <= 8; c++) {
        // Copy current state
        std::vector<Queen> newQueens = m_queens;
        // Set current queen to new possible position
        newQueens[q] = {r, c};

        // Skip if a queen already occupies this cell
        bool overlap = false;
        for (int other = 0; other < m_numQueens; other++) {
          if (other == q)
            continue;
          if (newQueens[q] == newQueens[other]) {
            overlap = true;
            break;
          }
        }
        if (overlap)
          continue;

        // Find number of conflicts in this state
        int newConflicts = getConflicts(newQueens);
        // Keep track of the newly examined state
        m_examined++;
        if (newConflicts < best.conflicts) {
          best = {newConflicts, newQueens};
        }
      }
    }
  }

  if (best.conflicts < m_conflicts) {
    m_conflicts = best.conflicts;
    m_queens = best.queens;
    m_transitions++;
  }
}
