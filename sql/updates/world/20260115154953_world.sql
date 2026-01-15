-- Grand Executor Mortuus
UPDATE `creature_template` SET `ScriptName`='npc_silverpine_grand_executor_mortuus' WHERE `entry`=44615;
-- Fallen Human
UPDATE `creature_template` SET `ScriptName`='npc_silverpine_fallen_human' WHERE `entry` IN (44592, 44593);

-- Raise Forsaken (The Warchief Cometh)
DELETE FROM `spell_script_names` WHERE `spell_id` = 83173 AND `ScriptName` = 'spell_silverpine_raise_forsaken_83173';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(83173, 'spell_silverpine_raise_forsaken_83173');
-- Forsaken Trooper Masterscript (The Warchief Cometh)
DELETE FROM `spell_script_names` WHERE `spell_id` = 83149 AND `ScriptName` = 'spell_silverpine_forsaken_trooper_masterscript_high_command';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(83149, 'spell_silverpine_forsaken_trooper_masterscript_high_command');

DELETE FROM `creature_text` WHERE `entry` IN (44365,44629,44640);
INSERT INTO `creature_text` (`entry`, `text_group`, `id`, `text`, `text_female`, `text_range`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) values
-- Lady Sylvanas Windrunner
('44365','0','0',NULL,'Where is that ogre-headed buffoon?','0','12','0','100','6','0','20459','VO_QE_SP_Sylvanas_SPEvent01'),
('44365','1','0',NULL,'Ah, speak of the devil...','0','12','0','100','1','0','20460','VO_QE_SP_Sylvanas_SPEvent02'),
('44365','2','0',NULL,'Warchief, so glad you could make it.','0','12','0','100','1','0','20461','VO_QE_SP_Sylvanas_SPEvent03'),
('44365','3','0',NULL,'With the death of the Lich King, many of the more intelligent Scourge became... unemployed. Those \'fiends,\' as you so delicately put it, are called val\'kyr. They are under my command now...','0','12','0','100','0','0','20462','VO_QE_SP_Sylvanas_SPEvent04'),
('44365','4','0',NULL,'...and they are part of the reason that I asked to see you.','0','12','0','100','1','0','20463','VO_QE_SP_Sylvanas_SPEvent05'),
('44365','5','0',NULL,'Very well, Warchief. I have solved the plight of the Forsaken!','0','12','0','100','5','0','20464','VO_QE_SP_Sylvanas_SPEvent06'),
('44365','6','0',NULL,'As a race, we Forsaken are unable to procreate.','0','12','0','100','274','0','20465','VO_QE_SP_Sylvanas_SPEvent07'),
('44365','7','0',NULL,'With the aid of the val\'kyr, we are now able to take the corpses of the fallen and create new Forsaken.','0','12','0','100','0','0','20466','VO_QE_SP_Sylvanas_SPEvent08'),
('44365','8','0',NULL,'Agatha, show the Warchief!','0','12','0','100','5','0','20467','VO_QE_SP_Sylvanas_SPEvent09'),
('44365','9','0',NULL,'Warchief, without these new Forsaken my people would die out... Our hold upon Gilneas and northern Lordaeron would crumble.','0','12','0','100','0','0','20468','VO_QE_SP_Sylvanas_SPEvent10'),
('44365','10','0',NULL,'Isn\'t it obvious, Warchief? I serve the Horde.','0','12','0','100','66','0','20469','VO_QE_SP_Sylvanas_SPEvent11'),
-- Garrosh Hellscream
('44629','0','0','This better be important, Sylvanas. You know how I detest this place and its foul stench. Why have you called for me?',NULL,'0','12','0','100','0','0','20496','VO_QE_Garrosh_SPEvent01'),
('44629','1','0','And more importantly, what are those Scourge fiends doing here?',NULL,'0','12','0','100','25','0','20497','VO_QE_Garrosh_SPEvent02'),
('44629','2','0','Get on with it, Sylvanas.',NULL,'0','12','0','100','1','0','20498','VO_QE_Garrosh_SPEvent03'),
('44629','3','0','What you have done here, Sylvanas... it goes against the laws of nature. Disgusting is the only word I have to describe it.',NULL,'0','12','0','100','0','0','20499','VO_QE_Garrosh_SPEvent04'),
('44629','4','0','Have you given any thought to what this means, Sylvanas?',NULL,'0','12','0','100','6','0','20500','VO_QE_Garrosh_SPEvent05'),
('44629','5','0','What difference is there between you and the Lich King now?',NULL,'0','12','0','100','6','0','20501','VO_QE_Garrosh_SPEvent06'),
('44629','6','0','Watch your clever mouth.',NULL,'0','12','0','100','397','0','20502','VO_QE_Garrosh_SPEvent07'),
('44629','7','0','Cromush, you stay behind and make sure the Banshee Queen is well \"guarded.\" I will be expecting a full report when next we meet.',NULL,'0','12','0','100','0','0','20503','VO_QE_Garrosh_SPEvent08'),
('44629','8','0','Remember, Sylvanas, eventually we all have to stand before our maker and face judgment. Your day may come sooner than others...',NULL,'0','12','0','100','0','0','20504','VO_QE_Garrosh_SPEvent09'),
-- High Warlord Cromush
('44640','0','0','ABBERATION!',NULL,'0','12','0','100','5','0','0','VO_QE_SP_Crommush_SPEvent01'),
('44640','1','0','As you command, Warchief.',NULL,'0','12','0','100','66','0','0','VO_QE_SP_Crommush_SPEvent01');

DELETE FROM `waypoint_data` WHERE `id` = 446290;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES
(446290, 0, 1377.69, 1048.67, 53.4456, 0, 0, 0, 0, 0, 0),
(446290, 1, 1373.69, 1052.17, 53.4456, 0, 0, 0, 0, 0, 0),
(446290, 2, 1365.23, 1058.61, 53.0524, 0, 0, 0, 0, 0, 0);

DELETE FROM `waypoint_data` WHERE `id` = 446402;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES
(446402, 0, 1412.05, 1089.42, 60.4771, 0, 0, 1, 0, 0, 0),
(446402, 1, 1405.38, 1095.16, 60.4774, 0, 0, 1, 0, 0, 0),
(446402, 2, 1392.62, 1093.5, 56.4067, 0, 0, 1, 0, 0, 0),
(446402, 3, 1380.65, 1083.27, 52.6221, 0, 0, 1, 0, 0, 0),
(446402, 4, 1372.86, 1062, 53.0398, 0, 0, 1, 0, 0, 0),
(446402, 5, 1376.69, 1048.61, 53.3362, 0, 0, 1, 0, 0, 0),
(446402, 6, 1375.32, 1046.38, 53.2336, 0, 0, 1, 0, 0, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=1 AND `SourceEntry`=83173 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=3 AND `ConditionValue2`=44592 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES 
(13, 1, 83173, 0, 0, 31, 0, 3, 44592, 0, 0, 'Raise Forsaken - Target Fallen Human (Male)');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=1 AND `SourceEntry`=83173 AND `SourceId`=0 AND `ElseGroup`=1 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=3 AND `ConditionValue2`=44593 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES 
(13, 1, 83173, 0, 1, 31, 0, 3, 44593, 0, 0, 'Raise Forsaken - Target Fallen Human (Female)');

