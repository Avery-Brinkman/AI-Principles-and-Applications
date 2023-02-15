#pragma once

#include <vector>

namespace GAME_NS {
  struct Queen {
    int row;
    int col;
    auto operator<=>(const Queen&) const = default;
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

    void improve();

    void display() const;

    void solve();

  private:
    void evaluateMove(BoardDescription& currentbest, int q, int r, int c);

    bool checkRepeated(BoardDescription& check);

    int m_numQueens;

    int m_range;

    std::vector<Queen> m_queens;

    int m_conflicts;

    int m_transitions = 0;

    int m_examined = 0;

    std::vector<std::vector<BoardDescription>> m_prevStates;
  };
} // namespace GAME_NS