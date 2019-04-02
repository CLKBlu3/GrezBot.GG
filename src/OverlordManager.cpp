#include "OverlordManager.h"
#include "BaseLocation.h"
#include "GrezBot.h"
#include "Util.h"

//Called From GrezBot OnUnitCreated, previoulsy Checked that it is an Overlord unit!!
//@unit Overlord Unit Created
//@bot	Bot that called this func
void OverlordManager::OnUnitCreated(const sc2::Unit * unit, GrezBot & bot)
{
		//Make overlord roam to an expansion or enemy scout enemy area
		const BaseLocation* enemyLoc = bot.Bases().getPlayerStartingBaseLocation(Players::Enemy);
		if (enemyLoc == nullptr) {//enemy Starting Location is unscouted yet
			return;
		}
		/*CCTilePosition altLoc = Bases().getNextExpansion(Players::Enemy);
			sc2::Point2D &dest = sc2::Point2D(altLoc.x, altLoc.y);
			const std::vector<sc2::Point2D> destVector = { dest };
			UnitMove(unit, destVector);*/
			//Scout enemy Bases
		const sc2::Point2D & dest = enemyLoc->getPosition();
		const sc2::Point2D & returnPos = bot.Bases().getPlayerStartingBaseLocation(Players::Self)->getPosition();
		const std::vector<sc2::Point2D> destVector = { dest, returnPos };
		OverlordMove(unit, destVector, bot);
}

//@unit Overlord unit to move
//@dest Destination Vector to go
//@bot	Bot that called this
void OverlordManager::OverlordMove(const sc2::Unit * unit, const std::vector<sc2::Point2D>& dest, GrezBot& bot)
{
	bool moved = false;
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
