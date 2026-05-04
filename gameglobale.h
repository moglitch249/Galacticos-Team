#ifndef GAMEGLOBALE_H
#define GAMEGLOBALE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

using namespace sf;

   const int WINDOW_WIDTH = 1600;
   const int WINDOW_HEIGHT = 900;
   extern int score;

   struct Vec2{
    float x, y;
   };

  //  Level
   struct Platform{
     bool solid;
     Vec2 position;
     Vec2 size;
     sf::Color color;
     sf::FloatRect rect;
     int textureId;
  };
    
    const int MAX_PLATFORMS = 64;
    const int MAX_PLAYERS = 2;
    const int MAX_LEVELS = 2;
    
    struct Level{
      Platform platforms[MAX_PLATFORMS];
      int platformCount;
      sf::Color bgColor;
      Vec2 spawnPoints[MAX_PLAYERS];
      Vec2 size;
   };

    extern std::vector<Level> levels;
  

// Player 
  struct Player{
      Vec2 pos;
      Vec2 velocity;
      float width;
      float height;
      int health;
      int maxHealth;
      bool onGround;
      bool facingRight;
      bool isAlive;
      int score;
      
      bool inputLeft;
      bool inputRight;
      bool inputJump;

      bool inputAttack;
      bool isAttacking;
      float attackTimer;
      float attackCooldown;
      sf::FloatRect attackBox;
    };

   
#endif
