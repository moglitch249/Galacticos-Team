#include "UI.h"
#include "gameglobale.h"
#include <string>
#include <SFML/Audio.hpp>

const sf::Color PLAYER_COLORS[2] = { sf::Color(80, 160, 255), sf::Color(255, 90, 90) };
const float BAR_WIDTH  = 220.f;
const float BAR_HEIGHT = 18.f;
const float BAR_Y      = 30.f;

static sf::Font g_font;
static bool     g_fontLoaded = false;
static sf::Music g_backgroundMusic;
static bool g_musicInitialized = false;

static void loadAssets() {
    if (!g_fontLoaded) {
        g_fontLoaded = g_font.openFromFile("assets/ArialNarrow7-9YJ9n.ttf") || g_font.openFromFile("assets/arial.ttf");
    }

    if (!g_musicInitialized) {
        if (g_backgroundMusic.openFromFile("assets/music.wav")) {
            g_backgroundMusic.setLooping(true);
            g_backgroundMusic.setVolume(40.f);
            g_backgroundMusic.play();
        }
        g_musicInitialized = true;
    }
}

static void drawWinDots(sf::RenderWindow& window, float xPos, int wins, int playerIndex) {
    for (int w = 0; w < wins; w++) {
        sf::CircleShape dot(8.f);
        dot.setFillColor(sf::Color::Yellow);
        dot.setOutlineColor(sf::Color(200, 180, 0));
        dot.setOutlineThickness(1.5f);
        float dotX = (playerIndex == 0) ? (xPos + BAR_WIDTH + 15.f + w * 22.f) : (xPos - 15.f - (w + 1) * 22.f);
        dot.setPosition({dotX, BAR_Y - 1.f});
        window.draw(dot);
    }
}

void drawHealthBars(sf::RenderWindow& window, Player players[], int p1Wins, int p2Wins) {
    loadAssets();

    for (int i = 0; i < MAX_PLAYERS; i++) {
        float xPos = (i == 0) ? 50.f : (1600.f - BAR_WIDTH - 50.f);
        int hp = players[i].isAlive ? players[i].health : 0;
        sf::RectangleShape bgBar({BAR_WIDTH, BAR_HEIGHT});
        bgBar.setPosition({xPos, BAR_Y});
        bgBar.setFillColor(sf::Color(30, 30, 30));
        bgBar.setOutlineThickness(2.5f);
        bgBar.setOutlineColor(sf::Color(70, 70, 70));
        window.draw(bgBar);

        float ratio = (float)hp / players[i].maxHealth;
        if (ratio > 0.f) {
            sf::RectangleShape fgBar({BAR_WIDTH * (ratio > 1.f ? 1.f : ratio), BAR_HEIGHT});
            fgBar.setPosition({xPos, BAR_Y});
            fgBar.setFillColor(ratio > 0.6f ? sf::Color(34, 139, 34) : (ratio > 0.3f ? sf::Color(184, 134, 11) : sf::Color(139, 0, 0)));
            window.draw(fgBar);
        }

        if (g_fontLoaded) {
            sf::Text nameText(g_font, i == 0 ? "PLAYER 1" : "PLAYER 2", 15);
            nameText.setFillColor(PLAYER_COLORS[i]);
            nameText.setStyle(sf::Text::Bold);
            nameText.setPosition({xPos, BAR_Y - 25.f});
            window.draw(nameText);
        }
        drawWinDots(window, xPos, (i == 0 ? p1Wins : p2Wins), i);
    }
}

void drawWinScreen(sf::RenderWindow& window, int winnerIndex, int p1Wins, int p2Wins) {
    sf::RectangleShape overlay({1600.f, 900.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 220));
    window.draw(overlay);
    loadAssets();
    sf::Text header(g_font, "MATCH OVER", 50);
    header.setFillColor(sf::Color::Yellow);
    header.setOrigin({header.getLocalBounds().size.x / 2.f, 0});
    header.setPosition({800.f, 250.f});
    window.draw(header);

    std::string msg = (winnerIndex == 0) ? "PLAYER 1 WINS!" : "PLAYER 2 WINS!";
    sf::Text winText(g_font, msg, 80);
    winText.setFillColor(sf::Color::White);
    winText.setStyle(sf::Text::Bold);
    winText.setOrigin({winText.getLocalBounds().size.x / 2.f, 0});
    winText.setPosition({800.f, 330.f});
    window.draw(winText);

    sf::Text subText(g_font, "PRESS R FOR MAIN MENU", 28);
    subText.setFillColor(sf::Color(200, 200, 200));
    subText.setOrigin({subText.getLocalBounds().size.x / 2.f, 0});
    subText.setPosition({800.f, 570.f});
    window.draw(subText);
}

void drawMainMenu(sf::RenderWindow& window, int selectedItem) {
    loadAssets();
    static sf::Texture menuBackgroundTexture;
    static bool menuBackgroundLoaded = false;

    if (!menuBackgroundLoaded) {
        menuBackgroundLoaded = menuBackgroundTexture.loadFromFile("assets/ui2.png");
    }

    if (menuBackgroundLoaded) {
        sf::Sprite menuBackground(menuBackgroundTexture);
        auto size = menuBackgroundTexture.getSize();
        if (size.x > 0 && size.y > 0) {
            menuBackground.setScale({1600.f / size.x, 900.f / size.y});
        }
        window.draw(menuBackground);
    } else {
        sf::RectangleShape bg({1600.f, 900.f});
        bg.setFillColor(sf::Color::Black);
        window.draw(bg);
    }

    sf::Text t1(g_font, "PLAY", 50); t1.setFillColor(selectedItem == 0 ? sf::Color::Yellow : sf::Color::White);
    t1.setOrigin({t1.getLocalBounds().size.x/2.f, 0}); t1.setPosition({800.f, 650.f}); window.draw(t1);
    sf::Text t2(g_font, "QUIT", 50); t2.setFillColor(selectedItem == 1 ? sf::Color::Yellow : sf::Color::White);
    t2.setOrigin({t2.getLocalBounds().size.x/2.f, 0}); t2.setPosition({800.f, 750.f}); window.draw(t2);

    static sf::Clock blinkClock;
    if ((int)(blinkClock.getElapsedTime().asSeconds() * 2) % 2 == 0) {
        sf::Text hint(g_font, "USE ARROW KEYS TO NAVIGATE", 20);
        hint.setFillColor(sf::Color(150, 150, 150));
        hint.setOrigin({ hint.getLocalBounds().size.x / 2.f, 0 });
        hint.setPosition({ 800.f, 850.f });
        window.draw(hint);
    }
}

void drawPauseScreen(sf::RenderWindow& window, int pauseSelection) {
    sf::RectangleShape overlay({1600.f, 900.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    loadAssets();
    sf::Text t(g_font, "PAUSED", 80); t.setOrigin({t.getLocalBounds().size.x/2.f, 0}); t.setPosition({800.f, 250.f}); window.draw(t);
    sf::Text r(g_font, "RESUME", 40); r.setFillColor(pauseSelection == 0 ? sf::Color::Yellow : sf::Color::White);
    r.setOrigin({r.getLocalBounds().size.x/2.f, 0}); r.setPosition({800.f, 450.f}); window.draw(r);
    sf::Text m(g_font, "QUIT TO MENU", 40); m.setFillColor(pauseSelection == 1 ? sf::Color::Yellow : sf::Color::White);
    m.setOrigin({m.getLocalBounds().size.x/2.f, 0}); m.setPosition({800.f, 550.f}); window.draw(m);
}

void drawCountdown(sf::RenderWindow& window, int count) {
    loadAssets();
    static bool countsoundLoaded = false;
    static sf::SoundBuffer countdownBuffer;
    static sf::Sound countdownSound(countdownBuffer);

    if (!countsoundLoaded) {
        if (countdownBuffer.loadFromFile("assets/countdown.wav")) {
            countdownSound.setBuffer(countdownBuffer);
            countsoundLoaded = true;
        }
    }

    static bool played = false;
    if (count == 3 && !played) { 
        countdownSound.stop();   
        countdownSound.play();
        played = true;
    } 

    if (count != 3) {
        played = false;
    }

    std::string txt = (count == 3) ? "3" : (count == 2) ? "2" : (count == 1) ? "1" : "FIGHT!";
    sf::Text t(g_font, txt, (count == 0) ? 90 : 120);
    t.setFillColor((count == 0) ? sf::Color::Red : sf::Color::White);
    t.setOrigin({t.getLocalBounds().size.x/2.f, t.getLocalBounds().size.y/2.f});
    t.setPosition({800.f, 400.f});
    window.draw(t);
}

void drawRoundNumber(sf::RenderWindow& window, int round) {
    loadAssets();
    sf::Text t(g_font, "ROUND " + std::to_string(round), 100);
    t.setOrigin({t.getLocalBounds().size.x/2.f, t.getLocalBounds().size.y/2.f});
    t.setPosition({800.f, 400.f});
    window.draw(t);
}