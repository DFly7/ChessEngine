#include "UCIInterface.hpp"
#include <string>
#include "View.hpp"
#include <utility>
#include "Board.hpp"
#include "MoveGenerator.hpp"
#include "Evaluation.hpp"
#include "Search.hpp"
#include "RenderData.hpp"

char getPieceletter(int index, Board &board)
{
    if (board.WhitePawns & (1ULL << index))
    {
        std::cout << "P" << " ";
        return 'P';
    }
    else if (board.WhiteKnight & (1ULL << index))
    {
        std::cout << "N" << " ";
        return 'N';
    }
    else if (board.WhiteBishop & (1ULL << index))
    {
        std::cout << "B" << " ";
        return 'B';
    }
    else if (board.WhiteRook & (1ULL << index))
    {
        std::cout << "R" << " ";
        std::cout << "R" << " ";
        return 'R';
    }
    else if (board.WhiteQueen & (1ULL << index))
    {
        std::cout << "Q" << " ";
        return 'Q';
    }
    else if (board.WhiteKing & (1ULL << index))
    {
        std::cout << "K" << " ";
        return 'K';
    }

    // Check if a black piece is on the square
    if (board.BlackPawns & (1ULL << index))
    {
        std::cout << "p" << " ";
        return 'p';
    }
    else if (board.BlackKnight & (1ULL << index))
    {
        std::cout << "n" << " ";
        return 'n';
    }
    else if (board.BlackBishop & (1ULL << index))
    {
        std::cout << "b" << " ";
        return 'b';
    }
    else if (board.BlackRook & (1ULL << index))
    {
        std::cout << "r" << " ";
        return 'r';
    }
    else if (board.BlackQueen & (1ULL << index))
    {
        std::cout << "q" << " ";
        return 'q';
    }
    else if (board.BlackKing & (1ULL << index))
    {
        std::cout << "k" << " ";
        return 'k';
    }
    else
    {
        return '\0';
    }
}

Move validatePlayerMove(int player, Move m, MoveGenerator &moveGenerator, Board &board)
{
    std::vector<Move> legalMoves;

    moveGenerator.updateBoardReferences(board);

    //    printf("piece name %c", m.pieceName);

    if (islower(m.pieceName) and player != 2)
    {
        return Move();
    }
    if (isupper(m.pieceName) and player != 1)
    {
        return Move();
    }

    switch (m.pieceName)
    {
    case 'P':
        legalMoves = moveGenerator.GenerateWhitePawnMoves();
        break;
    case 'N':
        legalMoves = moveGenerator.GenerateKnightMoves(1);
        break;
    case 'B':
        legalMoves = moveGenerator.GenerateBishopMoves(1);
        break;
    case 'R':
        legalMoves = moveGenerator.GenerateRookMoves(1);
        break;
    case 'Q':
        legalMoves = moveGenerator.GenerateQueenMoves(1);
        break;
    case 'K':
        legalMoves = moveGenerator.GenerateKingMoves(1);
        break;

    case 'p':
        legalMoves = moveGenerator.GenerateBlackPawnMoves();
        break;
    case 'n':
        legalMoves = moveGenerator.GenerateKnightMoves(2);
        break;
    case 'b':
        legalMoves = moveGenerator.GenerateBishopMoves(2);
        break;
    case 'r':
        legalMoves = moveGenerator.GenerateRookMoves(2);
        break;
    case 'q':
        legalMoves = moveGenerator.GenerateQueenMoves(2);
        break;
    case 'k':
        legalMoves = moveGenerator.GenerateKingMoves(2);
        break;
    }

    for (Move m2 : legalMoves)
    {
        if (m2.endR == m.endR and m2.endC == m.endC and m.startC == m2.startC and m.startR == m2.startR)
        {
            return m2;
        }
    }
    return Move();
}

// Function to run GUI mode (contains your old game loop, adapted)
void runGuiMode()
{
    std::cout << "Starting GUI Mode...\n";
    Board board;
    MoveGenerator moveGenerator;
    Evaluation evaluator;
    Search searcher;

    // Initialize your View
    View view;
    if (!view.init())
    { /* handle error */
        return;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        return;
    }

    std::cout << "Hello, World!\n";
    std::string start = "rnbqkbnr/pppp1ppp/8/3Pp3/8/8/PPPPP1PP/RNBQKBNR w KQkq e6";
    board.setBitBoard(start);

    RenderData renderData;
    renderData.populateRenderData(board);

    bool quit = false;
    int i = 0;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else
            {
                std::pair<int, int> movePair = view.handleEvents(e);

                if (movePair.first >= 0)
                {
                    Move m;
                    char letter = getPieceletter(movePair.first, board);
                    if (letter != '\0')
                    {
                        m = {letter, '\0', 0, movePair.first / 8, movePair.first % 8, movePair.second / 8, movePair.second % 8};

                        Move validatedMove = validatePlayerMove(board.player, m, moveGenerator, board);
                        std::cout << "Validated Move: " << validatedMove.pieceName << " from " << validatedMove.startR << "," << validatedMove.startC << " to " << validatedMove.endR << "," << validatedMove.endC << " " << validatedMove.enPassantMove << std::endl;

                        if (validatedMove.pieceName != '\0')
                        {
                            board.makeMove(validatedMove);
                            renderData.populateRenderData(board);

                            view.render(renderData);
                        }
                    }
                }
            }
        }

        // if (board.player == 2)
        // {
        //     Move m = searcher.search(board);
        //     std::cout << "AI Move: " << m.pieceName << " from " << m.startR << "," << m.startC << " to " << m.endR << "," << m.endC << std::endl;

        //     board.makeMove(m);
        //     renderData.populateRenderData(board);
        //     view.render(renderData);
        // }

        view.render(renderData);
        SDL_Delay(100);
    }
}

int main(int argc, const char *argv[])
{
    if (argc > 1 && std::string(argv[1]) == "--gui")
    {
        runGuiMode();
    }
    else
    {
        UCIInterface uci;
        uci.run();
    }
    return 0;
}
