#ifndef MOVEGENERATOR_HPP
#define MOVEGENERATOR_HPP

#include "Board.hpp"

class MoveGenerator
{
public:
    uint8_t castlingRights;

    uint64_t AllPieces;

    uint64_t WhitePieces;

    uint64_t WhitePawns;
    uint64_t WhiteRook;
    uint64_t WhiteKnight;
    uint64_t WhiteBishop;
    uint64_t WhiteQueen;
    uint64_t WhiteKing;

    uint64_t BlackPieces;

    uint64_t BlackPawns;
    uint64_t BlackRook;
    uint64_t BlackKnight;
    uint64_t BlackBishop;
    uint64_t BlackQueen;
    uint64_t BlackKing;

    int player;
    int enPassant;

    MoveGenerator();
    ~MoveGenerator();
    std::vector<Move> generateMoves(const Board &board);
    void updateBoardReferences(const Board &board);

    std::vector<Move> GenerateWhitePawnMoves();
    std::vector<Move> GenerateBlackPawnMoves();

    std::vector<Move> GenerateRookMoves(int player);
    std::vector<Move> GenerateBishopMoves(int player);
    std::vector<Move> GenerateKnightMoves(int player);
    std::vector<Move> GenerateKingMoves(int player);
    std::vector<Move> GenerateQueenMoves(int player);

    std::vector<Move> kingCastling(int player);
};

#endif /* MOVEGENERATOR_HPP */
