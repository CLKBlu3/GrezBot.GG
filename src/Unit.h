#pragma once

#include "Common.h"
#include "UnitType.h"

class GrezBot;

class Unit
{
    mutable GrezBot * m_bot;
    CCUnitID    m_unitID;
    UnitType    m_unitType;

    const sc2::Unit * m_unit;

public:

    Unit();

    Unit(const sc2::Unit * unit, GrezBot & bot);
    const sc2::Unit * getUnitPtr() const;
    const sc2::UnitTypeID & getAPIUnitType() const;

    bool operator < (const Unit & rhs) const;
    bool operator == (const Unit & rhs) const;

    const UnitType & getType() const;

    CCPosition getPosition() const;
    CCTilePosition getTilePosition() const;
    CCHealth getHitPoints() const;
    CCHealth getShields() const;
    CCHealth getEnergy() const;
    CCPlayer getPlayer() const;
    CCUnitID getID() const;
    float getBuildPercentage() const;
    int getWeaponCooldown() const;
    bool isCompleted() const;
    bool isBeingConstructed() const;
    bool isCloaked() const;
    bool isFlying() const;
    bool isAlive() const;
    bool isPowered() const;
    bool isIdle() const;
    bool isBurrowed() const;
    bool isValid() const;
    bool isTraining() const;
    bool isConstructing(const UnitType & type) const;

    void stop           () const;
    void attackUnit     (const Unit & target) const;
    void attackMove     (const CCPosition & targetPosition) const;
    void move           (const CCPosition & targetPosition) const;
    void move           (const CCTilePosition & targetTilePosition) const;
    void rightClick     (const Unit & target) const;
    void repair         (const Unit & target) const;
    void build          (const UnitType & buildingType, CCTilePosition pos) const;
    void buildTarget    (const UnitType & buildingType, const Unit & target) const;
    void train          (const UnitType & buildingType) const;
    void morph          (const UnitType & type) const;
};
