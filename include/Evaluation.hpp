#ifndef EVALUATION_HPP
#define EVALUATION_HPP

#include "Board.hpp"
class Evaluation
{
public:
    Evaluation();
    ~Evaluation();
    int evalPosition(const Board &board);

    // Knight piece-square table
    const int8_t knight_table[64] = {
        -50, -40, -30, -30, -30, -30, -40, -50,
        -40, -20, 0, 0, 0, 0, -20, -40,
        -30, 0, 10, 15, 15, 10, 0, -30,
        -30, 0, 15, 20, 20, 15, 0, -30,
        -30, 0, 15, 20, 20, 15, 0, -30,
        -30, 0, 10, 15, 15, 10, 0, -30,
        -40, -20, 0, 0, 0, 0, -20, -40,
        -50, -40, -30, -30, -30, -30, -40, -50};
};

#endif /* EVALUATION_HPP */
