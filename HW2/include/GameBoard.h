#pragma once

#include <vector>

namespace GAME_NS {
// Simple queen struct (basically just coords, didn't want to use a pair or vector or smthn)
struct Queen {
  int row;
  int col;
  auto operator<=>(const Queen&) const = default;
};

// The most important info we deal w
struct BoardDescription {
  int conflicts;
  std::vector<Queen> queens;
};

class GameBoard {
public:
  // Default
  GameBoard() = default;

  // Constructor
  GameBoard(int numQueens, int range, const std::vector<Queen>& queens);

  // Given a vector of queen positions, returns the number of conflicts the setup contains
  int getConflicts(const std::vector<Queen>& queens) const;

  // Checks if the position of 2 queens causes a conflict
  bool inConflict(const Queen& first, const Queen& second) const;

  // Looks at all possible moves, and applies the most optimal one (if possible)
  void improve();

  // Displays the queen positions and number of conflicts
  void display() const;

  // Attempts to solve the board, while also printing relevant information
  void solve();

private:
  // Returns the distance between two queens
  int distance(const Queen& first, const Queen& second) const;

  // Updates the currentBest if moving queen 'q' to ('r', 'c') is an improvement or equal
  void evaluateMove(BoardDescription& currentBest, int q, int r, int c);

  // Checks if a given Board Description is a repeat of a previous state to prevent getting stuck in
  // a loop
  bool checkRepeated(const BoardDescription& check) const;

  // The number of queens on the board
  int m_numQueens;

  // The range of influence a queen has (In normal chess, a king would have range of 1)
  int m_range;

  // Each queen
  std::vector<Queen> m_queens;

  // The number of conflicts the board currently has
  int m_conflicts;

  // The number of moves we have made
  int m_transitions = 0;

  // The number of moves we have considered (Note: if we find that a move causes two queens to
  // occupy the same cell or causes it to move off the board, we stop early and do not consider this
  // as an evaluation)
  unsigned int m_examined = 0;

  // The previous board states. This is erased and reset everytime the number of conflicts decreases
  std::vector<BoardDescription> m_prevStates;

  // Whether or not the current state is a local minimum
  bool m_localMin = false;
};
} // namespace GAME_NS