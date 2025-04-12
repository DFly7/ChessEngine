#include "../include/MoveGenerator.hpp"
#include "../include/Board.hpp"

MoveGenerator::MoveGenerator()
{
}

MoveGenerator::~MoveGenerator()
{
}

// Update internal state
void MoveGenerator::updateBoardReferences(const Board &board)
{
    castlingRights = board.castlingRights;
    AllPieces = board.AllPieces;
    WhitePieces = board.WhitePieces;
    WhitePawns = board.WhitePawns;
    WhiteRook = board.WhiteRook;
    WhiteKnight = board.WhiteKnight;
    WhiteBishop = board.WhiteBishop;
    WhiteQueen = board.WhiteQueen;
    WhiteKing = board.WhiteKing;
    BlackPieces = board.BlackPieces;
    BlackPawns = board.BlackPawns;
    BlackRook = board.BlackRook;
    BlackKnight = board.BlackKnight;
    BlackBishop = board.BlackBishop;
    BlackQueen = board.BlackQueen;
    BlackKing = board.BlackKing;
    player = board.player;
    enPassant = board.enPassant;
}

std::vector<Move> MoveGenerator::generateMoves(const Board &board)
{
    updateBoardReferences(board);

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

std::vector<Move> MoveGenerator::GenerateWhitePawnMoves()
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

std::vector<Move> MoveGenerator::GenerateBlackPawnMoves()
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

std::vector<Move> MoveGenerator::GenerateBishopMoves(int player)
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

std::vector<Move> MoveGenerator::GenerateRookMoves(int player)
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

std::vector<Move> MoveGenerator::GenerateQueenMoves(int player)
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

std::vector<Move> MoveGenerator::GenerateKnightMoves(int player)
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

std::vector<Move> MoveGenerator::GenerateKingMoves(int player)
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

std::vector<Move> MoveGenerator::kingCastling(int player)
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