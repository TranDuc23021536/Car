#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

    const int SCREEN_WIDTH = 1685;
    const int SCREEN_HEIGHT = 838;

    SDL_Event event;

    int road_speed = 10;

    int car_x = 50;
    int car_y = 200;
    int car_speed = 13;

    const int car_width = 190;
    const int car_height = 92;

    int e_x[5] = {900, 250, 600, 1500, 1200};
    int e_y[5] = {555, 295, 160, 425, 290};
    const int e_width = 40;
    const int e_height = 121;

    int enemys_speed = 10;
    int score = 0;
    bool quit = false;
    int temp = 4;
    int h =1;

    bool gameover = false;
    bool playGame = false;
    bool home = true;
    bool pause = false;
    bool hs = false;

    bool sound = true;

    extern int highscore;

    void writeHighScoreToFile(int highscore) {
        std::ofstream file("highscore.txt");
        if (file.is_open()) {
            file << highscore;
            file.close();
        }
    }

    int readHighScoreFromFile() {
        int highscore = 0;
        std::ifstream file("highscore.txt");
        if (file.is_open()) {
            file >> highscore;
            file.close();
        }
        return highscore;
    }

    void setThongSo(){
    score = 0;
    temp = 2;
    h = 1;
    car_x = 50;
    car_y = 200;
    e_x[0] = 950;
    e_x[1] = 150;
    e_x[2] = 700;
    e_x[3] = 1500;
    e_x[4] = 1200;
    e_y[0] = 555;
    e_y[1] = 295;
    e_y[2] = 160;
    e_y[3] = 425;
    e_y[4] = 290;
    enemys_speed = 10;
    road_speed = 10;
    highscore = readHighScoreFromFile();
    }

    const int yPositions[5] = {555, 295, 160, 425, 290};

    std::map<int, int> dem(const int e_y[], int size)
    {
        std::map<int, int> cnt;
        for (int i = 0; i < size; i++)
        {
            cnt[e_y[i]]++;
        }
        return cnt;
    }
#endif // DEFS_H_INCLUDED
