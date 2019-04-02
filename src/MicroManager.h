#pragma once

#include "Common.h"
#include "SquadOrder.h"
#include "Unit.h"

class GrezBot;

class MicroManager
{
    std::vector<Unit> m_units;

protected:

    GrezBot & m_bot;
    SquadOrder order;

    virtual void executeMicro(const std::vector<Unit> & targets) = 0;
    void trainSubUnits(const Unit & unit) const;

public:

    MicroManager(GrezBot & bot);

    const std::vector<Unit> & getUnits() const;

    void setUnits(const std::vector<Unit> & u);
    void execute(const SquadOrder & order);
    void regroup(const CCPosition & regroupPosition) const;

};