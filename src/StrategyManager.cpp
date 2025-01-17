#include "StrategyManager.h"
#include "GrezBot.h"
#include "JSONTools.h"
#include "Util.h"
#include "MetaType.h"

Strategy::Strategy()
{

}

Strategy::Strategy(const std::string & name, const CCRace & race, const BuildOrder & buildOrder, const Condition & scoutCondition, const Condition & attackCondition)
    : m_name            (name)
    , m_race            (race)
    , m_buildOrder      (buildOrder)
    , m_wins            (0)
    , m_losses          (0)
    , m_scoutCondition  (scoutCondition)
    , m_attackCondition (attackCondition)
{

}

// constructor
StrategyManager::StrategyManager(GrezBot & bot)
    : m_bot(bot)
{

}

void StrategyManager::onStart()
{
    readStrategyFile(m_bot.Config().ConfigFileLocation);
}

void StrategyManager::onFrame()
{

}

const Strategy & StrategyManager::getCurrentStrategy() const
{
    auto strategy = m_strategies.find(m_bot.Config().StrategyName);

    BOT_ASSERT(strategy != m_strategies.end(), "Couldn't find Strategy corresponding to strategy name: %s", m_bot.Config().StrategyName.c_str());

    return (*strategy).second;
}

const BuildOrder & StrategyManager::getOpeningBookBuildOrder() const
{
    return getCurrentStrategy().m_buildOrder;
}

bool StrategyManager::scoutConditionIsMet() const
{
    return getCurrentStrategy().m_scoutCondition.eval();
}

bool StrategyManager::attackConditionIsMet() const
{
    return getCurrentStrategy().m_attackCondition.eval();
}

bool StrategyManager::shouldExpandNow() const
{
    return false;
}

void StrategyManager::addStrategy(const std::string & name, const Strategy & strategy)
{
    m_strategies[name] = strategy;
}

const UnitPairVector StrategyManager::getBuildOrderGoal() const
{
    return std::vector<UnitPair>();
}

const UnitPairVector StrategyManager::getProtossBuildOrderGoal() const
{
    return std::vector<UnitPair>();
}

const UnitPairVector StrategyManager::getTerranBuildOrderGoal() const
{
    return std::vector<UnitPair>();
}

const UnitPairVector StrategyManager::getZergBuildOrderGoal() const
{
    return std::vector<UnitPair>();
}


void StrategyManager::onEnd(const bool isWinner)
{

}

void StrategyManager::readStrategyFile(const std::string & filename)
{
    CCRace race = m_bot.GetPlayerRace(Players::Self);
    std::string ourRace = Util::GetStringFromRace(race);

    std::ifstream file(filename);
    json j;
    file >> j;

    const char * strategyObject = "SC2API Strategy";

    // Parse the Strategy Options
    if (j.count(strategyObject) && j[strategyObject].is_object())
    {
        const json & strategy = j[strategyObject];

        // read in the various strategic elements
        JSONTools::ReadBool("ScoutHarassEnemy", strategy, m_bot.Config().ScoutHarassEnemy);
        JSONTools::ReadString("ReadDirectory", strategy, m_bot.Config().ReadDir);
        JSONTools::ReadString("WriteDirectory", strategy, m_bot.Config().WriteDir);

        // if we have set a strategy for the current race, use it
        if (strategy.count(ourRace.c_str()) && strategy[ourRace.c_str()].is_string())
        {
            m_bot.Config().StrategyName = strategy[ourRace.c_str()].get<std::string>();
        }

        // check if we are using an enemy specific strategy
        JSONTools::ReadBool("UseEnemySpecificStrategy", strategy, m_bot.Config().UseEnemySpecificStrategy);
        if (m_bot.Config().UseEnemySpecificStrategy && strategy.count("EnemySpecificStrategy") && strategy["EnemySpecificStrategy"].is_object())
        {
            // TODO: Figure out enemy name
            const std::string enemyName = "ENEMY NAME";
            const json & specific = strategy["EnemySpecificStrategy"];

            // check to see if our current enemy name is listed anywhere in the specific strategies
            if (specific.count(enemyName.c_str()) && specific[enemyName.c_str()].is_object())
            {
                const json & enemyStrategies = specific[enemyName.c_str()];

                // if that enemy has a strategy listed for our current race, use it
                if (enemyStrategies.count(ourRace.c_str()) && enemyStrategies[ourRace.c_str()].is_string())
                {
                    m_bot.Config().StrategyName = enemyStrategies[ourRace.c_str()].get<std::string>();
                    m_bot.Config().FoundEnemySpecificStrategy = true;
                }
            }
        }

        // Parse all the Strategies
        if (strategy.count("Strategies") && strategy["Strategies"].is_object())
        {
            const json & strategies = strategy["Strategies"];
            for (auto it = strategies.begin(); it != strategies.end(); ++it) 
            {
                const std::string & name = it.key();
                const json & val = it.value();              
                
                BOT_ASSERT(val.count("Race") && val["Race"].is_string(), "Strategy is missing a Race string");
                CCRace strategyRace = Util::GetRaceFromString(val["Race"].get<std::string>());
                
                BOT_ASSERT(val.count("OpeningBuildOrder") && val["OpeningBuildOrder"].is_array(), "Strategy is missing an OpeningBuildOrder array");
                BuildOrder buildOrder;
                const json & build = val["OpeningBuildOrder"];
                for (size_t b(0); b < build.size(); b++)
                {
                    if (build[b].is_string())
                    {
                        // this is the raw string for the build order item in the config file
                        const std::string & buildItem(build[b]);

                        size_t numitems = 1;
                        std::string itemName;
                        std::stringstream ss(buildItem);
                        bool startsWithNumber = isdigit(buildItem[0]); // first char digit or not

                        for (size_t i = 0; i < buildItem.length(); i++)
                        {
                            if (buildItem[i] == ' ') break;

                            if (!isdigit(buildItem[i]))
                            {
                                startsWithNumber = false;
                                break;
                            }
                        }

                        if (startsWithNumber)
                        {
                            // numitems and then itemName because they are space separated and format is "5 SCV" 
                            // can change to different delimiter if needed
                            ss >> numitems;
                            ss >> itemName;
                        }

                        else
                        {
                            // no number here so format is "SCV"
                            ss >> itemName;
                        }

                        // Construct the metatype based on the extracted type name
                        MetaType metaType(itemName, m_bot);
                        // Add the meta type numTimes to the build order	
                        for (size_t i(0); i < numitems; i++)
                        {
                            buildOrder.add(metaType);
                        }
                    }

                    else
                    {
                        BOT_ASSERT(false, "Build order item must be a string %s", name.c_str());
                        continue;
                    }
                }

                BOT_ASSERT(val.count("ScoutCondition") && val["ScoutCondition"].is_array(), "Strategy is missing a ScoutCondition array");
                Condition scoutCondition(val["ScoutCondition"], m_bot);
                
                BOT_ASSERT(val.count("AttackCondition") && val["AttackCondition"].is_array(), "Strategy is missing an AttackCondition array");
                Condition attackCondition(val["AttackCondition"], m_bot);

                addStrategy(name, Strategy(name, strategyRace, buildOrder, scoutCondition, attackCondition));
            }
        }
    }
}
