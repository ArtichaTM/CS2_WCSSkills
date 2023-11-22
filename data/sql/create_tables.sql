CREATE TABLE player (
    steam_id BIGINT UNSIGNED,
    lvl MEDIUMINT NOT NULL DEFAULT 1,
    xp INT NULL DEFAULT 0,
    last_update DATETIME,
    PRIMARY KEY (steam_id)
);
CREATE TABLE player_skill (
    steam_id BIGINT UNSIGNED,
    skill_id SMALLINT UNSIGNED,
    lvl MEDIUMINT NOT NULL DEFAULT 1,
    xp INT NULL DEFAULT 0,
    FOREIGN KEY (steam_id)
        REFERENCES player (steam_id)
        ON DELETE CASCADE,
    PRIMARY KEY (steam_id, skill_id)
);

CREATE TABLE task (
    steam_id BIGINT UNSIGNED,
    task_id TINYINT UNSIGNED,
    xp INT NOT NULL,
    status INT NOT NULL CHECK(status IN (0, 1)), /* in_progress, visible */
    FOREIGN KEY (steam_id)
        REFERENCES player (steam_id)
        ON DELETE CASCADE,
    PRIMARY KEY (steam_id, task_id)
);
CREATE TABLE player_setting_1 (
    steam_id BIGINT UNSIGNED,
    setting_id TINYINT UNSIGNED,
    value BOOL NOT NULL,
    FOREIGN KEY (steam_id)
        REFERENCES player (steam_id)
        ON DELETE CASCADE,
    PRIMARY KEY (steam_id, setting_id)
);
CREATE TABLE player_setting_2 (
    steam_id BIGINT UNSIGNED,
    setting_id TINYINT UNSIGNED,
    value FLOAT(10) NOT NULL,
    FOREIGN KEY (steam_id)
        REFERENCES player (steam_id)
        ON DELETE CASCADE,
    PRIMARY KEY (steam_id, setting_id)
);
CREATE TABLE player_setting_3 (
    steam_id BIGINT UNSIGNED,
    setting_id TINYINT UNSIGNED,
    value VARCHAR(44) NOT NULL,
    FOREIGN KEY (steam_id)
        REFERENCES player (steam_id)
        ON DELETE CASCADE,
    PRIMARY KEY (steam_id , setting_id)
);
CREATE TABLE player_skill_setting_1 (
    steam_id BIGINT UNSIGNED,
    skill_id SMALLINT UNSIGNED,
    setting_id MEDIUMINT UNSIGNED,
    value BOOL NOT NULL,
    FOREIGN KEY (steam_id , skill_id)
        REFERENCES player_skill (steam_id , skill_id)
        ON DELETE CASCADE,
    PRIMARY KEY (steam_id , skill_id , setting_id)
);
CREATE TABLE player_skill_setting_2 (
    steam_id BIGINT UNSIGNED,
    skill_id SMALLINT UNSIGNED,
    setting_id MEDIUMINT,
    value FLOAT(10) NOT NULL,
    FOREIGN KEY (steam_id , skill_id)
        REFERENCES player_skill (steam_id , skill_id)
        ON DELETE CASCADE,
    PRIMARY KEY (steam_id , skill_id , setting_id)
);
CREATE TABLE player_skill_setting_3 (
    steam_id BIGINT UNSIGNED,
    skill_id SMALLINT UNSIGNED,
    setting_id MEDIUMINT,
    value VARCHAR(44) NOT NULL,
    FOREIGN KEY (steam_id , skill_id)
        REFERENCES player_skill (steam_id , skill_id)
        ON DELETE CASCADE,
    PRIMARY KEY (steam_id , skill_id , setting_id)
);
CREATE TABLE storage_boosters (
    id SMALLINT UNSIGNED /*COMMENT 'personal id starts from 0 and ends with id 31 like in auto_increment'*/,
    steam_id BIGINT UNSIGNED,
    amount FLOAT UNSIGNED /*COMMENT 'Percent which adds to xp. 1.1 adds 10% to xp taken. Stacks'*/,
    length INT UNSIGNED /*COMMENT 'Amount of minutes user which booster working. Maximum: 45 days (64800 minutes)'*/,
    distribution INT NOT NULL CHECK(distribution IN (0, 1, 2))
        /*COMMENT 'Who takes additional xp from booster', solo, team, server */,
    FOREIGN KEY (steam_id)
        REFERENCES player (steam_id)
        ON DELETE CASCADE,
    PRIMARY KEY (steam_id, id)
)  /*COMMENT='Storage of users personal boosts'*/;
CREATE TABLE active_boosters (
    id SMALLINT UNSIGNED /*COMMENT 'personal id starts from 0 and ends with id 31 like in auto_increment'*/,
    steam_id BIGINT UNSIGNED,
    amount FLOAT /*COMMENT "Amount of minutes left for this booster"*/,
    time_end TIMESTAMP NOT NULL /*COMMENT 'Date when booster is disabled'*/,
    distribution INT NOT NULL CHECK(distribution IN (0, 1, 2))
        /*COMMENT 'Who takes additional xp from booster', solo, team, server */,
    FOREIGN KEY (steam_id)
        REFERENCES player (steam_id)
        ON DELETE CASCADE,
    PRIMARY KEY (steam_id, id)
) /*COMMENT='Current active boosters'*/;