#pragma once

#include <vector>

namespace GAME_NS {
struct Queen {
  int row;
  int col;
};

struct BoardDescription {
  int conflicts;
  std::vector<Queen> queens;
};

class GameBoard {
public:
  GameBoard() = default;

  GameBoard(int numQueens, int range, const std::vector<Queen>& queens);

  int getConflicts(const std::vector<Queen>& queens) const;

  bool inConflict(const Queen& first, const Queen& second) const;

  int distance(const Queen& first, const Queen& second) const;

  BoardDescription horizontal();
  BoardDescription vertical();
  BoardDescription diagonalPos();
  BoardDescription diagonalNeg();

  void improve();

private:
  int m_numQueens;

  int m_range;

  std::vector<Queen> m_queens;

  int m_conflicts;

  int m_transitions = 0;

  int m_examined = 0;
};
} // namespace GAME_NS