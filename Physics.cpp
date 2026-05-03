#include "Physics.h"
#include <algorithm>

using namespace sf;

namespace {
const float GRAVITY     = 950.f;
const float KNOCKBACK_X = 400.f;
const float KNOCKBACK_Y = -250.f;
const int   ATTACK_DMG  = 10;
}

void physicsUpdate(Player& p, float dt) {
    if (!p.isAlive) return;

    p.velocity.y += GRAVITY * dt;

    p.pos.x += p.velocity.x * dt;
    p.pos.y += p.velocity.y * dt;

    if (p.pos.y + p.height >= 720.f) {
        p.pos.y      = 720.f - p.height;
        p.velocity.y = 0.f;
        p.onGround   = true;
    } else {
        p.onGround = false;
    }

    if (p.pos.x < 0.f)                   p.pos.x = 0.f;
    if (p.pos.x + p.width > WINDOW_WIDTH) p.pos.x = WINDOW_WIDTH - p.width;
}

void resolvePlayerCollision(Player& a, Player& b) {
    if (!a.isAlive || !b.isAlive) return;

    float aRight  = a.pos.x + a.width;
    float bRight  = b.pos.x + b.width;
    float aBottom = a.pos.y + a.height;
    float bBottom = b.pos.y + b.height;

    if (a.pos.x >= bRight  || b.pos.x >= aRight)  return;
    if (a.pos.y >= bBottom || b.pos.y >= aBottom)  return;

    float overlapX = std::min(aRight, bRight)   - std::max(a.pos.x, b.pos.x);
    float overlapY = std::min(aBottom, bBottom) - std::max(a.pos.y, b.pos.y);

    if (overlapX < overlapY) {
        float push = overlapX / 2.f;
        if (a.pos.x < b.pos.x) { a.pos.x -= push; b.pos.x += push; }
        else                    { a.pos.x += push; b.pos.x -= push; }

        if (a.pos.x < b.pos.x) {
            if (a.velocity.x > 0.f) a.velocity.x = 0.f;
            if (b.velocity.x < 0.f) b.velocity.x = 0.f;
        } else {
            if (a.velocity.x < 0.f) a.velocity.x = 0.f;
            if (b.velocity.x > 0.f) b.velocity.x = 0.f;
        }
    } else {
        float push = overlapY / 2.f;
        if (a.pos.y < b.pos.y) {
            a.pos.y -= push; b.pos.y += push;
            if (a.velocity.y > 0.f) a.velocity.y = 0.f;
            if (b.velocity.y < 0.f) b.velocity.y = 0.f;
            b.onGround = true;
        } else {
            a.pos.y += push; b.pos.y -= push;
            if (b.velocity.y > 0.f) b.velocity.y = 0.f;
            if (a.velocity.y < 0.f) a.velocity.y = 0.f;
            a.onGround = true;
        }
    }
}

void handleCombat(Player& attacker, Player& defender) {
    if (!attacker.isAttacking || !attacker.isAlive || !defender.isAlive)
        return;

    FloatRect defenderRect({defender.pos.x, defender.pos.y},
                           {defender.width,  defender.height});

    if (!attacker.attackBox.findIntersection(defenderRect))
        return;

    defender.health -= ATTACK_DMG;
    if (defender.health <= 0) {
        defender.health  = 0;
        defender.isAlive = false;
    }

    defender.velocity.x = attacker.facingRight ? +KNOCKBACK_X : -KNOCKBACK_X;
    defender.velocity.y = KNOCKBACK_Y;
    defender.onGround   = false;
}


void resolvePlatformCollision(Player& p, const Platform& plat){
    //palyer
    float left = p.pos.x;
    float right = p.pos.x + p.width;
    float top = p.pos.y;
    float bottom = p.pos.y + p.height;
    
    //platform
    float platleft = plat.position.x;
    float platright = plat.position.x + plat.size.x;
    float platTop = plat.position.y;
    float platbot = plat.position.y + plat.size.y;


    //not overlapping
    if (right <= platleft || left >= platright || top >= platTop || bottom <= platbot)
    {
        return;
    }
    //overlapping
    float overlapX = std::min(right, platright) - std::max(left, platleft);
    float overlapY = std::min(bottom, platbot) - std::max(top, platTop);
    if (overlapX<overlapY)
    {
        if (left < platleft)
        {
            p.pos.x -= overlapX;
        }
        else{
            p.pos.x += overlapX;
        }
    
    }
    else{
        if (top > platTop)
        {
            p.pos.y -= overlapY;
            p.velocity.y = 0;
            p.onGround = true;
        }
        else{
            p.pos.y += overlapY;
        }
    }





}