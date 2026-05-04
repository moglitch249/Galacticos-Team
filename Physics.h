#pragma once
#include "gameglobale.h"

void physicsUpdate(Player& p, float dt, int levelId);
void resolvePlayerCollision(Player& a, Player& b);
void handleCombat(Player& attacker, Player& defender);
void resolvePlatformCollision(Player& p, const Platform& plat);