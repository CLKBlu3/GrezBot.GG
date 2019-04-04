#include "GrezBot.h"
#include "Util.h"
#include "OverlordManager.h"
#include <iostream>

//MAYBE WORKER CREATION SHOULD BE MANAGED SOMEWHERE ELSE?
const int MAX_TOTAL_WORKERS = 80; //Expected max workers for zerg [70-80]
const int MAX_MINERAL_WORKERS = 62; //1-1,5 workers per mineral patch
const int MAX_GAS_WORKERS = 18; //6 gases x 3 workers at max Saturation
const int HARASSMENT_WORKERS_EXPECTED = 2; //2*Hatch, expected damage received from harassment (this will be HARDLIMIT = MAX TOTAL + HARASSMENTS_WORKERS)

GrezBot::GrezBot()
    : m_map(*this)
    , m_bases(*this)
    , m_unitInfo(*this)
    , m_workers(*this)
    , m_gameCommander(*this)
    , m_strategy(*this)
    , m_techTree(*this)
	, m_production(*this)
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
	//Calculate ExpectedEnemyBases and Race
	m_production.onStart();
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
	
	m_production.onFrame();
	OverlordManager::onFrame(*this);
	//Implement this managers for each unit type?
	//LarvaeManager();
	//DroneManager();
	//QueenManager();

	BuildWorkers();
	BuildHatchery();

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
void GrezBot::UnitMove(const sc2::Unit * unit, std::vector<sc2::Point2D> & dest) {
	if (unit->unit_type == sc2::UNIT_TYPEID::ZERG_OVERLORD) OverlordManager::OverlordMove(unit, dest, *this);
}

//Checks if it is a good moment to make more workers AND if we shall be doing them.
//@		Creates new workers for each base (or not)
/* RULES TO CHECK:
	-if max workers of gas/minerals are not exceeded
	-keep a saturation proportion of 1-1,5
	-DRONE RULE FOR UNIT TO CREATE:
		-MINERAL RULING:
			-REACH 12-16 MINERAL WORKERS / HATCH AS FAST AS POSSIBLE
		-GAS RULING BASED ON PLAN:
			-ZERGLING: 0 GAS
			-BANELING/ROACH 1-2 GASES
			-RAVAGER/HYDRALISK 2 GASES
			-MUTALISK 3 GASES
			-BIGGER --> 6 GASES 
*/
void GrezBot::BuildWorkers() {
	int actDrones = getDroneCount();
}

int GrezBot::getDroneCount() {
	int actDrones = 0;
	for (auto & unit : this->GetUnits()) {
		if (unit.getUnitPtr()->unit_type == sc2::UNIT_TYPEID::ZERG_DRONE) {
			if (unit.isValid()) actDrones++;
		}
	}
	return actDrones;
}

//Checks if it is a good moment to make a new Hatchery/Expansion and Creates if it's the case
/* RULES TO CHECK:
	-HATCHERY CONSTRAINT RULE: Number of hatcheries = 1,5-3xExpectedEnemyBases (except zerg --> Try to keep 1-2xExpectedEnemyBases
	WHEN TO BUILD:
	-IF TOTAL_HATCH < 2 --> BUILD A NEW ONE WHEN POSSIBLE AND SATURATE IT
	-IF TOTAL_HATCH < 1,5-2xExpectedEnemyBases (GENERALIZED)
		-IF HATCHERY HAVE < 3 LARVAE
			-IF EXPECTED_MINERALS WHEN 3 IDLE LARVAE ON ALL HATCHERIES >= 300 --> BUILD HATCH + 5 DRONES TO SUPPORT IT
			-ELSE BUILD 5 MORE DRONES [5 DRONE RULE]
*/
void GrezBot::BuildHatchery() {

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