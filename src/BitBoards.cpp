//
//  BitBoards.cpp
//  BitBoardStuff
//
//  Created by Darragh Flynn on 15/09/2024.
//

#include "../include/BitBoards.hpp"
#include <cctype>

/// Takes in input to inialise the board positions - sets all bit boards
/// - Parameter fen: fen notaion string
BitBoards::BitBoards(std::string fen)
{
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
};

/// Looks at all the BitBoards in class and returns an evaluation score
int BitBoards::evalPosition()
{
    int eval = 0;

    int numBitsSetP = __builtin_popcountll(WhitePawns);
    int numBitsSetR = __builtin_popcountll(WhiteRook);
    int numBitsSetN = __builtin_popcountll(WhiteKnight);
    int numBitsSetB = __builtin_popcountll(WhiteBishop);
    int numBitsSetQ = __builtin_popcountll(WhiteQueen);
    int numBitsSetK = __builtin_popcountll(WhiteKing);

    int numBitsSetp = __builtin_popcountll(BlackPawns);
    int numBitsSetr = __builtin_popcountll(BlackRook);
    int numBitsSetn = __builtin_popcountll(BlackKnight);
    int numBitsSetb = __builtin_popcountll(BlackBishop);
    int numBitsSetq = __builtin_popcountll(BlackQueen);
    int numBitsSetk = __builtin_popcountll(BlackKing);

    int evalW = (numBitsSetP * 100) + (numBitsSetR * 500) + (numBitsSetN * 300) + (numBitsSetB * 300) + (numBitsSetQ * 900) + (numBitsSetK * 100000);

    int evalB = (numBitsSetp * 100) + (numBitsSetr * 500) + (numBitsSetn * 300) + (numBitsSetb * 300) + (numBitsSetq * 900) + (numBitsSetk * 100000);
    eval = evalW - evalB;

    // Knight Stuff
    uint64_t knight = WhiteKnight;
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
    uint64_t knight2 = BlackKnight;
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

/// Generates all possible moves for a player
/// - Parameter player: int 1 for white 2 for black
std::vector<Move> BitBoards::GenerateAllMoves(int player)
{
    std::vector<Move> legalMoves;
    std::vector<Move> legalMovesP;
    std::vector<Move> legalMovesR;
    std::vector<Move> legalMovesB;
    std::vector<Move> legalMovesK;
    std::vector<Move> legalMovesN;
    std::vector<Move> legalMovesQ;

    if (player == 1)
    {
        legalMovesP = GenerateWhitePawnMoves();
    }
    else
    {
        legalMovesP = GenerateBlackPawnMoves();
    }

    legalMovesR = GenerateRookMoves(player);
    legalMovesB = GenerateBishopMoves(player);
    legalMovesK = GenerateKingMoves(player);
    legalMovesN = GenerateKnightMoves(player);
    legalMovesQ = GenerateQueenMoves(player);

    legalMoves.insert(legalMoves.end(), legalMovesP.begin(), legalMovesP.end());
    legalMoves.insert(legalMoves.end(), legalMovesR.begin(), legalMovesR.end());
    legalMoves.insert(legalMoves.end(), legalMovesB.begin(), legalMovesB.end());
    legalMoves.insert(legalMoves.end(), legalMovesK.begin(), legalMovesK.end());
    legalMoves.insert(legalMoves.end(), legalMovesN.begin(), legalMovesN.end());
    legalMoves.insert(legalMoves.end(), legalMovesQ.begin(), legalMovesQ.end());

    return legalMoves;
}

std::vector<Move> BitBoards::GenerateWhitePawnMoves()
{
    std::vector<Move> legalMoves;

    uint64_t pawn = WhitePawns;

    uint64_t getOnlyPawn = 0ULL;
    uint64_t pushOne = 0ULL;
    uint64_t pushTwo = 0ULL;
    uint64_t takeRight = 0ULL;
    uint64_t takeLeft = 0ULL;

    int index = __builtin_ctzll(pawn);
    //    printf("I after buildtin %d", index);

    //    printBitboard(pawn);

    for (int i = index; i < 64; i++)
    {
        getOnlyPawn = 0ULL;
        pushOne = 0ULL;
        pushTwo = 0ULL;
        takeRight = 0ULL;
        takeLeft = 0ULL;

        i = __builtin_ctzll(pawn);

        if (pawn == 0)
        {
            break;
        }

        // Bit board with only the pawn in question on it
        getOnlyPawn |= (1ULL << (i));

        // shift to get bitboard with the pawn pushed one square forward
        pushOne = getOnlyPawn << 8;

        // If empty add
        if (!(pushOne & AllPieces))
        {
            legalMoves.push_back(Move('P', '\0', 0, i / 8, i % 8, (i / 8) + 1, i % 8));
        };

        if ((i / 8) == 1 and !(pushOne & AllPieces))
        {
            pushTwo = getOnlyPawn << 16;

            if (!(pushTwo & AllPieces))
            {
                legalMoves.push_back(Move('P', '\0', 0, i / 8, i % 8, (i / 8) + 2, i % 8));
            }
        }

        if ((i % 8) != 7)
        {
            takeRight = pushOne << 1;

            if (takeRight & BlackPieces)
            {
                legalMoves.push_back(Move('P', 'Y', 0, i / 8, i % 8, (i / 8) + 1, (i % 8) + 1));
            }
        }

        if ((i % 8) != 0)
        {
            takeLeft = pushOne >> 1;

            if (takeLeft & BlackPieces)
            {
                legalMoves.push_back(Move('P', 'Y', 0, i / 8, i % 8, (i / 8) + 1, (i % 8) - 1));
            }
        }
        pawn &= pawn - 1;
    }

    return legalMoves;
};

std::vector<Move> BitBoards::GenerateBlackPawnMoves()
{
    std::vector<Move> legalMoves;

    uint64_t pawn = BlackPawns;

    uint64_t getOnlyPawn = 0ULL;
    uint64_t pushOne = 0ULL;
    uint64_t pushTwo = 0ULL;
    uint64_t takeRight = 0ULL;
    uint64_t takeLeft = 0ULL;

    int index = __builtin_ctzll(pawn);

    for (int i = index; i < 64; i++)
    {
        getOnlyPawn = 0ULL;
        pushOne = 0ULL;
        pushTwo = 0ULL;
        takeRight = 0ULL;
        takeLeft = 0ULL;

        i = __builtin_ctzll(pawn);

        if (pawn == 0)
        {
            break;
        }

        // Bit board with only the pawn in question on it
        getOnlyPawn |= (1ULL << (i));

        // shift to get bitboard with the pawn pushed one square forward
        pushOne = getOnlyPawn >> 8;

        // If empty add
        if (!(pushOne & AllPieces))
        {
            legalMoves.push_back(Move('p', '\0', 0, i / 8, i % 8, (i / 8) - 1, i % 8));
        };

        if ((i / 8) == 6 and !(pushOne & AllPieces))
        {
            pushTwo = getOnlyPawn >> 16;
            if (!(pushTwo & AllPieces))
            {
                legalMoves.push_back(Move('p', '\0', 0, i / 8, i % 8, (i / 8) - 2, i % 8));
            }
        }

        if ((i % 8) != 0)
        {
            takeRight = pushOne >> 1;

            if (takeRight & WhitePieces)
            {
                legalMoves.push_back(Move('p', 'Y', 0, i / 8, i % 8, (i / 8) - 1, (i % 8) - 1));
            }
        }

        if ((i % 8) != 7)
        {
            takeLeft = pushOne << 1;

            if (takeLeft & WhitePieces)
            {
                legalMoves.push_back(Move('p', 'Y', 0, i / 8, i % 8, (i / 8) - 1, (i % 8) + 1));
            }
        }

        //        printBitboard(getOnlyPawn);
        //        printf("row: %d - col: %d\n", i/8, i % 8);

        pawn &= pawn - 1;
    }

    return legalMoves;
};

std::vector<Move> BitBoards::GenerateBishopMoves(int player)
{
    std::vector<Move> legalMoves;

    // optional to decide which board
    uint64_t bishop = (player == 1) ? WhiteBishop : BlackBishop;

    uint64_t ownColor = (player == 1) ? WhitePieces : BlackPieces;

    uint64_t oppositeColor = (player == 1) ? BlackPieces : WhitePieces;

    char letter = (player == 1) ? 'B' : 'b';
    //

    uint64_t getOnlyBishop = 0ULL;

    int index = __builtin_ctzll(bishop);

    for (int i = index; i < 64; i++)
    {
        getOnlyBishop = 0ULL;

        i = __builtin_ctzll(bishop);

        if (bishop == 0)
        {
            break;
        }

        // Bit board with only the pawn in question on it
        getOnlyBishop |= (1ULL << (i));
        int r = i / 8;
        int c = i % 8;

        std::pair<int, int> offSet[] = {{-1, 1}, {-1, -1}, {1, 1}, {1, -1}};
        for (std::pair<int, int> offS : offSet)
        {
            int newR = r + offS.first, newC = c + offS.second;

            while (newR >= 0 && newR < 8 && newC >= 0 && newC < 8)
            {
                //                printf("new bit = %d\n", (newR*8 + newC));

                uint64_t attackSquare = 1ULL << (newR * 8 + newC);

                if (attackSquare & ownColor)
                {
                    break;
                }
                else if (attackSquare & oppositeColor)
                {
                    legalMoves.push_back(Move(letter, 'Y', 0, r, c, newR, newC));
                    break;
                }
                legalMoves.push_back(Move(letter, '\0', 0, r, c, newR, newC));

                newR = newR + offS.first;
                newC = newC + offS.second;
            }
        }

        //        printBitboard(getOnlyBishop);

        bishop &= bishop - 1;
    }

    return legalMoves;
};

std::vector<Move> BitBoards::GenerateRookMoves(int player)
{
    std::vector<Move> legalMoves;

    // optional to decide which board
    uint64_t rook = (player == 1) ? WhiteRook : BlackRook;

    uint64_t ownColor = (player == 1) ? WhitePieces : BlackPieces;

    uint64_t oppositeColor = (player == 1) ? BlackPieces : WhitePieces;

    char letter = (player == 1) ? 'R' : 'r';
    //

    uint64_t getOnlyRook = 0ULL;

    int index = __builtin_ctzll(rook);

    for (int i = index; i < 64; i++)
    {
        getOnlyRook = 0ULL;

        i = __builtin_ctzll(rook);

        if (rook == 0)
        {
            break;
        }

        // Bit board with only the pawn in question on it
        getOnlyRook |= (1ULL << (i));
        int r = i / 8;
        int c = i % 8;

        std::pair<int, int> offSet[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        for (std::pair<int, int> offS : offSet)
        {
            int newR = r + offS.first, newC = c + offS.second;

            while (newR >= 0 && newR < 8 && newC >= 0 && newC < 8)
            {
                //                printf("new bit = %d\n", (newR*8 + newC));

                uint64_t attackSquare = 1ULL << (newR * 8 + newC);

                if (attackSquare & ownColor)
                {
                    break;
                }
                else if (attackSquare & oppositeColor)
                {
                    legalMoves.push_back(Move(letter, 'Y', 0, r, c, newR, newC));
                    break;
                }
                legalMoves.push_back(Move(letter, '\0', 0, r, c, newR, newC));

                newR = newR + offS.first;
                newC = newC + offS.second;
            }
        }
        rook &= rook - 1;
    }

    return legalMoves;
};

std::vector<Move> BitBoards::GenerateQueenMoves(int player)
{
    std::vector<Move> legalMoves;

    // optional to decide which board
    uint64_t rook = (player == 1) ? WhiteQueen : BlackQueen;

    uint64_t ownColor = (player == 1) ? WhitePieces : BlackPieces;

    uint64_t oppositeColor = (player == 1) ? BlackPieces : WhitePieces;

    char letter = (player == 1) ? 'Q' : 'q';
    //
    uint64_t getOnlyRook = 0ULL;

    int index = __builtin_ctzll(rook);

    for (int i = index; i < 64; i++)
    {
        getOnlyRook = 0ULL;

        i = __builtin_ctzll(rook);

        if (rook == 0)
        {
            break;
        }

        // Bit board with only the pawn in question on it
        getOnlyRook |= (1ULL << (i));
        int r = i / 8;
        int c = i % 8;

        std::pair<int, int> offSet[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        for (std::pair<int, int> offS : offSet)
        {
            int newR = r + offS.first, newC = c + offS.second;

            while (newR >= 0 && newR < 8 && newC >= 0 && newC < 8)
            {
                uint64_t attackSquare = 1ULL << (newR * 8 + newC);

                if (attackSquare & ownColor)
                {
                    break;
                }
                else if (attackSquare & oppositeColor)
                {
                    legalMoves.push_back(Move(letter, 'Y', 0, r, c, newR, newC));
                    break;
                }
                legalMoves.push_back(Move(letter, '\0', 0, r, c, newR, newC));

                newR = newR + offS.first;
                newC = newC + offS.second;
            }
        }
        //        printBitboard(getOnlyRook);
        rook &= rook - 1;
    }

    // optional to decide which board
    uint64_t bishop = (player == 1) ? WhiteQueen : BlackQueen;
    //

    uint64_t getOnlyBishop = 0ULL;

    int index2 = __builtin_ctzll(bishop);

    for (int i = index2; i < 64; i++)
    {
        getOnlyBishop = 0ULL;

        i = __builtin_ctzll(bishop);

        if (bishop == 0)
        {
            break;
        }

        // Bit board with only the pawn in question on it
        getOnlyBishop |= (1ULL << (i));
        int r = i / 8;
        int c = i % 8;

        std::pair<int, int> offSet[] = {{-1, 1}, {-1, -1}, {1, 1}, {1, -1}};
        for (std::pair<int, int> offS : offSet)
        {
            int newR = r + offS.first, newC = c + offS.second;

            while (newR >= 0 && newR < 8 && newC >= 0 && newC < 8)
            {

                uint64_t attackSquare = 1ULL << (newR * 8 + newC);

                if (attackSquare & ownColor)
                {
                    break;
                }
                else if (attackSquare & oppositeColor)
                {
                    legalMoves.push_back(Move(letter, 'Y', 0, r, c, newR, newC));
                    break;
                }
                legalMoves.push_back(Move(letter, '\0', 0, r, c, newR, newC));

                newR = newR + offS.first;
                newC = newC + offS.second;
            }
        }
        //        printBitboard(getOnlyBishop);
        bishop &= bishop - 1;
    }

    return legalMoves;
}

std::vector<Move> BitBoards::GenerateKnightMoves(int player)
{
    std::vector<Move> legalMoves;

    // optional to decide which board
    uint64_t knight = (player == 1) ? WhiteKnight : BlackKnight;

    uint64_t ownColor = (player == 1) ? WhitePieces : BlackPieces;

    uint64_t oppositeColor = (player == 1) ? BlackPieces : WhitePieces;

    char letter = (player == 1) ? 'N' : 'n';

    uint64_t getOnlyKnight = 0ULL;

    int index = __builtin_ctzll(knight);

    for (int i = index; i < 64; i++)
    {
        getOnlyKnight = 0ULL;

        i = __builtin_ctzll(knight);

        if (knight == 0)
        {
            break;
        }

        // Bit board with only the pawn in question on it
        getOnlyKnight |= (1ULL << (i));

        int r = i / 8;
        int c = i % 8;
        std::pair<int, int> offSet[] = {{2, -1}, {2, 1}, {-2, 1}, {-2, -1}, {1, -2}, {1, 2}, {-1, 2}, {-1, -2}};
        for (std::pair<int, int> offS : offSet)
        {
            int newR = r + offS.first, newC = c + offS.second;
            if (newR >= 0 && newR < 8 && newC >= 0 && newC < 8)
            {
                uint64_t attackSquare = 1ULL << (newR * 8 + newC);
                if (attackSquare & ownColor)
                {
                    newR = newR + offS.first;
                    newC = newC + offS.second;
                    continue;
                }
                else if (attackSquare & oppositeColor)
                {
                    legalMoves.push_back(Move(letter, 'Y', 0, r, c, newR, newC));
                    newR = newR + offS.first;
                    newC = newC + offS.second;
                    continue;
                }
                legalMoves.push_back(Move(letter, '\0', 0, r, c, newR, newC));
            }
            newR = newR + offS.first;
            newC = newC + offS.second;
        }
        //        printBitboard(getOnlyKnight);
        knight &= knight - 1;
    }
    return legalMoves;
}

// int direction = (1 == getColor()) ? 1 : -1;

std::vector<Move> BitBoards::GenerateKingMoves(int player)
{
    std::vector<Move> legalMoves;

    std::pair<int, int> offSet[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    // optional to decide which board
    uint64_t king = (player == 1) ? WhiteKing : BlackKing;

    uint64_t ownColor = (player == 1) ? WhitePieces : BlackPieces;

    uint64_t oppositeColor = (player == 1) ? BlackPieces : WhitePieces;

    char letter = (player == 1) ? 'K' : 'k';

    //    printBitboard(king);

    uint64_t getOnlyKing = 0ULL;

    int index = __builtin_ctzll(king);

    for (int i = index; i < 64; i++)
    {
        getOnlyKing = 0ULL;

        i = __builtin_ctzll(king);

        if (king == 0)
        {
            break;
        }

        //        printf("kingBoard");
        // Bit board with only the pawn in question on it
        getOnlyKing |= (1ULL << (i));
        //        printBitboard(getOnlyKing);

        int r = i / 8;
        int c = i % 8;

        for (std::pair<int, int> offS : offSet)
        {
            int newR = r + offS.first, newC = c + offS.second;

            if (newR >= 0 && newR < 8 && newC >= 0 && newC < 8)
            {
                uint64_t attackSquare = 1ULL << (newR * 8 + newC);
                if (attackSquare & ownColor)
                {
                    newR = newR + offS.first;
                    newC = newC + offS.second;
                    continue;
                }
                else if (attackSquare & oppositeColor)
                {
                    legalMoves.push_back(Move(letter, 'Y', 0, r, c, newR, newC));
                    newR = newR + offS.first;
                    newC = newC + offS.second;
                    continue;
                }
                legalMoves.push_back(Move(letter, '\0', 0, r, c, newR, newC));
            }
            newR = newR + offS.first;
            newC = newC + offS.second;
        }

        //        printBitboard(getOnlyKing);
        king &= king - 1;
    }

    std::vector<Move> castle = kingCastling(player);

    legalMoves.insert(legalMoves.end(), castle.begin(), castle.end());

    return legalMoves;
}

std::vector<Move> BitBoards::kingCastling(int player)
{
    std::vector<Move> legalMoves;

    char letter = (player == 1) ? 'K' : 'k';

    uint64_t king2 = (player == 1) ? WhiteKing : BlackKing;

    uint64_t getOnlyKing2 = 0ULL;
    int index2 = __builtin_ctzll(king2);
    getOnlyKing2 |= (1ULL << (index2));
    int r = index2 / 8;
    int c = index2 % 8;

    if ((WhiteKing & getOnlyKing2) and (r == 0 and c == 4) and (WhiteRook & 0b10000000) and (castlingRights & 0b1) and !(0b1100000 & AllPieces))
    {
        legalMoves.push_back(Move(letter, '\0', 1, r, c, r, c + 2));
    }

    if ((WhiteKing & getOnlyKing2) and (r == 0 and c == 4) and (WhiteRook & 0b1) and (castlingRights & 0b10) and !(0b1110 & AllPieces))
    {
        legalMoves.push_back(Move(letter, '\0', 2, r, c, r, c - 2));
    }
    if ((BlackKing & getOnlyKing2) and (r == 7 and c == 4) and (BlackRook & 0b1000000000000000000000000000000000000000000000000000000000000000) and (castlingRights & 0b0100) and !(0b110000000000000000000000000000000000000000000000000000000000000 & AllPieces))
    {
        legalMoves.push_back(Move(letter, '\0', 1, r, c, r, c + 2));
    }
    if ((BlackKing & getOnlyKing2) and (r == 7 and c == 4) and (BlackRook & 0b100000000000000000000000000000000000000000000000000000000) and (castlingRights & 0b1000) and !(0b111000000000000000000000000000000000000000000000000000000000 & AllPieces))
    {
        legalMoves.push_back(Move(letter, '\0', 2, r, c, r, c - 2));
    }

    return legalMoves;
}

bool BitBoards::castlingChecks(Move move)
{

    uint64_t StartSquare = 1ULL << (move.startR * 8 + move.startC);

    if (move.castling == 1 and (StartSquare & WhiteKing) and (castlingRights & 0b00000001) and (0b10000000 & WhiteRook))
    {
        uint64_t whereRookgonnaBe = 0b100000;
        uint64_t whereRookis = 0b10000000;

        WhiteRook = WhiteRook ^ whereRookis;
        WhiteRook = WhiteRook | whereRookgonnaBe;

        castlingRights = castlingRights ^ 0b00000011;
    }

    if (move.castling == 1 and (StartSquare & BlackKing) and (castlingRights & 0b00000100) and (0b1000000000000000000000000000000000000000000000000000000000000000 and BlackRook))
    {
        uint64_t whereRookgonnaBe = 0b10000000000000000000000000000000000000000000000000000000000000;
        uint64_t whereRookis = 0b1000000000000000000000000000000000000000000000000000000000000000;

        BlackRook = BlackRook ^ whereRookis;
        BlackRook = BlackRook | whereRookgonnaBe;

        castlingRights = castlingRights ^ 0b00001100;
    }

    if (move.castling == 2 and (StartSquare & WhiteKing) and (castlingRights & 0b00000010) and (0b1 & WhiteRook))
    {
        uint64_t whereRookgonnaBe = 0b1000;
        uint64_t whereRookis = 0b1;

        WhiteRook = WhiteRook ^ whereRookis;
        WhiteRook = WhiteRook | whereRookgonnaBe;

        castlingRights = castlingRights ^ 0b00000011;
    }

    if (move.castling == 2 and (StartSquare & BlackKing) and (castlingRights & 0b00001000) and (0b100000000000000000000000000000000000000000000000000000000 & BlackRook))
    {
        uint64_t whereRookgonnaBe = 0b100000000000000000000000000000000000000000000000000000000000;
        uint64_t whereRookis = 0b100000000000000000000000000000000000000000000000000000000;

        BlackRook = BlackRook ^ whereRookis;
        BlackRook = BlackRook | whereRookgonnaBe;

        castlingRights = castlingRights ^ 0b00001100;
    }

    return true;
};

void BitBoards::kingInCheck(int player)
{

    printf("PLAYER TO CHECK %d\n", player);

    char kingLetter = (player == 1) ? 'K' : 'k';

    uint64_t &kingBB = getBitBoard(kingLetter);
    printBitboard(kingBB);

    uint64_t getOnlyKing2 = 0ULL;
    int index2 = __builtin_ctzll(kingBB);
    getOnlyKing2 |= (1ULL << (index2));
    int r = index2 / 8;
    int c = index2 % 8;

    printf("Row %d, COL %d\n", r, c);

    uint64_t slideAttack = 0ULL;

    // get opposite color rook board
    char rookLetter = (kingLetter == 'K') ? 'r' : 'R';

    uint64_t &rookBB = getBitBoard(rookLetter);
    //    printBitboard(rookBB);

    uint64_t allButRook = AllPieces ^ rookBB;

    for (int i = r; i >= 0; i--)
    {
        slideAttack |= kingBB >> (8 * i);
        if (slideAttack & allButRook)
        {
            break;
        }
    }
    int offUp = 1;
    for (int i = r; i <= 7; i++)
    {
        slideAttack |= kingBB << (8 * offUp);
        offUp++;
        if (slideAttack & allButRook)
        {
            break;
        }
    }
    int offRight = 0;
    for (int i = c; i >= 0; i--)
    {
        slideAttack |= kingBB >> (1 * offRight);
        offRight++;
        if (slideAttack & allButRook)
        {
            break;
        }
    }
    int off = 0;
    for (int i = c; i <= 7; i++)
    {
        slideAttack |= kingBB << (1 * off);
        off++;
        if (slideAttack & allButRook)
        {
            break;
        }
    }

    slideAttack = slideAttack ^ kingBB;

    printBitboard(slideAttack);
    printf("SLDIE ATTACK\n");

    printBitboard(allButRook);

    //    uint64_t underAttack = slideAttack

    if (rookBB & slideAttack)
    {
        printf("KING in CHECK");
    }
}

void BitBoards::makeMove(Move &move)
{
    uint64_t attackSquare = 1ULL << (move.endR * 8 + move.endC);

    uint64_t StartSquare = 1ULL << (move.startR * 8 + move.startC);

    if (move.castling != 0)
    {
        castlingChecks(move);
    }
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

    //    kingInCheck(player);
}

void BitBoards::undoMove(Move move)
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
};

uint64_t &BitBoards::getBitBoard(char pieceType)
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

void BitBoards::printBitboard(uint64_t board)
{
    std::bitset<64> bits(board);
    for (int rank = 7; rank >= 0; rank--)
    {
        std::cout << rank + 1 << " ";

        for (int col = 0; col < 8; col++)
        {
            std::cout << bits[rank * 8 + col] << " ";
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

void BitBoards::printBitBoards()
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

char BitBoards::getPieceletter(int index)
{
    if (WhitePawns & (1ULL << index))
    {
        std::cout << "P" << " ";
        return 'P';
    }
    else if (WhiteKnight & (1ULL << index))
    {
        std::cout << "N" << " ";
        return 'N';
    }
    else if (WhiteBishop & (1ULL << index))
    {
        std::cout << "B" << " ";
        return 'B';
    }
    else if (WhiteRook & (1ULL << index))
    {
        std::cout << "R" << " ";
        std::cout << "R" << " ";
        return 'R';
    }
    else if (WhiteQueen & (1ULL << index))
    {
        std::cout << "Q" << " ";
        return 'Q';
    }
    else if (WhiteKing & (1ULL << index))
    {
        std::cout << "K" << " ";
        return 'K';
    }

    // Check if a black piece is on the square
    if (BlackPawns & (1ULL << index))
    {
        std::cout << "p" << " ";
        return 'p';
    }
    else if (BlackKnight & (1ULL << index))
    {
        std::cout << "n" << " ";
        return 'n';
    }
    else if (BlackBishop & (1ULL << index))
    {
        std::cout << "b" << " ";
        return 'b';
    }
    else if (BlackRook & (1ULL << index))
    {
        std::cout << "r" << " ";
        return 'r';
    }
    else if (BlackQueen & (1ULL << index))
    {
        std::cout << "q" << " ";
        return 'q';
    }
    else if (BlackKing & (1ULL << index))
    {
        std::cout << "k" << " ";
        return 'k';
    }
    else
    {
        return '\0';
    }
}

RenderData BitBoards::populateRenderData()
{
    RenderData data;
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
                data.pieceTypes.push_back('P');
                data.pieceXPositions.push_back(row);
                data.pieceYPositions.push_back(col);
            }
            else if (WhiteKnight & (1ULL << index))
            {
                //                std::cout << "N" << " ";
                data.pieceTypes.push_back('N');
                data.pieceXPositions.push_back(row);
                data.pieceYPositions.push_back(col);
            }
            else if (WhiteBishop & (1ULL << index))
            {
                //                std::cout << "B" << " ";
                data.pieceTypes.push_back('B');
                data.pieceXPositions.push_back(row);
                data.pieceYPositions.push_back(col);
            }
            else if (WhiteRook & (1ULL << index))
            {
                //                std::cout << "R" << " ";
                data.pieceTypes.push_back('R');
                data.pieceXPositions.push_back(row);
                data.pieceYPositions.push_back(col);
            }
            else if (WhiteQueen & (1ULL << index))
            {
                //                std::cout << "Q" << " ";
                data.pieceTypes.push_back('Q');
                data.pieceXPositions.push_back(row);
                data.pieceYPositions.push_back(col);
            }
            else if (WhiteKing & (1ULL << index))
            {
                //                std::cout << "K" << " ";
                data.pieceTypes.push_back('K');
                data.pieceXPositions.push_back(row);
                data.pieceYPositions.push_back(col);
            }

            // Check if a black piece is on the square
            if (BlackPawns & (1ULL << index))
            {
                //                std::cout << "p" << " ";
                data.pieceTypes.push_back('p');
                data.pieceXPositions.push_back(row);
                data.pieceYPositions.push_back(col);
            }
            else if (BlackKnight & (1ULL << index))
            {
                //                std::cout << "n" << " ";
                data.pieceTypes.push_back('n');
                data.pieceXPositions.push_back(row);
                data.pieceYPositions.push_back(col);
            }
            else if (BlackBishop & (1ULL << index))
            {
                //                std::cout << "b" << " ";
                data.pieceTypes.push_back('b');
                data.pieceXPositions.push_back(row);
                data.pieceYPositions.push_back(col);
            }
            else if (BlackRook & (1ULL << index))
            {
                //                std::cout << "r" << " ";
                data.pieceTypes.push_back('r');
                data.pieceXPositions.push_back(row);
                data.pieceYPositions.push_back(col);
            }
            else if (BlackQueen & (1ULL << index))
            {
                //                std::cout << "q" << " ";
                data.pieceTypes.push_back('q');
                data.pieceXPositions.push_back(row);
                data.pieceYPositions.push_back(col);
            }
            else if (BlackKing & (1ULL << index))
            {
                //                std::cout << "k" << " ";
                data.pieceTypes.push_back('k');
                data.pieceXPositions.push_back(row);
                data.pieceYPositions.push_back(col);
            }
            if (!(AllPieces & (1ULL << index)))
            {
                //                std::cout << "0" << " ";
            }
        }
        //        std::cout << std::endl;
    }
    return data;
}

bool BitBoards::validatePlayerMove(int player, Move m)
{
    std::vector<Move> legalMoves;

    //    printf("piece name %c", m.pieceName);

    if (islower(m.pieceName) and player != 2)
    {
        return false;
    }
    if (isupper(m.pieceName) and player != 1)
    {
        return false;
    }

    switch (m.pieceName)
    {
    case 'P':
        legalMoves = GenerateWhitePawnMoves();
        break;
    case 'N':
        legalMoves = GenerateKnightMoves(1);
        break;
    case 'B':
        legalMoves = GenerateBishopMoves(1);
        break;
    case 'R':
        legalMoves = GenerateRookMoves(1);
        break;
    case 'Q':
        legalMoves = GenerateQueenMoves(1);
        break;
    case 'K':
        legalMoves = GenerateKingMoves(1);
        break;

    case 'p':
        legalMoves = GenerateBlackPawnMoves();
        break;
    case 'n':
        legalMoves = GenerateKnightMoves(2);
        break;
    case 'b':
        legalMoves = GenerateBishopMoves(2);
        break;
    case 'r':
        legalMoves = GenerateRookMoves(2);
        break;
    case 'q':
        legalMoves = GenerateQueenMoves(2);
        break;
    case 'k':
        legalMoves = GenerateKingMoves(2);
        break;
    }

    for (Move m2 : legalMoves)
    {
        if (m2.endR == m.endR and m2.endC == m.endC and m.startC == m2.startC and m.startR == m2.startR)
        {
            return true;
        }
    }
    return false;
}

Move BitBoards::AImove()
{
    Move best;
    std::vector<Move> legalMoves;
    legalMoves = GenerateAllMoves(2);
    //    printLegalMoves(legalMoves);

    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    int minEval = std::numeric_limits<int>::max();
    for (Move m : legalMoves)
    {
        makeMove(m);
        int eval = minimax(4, true, alpha, beta);

        if (eval < minEval)
        {
            minEval = eval;
            best = m;
        }
        undoMove(m);
        if (m.castling == 2)
        {
            return m;
        }
    }
    return best;
}

// Minimax algorithm
int BitBoards::minimax(int depth, bool maximizingPlayer, int alpha, int beta)
{
    if (depth == 0)
    {
        return evalPosition();
    }

    if (maximizingPlayer)
    {
        std::vector<Move> legalMoves = GenerateAllMoves(1);
        int maxEval = std::numeric_limits<int>::min();

        for (Move m : legalMoves)
        {
            makeMove(m);
            int maximiseEval = minimax(depth - 1, false, alpha, beta);

            undoMove(m);
            maxEval = std::max(maximiseEval, maxEval);

            alpha = std::max(alpha, maxEval);
            if (beta <= alpha)
            {
                // Prune the branch, since the minimizing player would never allow this outcome
                break;
            }
        }
        return maxEval;
    }
    else
    {
        std::vector<Move> legalMoves = GenerateAllMoves(2);
        int minEval = std::numeric_limits<int>::max();

        for (Move m : legalMoves)
        {
            makeMove(m);
            int minimiseEval = minimax(depth - 1, true, alpha, beta);
            undoMove(m);
            minEval = std::min(minimiseEval, minEval);

            beta = std::min(beta, minEval);
            if (beta <= alpha)
            {
                // Prune the branch, since the maximizing player would never allow this outcome
                break;
            }
        }
        return minEval;
    }

    return 0;
}

void BitBoards::printLegalMoves(const std::vector<Move> legalMoves)
{
    std::cout << "Legal Moves:\n";
    for (const Move &move : legalMoves)
    {
        std::cout << move.pieceName << " from (" << move.startR << ", " << move.startC << ") to (" << move.endR << ", " << move.endC << ")";
        if (move.capture)
        {
            std::cout << " (capture)";
        }
        std::cout << std::endl;
    }
}

void BitBoards::printMove(const Move &move)
{
    std::cout << move.pieceName << " from (" << move.startR << ", " << move.startC << ") to (" << move.endR << ", " << move.endC << ")";
    if (move.capture != '\0')
    {
        std::cout << " (captured " << move.capture << ")";
    }
    std::cout << std::endl;
}
