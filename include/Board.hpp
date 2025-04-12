#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include "Move.hpp"
class Board
{
public:
    //    Allocate a single bit for each castle right (4 bits in total):
    //    Bit 0: Kingside castle right for the white king
    //    Bit 1: Queenside castle right for the white king
    //    Bit 2: Kingside castle right for the black king
    //    Bit 3: Queenside castle right for the black king
    uint8_t castlingRights = 0b00000000;

    uint64_t AllPieces = 0ULL;

    uint64_t WhitePieces = 0ULL;

    uint64_t WhitePawns = 0ULL;
    uint64_t WhiteRook = 0ULL;
    uint64_t WhiteKnight = 0ULL;
    uint64_t WhiteBishop = 0ULL;
    uint64_t WhiteQueen = 0ULL;
    uint64_t WhiteKing = 0ULL;

    uint64_t BlackPieces = 0ULL;

    uint64_t BlackPawns = 0ULL;
    uint64_t BlackRook = 0ULL;
    uint64_t BlackKnight = 0ULL;
    uint64_t BlackBishop = 0ULL;
    uint64_t BlackQueen = 0ULL;
    uint64_t BlackKing = 0ULL;

    int player;
    int enPassant;

    Board();
    ~Board();
    void setBitBoard(const std::string &fen);
    void makeMove(const Move &move);
    void undoMove(const Move &move);
};

#endif /* BOARD_HPP */
