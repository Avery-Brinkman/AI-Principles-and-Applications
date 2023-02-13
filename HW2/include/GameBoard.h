#pragma once

#include <memory>
#include <vector>

namespace GAME_NS {
  struct Queen {
    int row;
    int col;
  };

  class GameBoard {
  public:
  private:
    int m_numQueens;

    int m_range;

    std::vector<std::shared_ptr<Queen>> m_queens;

    int m_conflicts;

    int m_transitions = 0;

    int m_examined = 0;
  };
}