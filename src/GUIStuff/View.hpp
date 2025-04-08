//
//  View.hpp
//  GUIChess
//
//  Created by Darragh Flynn on 11/09/2024.
//

#ifndef View_hpp
#define View_hpp

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
// #include "Model.hpp"
// #include "Controller.hpp"
#include "TextureManager.hpp"
#include "RenderData.hpp"

class View
{
public:
    View();
    ~View();
    bool init();
    std::pair<int, int> handleEvents(SDL_Event &e);
    void render(RenderData data);
    void renderBoard();
    void renderPieces(RenderData data);
    void renderSelect(int x, int y);
    void cleanup();

private:
    TextureManager *textureMan;
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool quit;
    const int WINDOW_WIDTH = 640;
    const int WINDOW_HEIGHT = 640;
    const int BOARD_SIZE = 8;
    const int TILE_SIZE = WINDOW_WIDTH / BOARD_SIZE;
    bool isCLick = false;
    int clickedX;
    int clickedY;
    int endX;
    int endY;
};
#endif /* View_hpp */
