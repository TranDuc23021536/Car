#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "defs.h"
#include "Game.h"
#include <SDL_ttf.h>

int main(int argc, char *argv[])
{
    Graphics graphics;
    graphics.initObject();

    Mix_Music *gMusic = graphics.loadMusic("assets\\Tokyo_drift.mp3");
    Mix_Music *gGameover = graphics.loadMusic("assets\\gameover_sound.mp3");
    graphics.play(gMusic);
    Mix_Chunk *gDrift = graphics.loadSound("assets\\Car_drift.mp3");
    ScrollingBackground background;
    background.setTexture(graphics.loadTexture("img\\Road.png"));

    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    int x, y;
    int a, b, c;


    while (!quit) {
        graphics.prepareScene();
        cerr << x << ", " << y << endl;
        if(h == 0){
            if(score > highscore)
            {
                highscore = score;
                writeHighScoreToFile(highscore);
            }
            gameover = true;
        }

        while (SDL_PollEvent(&event)) {
            SDL_GetMouseState(&x, &y);
            if(event.type == SDL_QUIT)
            {
                quit = true;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN && x >= 1337 && x <= 1578 && y >= 621 && y <= 666 && hs != true)
            {
                quit = true;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN && x >= 1337 && x <= 1578 && y >= 543 && y <= 595)
            {
                hs = true;
                home = false;
                playGame = false;
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN && playGame != true)
            {
                    home = true;
                    hs = false;
            }

            if (gameover && event.type == SDL_MOUSEBUTTONDOWN)
                {
                if (x >= 700 && x <= 792 && y >= 603 && y <= 685)
                {
                // Khởi động lại trò chơi
                    playGame = true;
                    home = false;
                    gameover = false;
                    graphics.play(gDrift);
                    graphics.play(gMusic);
                    setThongSo();
                }
                else if (x >= 850 && x <= 940 && y >= 590 && y <= 670)
                    {
                    home = true;
                    playGame = false;
                    gameover = false;
                }
            }
            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
               if((x >= 1337 && x <= 1578 && y >= 466 && y <= 517 && (home || gameover)))
                {
                    playGame = true;
                    home = false;
                    gameover = false;
                    graphics.play(gDrift);
                    graphics.play(gMusic);
                    setThongSo();
                }

            }
            if(event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == SDLK_SPACE && playGame && !pause){
                    a = road_speed;
                    b = enemys_speed;
                    c = car_speed;
                    road_speed = 0;
                    enemys_speed = 0;
                    car_speed = 0;
                    pause = true;
                }
                else if(event.key.keysym.sym == SDLK_SPACE && playGame && pause)
                {
                    pause = false;
                    road_speed = a;
                    enemys_speed = b;
                    car_speed = c;
                }

                if(event.key.keysym.sym == SDLK_RETURN)
                {
                    playGame = true;
                    home = false;
                    gameover = false;
                    graphics.play(gDrift);
                    graphics.play(gMusic);
                    setThongSo();
                }

            }
        }

        if(playGame)
        {
            background.scroll(road_speed);
            graphics.renderBgr(background);
            if (currentKeyStates[SDL_SCANCODE_UP]) graphics.play(gDrift);
            if (currentKeyStates[SDL_SCANCODE_UP])
                car_y -= car_speed;
            if (currentKeyStates[SDL_SCANCODE_DOWN]) graphics.play(gDrift);
            if (currentKeyStates[SDL_SCANCODE_DOWN])
                car_y += car_speed;

            graphics.drawObject();
            graphics.drawScore();
        }

        if(home)
        {
            graphics.drawMenu();
        }

        if(hs)
        {
            graphics.drawHighScore();
        }

        if(gameover)
        {
            Mix_PauseMusic();
        }

        SDL_Delay(10);
    }

    if (gDrift != nullptr) Mix_FreeChunk( gDrift);
    if (gMusic != nullptr) Mix_FreeMusic( gMusic );
    SDL_DestroyTexture(background.texture);
    graphics.quit();
    return 0;
}
