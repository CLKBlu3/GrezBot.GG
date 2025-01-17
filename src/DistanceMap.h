#pragma once

#include "Common.h"
#include <map>

class GrezBot;


class DistanceMap 
{
    int m_width;
    int m_height;
    CCTilePosition m_startTile;

    // 2D matrix storing distances from the start tile
    std::vector<std::vector<int>> m_dist;

    std::vector<CCTilePosition> m_sortedTiles;
    
public:
    
    DistanceMap();
    void computeDistanceMap(GrezBot & m_bot, const CCTilePosition & startTile);

    int getDistance(int tileX, int tileY) const;
    int getDistance(const CCTilePosition & pos) const;
    int getDistance(const CCPosition & pos) const;

    // given a position, get the position we should move to to minimize distance
    const std::vector<CCTilePosition> & getSortedTiles() const;
    const CCTilePosition & getStartTile() const;

    void draw(GrezBot & bot) const;
};