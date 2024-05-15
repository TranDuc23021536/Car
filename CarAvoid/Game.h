//#ifndef GAME_H_INCLUDED
//#define GAME_H_INCLUDED
//#include "Graphics.h"
//struct Mouse {
//    int x, y;
//    int dx = 0;
//    int dy = 0;
//    int speed = 2;
//    void move() {
//        x += dx;
//        y += dy;
//    }
//    void turnNorth() {
//        dy = -speed;
//        dx = 0;
//    }
//    void turnSouth() {
//        dy = speed;
//        dx = 0;
//    }
//
//    void turnWest() {
//        dy = 0;
//        dx = -speed;
//    }
//
//    void turnEast() {
//        dy = 0;
//        dx = speed;
//    }
//};
//
//void render(const Mouse& mouse, const Graphics &graphics) {
//    SDL_Rect filled_rect;
//    filled_rect.x = mouse.x;
//    filled_rect.y = mouse.y;
//    filled_rect.w = 10;
//    filled_rect.h = 10;
//    SDL_SetRenderDrawColor(graphics.renderer, 200,200,200,200);
//    SDL_RenderFillRect(graphics.renderer, &filled_rect);
//}
//
//bool gameOver(const Mouse &mouse) {
//    return mouse.x < 0 || mouse.x >= SCREEN_WIDTH || mouse.y < 0 || mouse.y >= SCREEN_HEIGHT;
//}
//
//#endif // GAME_H_INCLUDED
//
#ifndef GAME_H
#define GAME_H

#define INITIAL_SPEED 4
#define CHEESE_SIZE 10
#define MOUSE_SIZE 30

bool inside(int x, int y, SDL_Rect r) {
    return x > r.x && x < r.x+r.w && y > r.y && y < r.y+r.h;
}

bool overlap(const SDL_Rect& r1, const SDL_Rect& r2) {
    return inside(r1.x, r1.y, r2) || inside(r1.x + r1.w, r1.y, r2) ||
            inside(r1.x, r1.y+r1.h, r2) || inside(r1.x+r1.w, r1.y+r1.h, r2);
}



struct Mouse {
    SDL_Rect rect;
    int dy = 0;
    int speed = INITIAL_SPEED;

    Mouse(int x, int y) {
        rect.x = x;
        rect.y = y;
        rect.h = MOUSE_SIZE;
        rect.w = MOUSE_SIZE;
    }
    void move() {
        rect.y += dy;
    }
    void turnNorth() {
        dy = -speed;
    }
    void turnSouth() {
        dy = speed;

    }
};

bool gameOver(const Mouse& mouse) {
    return mouse.rect.x < 0 || mouse.rect.x >= SCREEN_WIDTH ||
           mouse.rect.y < 0 || mouse.rect.y >= SCREEN_HEIGHT;
}

#endif // GAME_H
