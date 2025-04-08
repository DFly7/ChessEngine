//
//  View.cpp
//  GUIChess
//
//  Created by Darragh Flynn on 11/09/2024.
//

#include "View.hpp"
#include <utility>

View::View()
{

    window = nullptr;
    renderer = nullptr;
    quit = false;
}

View::~View()
{
    cleanup();
}

bool View::init()
{

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window
    window = SDL_CreateWindow("ChessBoard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return false;
    }

    textureMan = new TextureManager(renderer);

    bool a = textureMan->loadTexture('p', "Resources/Images/Chess_pdt60.png");
    bool b = textureMan->loadTexture('b', "Resources/Images/Chess_bdt60.png");
    bool c = textureMan->loadTexture('k', "Resources/Images/Chess_kdt60.png");
    bool d = textureMan->loadTexture('n', "Resources/Images/Chess_ndt60.png");
    bool e = textureMan->loadTexture('q', "Resources/Images/Chess_qdt60.png");
    bool f = textureMan->loadTexture('r', "Resources/Images/Chess_rdt60.png");

    bool a1 = textureMan->loadTexture('P', "Resources/Images/Chess_plt60.png");
    bool b1 = textureMan->loadTexture('B', "Resources/Images/Chess_blt60.png");
    bool c1 = textureMan->loadTexture('K', "Resources/Images/Chess_klt60.png");
    bool d1 = textureMan->loadTexture('N', "Resources/Images/Chess_nlt60.png");
    bool e1 = textureMan->loadTexture('Q', "Resources/Images/Chess_qlt60.png");
    bool f1 = textureMan->loadTexture('R', "Resources/Images/Chess_rlt60.png");

    //    /Users/darraghflynn/Documents/Projects/CPP/GUIChess/GUIChess/newChess/Images/BBishop.png

    return a && b && c && d && e && f && a1 && b1 && c1 && d1 && e1 && f1;
}

void View::render(RenderData data)
{
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Render board and pieces
    renderBoard();
    renderPieces(data);

    if (isCLick)
    {
        renderSelect(clickedX, clickedY);
    }

    // Update screen
    SDL_RenderPresent(renderer);
}

void View::renderPieces(RenderData data)
{

    for (int i = 0; i < data.pieceTypes.size(); ++i)
    {
        SDL_Rect destRect = {(data.pieceYPositions[i] * TILE_SIZE), ((7 - data.pieceXPositions[i]) * TILE_SIZE), TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, textureMan->getTexture(data.pieceTypes[i]), nullptr, &destRect);
    }
}

void View::renderBoard()
{
    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int col = 0; col < BOARD_SIZE; ++col)
        {
            SDL_Color color = ((row + col) % 2 == 0) ? SDL_Color{231, 231, 231, 255} : SDL_Color{144, 202, 238, 255};
            SDL_SetRenderDrawColor(renderer, color.g, color.r, color.b, color.a);
            SDL_Rect tileRect = {row * TILE_SIZE, col * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &tileRect);
        }
    }
}

void View::renderSelect(int x, int y)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Draw a transparent blue square
    SDL_Rect fillRect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE}; // x, y, width, height
    SDL_SetRenderDrawColor(renderer, 100, 149, 237, 128);                     // Blue color with 50% transparency
    SDL_RenderFillRect(renderer, &fillRect);
}

void View::cleanup()
{

    textureMan->~TextureManager();
    free(textureMan);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

std::pair<int, int> View::handleEvents(SDL_Event &e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        //        printf("down");
        int x, y;
        SDL_GetMouseState(&x, &y);
        //        std::cout << "Mouse clicked at: (" << (7-y/TILE_SIZE) << ", " << (x/TILE_SIZE) << ")" << std::endl;
        if (isCLick)
        {
            isCLick = !isCLick;
            endX = x / TILE_SIZE;
            endY = y / TILE_SIZE;
            //            controller->Makemove(7-clickedY,clickedX, 7-endY, endX, model->getPlayer());
            return std::make_pair((((7 - clickedY) * 8) + clickedX), (((7 - endY) * 8) + endX));
        }
        else
        {
            isCLick = !isCLick;
            clickedX = x / TILE_SIZE;
            clickedY = y / TILE_SIZE;
        }
    }
    if (e.type == SDL_MOUSEBUTTONUP)
    {
        //        printf("up");
    }

    return std::make_pair(-1, -1);
}
