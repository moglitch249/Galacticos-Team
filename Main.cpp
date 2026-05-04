#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include "gameglobale.h"
#include "Level.h"
#include "Player.h"
#include "Physics.h"
#include "Render.h"
#include "UI.h"

int score = 0;
std::vector<Level> levels;
enum GameState { MENU, ROUND_START, COUNTDOWN, PLAYING, GAME_OVER, PAUSED };

int main() {
    sf::RenderWindow window(sf::VideoMode({(unsigned int)WINDOW_WIDTH, (unsigned int)WINDOW_HEIGHT}), "STICK FIGHT");
    window.setFramerateLimit(60);

    GameState currentState = MENU;
    float countdownTimer = 3.9f, roundTimer = 2.0f;
    int currentRound = 1, winnerIndex = -1, menuSelection = 0, pauseSelection = 0, p1Wins = 0, p2Wins = 0;

    Level currentLevel;
    loadLevel(currentLevel, 0);
    Player players[MAX_PLAYERS];

    for (int i = 0; i < MAX_PLAYERS; i++) {
        playerInit(players[i], i, currentLevel);
    }

     // playerInit(players[0], 0, currentLevel); playerInit(players[1], 1, currentLevel);
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            if (auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (currentState == MENU) {
                    if (keyPressed->code == sf::Keyboard::Key::Up) menuSelection = 0;
                    if (keyPressed->code == sf::Keyboard::Key::Down) menuSelection = 1;
                    if (keyPressed->code == sf::Keyboard::Key::Enter) {
                        if (menuSelection == 0) {
                            currentRound = 1; p1Wins = 0; p2Wins = 0;
                            playerInit(players[0], 0, currentLevel); playerInit(players[1], 1, currentLevel);
                            players[0].pos.x = 300.f; players[1].pos.x = 1300.f;
                            currentState = ROUND_START;
                        } else window.close();
                    }
                } else if (currentState == PAUSED) {
                    if (keyPressed->code == sf::Keyboard::Key::Up) pauseSelection = 0;
                    if (keyPressed->code == sf::Keyboard::Key::Down) pauseSelection = 1;
                    if (keyPressed->code == sf::Keyboard::Key::Enter) {
                        if (pauseSelection == 0) currentState = PLAYING;
                        else currentState = MENU;
                    }
                } else if (keyPressed->code == sf::Keyboard::Key::Escape && (currentState == PLAYING || currentState == ROUND_START)) {
                    currentState = PAUSED; pauseSelection = 0;
                } else if (keyPressed->code == sf::Keyboard::Key::R && currentState == GAME_OVER) {
                    currentState = MENU;
                }
            }
        }

        int levelId = (currentRound == 2) ? 1 : 0;
        if (currentState == ROUND_START) {
            roundTimer -= dt; if (roundTimer <= 0.f) { roundTimer = 2.0f; countdownTimer = 3.9f; currentState = COUNTDOWN; }
        } else if (currentState == COUNTDOWN) {
            countdownTimer -= dt; if (countdownTimer <= 0.f) currentState = PLAYING;
        } else if (currentState == PLAYING) {
            playerReadInputForIndex(players[0], 0); playerReadInputForIndex(players[1], 1);
            playerUpdate(players[0], dt); playerUpdate(players[1], dt);
            physicsUpdate(players[0], dt, levelId); physicsUpdate(players[1], dt,levelId);
            for (int i = 0; i < currentLevel.platformCount; i++)
            {
                resolvePlatformCollision(players[0], currentLevel.platforms[i]);
            }
            for (int i = 0; i < currentLevel.platformCount; i++)
            {
                resolvePlatformCollision(players[1], currentLevel.platforms[i]);
            }
            
            resolvePlayerCollision(players[0], players[1]);
            handleCombat(players[0], players[1]); handleCombat(players[1], players[0]);

            if (!players[0].isAlive || !players[1].isAlive) {
                winnerIndex = !players[0].isAlive ? 1 : 0;
                if (winnerIndex == 0) p1Wins++; else p2Wins++;
                
                if (p1Wins == 2 || p2Wins == 2) {
                    currentState = GAME_OVER;
                } else {
                    currentRound++;
                    playerInit(players[0], 0, currentLevel); 
                    playerInit(players[1], 1, currentLevel);
                    players[0].pos.x = 300.f; 
                    players[1].pos.x = 1300.f;
                    roundTimer = 2.0f; 
                    currentState = ROUND_START;
                }
            }
        }

        window.clear(sf::Color::Black);
        if (currentState == MENU) drawMainMenu(window, menuSelection);
        else {
            int levelId = (currentRound == 2) ? 1 : 0;
            drawBackground(window,levelId);drawLevel(window, currentLevel, levelId);
            drawPlayer(window, players[0], 0, dt); drawPlayer(window, players[1], 1, dt);
            window.setView(window.getDefaultView());
            drawHealthBars(window, players, p1Wins, p2Wins);
            if (currentState == ROUND_START) drawRoundNumber(window, currentRound);
            else if (currentState == COUNTDOWN) drawCountdown(window, (int)countdownTimer);
            else if (currentState == PAUSED) drawPauseScreen(window, pauseSelection);
            else if (currentState == GAME_OVER) drawWinScreen(window, winnerIndex, p1Wins, p2Wins);
        }
        window.display();
    }
    return 0;
}