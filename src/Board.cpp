#include "../include/Board.hpp"
#include <iostream>
#include <sstream>
#include <cctype>
#include "../include/Move.hpp"
Board::Board()
{
}

Board::~Board()
{
}

void Board::setBitBoard(const std::string &fen_full)
{
    clearBoard();
    std::cout << "Setting bitboard from FEN: " << fen_full << std::endl;

    std::istringstream iss(fen_full);
    std::string fen;
    std::string player;
    std::string castling;
    std::string enPassant;

    iss >> fen;
    iss >> player;
    iss >> castling;
    iss >> enPassant;

    this->player = (player == "w") ? 1 : 2;

    std::cout << "FEN part: " << fen << std::endl;

    std::cout << "FEN full: " << fen_full << std::endl;

    std::cout << "Player: " << player << std::endl;
    std::cout << "Castling: " << castling << std::endl;
    std::cout << "En Passant: " << enPassant << std::endl;

    int row = 7;
    int col = 0;
    for (int i = 0; i < fen.length(); i++)
    {
        int index = row * 8 + col;
        if (fen[i] == '/')
        {
            row--;
            col = 0;
            continue;
        };
        if (isdigit(fen[i]))
        {
            col = col + (fen[i] - '0');
            continue;
        };

        if (isupper(fen[i]))
        {
            if (fen[i] == 'P')
            {
                WhitePawns |= (1ULL << index);
            }
            else if (fen[i] == 'N')
            {
                WhiteKnight |= (1ULL << index);
            }
            else if (fen[i] == 'K')
            {
                WhiteKing |= (1ULL << index);
            }
            else if (fen[i] == 'Q')
            {
                WhiteQueen |= (1ULL << index);
            }
            else if (fen[i] == 'R')
            {
                WhiteRook |= (1ULL << index);
            }
            else if (fen[i] == 'B')
            {
                WhiteBishop |= (1ULL << index);
            }
        }
        else
        {
            if (fen[i] == 'p')
            {
                BlackPawns |= (1ULL << index);
            }
            else if (fen[i] == 'n')
            {
                BlackKnight |= (1ULL << index);
            }
            else if (fen[i] == 'k')
            {
                BlackKing |= (1ULL << index);
            }
            else if (fen[i] == 'q')
            {
                BlackQueen |= (1ULL << index);
            }
            else if (fen[i] == 'r')
            {
                BlackRook |= (1ULL << index);
            }
            else if (fen[i] == 'b')
            {
                BlackBishop |= (1ULL << index);
            }
        }
        col += 1;
    }

    BlackPieces = BlackKing | BlackRook | BlackPawns | BlackQueen | BlackBishop | BlackKnight;
    WhitePieces = WhiteKing | WhiteRook | WhitePawns | WhiteQueen | WhiteBishop | WhiteKnight;
    AllPieces = BlackPieces | WhitePieces;

    printBitBoards();
}

void Board::clearBoard()
{
    WhitePawns = 0ULL;
    WhiteRook = 0ULL;
    WhiteKnight = 0ULL;
    WhiteBishop = 0ULL;
    WhiteQueen = 0ULL;
    WhiteKing = 0ULL;

    BlackPawns = 0ULL;
    BlackRook = 0ULL;
    BlackKnight = 0ULL;
    BlackBishop = 0ULL;
    BlackQueen = 0ULL;
    BlackKing = 0ULL;

    AllPieces = 0ULL;
    WhitePieces = 0ULL;
    BlackPieces = 0ULL;

    player = 0;
    enPassant = 0;
}

void Board::makeMove(Move &move)
{
    uint64_t attackSquare = 1ULL << (move.endR * 8 + move.endC);

    uint64_t StartSquare = 1ULL << (move.startR * 8 + move.startC);

    if (move.pieceName == 'R')
    {
        if (move.startC == 0 and move.startR == 0)
        {
            castlingRights = castlingRights ^ 0b00000010;
        }
        if (move.startC == 7 and move.startR == 0)
        {
            castlingRights = castlingRights ^ 0b00000001;
        }
    }

    if (move.pieceName == 'r')
    {
        if (move.startC == 0 and move.startR == 7)
        {
            castlingRights = castlingRights ^ 0b00001000;
        }
        if (move.startC == 7 and move.startR == 7)
        {
            castlingRights = castlingRights ^ 0b00000100;
        }
    }

    if (attackSquare & BlackPawns)
    {
        BlackPawns = BlackPawns ^ attackSquare;
        move.capture = 'p';
    }
    else if (attackSquare & BlackBishop)
    {
        BlackBishop = BlackBishop ^ attackSquare;
        move.capture = 'b';
    }
    else if (attackSquare & BlackRook)
    {
        BlackRook = BlackRook ^ attackSquare;
        move.capture = 'r';
    }
    else if (attackSquare & BlackKnight)
    {
        BlackKnight = BlackKnight ^ attackSquare;
        move.capture = 'n';
    }
    else if (attackSquare & BlackQueen)
    {
        BlackQueen = BlackQueen ^ attackSquare;
        move.capture = 'q';
    }
    else if (attackSquare & BlackKing)
    {
        BlackKing = BlackKing ^ attackSquare;
        move.capture = 'k';
    }
    if (attackSquare & WhitePawns)
    {
        WhitePawns = WhitePawns ^ attackSquare;
        move.capture = 'P';
    }
    else if (attackSquare & WhiteBishop)
    {
        WhiteBishop = WhiteBishop ^ attackSquare;
        move.capture = 'B';
    }
    else if (attackSquare & WhiteRook)
    {
        WhiteRook = WhiteRook ^ attackSquare;
        move.capture = 'R';
    }
    else if (attackSquare & WhiteKnight)
    {
        WhiteKnight = WhiteKnight ^ attackSquare;
        move.capture = 'N';
    }
    else if (attackSquare & WhiteQueen)
    {
        WhiteQueen = WhiteQueen ^ attackSquare;
        move.capture = 'Q';
    }
    else if (attackSquare & WhiteKing)
    {
        WhiteKing = WhiteKing ^ attackSquare;
        move.capture = 'K';
    }

    uint64_t &sourceBitBoard = getBitBoard(move.pieceName);

    sourceBitBoard = sourceBitBoard ^ StartSquare;

    sourceBitBoard = sourceBitBoard | attackSquare;

    if (0b1111111100000000000000000000000000000000000000000000000000000000 & WhitePawns)
    {
        uint64_t wherePawnIs = 0b1111111100000000000000000000000000000000000000000000000000000000 & WhitePawns;

        WhiteQueen |= (0b1111111100000000000000000000000000000000000000000000000000000000 & WhitePawns);
        WhitePawns = WhitePawns ^ wherePawnIs;
        move.promotion = 'Q';
    }

    if (0b11111111 & BlackPawns)
    {
        uint64_t wherePawnIs = 0b11111111 & BlackPawns;

        BlackQueen |= (0b11111111 & BlackPawns);
        BlackPawns = BlackPawns ^ wherePawnIs;
        move.promotion = 'q';
    }

    // pass in the oposite king of player who just moved
    //    int player = (islower(move.pieceName)) ? 1 : 2;

    BlackPieces = BlackKing | BlackRook | BlackPawns | BlackQueen | BlackBishop | BlackKnight;
    WhitePieces = WhiteKing | WhiteRook | WhitePawns | WhiteQueen | WhiteBishop | WhiteKnight;
    AllPieces = BlackPieces | WhitePieces;
}

uint64_t &Board::getBitBoard(char pieceType)
{
    switch (pieceType)
    {
    case 'P':
        return WhitePawns;
    case 'N':
        return WhiteKnight;
    case 'B':
        return WhiteBishop;
    case 'R':
        return WhiteRook;
    case 'Q':
        return WhiteQueen;
    case 'K':
        return WhiteKing;

    case 'p':
        return BlackPawns;
    case 'n':
        return BlackKnight;
    case 'b':
        return BlackBishop;
    case 'r':
        return BlackRook;
    case 'q':
        return BlackQueen;
    case 'k':
        return BlackKing;
    }
    return AllPieces;
}

void Board::undoMove(Move &move)
{
    uint64_t EndSquare = 1ULL << (move.endR * 8 + move.endC);

    uint64_t StartSquare = 1ULL << (move.startR * 8 + move.startC);

    if (move.promotion != '\0')
    {
        if (move.promotion == 'Q')
        {
            uint64_t &promotedQ = getBitBoard('Q');
            promotedQ = promotedQ ^ EndSquare;
            WhitePawns |= EndSquare;
        }

        else if (move.promotion == 'q')
        {
            uint64_t &promotedQ = getBitBoard('q');
            promotedQ = promotedQ ^ EndSquare;
            BlackPawns |= EndSquare;
        }
    }

    if (move.castling == 1 and move.pieceName == 'k')
    {

        uint64_t whereRookgonnaBe = 0b1000000000000000000000000000000000000000000000000000000000000000;
        uint64_t whereRookis = 0b10000000000000000000000000000000000000000000000000000000000000;
        if (BlackRook & whereRookis)
        {
            BlackRook = BlackRook ^ whereRookis;
            BlackRook = BlackRook | whereRookgonnaBe;

            castlingRights = castlingRights | 0b00001000;
        }
    }
    if (move.castling == 1 and move.pieceName == 'K')
    {

        uint64_t whereRookgonnaBe = 0b10000000;
        uint64_t whereRookis = 0b100000;

        if (WhiteRook & whereRookis)
        {
            WhiteRook = WhiteRook ^ whereRookis;
            WhiteRook = WhiteRook | whereRookgonnaBe;

            castlingRights = castlingRights | 0b00000001;
        }
    }

    if (move.castling == 2 and move.pieceName == 'k')
    {

        uint64_t whereRookgonnaBe = 0b100000000000000000000000000000000000000000000000000000000;
        uint64_t whereRookis = 0b100000000000000000000000000000000000000000000000000000000000;

        if (BlackRook & whereRookis)
        {
            BlackRook = BlackRook ^ whereRookis;
            BlackRook = BlackRook | whereRookgonnaBe;

            castlingRights = castlingRights | 0b00001000;
        }
    }
    if (move.castling == 2 and move.pieceName == 'K')
    {

        uint64_t whereRookgonnaBe = 0b1;
        uint64_t whereRookis = 0b1000;

        if (WhiteRook & whereRookis)
        {
            WhiteRook = WhiteRook ^ whereRookis;
            WhiteRook = WhiteRook | whereRookgonnaBe;

            castlingRights = castlingRights | 0b00000010;
        }
    }

    // Clear the bit of where the move moved the piece
    uint64_t &pieceMovedBB = getBitBoard(move.pieceName);
    pieceMovedBB = pieceMovedBB ^ EndSquare;

    pieceMovedBB = pieceMovedBB | StartSquare;

    if (move.capture != '\0')
    {
        // ReAdd captured piece
        uint64_t &pieceCapturedBB = getBitBoard(move.capture);
        pieceCapturedBB = pieceCapturedBB | EndSquare;
    }

    BlackPieces = BlackKing | BlackRook | BlackPawns | BlackQueen | BlackBishop | BlackKnight;
    WhitePieces = WhiteKing | WhiteRook | WhitePawns | WhiteQueen | WhiteBishop | WhiteKnight;
    AllPieces = BlackPieces | WhitePieces;
}

void Board::printBitBoards()
{

    // Iterate through each square on the board
    for (int row = 7; row >= 0; --row)
    {
        std::cout << row + 1 << " ";

        for (int col = 0; col < 8; ++col)
        {
            int index = row * 8 + col;

            // Check if a white piece is on the square
            if (WhitePawns & (1ULL << index))
            {
                std::cout << "P" << " ";
            }
            else if (WhiteKnight & (1ULL << index))
            {
                std::cout << "N" << " ";
            }
            else if (WhiteBishop & (1ULL << index))
            {
                std::cout << "B" << " ";
            }
            else if (WhiteRook & (1ULL << index))
            {
                std::cout << "R" << " ";
            }
            else if (WhiteQueen & (1ULL << index))
            {
                std::cout << "Q" << " ";
            }
            else if (WhiteKing & (1ULL << index))
            {
                std::cout << "K" << " ";
            }

            // Check if a black piece is on the square
            if (BlackPawns & (1ULL << index))
            {
                std::cout << "p" << " ";
            }
            else if (BlackKnight & (1ULL << index))
            {
                std::cout << "n" << " ";
            }
            else if (BlackBishop & (1ULL << index))
            {
                std::cout << "b" << " ";
            }
            else if (BlackRook & (1ULL << index))
            {
                std::cout << "r" << " ";
            }
            else if (BlackQueen & (1ULL << index))
            {
                std::cout << "q" << " ";
            }
            else if (BlackKing & (1ULL << index))
            {
                std::cout << "k" << " ";
            }
            if (!(AllPieces & (1ULL << index)))
            {
                std::cout << "0" << " ";
            }
        }
        std::cout << std::endl;
    }

    std::cout << "  ";

    for (int i = 0; i < 8; ++i)
    {
        std::cout << char('a' + i) << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}