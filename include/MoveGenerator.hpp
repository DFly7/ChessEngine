#ifndef MOVEGENERATOR_HPP
#define MOVEGENERATOR_HPP

#include "Board.hpp"

class MoveGenerator
{
public:
    MoveGenerator();
    ~MoveGenerator();
    void generateMoves(const Board &board);
};

#endif /* MOVEGENERATOR_HPP */
