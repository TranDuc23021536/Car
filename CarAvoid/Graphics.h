#ifndef _GRAPHICS__H
#define _GRAPHICS__H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <bits/stdc++.h>
#include "defs.h"
#include <vector>
using namespace std;
int highscore = 0;

struct ScrollingBackground {
    SDL_Texture* texture;
    int scrollingOffset = 0;
    int width, height;

    void setTexture(SDL_Texture* _texture) {
        texture = _texture;
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    }

    void scroll(int distance) {
        scrollingOffset -= distance;
        if(scrollingOffset < 0) {
            scrollingOffset = width;
        }
    }
};

struct Graphics {
    SDL_Renderer *renderer;
	SDL_Window *window;
	SDL_Texture* car, *enemy, *menu, *sound_on, *sound_off, *pause_;
	SDL_Texture* Score, *highScore, *cup, *thua, *replay, *returnHome, *explosion;
	TTF_Font* font;

	void logErrorAndExit(const char* msg, const char* error)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
    }

	void initSDL() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
            logErrorAndExit("SDL_Init", SDL_GetError());

        window = SDL_CreateWindow("Lamborghini Racing Challenge", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
            logErrorAndExit( "SDL_image error:", IMG_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
        logErrorAndExit( "SDL_mixer could not initialize! SDL_mixer Error: %s\n",
                Mix_GetError() );
        }

        if (TTF_Init() == -1) {
            logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ",
                             TTF_GetError());
        }
    }

	void prepareScene(SDL_Texture * background)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy( renderer, background, NULL, NULL);
    }

    void prepareScene()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    void presentScene()
    {
        SDL_RenderPresent(renderer);
    }

    SDL_Texture *loadTexture(const char *filename)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL)
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

        return texture;
    }

    void renderTexture(SDL_Texture *texture, int x, int y)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }

    void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        dest.w = src->w;
        dest.h = src->h;

        SDL_RenderCopy(renderer, texture, src, &dest);
    }

    void renderBgr(const ScrollingBackground &bgr) {
        renderTexture(bgr.texture, bgr.scrollingOffset, 0);
        renderTexture(bgr.texture, bgr.scrollingOffset - bgr.width, 0);
    }

    Mix_Music *loadMusic(const char* path)
    {
        Mix_Music *gMusic = Mix_LoadMUS(path);
        if (gMusic == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                "Could not load music! SDL_mixer Error: %s", Mix_GetError());
        }
        return gMusic;
    }

    void play(Mix_Music *gMusic)
    {
        if (gMusic == nullptr) return;

        if (Mix_PlayingMusic() == 0) {
            Mix_PlayMusic( gMusic, -1 );
        }
        else if( Mix_PausedMusic() == 1 ) {
            Mix_ResumeMusic();
        }
    }

    Mix_Chunk* loadSound(const char* path) {
        Mix_Chunk* gChunk = Mix_LoadWAV(path);
        if (gChunk == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
               "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
        }
        return gChunk;
    }

    void play(Mix_Chunk* gChunk) {
        if (gChunk != nullptr) {
            Mix_PlayChannel( -1, gChunk, 0 );
        }
    }

    TTF_Font* loadFont(const char* path, int size)
    {
        TTF_Font* gFont = TTF_OpenFont( path, size );
        if (gFont == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Load font %s", TTF_GetError());
        }
        return gFont;
    }

     SDL_Texture* renderText(const char* text,
                            TTF_Font* font, SDL_Color textColor)
    {
        SDL_Surface* textSurface =
                TTF_RenderText_Solid( font, text, textColor );
        if( textSurface == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Render text surface %s", TTF_GetError());
            return nullptr;
        }

        SDL_Texture* texture =
                SDL_CreateTextureFromSurface( renderer, textSurface );
        if( texture == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Create texture from text %s", SDL_GetError());
        }
        SDL_FreeSurface( textSurface );
        return texture;
    }

    void drawObject()
    {
        srand(static_cast<unsigned int>(time(0)));

        renderTexture(car, car_x, car_y);
        SDL_Rect carRect = { car_x - 20, car_y - 20, car_width - 20, car_height - 20 };
        for(int i = 0; i < 5; i++)
        {
            renderTexture(enemy, e_x[i], e_y[i]);
            SDL_Rect enemy1Rect = { e_x[i] - 20, e_y[i] - 20, e_width - 20, e_height - 20 };
            e_x[i] -= enemys_speed;

            if(SDL_HasIntersection(&carRect, &enemy1Rect)) {
                h = 0;
                renderTexture(explosion, car_x- 10, car_y - 80);
            }

            if(car_y < 150 || car_y > 620)
            {
                h = 0;
                renderTexture(explosion, car_x - 10, car_y - 80);
            }

            if (e_x[i] + e_width <= 0)
            {
                bool temp = false;
                int new_x, new_y;
                map<int, int> cnt = dem(e_y, 5);

                while (!temp) {
                    new_x = SCREEN_WIDTH;
                    new_y = yPositions[rand() % 5];
                    if (cnt[new_y] < 2)
                    {
                        temp = true;
                        cnt[e_y[i]]--;
                        cnt[new_y]++;
                    }
            }

            e_x[i] = new_x;
            e_y[i] = new_y;
            score++;
        }
    }
        if(score == temp)
        {
            temp += score;
            enemys_speed +=2;
            road_speed +=2;
        }

        if(gameover)
        {
            road_speed = 0;
            enemys_speed = 0;
            car_speed = 0;
            renderTexture (thua, 560, 100);
            renderTexture (replay, 700, 600);
            renderTexture(returnHome, 850, 590);
        }
        else
        {
            car_speed = 10;
        }

        if(pause )
        {
            renderTexture(pause_, 800, 370);
        }

        presentScene();
    }

    void drawScore()
    {
        string diem = "Score: " + to_string(score);
        const char* scoreText = diem.c_str();
        font = loadFont("assets/Purisa-BoldOblique.ttf", 50);
        SDL_Color color = {255, 255, 255, 255};
        Score = renderText(scoreText, font, color);

        renderTexture(Score, 1350, 10);
        presentScene();
    }

    void drawMenu()
    {
        prepareScene(menu);
//        if(sound) renderTexture(sound_on, 1580, 1);
//        else renderTexture(sound_off, 1580, 1);
        presentScene();
    }

    void drawPause()
    {
        renderTexture(pause_, 600, 100);
        presentScene();
    }

    void drawHighScore()
    {
        prepareScene(cup);
        string diemCao = "High Score: " + to_string(highscore);
        const char* highScoreText = diemCao.c_str();
        font = loadFont("assets/Purisa-BoldOblique.ttf", 50);
        SDL_Color color = {255, 255, 255, 255};
        highScore = renderText(highScoreText, font, color);

        renderTexture(highScore, SCREEN_WIDTH/2 - 210, SCREEN_HEIGHT/2);
        presentScene();
    }

     void initObject()
    {
        initSDL();
        car = loadTexture("img\\car.png");
        enemy = loadTexture("img\\enemy.png");
        menu = loadTexture("img\\menu.png");
        sound_on = loadTexture("img\\sound_on.png");
        sound_off = loadTexture("img\\sound_off.png");
        pause_ = loadTexture("img\\play.png");
        cup = loadTexture("img\\cup.jpg");
        highscore = readHighScoreFromFile();
        thua = loadTexture("img\\gameover.png");
        replay = loadTexture("img\\ret.png");
        returnHome = loadTexture("img\\levelsel.png");
        explosion = loadTexture("img\\explosion.png");
    }


    void quit()
    {
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(Score);
        SDL_DestroyTexture(menu);
        SDL_DestroyTexture(car);
        SDL_DestroyTexture(enemy);
        TTF_CloseFont(font);
        SDL_Quit();
        Mix_Quit();
        TTF_Quit();

    }
};

#endif // _GRAPHICS__H
