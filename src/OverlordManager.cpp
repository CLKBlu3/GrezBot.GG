#include "OverlordManager.h"
#include "BaseLocation.h"
#include "GrezBot.h"
#include "Util.h"
#include "BaseLocationManager.h"


const int distanceTreshold = 20;

void OverlordManager::onStart(GrezBot& bot) {
	//OnStart --> Move to enemy Natural, if opponent is not building when we arrive --> Prepare for all-in?
	for (auto & unit : bot.UnitInfo().getUnits(Players::Self)) {
		if (unit.getType().isOverlord()) {
			sc2::Point2D enemyLoc = bot.Bases().getPlayerStartingBaseLocation(Players::Enemy)->getPosition();
			AddNextMove(unit.getUnitPtr(), enemyLoc, bot);
			break;
		}
	}
}

void OverlordManager::onFrame(GrezBot& bot) {
	for (auto & unit : bot.UnitInfo().getUnits(Players::Self)) {
		if (unit.getType().isOverlord()) {
			if (unit.isIdle()) {
				//make it do something
				HandleIdleOverlod(unit.getUnitPtr(), bot);
			}
		}
	}
}

//Called From GrezBot OnUnitCreated, previoulsy Checked that it is an Overlord unit!!
//@unit Overlord Unit Created
//@bot	Bot that called this func
void OverlordManager::OnUnitCreated(const sc2::Unit * unit, GrezBot & bot)
{
		//Make overlord roam to our natural
		sc2::Point2D nextExp;
		nextExp.x = bot.Bases().getNextExpansion(Players::Self).x;
		nextExp.y = bot.Bases().getNextExpansion(Players::Self).y;
		AddNextMove(unit, nextExp, bot);
}

//@unit Overlord unit to move
//@dest Destination Vector to go
//@bot	Bot that called this
void OverlordManager::OverlordMove(const sc2::Unit * unit, std::vector<sc2::Point2D>& dest, GrezBot& bot)
{
	bool moved = false;

	if (unit->health < (unit->health_max *0.75f)) {
		//Scouting overloard has been harassed, back off?
		if (EnemyOnSight(unit->pos, bot)) { //back off
			dest.clear(); //clear orders
			dest.push_back(fleePos(bot)); //Return to base
			bot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::MOVE, fleePos(bot), true);
			moved = true;
		}
	}
	//Look if the unit orders contains the move order
	for (sc2::UnitOrder order : unit->orders) {
		if (order.ability_id == sc2::ABILITY_ID::MOVE && (unit->pos.x == dest[dest.size() - 1].x && unit->pos.y == dest[dest.size() - 1].y)) {
			moved = true; //moved is completed
		}
	}

	if (!moved) { //lets move it to the next dest
		for (auto d : dest) {
			bot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::MOVE, d, true);
		}
	}
}

bool OverlordManager::EnemyOnSight(const sc2::Point2D pos, GrezBot& bot) {
	for (auto & unit : bot.UnitInfo().getUnits(Players::Enemy))
	{
		if (Util::Dist(unit, pos) < distanceTreshold)
		{
			return true;
		}
	}
	return false;
}

sc2::Point2D OverlordManager::fleePos(GrezBot& bot) {
	return bot.Bases().getPlayerStartingBaseLocation(Players::Self)->getPosition();
}

void OverlordManager::HandleIdleOverlod(const sc2::Unit* unit, GrezBot& bot)
{
	if (unit != nullptr) { //valid and idle Overlord
		//move it to a new location
		const std::vector<const BaseLocation*> baseLoc = bot.Bases().getBaseLocations();
		std::vector<sc2::Point2D> destVector;
		for (const BaseLocation* bL : baseLoc) { //new random location
			int scoutDistance = bot.Map().getGroundDistance(unit->pos, bL->getPosition());
			destVector.push_back(bL->getPosition());
		}

		const sc2::Point2D & dest = destVector[rand() % destVector.size()];
		AddNextMove(unit, dest, bot);
	}
}

void OverlordManager::AddNextMove(const sc2::Unit* unit, const sc2::Point2D dest, GrezBot & bot) {
	//Overlord may be idle or not!
	if (unit->orders.empty()) {
		//unit was idle!
		std::vector<sc2::Point2D> move = { dest };
		OverlordManager::OverlordMove(unit, move, bot);
	}
}


