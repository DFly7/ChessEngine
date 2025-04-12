#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "Board.hpp"
#include "MoveGenerator.hpp"
#include "Evaluation.hpp"

class Search
{
private:
    MoveGenerator moveGenerator;
    Evaluation evaluation;
    int maxDepth;

public:
    Search();
    ~Search();
    Move search(Board &board);

    int minimax(Board &board, int depth, bool maximizingPlayer, int alpha, int beta);
};

#endif /* SEARCH_HPP */
