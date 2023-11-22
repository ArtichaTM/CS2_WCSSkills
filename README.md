# WCSSkills
Default Warcraft plugin for CS2 with one difference: instead of races, player can choose skills. Systems can be considered as in game [**Fate: Grand Order**](https://ru.wikipedia.org/wiki/Fate/Grand_Order)
## Main principles
### Status effects
Every, every skill applies [Status effect(-s)](https://fategrandorder.fandom.com/wiki/Status_Effects) (further SE). SE activation can be 3 types:
1. **Instant**: on apply, SE immediately makes action and does not add to SE list
1. **Periodical**: SE triggers with some period of time
1. **On event**: triggers on some event. Taking damage, dealing damage, activation some other SE and extra

Must be noted, this activations can be mixed. For example, skill that instantly heals 30% hp and then 5% every second. This SE will consider both as instant and periodical.

### Skills
[Skills](https://fategrandorder.fandom.com/wiki/Skills) can be considered as ultimate in default CS, but each individually doesn't powerful as ultimates. On skill activation buff applied to user, but this action can be canceled by debuff, applied on user

### Traits
There's no good information about traits in any wiki in F/GO, but you can read names about them in [SE](https://apps.atlasacademy.io/db/NA/buffs)/[functions](https://apps.atlasacademy.io/db/NA/funcs)/[entities](https://apps.atlasacademy.io/db/NA/entities)/[skills](https://apps.atlasacademy.io/db/NA/skills)/[noble phantasms](https://apps.atlasacademy.io/db/NA/noble-phantasms) search page or select some servant in this [search](https://apps.atlasacademy.io/db/NA/servants) page and go to _"Traits"_ section.

Every skill or SE have traits. For example, healing skills usually have "Light" trait, which is enemy for darkness, so you can't have light and darkness in one skill set which leads to disabling both skills at spawn, for more information you can try to read jsons in data folder

Note, that some traits can exist in F/GO, but not in CS2.
1. Exist in F/GO, does not in CS2: Critical Rate up
1. Exist in CS2, does not in F/GO:

## Porting information
### Overall information
**Arts**/**Quick**/**Buster**:
* Arts: attacking with zeus, knife, grenades
* Buster: attacking with rifles, machine guns
* Quick: attacking with pistols, shotguns, SMGs

Classes now remains unused, but I'll think about something interesting

**Alignments**:
* Lawful, Neutral, Chaotic considers as the same category and can't be in one SE/skill more than 2 times
* * _Lawful_: direct fight with players, using weapons
* * _Neutral_: both applies to neutral
* * _Chaotic_: fighting with players through skills
* Good, Neutral, Evil considers as the same category and can't be in one SE/skill more than 2 times
* * _Good_: Buffing teammates, resurrections and other support skills/SE
* * _Neutral_: Debuff-ing enemy, dealing direct damage to enemy, building spells
* * _Evil_: [CCs](https://en.wikipedia.org/wiki/Crowd_control_(video_games))

Note, that ruining game for teammates are evil-evil alignment, and there's 2 options to counter it: kick and ban

### Status effects
SE considered to be same and working in the same way, except in CS2 even status effect has traits. New information about this will be added later
Duration of SE can be 2 times:
* Turns: In CS2 implemented as time. 1 turn in F/GO considered as 5-10 seconds in CS2. This value can be changed based on SE/skill effects
* Times: status effect or skill will trigger on specific event. Usually all skills of category are event-based. Usually ported from F/GO as is
## Error codes:
Sometimes plugin raises exceptions, but metamod allows only 7 symbols to write in error char[]. So, here are transcriptions of errors:
* WDL1: Can't open file data/json/skills.json
* WDL2: Can't open file data/json/se.json
* WDL3: Can't open file data/json/traits.json
* WDL4: Can't open file data/sql/create_tables.sql
* IMR1: Trying to init already initialized InfoManager
* IMR2: Trying to close already closed InfoManager
* DB00: General query fault. In normal circumstances can't be raised to console, because usually catched in code
* DB01: Failed to initialize sqlite3 library. Can occur when something wrong with SQLite3 library
* DB02: Failed to open conn. Can occur, when path to database (data/database.db) blocker somehow
* DB03: Failed to prepare check tables statement
* DB04: Failed to read tables. Occurs when WCSS trying to list all created tables, but return unexpected
* DB05: Can't open file data/sql/create_tables.sql
* DB06: File data/sql/create_tables.sql larger than expected (usually 4096 symbols)
* DB07: Failed to prepare insert values. Can occur when insert data is invalid
* FUNCS1: Trying to get function without functions initialization (folder src/functions)
* FUNCS2: Trying to get function that doesn't exist. Occur, when function number mentioned in json files, but didn't defined in source code
* T_DO0: Ticker delay overflow. Raised by src/includes/ticker when something trying to delay task by amount exceeding maximum limit


Notes:
* Difference between WDL4 and DB05: WDL4 raised when file doesn't exist at all, while DB05 raised when file can't be read