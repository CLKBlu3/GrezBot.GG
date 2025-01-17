#pragma once

#include "Common.h"

class GrezBot;

class UnitType
{
    mutable GrezBot * m_bot;
    
    sc2::UnitTypeID m_type;

public:

    UnitType();

    UnitType(const sc2::UnitTypeID & type, GrezBot & bot);
    sc2::UnitTypeID getAPIUnitType() const;
    bool is(const sc2::UnitTypeID & type) const;


    bool operator < (const UnitType & rhs) const;
    bool operator == (const UnitType & rhs) const;

    std::string getName() const;
    CCRace getRace() const;
    
    bool isValid() const;
    bool isBuilding() const;
    bool isCombatUnit() const;
    bool isSupplyProvider() const;
    bool isResourceDepot() const;
    bool isRefinery() const;
    bool isDetector() const;
    bool isGeyser() const;
    bool isMineral() const;
    bool isWorker() const;
    bool isMorphedBuilding() const;
    bool canAttack() const;
    bool canMove() const;
    bool isAddon() const;
    CCPositionType getAttackRange() const;
    int tileWidth() const;
    int tileHeight() const;
    int supplyProvided() const;
    int supplyRequired() const;
    int mineralPrice() const;
    int gasPrice() const;

    const std::vector<UnitType> & whatBuilds() const;

    static UnitType GetUnitTypeFromName(const std::string & name, GrezBot & bot);

    // lazy functions to let me know if this type is a special type
    bool isOverlord() const;
    bool isLarva() const;
    bool isEgg() const;
    bool isQueen() const;
    bool isTank() const;
};
