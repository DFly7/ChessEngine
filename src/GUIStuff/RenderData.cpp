//
//  RenderData.cpp
//  CHESSGUIBB
//
//  Created by Darragh Flynn on 03/10/2024.
//

#include "RenderData.hpp"

void RenderData::populateRenderData(const Board &board)
{
    pieceTypes.clear();
    pieceXPositions.clear();
    pieceYPositions.clear();

    uint64_t WhitePawns = board.WhitePawns;
    uint64_t WhiteKnight = board.WhiteKnight;
    uint64_t WhiteBishop = board.WhiteBishop;
    uint64_t WhiteRook = board.WhiteRook;
    uint64_t WhiteQueen = board.WhiteQueen;
    uint64_t WhiteKing = board.WhiteKing;

    uint64_t BlackPawns = board.BlackPawns;
    uint64_t BlackKnight = board.BlackKnight;
    uint64_t BlackBishop = board.BlackBishop;
    uint64_t BlackRook = board.BlackRook;
    uint64_t BlackQueen = board.BlackQueen;
    uint64_t BlackKing = board.BlackKing;

    uint64_t AllPieces = board.AllPieces;

    // Iterate through each square on the board
    for (int row = 7; row >= 0; --row)
    {
        //        std::cout << row+1 << " ";

        for (int col = 0; col < 8; ++col)
        {
            int index = row * 8 + col;

            // Check if a white piece is on the square
            if (WhitePawns & (1ULL << index))
            {
                //                std::cout << "P" << " ";
                pieceTypes.push_back('P');
                pieceXPositions.push_back(row);
                pieceYPositions.push_back(col);
            }
            else if (WhiteKnight & (1ULL << index))
            {
                //                std::cout << "N" << " ";
                pieceTypes.push_back('N');
                pieceXPositions.push_back(row);
                pieceYPositions.push_back(col);
            }
            else if (WhiteBishop & (1ULL << index))
            {
                //                std::cout << "B" << " ";
                pieceTypes.push_back('B');
                pieceXPositions.push_back(row);
                pieceYPositions.push_back(col);
            }
            else if (WhiteRook & (1ULL << index))
            {
                //                std::cout << "R" << " ";
                pieceTypes.push_back('R');
                pieceXPositions.push_back(row);
                pieceYPositions.push_back(col);
            }
            else if (WhiteQueen & (1ULL << index))
            {
                //                std::cout << "Q" << " ";
                pieceTypes.push_back('Q');
                pieceXPositions.push_back(row);
                pieceYPositions.push_back(col);
            }
            else if (WhiteKing & (1ULL << index))
            {
                //                std::cout << "K" << " ";
                pieceTypes.push_back('K');
                pieceXPositions.push_back(row);
                pieceYPositions.push_back(col);
            }

            // Check if a black piece is on the square
            if (BlackPawns & (1ULL << index))
            {
                //                std::cout << "p" << " ";
                pieceTypes.push_back('p');
                pieceXPositions.push_back(row);
                pieceYPositions.push_back(col);
            }
            else if (BlackKnight & (1ULL << index))
            {
                //                std::cout << "n" << " ";
                pieceTypes.push_back('n');
                pieceXPositions.push_back(row);
                pieceYPositions.push_back(col);
            }
            else if (BlackBishop & (1ULL << index))
            {
                //                std::cout << "b" << " ";
                pieceTypes.push_back('b');
                pieceXPositions.push_back(row);
                pieceYPositions.push_back(col);
            }
            else if (BlackRook & (1ULL << index))
            {
                //                std::cout << "r" << " ";
                pieceTypes.push_back('r');
                pieceXPositions.push_back(row);
                pieceYPositions.push_back(col);
            }
            else if (BlackQueen & (1ULL << index))
            {
                //                std::cout << "q" << " ";
                pieceTypes.push_back('q');
                pieceXPositions.push_back(row);
                pieceYPositions.push_back(col);
            }
            else if (BlackKing & (1ULL << index))
            {
                //                std::cout << "k" << " ";
                pieceTypes.push_back('k');
                pieceXPositions.push_back(row);
                pieceYPositions.push_back(col);
            }
            if (!(AllPieces & (1ULL << index)))
            {
                //                std::cout << "0" << " ";
            }
        }
        //        std::cout << std::endl;
    }
}