#include <algorithm>
#include <iostream>
#include <ranges>

#include "GameBoard.h"

using namespace GAME_NS;

GameBoard::GameBoard(int numQueens, int range, const std::vector<Queen>& queens)
  : m_numQueens{ numQueens }, m_range{ range }, m_queens{ queens } {

  // Find out how many conflicts we start with
  m_conflicts = getConflicts(m_queens);

  m_prevStates = std::vector<std::vector<BoardDescription>>(m_conflicts + 1, std::vector<BoardDescription>{});
  m_prevStates[m_conflicts].push_back({ m_conflicts, m_queens });
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
  BoardDescription best = { m_conflicts, m_queens };

  // Look at each queen
  for (int q = 0; q < m_numQueens; q++) {
    // Look at each postition on the board
    for (int r = 1; r <= 8; r++) {
      for (int c = 1; c <= 8; c++) {
        evaluateMove(best, q, r, c);
      }
    }
  }

  if (best.conflicts <= m_conflicts) {
    if (best.queens == m_queens) {
      m_localMin = true;
      return;
    }

    m_conflicts = best.conflicts;
    m_queens = best.queens;
    m_transitions++;

    m_prevStates.at(best.conflicts).push_back(best);
  }
}

void GameBoard::display() const {
  std::cout << "Queen postions: ";
  for (int q = 0; q < m_numQueens; q++) {
    std::cout << "(" << m_queens[q].row << ", " << m_queens[q].col << ")";
    if (q + 1 < m_numQueens)
      std::cout << ",";
    std::cout << " ";
  }
  std::cout << "Conflicts: " << m_conflicts << std::endl;
}

void GameBoard::solve() {
  std::cout << "Starting state:" << std::endl;
  display();
  std::cout << std::endl;

  int shown = 0;
  bool searching = true;

  // Go until solution is found
  while (searching) {
    // Improve
    improve();

    // Check for solution
    if (m_conflicts == 0) {
      std::cout << "Solution found!" << std::endl;
      searching = false;
    }

    // Check for local minimum
    if (m_localMin) {
      std::cout << "Local minimum reached!" << std::endl;
      searching = false;
    }

    // Check for max transitions
    if (m_transitions > 60) {
      std::cout << "Maximum transition limit reached!" << std::endl;
      searching = false;
    }

    // Show first 4
    if (shown < 4 && searching) {
      display();
      shown++;
    }
  }

  std::cout << std::endl << "Final state: " << std::endl;
  display();
  std::cout << "Transitions: " << m_transitions << std::endl;
  std::cout << "Examined states: " << m_examined << std::endl;
}

void GameBoard::evaluateMove(BoardDescription& currentBest, int q, int r, int c) {
  // Skip step if new position is the same as the current position
  if (m_queens[q] == Queen{ r, c })
    return;

  // Copy current state
  std::vector<Queen> newQueens = m_queens;
  // Set current queen to new possible position
  newQueens[q] = { r, c };

  // Skip if a queen already occupies this cell
  for (int other = 0; other < m_numQueens; other++) {
    if (other == q)
      continue;
    if (newQueens[q] == newQueens[other]) {
      return;
    }
  }

  // Find number of conflicts in this state
  BoardDescription newBest = { getConflicts(newQueens), newQueens };
  // Keep track of the newly examined state
  m_examined++;
  if ((newBest.conflicts <= currentBest.conflicts) && (!checkRepeated(newBest))) {
    currentBest = newBest;
  }
}

bool GameBoard::checkRepeated(BoardDescription& check) {
  for (int i = 0; i < m_prevStates.at(check.conflicts).size(); i++) {
    if ((m_prevStates.at(check.conflicts)).at(i).queens == check.queens)
      return true;
  }
  return false;
}
