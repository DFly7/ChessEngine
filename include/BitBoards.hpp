//
//  BitBoards.hpp
//  BitBoardStuff
//
//  Created by Darragh Flynn on 15/09/2024.
//

#ifndef BitBoards_hpp
#define BitBoards_hpp

#include <stdio.h>
#include <cstdint>
#include <string>
#include <cstdio>
#include <vector>
#include "Move.hpp"
#include <iostream>
#include "RenderData.hpp"

class BitBoards
{
public:
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

    bool whiteKingInCheck = false;
    bool blackKingInCheck = false;

    void kingInCheck(int player);

    //    Allocate a single bit for each castle right (4 bits in total):
    //    Bit 0: Kingside castle right for the white king
    //    Bit 1: Queenside castle right for the white king
    //    Bit 2: Kingside castle right for the black king
    //    Bit 3: Queenside castle right for the black king

    //    uint8_t castlingRights = 0b00001111;
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

    BitBoards(std::string fen);

    std::vector<Move> GenerateAllMoves(int player);

    std::vector<Move> GenerateWhitePawnMoves();
    std::vector<Move> GenerateBlackPawnMoves();

    // Work for both colours
    std::vector<Move> GenerateBishopMoves(int player);
    std::vector<Move> GenerateRookMoves(int player);
    std::vector<Move> GenerateKnightMoves(int player);
    std::vector<Move> GenerateKingMoves(int player);
    std::vector<Move> kingCastling(int player);

    std::vector<Move> GenerateQueenMoves(int player);

    int minimax(int depth, bool maximizingPlayer, int alpha, int beta);

    Move AImove();

    bool castlingChecks(Move move);

    void makeMove(Move &move);
    void undoMove(Move move);

    bool validatePlayerMove(int player, Move m);

    int evalPosition();

    RenderData populateRenderData();

    char getPieceletter(int index);

    void printLegalMoves(const std::vector<Move> legalMoves);
    void printMove(const Move &move);

    uint64_t &getBitBoard(char pieceType);
    void printBitboard(uint64_t board);
    void printBitBoards();
};

#endif /* BitBoards_hpp */
