#pragma once

#include "Common.h"
#include "BaseLocation.h"

class GrezBot;

namespace OverlordManager {
	void OnUnitCreated(const sc2::Unit* unit, GrezBot& bot);
	void OverlordMove(const sc2::Unit * unit, const std::vector<sc2::Point2D> & dest, GrezBot& bot);
}