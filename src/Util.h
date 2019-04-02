#pragma once

#include "Common.h"
#include "UnitType.h"

class GrezBot;
class Unit;

namespace Util
{
    CCRace          GetRaceFromString(const std::string & str);
    CCTilePosition  GetTilePosition(const CCPosition & pos);
    CCPosition      GetPosition(const CCTilePosition & tile);
    std::string     GetStringFromRace(const CCRace & race);
    bool            UnitCanMetaTypeNow(const Unit & unit, const UnitType & type, GrezBot & m_bot);
    UnitType        GetTownHall(const CCRace & race, GrezBot & bot);
    UnitType        GetRefinery(const CCRace & race, GrezBot & bot);
    UnitType        GetSupplyProvider(const CCRace & race, GrezBot & bot);
    CCPosition      CalcCenter(const std::vector<Unit> & units);
    bool            IsZerg(const CCRace & race);
    bool            IsProtoss(const CCRace & race);
    bool            IsTerran(const CCRace & race);
    CCPositionType  TileToPosition(float tile);

    sc2::BuffID     GetBuffFromName(const std::string & name, GrezBot & bot);
    sc2::AbilityID  GetAbilityFromName(const std::string & name, GrezBot & bot);

    float Dist(const Unit & unit, const CCPosition & p2);
    float Dist(const Unit & unit1, const Unit & unit2);
    float Dist(const CCPosition & p1, const CCPosition & p2);
    CCPositionType DistSq(const CCPosition & p1, const CCPosition & p2);
};
