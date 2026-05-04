#include "Render.h"
#include <cmath>
#include "Level.h"
#include <SFML/Audio.hpp>
#include <cstdint>

using namespace sf;

namespace {
constexpr float FRAME_WIDTH = 96.f;
constexpr float FRAME_HEIGHT = 96.f;
constexpr float SPRITE_SCALE = 3.0f;
constexpr float SPRITE_CENTER_X = FRAME_WIDTH * 0.5f;
constexpr float SPRITE_FEET_Y = FRAME_HEIGHT;
constexpr float FEET_OFFSET_Y = 45.f;
}

static void drawStickFigure(RenderWindow& window, Player& p, int playerIndex) {
    Color playerColors[2] = {
        Color(70, 130, 255),
        Color(220, 60, 60)
    };
    Color col = playerColors[playerIndex % 2];

    CircleShape head(20);
    head.setFillColor(col);
    head.setPosition(Vector2f(p.pos.x, p.pos.y));
    window.draw(head);

    RectangleShape body(Vector2f(4, 50));
    body.setFillColor(col);
    body.setPosition(Vector2f(p.pos.x + 18, p.pos.y + 40));
    window.draw(body);

    RectangleShape leg1(Vector2f(4, 35));
    leg1.setFillColor(col);
    leg1.setPosition(Vector2f(p.pos.x + 18, p.pos.y + 90));
    leg1.setRotation(degrees(20));
    window.draw(leg1);

    RectangleShape leg2(Vector2f(4, 35));
    leg2.setFillColor(col);
    leg2.setPosition(Vector2f(p.pos.x + 18, p.pos.y + 90));
    leg2.setRotation(degrees(-20));
    window.draw(leg2);
}

static void drawAnimatedSprite(RenderWindow& window, Texture& texture, int frame, Player& p) {
    Sprite sprite(texture);
    sprite.setTextureRect(IntRect({frame * static_cast<int>(FRAME_WIDTH), 0}, {static_cast<int>(FRAME_WIDTH), static_cast<int>(FRAME_HEIGHT)}));
    const float PLAYER_CENTER_X = p.pos.x + (p.width * 0.5f);
    const float PLAYER_FEET_Y = p.pos.y + p.height;

    sprite.setOrigin({SPRITE_CENTER_X, SPRITE_FEET_Y});
    sprite.setPosition({PLAYER_CENTER_X, PLAYER_FEET_Y + FEET_OFFSET_Y});
    if (p.facingRight) {
        sprite.setScale({SPRITE_SCALE, SPRITE_SCALE});
    } else {
        sprite.setScale({-SPRITE_SCALE, SPRITE_SCALE});
    }

    window.draw(sprite);
}


void drawArrow(sf::RenderWindow& window, const Player& p, int playerIndex)
{
    static const sf::Color playerColors[2] = {
        sf::Color(70, 130, 255),
        sf::Color(220, 60, 60)
    };

    static float animTime = 0.f;
    animTime += 0.016f;

    float offsetY = std::sin(animTime * 4.f) * 6.f;
    float alpha   = 180 + std::sin(animTime * 6.f) * 75.f;

    sf::ConvexShape arrow;
    arrow.setPointCount(3);

    arrow.setPoint(0, sf::Vector2f(0.f, 0.f));
    arrow.setPoint(1, sf::Vector2f(20.f, 0.f));
    arrow.setPoint(2, sf::Vector2f(10.f, 15.f));

    sf::Color col = playerColors[playerIndex % 2];
    col.a = static_cast<std::uint8_t>(alpha);

    arrow.setFillColor(col);
    arrow.setOutlineColor(sf::Color::Black);
    arrow.setOutlineThickness(2.f);

    float centerX = p.pos.x + p.width * 0.5f;
    float topY    = p.pos.y - 60.f + offsetY;

    arrow.setPosition(sf::Vector2f(centerX - 10.f, topY));

    window.draw(arrow);
}


void drawPlayer(RenderWindow& window, Player& p, int playerIndex, float dt) {
    static Texture runTexture;
    static bool triedLoadRunTexture = false;
    static bool runTextureLoaded = false;
    static Texture idleTexture;
    static bool triedLoadIdleTexture = false;
    static bool idleTextureLoaded = false;
    static Texture attackTexture;
    static bool triedLoadAttackTexture = false;
    static bool attackTextureLoaded = false;
    static float runAnimTimer = 0.f;
    static int runFrame = 0;
    static float idleAnimTimer = 0.f;
    static int idleFrame = 0;
    static bool attackVisualActive[MAX_PLAYERS] = {false};
    static int attackVisualFrame[MAX_PLAYERS] = {0};
    static float attackVisualTimer[MAX_PLAYERS] = {0.f};
    static bool attackSoundLatched[MAX_PLAYERS] = {false};
    static int drawCallsThisFrame = 0;

    if (!triedLoadRunTexture) {
        runTextureLoaded = runTexture.loadFromFile("assets/Sprites/RUN.png");
        if (!runTextureLoaded) {
            runTextureLoaded = runTexture.loadFromFile("assets/sprites/RUN.png");
        }
        triedLoadRunTexture = true;
    }

    if (!triedLoadIdleTexture) {
        idleTextureLoaded = idleTexture.loadFromFile("assets/Sprites/IDLE.png");
        if (!idleTextureLoaded) {
            idleTextureLoaded = idleTexture.loadFromFile("assets/sprites/IDLE.png");
        }
        triedLoadIdleTexture = true;
    }

    if (!triedLoadAttackTexture) {
        attackTextureLoaded = attackTexture.loadFromFile("assets/Sprites/ATTACK 1.png");
        if (!attackTextureLoaded) {
            attackTextureLoaded = attackTexture.loadFromFile("assets/sprites/ATTACK 1.png");
        }
        triedLoadAttackTexture = true;
    }

    if (drawCallsThisFrame == 0) {
        const float runFrameDuration = 0.06f;
        const float idleFrameDuration = 0.10f;
        runAnimTimer += dt;
        idleAnimTimer += dt;
        while (runAnimTimer >= runFrameDuration) {
            runAnimTimer -= runFrameDuration;
            runFrame = (runFrame + 1) % 16;
        }
        while (idleAnimTimer >= idleFrameDuration) {
            idleAnimTimer -= idleFrameDuration;
            idleFrame = (idleFrame + 1) % 10;
        }
    }

    const bool attackStarted = p.isAttacking && !attackSoundLatched[playerIndex];
    if (attackStarted) {
        attackVisualActive[playerIndex] = true;
        attackVisualFrame[playerIndex] = 0;
        attackVisualTimer[playerIndex] = 0.f;
        static SoundBuffer hitBuffer;
        static Sound hitSound(hitBuffer);
        static bool hitLoaded = hitBuffer.loadFromFile("assets/hit 1.wav");
        if (hitLoaded) {
            if (hitSound.getStatus() == SoundSource::Status::Playing) {
                hitSound.stop();
            }
            hitSound.play();
        }
    }
    attackSoundLatched[playerIndex] = p.isAttacking;

    if (attackVisualActive[playerIndex]) {
        const float attackFrameDuration = 0.05f;
        int attackFrameCount = 7;
        if (attackTextureLoaded) {
            const unsigned int textureWidth = attackTexture.getSize().x;
            if (textureWidth >= static_cast<unsigned int>(FRAME_WIDTH)) {
                attackFrameCount = static_cast<int>(textureWidth / static_cast<unsigned int>(FRAME_WIDTH));
            }
        }

        attackVisualTimer[playerIndex] += dt;
        while (attackVisualTimer[playerIndex] >= attackFrameDuration) {
            attackVisualTimer[playerIndex] -= attackFrameDuration;
            attackVisualFrame[playerIndex]++;
        }

        if (attackVisualFrame[playerIndex] >= attackFrameCount) {
            attackVisualActive[playerIndex] = false;
            attackVisualFrame[playerIndex] = 0;
            attackVisualTimer[playerIndex] = 0.f;
        }
    }

    if (!p.isAlive) {
        drawStickFigure(window, p, playerIndex);
    } else if (attackVisualActive[playerIndex]) {
        if (attackTextureLoaded) {
            drawAnimatedSprite(window, attackTexture, attackVisualFrame[playerIndex], p);
        } else {
            drawStickFigure(window, p, playerIndex);
        }
    } else if (std::abs(p.velocity.x) > 20.f) {
        if (runTextureLoaded) {
            drawAnimatedSprite(window, runTexture, runFrame, p);
        } else {
            drawStickFigure(window, p, playerIndex);
        }
    } else {
        if (idleTextureLoaded) {
            drawAnimatedSprite(window, idleTexture, idleFrame, p);
        } else {
            drawStickFigure(window, p, playerIndex);
        }
    }

    drawCallsThisFrame++;
    if (drawCallsThisFrame >= MAX_PLAYERS) {
        drawCallsThisFrame = 0;
    }

    drawArrow(window, p, playerIndex);
}


void drawBackground(RenderWindow& window, int levelId) {
    static Texture backgroundTexture1;
    static Texture backgroundTexture2;
  static bool textureLoaded1 = backgroundTexture1.loadFromFile("assets/hero.png");
  static bool textureLoaded2 = backgroundTexture2.loadFromFile("assets/level2.png");
    bool isLevel2 = (levelId==1);
    Texture* currentTexture = isLevel2 ? &backgroundTexture2 : &backgroundTexture1;
    bool textureLoaded = isLevel2 ? textureLoaded2 : textureLoaded1;
    if (textureLoaded) {
        Sprite background(*currentTexture);
        const Vector2u textureSize = currentTexture->getSize();
        if (textureSize.x > 0 && textureSize.y > 0) {
            background.setScale(Vector2f(
                static_cast<float>(WINDOW_WIDTH) / static_cast<float>(textureSize.x),
                static_cast<float>(WINDOW_HEIGHT) / static_cast<float>(textureSize.y)
            ));
        }
        window.draw(background);
        return;
    }

    RectangleShape bg(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    bg.setFillColor(Color(30, 30, 30));
    window.draw(bg);
}
void drawLevel(RenderWindow& window, Level& level) {
    for (int i = 0; i < level.platformCount; i++) {
        RectangleShape platform(Vector2f(
            level.platforms[i].size.x,
            level.platforms[i].size.y
        ));
        platform.setPosition(Vector2f(
            level.platforms[i].position.x,
            level.platforms[i].position.y
        ));
        platform.setFillColor(level.platforms[i].color);
        window.draw(platform);
    }
}

void drawLevel(sf::RenderWindow& window, const Level& level, int levelId) {
    static sf::Texture platformTexture1;
    static sf::Texture platformTexture2;
    static bool texLoaded1 = platformTexture1.loadFromFile("assets/platform.png");
    static bool texLoaded2 = platformTexture2.loadFromFile("assets/LavaW_StonePlatform1.png");

    bool isLevel2 = (levelId == 1);
    sf::Texture& currentTexture = isLevel2 ? platformTexture2 : platformTexture1;
    bool texLoaded = isLevel2 ? texLoaded2 : texLoaded1;
    for (int i = 0; i < level.platformCount; i++) {
        sf::RectangleShape rect(sf::Vector2f(
            level.platforms[i].size.x,
            level.platforms[i].size.y
        ));
        rect.setPosition(sf::Vector2f(
            level.platforms[i].position.x,
            level.platforms[i].position.y
        ));
       if (texLoaded) {
    sf::Sprite platformSprite(currentTexture);
    platformSprite.setPosition(sf::Vector2f(level.platforms[i].position.x, level.platforms[i].position.y));
    platformSprite.setScale(sf::Vector2f(level.platforms[i].size.x / currentTexture.getSize().x, level.platforms[i].size.y / currentTexture.getSize().y));
    window.draw(platformSprite);
} else {
    rect.setFillColor(level.platforms[i].color);
    window.draw(rect);
}
    }
}