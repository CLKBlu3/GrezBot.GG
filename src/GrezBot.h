#pragma once

#include "Common.h"

#include "sc2api/sc2_api.h"
#include "MapTools.h"
#include "BaseLocationManager.h"
#include "UnitInfoManager.h"
#include "WorkerManager.h"
#include "BotConfig.h"
#include "GameCommander.h"
#include "BuildingManager.h"
#include "StrategyManager.h"
#include "TechTree.h"
#include "MetaType.h"
#include "Unit.h"

class GrezBot : public sc2::Agent 
{
    MapTools                m_map;
    BaseLocationManager     m_bases;
    UnitInfoManager         m_unitInfo;
    WorkerManager           m_workers;
    StrategyManager         m_strategy;
    BotConfig               m_config;
    TechTree                m_techTree;
    GameCommander           m_gameCommander;

	sc2::Race				m_playerRace[2];
	ProductionManager		m_production;

    std::vector<Unit>       m_allUnits;
    std::vector<CCPosition> m_baseLocations;

    void setUnits();

    void OnError(const std::vector<sc2::ClientError> & client_errors, 
                 const std::vector<std::string> & protocol_errors = {}) override;

public:

    GrezBot();

    void OnGameStart() override;
    void OnStep() override;
	void OnGameEnd() override;

          BotConfig & Config();
          WorkerManager & Workers();
    const BaseLocationManager & Bases() const;
    const MapTools & Map() const;
    const UnitInfoManager & UnitInfo() const;
    const StrategyManager & Strategy() const;
    const TypeData & Data(const UnitType & type) const;
    const TypeData & Data(const CCUpgrade & type) const;
    const TypeData & Data(const MetaType & type) const;
    const TypeData & Data(const Unit & unit) const;
    CCRace GetPlayerRace(int player) const;
    CCPosition GetStartLocation() const;

    int GetCurrentFrame() const;
    int GetMinerals() const;
    int GetCurrentSupply() const;
    int GetMaxSupply() const;
    int GetGas() const;
    Unit GetUnit(const CCUnitID & tag) const;
    const std::vector<Unit> & GetUnits() const;
    const std::vector<CCPosition> & GetStartLocations() const;

	//Added func
	void OnUnitCreated(const sc2::Unit* unit);
	void UnitMove(const sc2::Unit* unit, std::vector<sc2::Point2D> & dest);
	void BuildWorkers();
	int getDroneCount();
	void BuildHatchery();
};