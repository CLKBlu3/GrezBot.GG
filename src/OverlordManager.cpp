#include "OverlordManager.h"
#include "BaseLocation.h"
#include "GrezBot.h"
#include "Util.h"


const int distanceTreshold = 20;

void OverlordManager::onFrame(GrezBot& bot) {

}

//Called From GrezBot OnUnitCreated, previoulsy Checked that it is an Overlord unit!!
//@unit Overlord Unit Created
//@bot	Bot that called this func
void OverlordManager::OnUnitCreated(const sc2::Unit * unit, GrezBot & bot)
{
		//Make overlord roam to an expansion or enemy scout enemy area
		const BaseLocation* enemyLoc = bot.Bases().getPlayerStartingBaseLocation(Players::Enemy);
		if (enemyLoc) { //enemyLoc is known
			//scout
			int scoutDistance = bot.Map().getGroundDistance(unit->pos, enemyLoc->getPosition());
			bool inEnemyZone = enemyLoc->containsPosition(unit->pos);
			const sc2::Point2D & dest = enemyLoc->getPosition();
			const sc2::Point2D & returnPos = fleePos(bot);
			
			std::vector<sc2::Point2D> destVector = { dest, returnPos };
			
			if (unit->health < (unit->health_max *0.75f)) {
				//Scouting overloard has been harassed, back off?
				if (enemyOnSight(unit->pos, bot)) { //back off
					destVector.clear(); //clear orders
					destVector.push_back(fleePos(bot)); //Return to base
				}
			}
			
			OverlordMove(unit, destVector, bot);
		}
}

//@unit Overlord unit to move
//@dest Destination Vector to go
//@bot	Bot that called this
void OverlordManager::OverlordMove(const sc2::Unit * unit, std::vector<sc2::Point2D>& dest, GrezBot& bot)
{
	bool moved = false;

	if (unit->health < (unit->health_max *0.75f)) {
		//Scouting overloard has been harassed, back off?
		if (enemyOnSight(unit->pos, bot)) { //back off
			dest.clear(); //clear orders
			dest.push_back(fleePos(bot)); //Return to base
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

bool OverlordManager::enemyOnSight(const sc2::Point2D pos, GrezBot& bot) {
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


