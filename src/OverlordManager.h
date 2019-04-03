#pragma once

#include "Common.h"
#include "BaseLocation.h"

class GrezBot;

namespace OverlordManager {
	void onFrame(GrezBot& bot);
	void OnUnitCreated(const sc2::Unit* unit, GrezBot& bot);
	void OverlordMove(const sc2::Unit * unit, std::vector<sc2::Point2D> & dest, GrezBot& bot);
	bool enemyOnSight(const sc2::Point2D pos, GrezBot& bot);
	sc2::Point2D fleePos(GrezBot& bot);
	void handleIdleOverlod(const sc2::Unit* unit, GrezBot& bot);
	void addNextMove(const sc2::Unit* unit, const sc2::Point2D, GrezBot& bot);
}