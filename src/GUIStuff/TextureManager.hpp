//
//  TextureManager.hpp
//  GUIChess
//
//  Created by Darragh Flynn on 11/09/2024.
//

#ifndef TextureManager_hpp
#define TextureManager_hpp

#include <SDL.h>
#include <SDL_image.h>
#include <unordered_map>
#include <string>
#include <iostream>

class TextureManager
{
public:
    SDL_Renderer *renderer;
    std::unordered_map<char, SDL_Texture *> textures;

    TextureManager(SDL_Renderer *renderer);
    ~TextureManager();

    bool loadTexture(char id, const std::string &path);
    SDL_Texture *getTexture(char id) const;
};

#endif /* TextureManager_hpp */
