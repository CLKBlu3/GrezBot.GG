#pragma once

#include "Common.h"
#include "MicroManager.h"

class GrezBot;

class MeleeManager: public MicroManager
{

public:

    MeleeManager(GrezBot & bot);
    void    executeMicro(const std::vector<Unit> & targets);
    void    assignTargets(const std::vector<Unit> & targets);
    int     getAttackPriority(Unit attacker, const Unit & unit);
    Unit    getTarget(Unit meleeUnit, const std::vector<Unit> & targets);
    bool    meleeUnitShouldRetreat(Unit meleeUnit, const std::vector<Unit> & targets);
};
