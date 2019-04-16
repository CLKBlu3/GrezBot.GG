## AI OBJECTIVE OVERVIEW LIST
This is the objective for the initial version of this Zerg AI.

### Recommended Read 
API recommended read: if you dont mind i recommend to read all of unit, units, position, bot_ai, and the three game_* files to learn what is possible already
it will help you :)

### Expansion Scheme
- [ ] Take natural on 17 supply
- [ ] Take 3rd between 30 and 40 supply
- [ ] Expand IF 300minerals && no larvae && m_expansions <= 2x enemy_expansions
- [ ] Otherwise keep building Drones and/or army
- [ ] Build N+2 Queens (N = m_expansions) -> 1 Queen/base + 2 Defensive/Creep queens

### Macro Cycle
- [ ] Inject Bases when able
- [ ] If supply is not enough -> Build more overlords
- [ ] Move units, attack command if enough army
- [ ] Spread Creep
- [ ] Repeat

### Overlord Management
- [ ] Move original overlord to enemy Natural
- [ ] Move 1st created overlord to own Natural
- [ ] Make other overlords move between your expansions
- [ ] Build Overlords when necessary

### Worker Management
- [ ] Cap max workers to 75
- [ ] Cap max mineral workers to 48 (3 Saturated bases)
- [ ] Cap gas workers to 18 (3 Saturated bases)
- [ ] Extra Workers x2  -> Generate 2 extra workers when Harassment expected

### Army Management
- [ ] Generate 1/3 Hydras and 2/3 Roaches
- [ ] Timing attacks -> At finishing upgrades or when army is big
- [ ] Replenish army in the background while attacking

### BASIC BUILD ORDER
- [ ] 13 Overlord
- [ ] 17 Hatchery
- [ ] 18 Gas
- [ ] 17 Spawning Pool
- [ ] 19 Overlord
- [ ] 20 2x Queens
- [ ] @Spawning Pool -> 4x Zerglings
- [ ] 25 Metabolic boost (Ling speed)
- [ ] 30 Take 3rd Exp
- [ ] 33 Overlord
- [ ] 36 Overlord
- [ ] 40 2x Queens -> These are the 2x Extra Queens for defense/creep
- [ ] 41 Roach Warren
- [ ] @3rdExp 1x Queen

##### Guidelines from here
- [ ] @Roach Warren -> 8x Roach
- [ ] @2baseSaturation -> Take 2x Gas
- [ ] @2base mineral + 3 gas saturation -> Upgrade main to Lair
- [ ] @75% Lair -> 2x Evo Chamber
- [ ] @Lair -> Hydralisk Den
- [ ] @Lair -> Overseer x 1
- [ ] Take 2 additional gases on 8/16 workers in 3rd Exp
- [ ] @Evo Chambers -> +1  missile +1 Carpace
- [ ] @Hydra den -> Muscular Augments (Hydra upgrade) + Gliar Reconstitution (Roach speed)
- [ ] Mass army Following Army Management idea (2:1 Roach:Hydra ratio)
- [ ] @+1/+1 -> Take +2/+2
- [ ] @3 base mineral + gas saturation -> Infestation Pit
- [ ] @Infestation Pit -> Hive
- [ ] @Hive -> Take +3/+3

##### Additional points
- [ ] From this point -> 1x Queen when each expansion is finished
- [ ] Follow Expansion scheme 
- [ ] Follow Overlord scheme
- [ ] Keep Droning until 3 base saturation
- [ ] Timing Attacks -> When +1/+1 upgrade is finished, build army in the background and redo afterwards

##### Adaptations
- [ ] Add 1x Spore Crawler per base if we detect sometype of drop/flying harassment unit
- [ ] Add 1-2x Spines per base if we detect sometype of rush

##### Other ideas
- [ ] Try to make walls with evo chambers
- [ ] Redo destroyed building (evo chambers/pool/etc)

##### Late Game Additions
- [ ] Get Spire while building infestation pit and transition into broodlords late game (+ Great Spire @Hive)?
- [ ] Get Ultralisk Cavern and transition into Ultralisks?
- [ ] Composition: 1/2 Hydra, 1/3 Ultra/Broodlord, 1/6 Roach?? idk






