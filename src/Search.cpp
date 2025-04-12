#include "../include/Search.hpp"
#include "../include/Board.hpp"
#include "../include/MoveGenerator.hpp"
#include <limits>
#include <iostream>
Search::Search() : maxDepth(5), moveGenerator(), evaluation()
{
}

Search::~Search()
{
}

Move Search::search(Board &board)
{
    Move bestMove;
    std::vector<Move> legalMoves = moveGenerator.generateMoves(board);

    if (legalMoves.empty())
    {
        return Move(); // Return empty move if no legal moves
    }

    // Determine if we're searching for white (player=1) or black (player=2)
    bool isWhite = (board.player == 1);

    // White maximises, Black minimises
    int bestScore = isWhite ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

    for (Move move : legalMoves)
    {
        board.makeMove(move);
        // Start minimax with the opposite player
        int score = minimax(board, maxDepth - 1, !isWhite,
                            std::numeric_limits<int>::min(),
                            std::numeric_limits<int>::max());
        board.undoMove(move);

        // Update best move based on player
        if ((isWhite && score > bestScore) || (!isWhite && score < bestScore))
        {
            bestScore = score;
            bestMove = move;
        }

        // Shortcut for castling (optional - remove if not needed)
        if (move.castling == 2)
        {
            return move;
        }
    }

    return bestMove;
}

// Minimax algorithm
int Search::minimax(Board &board, int depth, bool maximizingPlayer, int alpha, int beta)
{
    // Base case: at leaf node or terminal position
    if (depth == 0)
    {
        return evaluation.evalPosition(board);
    }

    std::vector<Move> legalMoves = moveGenerator.generateMoves(board);

    // If maximizing player (White)
    if (maximizingPlayer)
    {
        int maxEval = std::numeric_limits<int>::min();

        for (Move move : legalMoves)
        {
            board.makeMove(move);
            int eval = minimax(board, depth - 1, false, alpha, beta);
            board.undoMove(move);

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, maxEval);

            if (beta <= alpha)
            {
                break; // Beta cutoff
            }
        }

        return maxEval;
    }
    // Minimizing player (Black)
    else
    {
        int minEval = std::numeric_limits<int>::max();

        for (Move move : legalMoves)
        {
            board.makeMove(move);
            int eval = minimax(board, depth - 1, true, alpha, beta);
            board.undoMove(move);

            minEval = std::min(minEval, eval);
            beta = std::min(beta, minEval);

            if (beta <= alpha)
            {
                break; // Alpha cutoff
            }
        }

        return minEval;
    }
}