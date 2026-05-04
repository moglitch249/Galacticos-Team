#include "gameglobale.h"
#include "Level.h"

static void addPlatform(Level& level, float x, float y, float width, float height, sf::Color color) {    
    int i = level.platformCount;
    level.platforms[i].position = {x, y};
    level.platforms[i].size     = {width, height};
    level.platforms[i].color    = color;
    level.platformCount++;
}

void loadLevel(Level& level, int levelId) {
    level.platformCount = 0;
    level.size  = {1920.f,1080.f};

    sf::Color groundColor = sf::Color(80,  80,  90);
    sf::Color mid    = sf::Color(100, 110, 130);

    if (levelId == 0) {
        addPlatform(level, 470,  480, 600, 35,  mid);    // center
        addPlatform(level, 150,  580, 200, 35,  mid);    // left mid
        addPlatform(level, 1200,  580, 200, 35,  mid);    // right mid


        // Set Spawn points for each player

        for(int i = 0; i < MAX_PLAYERS; i++){
            level.spawnPoints[i] = {200.f + i * 1000.f, 300.f};
        }

    }


    if (levelId == 1) {
        addPlatform(level, 0,    690, 1920, 40, groundColor); // floor
        addPlatform(level, 0,  0, 50, 790,  mid);    // left wall
        addPlatform(level, 1550,  0, 50, 790,  mid);    // right wall


        // Set Spawn points for each player

        for(int i = 0; i < MAX_PLAYERS; i++){
            level.spawnPoints[i] = {200.f + i * 1000.f, 300.f};
        }

    }
}

void resetLevel(Level& level, int levelId) {
    loadLevel(level, levelId);
}

Vec2 getSpawnPoint(const Level& level, int playerId) {
    if (playerId >= 0 && playerId < 4)
        return level.spawnPoints[playerId];
    return {640.f, 300.f};
}