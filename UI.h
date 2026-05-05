#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "gameglobale.h"

void drawHealthBars(sf::RenderWindow& window, Player players[], int p1Wins, int p2Wins);
void drawWinScreen(sf::RenderWindow& window, int winnerIndex, int p1Wins, int p2Wins);
void drawPauseScreen(sf::RenderWindow& window, int pauseSelection);
void drawCountdown(sf::RenderWindow& window, int count);
void drawMainMenu(sf::RenderWindow& window, int selectedItem);
void drawRoundNumber(sf::RenderWindow& window, int round);