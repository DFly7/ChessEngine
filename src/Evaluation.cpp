#include "../include/Evaluation.hpp"
#include "../include/Board.hpp"

Evaluation::Evaluation()
{
}

Evaluation::~Evaluation()
{
}

/// Looks at all the BitBoards in class and returns an evaluation score
int Evaluation::evalPosition(const Board &board)
{
    int eval = 0;

    int numBitsSetP = __builtin_popcountll(board.WhitePawns);
    int numBitsSetR = __builtin_popcountll(board.WhiteRook);
    int numBitsSetN = __builtin_popcountll(board.WhiteKnight);
    int numBitsSetB = __builtin_popcountll(board.WhiteBishop);
    int numBitsSetQ = __builtin_popcountll(board.WhiteQueen);
    int numBitsSetK = __builtin_popcountll(board.WhiteKing);

    int numBitsSetp = __builtin_popcountll(board.BlackPawns);
    int numBitsSetr = __builtin_popcountll(board.BlackRook);
    int numBitsSetn = __builtin_popcountll(board.BlackKnight);
    int numBitsSetb = __builtin_popcountll(board.BlackBishop);
    int numBitsSetq = __builtin_popcountll(board.BlackQueen);
    int numBitsSetk = __builtin_popcountll(board.BlackKing);

    int evalW = (numBitsSetP * 100) + (numBitsSetR * 500) + (numBitsSetN * 300) + (numBitsSetB * 300) + (numBitsSetQ * 900) + (numBitsSetK * 100000);

    int evalB = (numBitsSetp * 100) + (numBitsSetr * 500) + (numBitsSetn * 300) + (numBitsSetb * 300) + (numBitsSetq * 900) + (numBitsSetk * 100000);
    eval = evalW - evalB;

    // Knight Stuff
    uint64_t knight = board.WhiteKnight;
    int score = 0;

    int index = __builtin_ctzll(knight);

    for (int i = index; i < 64; i++)
    {

        i = __builtin_ctzll(knight);

        if (knight == 0)
        {
            break;
        }
        score += knight_table[i];
        // Bit board with only the pawn in question on it
        knight &= knight - 1; // Clear the least significant set bit
    }
    eval += score;

    // Knight Stuff
    uint64_t knight2 = board.BlackKnight;
    int score2 = 0;

    int index2 = __builtin_ctzll(knight2);

    for (int i = index2; i < 64; i++)
    {

        i = __builtin_ctzll(knight2);

        //        if (knight2 == 0){
        //            break;
        //        }
        score2 -= knight_table[i];
        // Bit board with only the pawn in question on it
        knight2 &= knight2 - 1; // Clear the least significant set bit
    }
    eval += score2;
    // Knight

    return eval;
}