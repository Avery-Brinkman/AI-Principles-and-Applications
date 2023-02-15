#include <algorithm>
#include <iostream>
#include <ranges>

#include "GameBoard.h"

using namespace GAME_NS;

GameBoard::GameBoard(int numQueens, int range, const std::vector<Queen>& queens)
    : m_numQueens{numQueens}, m_range{range}, m_queens{queens} {

  // Find out how many conflicts we start with
  m_conflicts = getConflicts(m_queens);

  // Initialize vector to track previous states and store current one

  m_prevStates = {{m_conflicts, m_queens}};
  // m_prevStates[m_conflicts].push_back({ m_conflicts, m_queens });
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

void GameBoard::improve() {
  // Start by comparing to current state
  BoardDescription best = {m_conflicts, m_queens};

  // Look at each queen
  for (int q = 0; q < m_numQueens; q++) {
    // Look at each postition on the board
    for (int r = 1; r <= 8; r++) {
      for (int c = 1; c <= 8; c++) {
        // Check if moving this queen to this position is an improvement and update best accordingly
        evaluateMove(best, q, r, c);
      }
    }
  }

  // If the best possible option is the same or better than our current state...
  if (best.conflicts <= m_conflicts) {
    // See if the best possible option is the same as the current state (i.e. local min)
    if (best.queens == m_queens) {
      m_localMin = true;
      // Stop trying to improve
      return;
    }

    // If it's not an improvement, track this state so it's not repeated
    if (best.conflicts == m_conflicts)
      m_prevStates.push_back(best);
    // If it is an improvement, clear previous states and start over tracking
    else
      m_prevStates = {best};

    // Update our state to the better one
    m_conflicts = best.conflicts;
    m_queens = best.queens;
    m_transitions++;
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

    // Show first 4
    if (shown < 4) {
      display();
      shown++;
    }

    // Check for solution
    if (m_conflicts == 0) {
      std::cout << "Solution found!" << std::endl;
      searching = false;
    }
    // Check for local minimum
    else if (m_localMin) {
      std::cout << "Local minimum reached!" << std::endl;
      searching = false;
    }
    // Check for max transitions
    else if (m_transitions >= 60) {
      std::cout << "Maximum transition limit reached!" << std::endl;
      searching = false;
    }
  }

  std::cout << std::endl << "Final state: " << std::endl;
  display();
  std::cout << "Transitions: " << m_transitions << std::endl;
  std::cout << "Examined states: " << m_examined << std::endl;
}

int GameBoard::distance(const Queen& first, const Queen& second) const {
  // Simple distance calculation
  return (int)sqrt(pow(second.row - first.row, 2) + pow(second.col - first.col, 2));
}

void GameBoard::evaluateMove(BoardDescription& currentBest, int q, int r, int c) {
  // Skip if the new position is the same as the current position
  if (m_queens[q] == Queen{r, c})
    return;

  // Copy current state
  std::vector<Queen> newQueens = m_queens;
  // Set current queen to new position
  newQueens[q] = {r, c};

  // Skip if a queen already occupies this new cell
  for (int other = 0; other < m_numQueens; other++) {
    // Don't compare against self
    if (other == q)
      continue;
    if (newQueens[q] == newQueens[other])
      return;
  }

  // Find number of conflicts in this state
  BoardDescription newBest = {getConflicts(newQueens), newQueens};
  // Keep track of the fact that we have examined another state
  m_examined++;

  // If it's an improvement or equal, and is not a repeated state, update best possible move
  if ((newBest.conflicts <= currentBest.conflicts) && (!checkRepeated(newBest)))
    currentBest = newBest;
}

bool GameBoard::checkRepeated(const BoardDescription& check) const {
  // Look at each previous state
  for (auto const& state : m_prevStates) {
    // Check if the previous state is the same as the one we're looking at
    if (state.queens == check.queens)
      return true;
  }
  return false;
}
