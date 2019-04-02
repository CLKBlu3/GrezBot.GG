#include "GrezBot.h"
#include "Util.h"
#include "OverlordManager.h"
#include <iostream>
#include <random>

GrezBot::GrezBot()
    : m_map(*this)
    , m_bases(*this)
    , m_unitInfo(*this)
    , m_workers(*this)
    , m_gameCommander(*this)
    , m_strategy(*this)
    , m_techTree(*this)
{
    
}

void GrezBot::OnGameStart() 
{
    m_config.readConfigFile();

    // add all the possible start locations on the map
    for (auto & loc : Observation()->GetGameInfo().enemy_start_locations)
    {
        m_baseLocations.push_back(loc);
    }
    m_baseLocations.push_back(Observation()->GetStartLocation());
    
	auto playerID = Observation()->GetPlayerID();
	for (auto & playerInfo : Observation()->GetGameInfo().player_info) {
		if (playerInfo.player_id == playerID) {
			m_playerRace[Players::Self] = playerInfo.race_actual;
		}
		else
		{
			m_playerRace[Players::Enemy] = playerInfo.race_requested;
		}
	}

    setUnits();
    m_techTree.onStart();
    m_strategy.onStart();
    m_map.onStart();
    m_unitInfo.onStart();
    m_bases.onStart();
    m_workers.onStart();

    m_gameCommander.onStart();
}

void GrezBot::OnStep()
{
    setUnits();
    m_map.onFrame();
    m_unitInfo.onFrame();
    m_bases.onFrame();
    m_workers.onFrame();
    m_strategy.onFrame();

    m_gameCommander.onFrame();
	//OverlordManager();
	//LarvaeManager();
	//DroneManager();
	//QueenManager();

    Debug()->SendDebug();
}

//OnUnitCreated --> Manages Unit behaviour on create
//@unit unit created
void GrezBot::OnUnitCreated(const sc2::Unit* unit) {
	//Unit created --> manage it
	m_unitInfo.onFrame();
	if (unit->unit_type == sc2::UNIT_TYPEID::ZERG_OVERLORD) OverlordManager::OnUnitCreated(unit, *this);
}

//Unit Move command
//@unit unit to move
//@dest destination
void GrezBot::UnitMove(const sc2::Unit * unit, const std::vector<sc2::Point2D> & dest) {
	bool moved = false;
	//Look if the unit orders contains the move order
	for (sc2::UnitOrder order : unit->orders) {
		if (order.ability_id == sc2::ABILITY_ID::MOVE && (unit->pos.x == dest[dest.size()-1].x && unit->pos.y == dest[dest.size() - 1].y)) {
			moved = true; //moved is completed
		}
	}
	
	if (!moved) { //lets move it to the next dest
		for (auto d : dest) {
			Actions()->UnitCommand(unit, sc2::ABILITY_ID::MOVE, d, true);
		}
	}
}

void GrezBot::OnGameEnd()
{
	//Gather data ?
}

void GrezBot::setUnits()
{
    m_allUnits.clear();
    Control()->GetObservation();
    for (auto & unit : Observation()->GetUnits())
    {
        m_allUnits.push_back(Unit(unit, *this));    
    }
}

CCRace GrezBot::GetPlayerRace(int player) const
{
    auto playerID = Observation()->GetPlayerID();
    for (auto & playerInfo : Observation()->GetGameInfo().player_info)
    {
        if (playerInfo.player_id == playerID)
        {
            return playerInfo.race_actual;
        }
    }

    BOT_ASSERT(false, "Didn't find player to get their race");
    return sc2::Race::Random;
}

BotConfig & GrezBot::Config()
{
     return m_config;
}

const MapTools & GrezBot::Map() const
{
    return m_map;
}

const StrategyManager & GrezBot::Strategy() const
{
    return m_strategy;
}

const BaseLocationManager & GrezBot::Bases() const
{
    return m_bases;
}

const UnitInfoManager & GrezBot::UnitInfo() const
{
    return m_unitInfo;
}

int GrezBot::GetCurrentFrame() const
{
    return (int)Observation()->GetGameLoop();
}

const TypeData & GrezBot::Data(const UnitType & type) const
{
    return m_techTree.getData(type);
}

const TypeData & GrezBot::Data(const Unit & unit) const
{
    return m_techTree.getData(unit.getType());
}

const TypeData & GrezBot::Data(const CCUpgrade & type) const
{
    return m_techTree.getData(type);
}

const TypeData & GrezBot::Data(const MetaType & type) const
{
    return m_techTree.getData(type);
}

WorkerManager & GrezBot::Workers()
{
    return m_workers;
}

int GrezBot::GetCurrentSupply() const
{
    return Observation()->GetFoodUsed();
}

int GrezBot::GetMaxSupply() const
{
    return Observation()->GetFoodCap();
}

int GrezBot::GetMinerals() const
{
    return Observation()->GetMinerals();
}

int GrezBot::GetGas() const
{
    return Observation()->GetVespene();
}

Unit GrezBot::GetUnit(const CCUnitID & tag) const
{
    return Unit(Observation()->GetUnit(tag), *(GrezBot *)this);
}

const std::vector<Unit> & GrezBot::GetUnits() const
{
    return m_allUnits;
}

CCPosition GrezBot::GetStartLocation() const
{
    return Observation()->GetStartLocation();
}

const std::vector<CCPosition> & GrezBot::GetStartLocations() const
{
    return m_baseLocations;
}

void GrezBot::OnError(const std::vector<sc2::ClientError> & client_errors, const std::vector<std::string> & protocol_errors)
{
    
}