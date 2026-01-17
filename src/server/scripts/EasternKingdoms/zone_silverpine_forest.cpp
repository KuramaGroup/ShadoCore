/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* ScriptData
SDName: Silverpine_Forest
SD%Complete: 100
SDComment: Quest support: 435
SDCategory: Silverpine Forest
EndScriptData */

/* ContentData
npc_deathstalker_erland
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "Player.h"

#include "CellImpl.h"
#include "CombatAI.h"
#include "CreatureAIImpl.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "GridNotifiers.h"
#include "Group.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Object.h"
#include "ScriptedGossip.h"
#include "SharedDefines.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "SpellMgr.h"
#include "TemporarySummon.h"
#include "Unit.h"
#include "Vehicle.h"

Position const OrgrimmarPortalPos[3] =
{
    { 1358.62f, 1054.72f, 53.1200f, 0.0f },
    { 1393.27f, 1021.20f, 53.2225f, 0.0f },
    { 1404.71f, 1063.73f, 60.5617f, 0.0f }
};

Position const HellscreamElitePos[16] =
{
    { 1387.90f, 1029.71f, 53.21853f, 2.827433f },
    { 1389.79f, 1024.51f, 53.20833f, 2.827433f },
    { 1388.05f, 1026.91f, 53.20833f, 2.827433f },
    { 1388.16f, 1020.88f, 53.25523f, 2.827433f },
    { 1385.79f, 1025.99f, 53.22593f, 2.827433f },
    { 1386.69f, 1023.26f, 53.24393f, 2.827433f },
    { 1384.33f, 1022.04f, 53.28123f, 2.827433f },
    { 1391.10f, 1027.73f, 53.20483f, 2.827433f },

    { 1359.10f, 1046.55f, 52.97053f, 5.253441f },
    { 1360.89f, 1051.81f, 53.19793f, 5.253441f },
    { 1360.75f, 1048.84f, 53.12893f, 5.253441f },
    { 1364.43f, 1053.62f, 53.29343f, 5.253441f },
    { 1363.08f, 1048.15f, 53.22223f, 5.253441f },
    { 1364.08f, 1050.84f, 53.29163f, 5.253441f },
    { 1366.69f, 1050.31f, 53.34203f, 5.253441f },
    { 1357.85f, 1050.12f, 52.99823f, 5.253441f }
};

Position const GarroshPos = { 1402.45f, 1061.62f, 60.56173f, 3.926991f };

Position const GarroshJumpPos = { 1378.65f, 1044.23f, 53.8389f, 5.51524f };

Position const CromushPos = { 1404.71f, 1063.73f, 60.5617f, 2.827433f };

Position const AgathaPreRisePos = { 1364.02f, 1028.54f, 66.99143f };

Position const AgathaRisePos = { 1368.65f, 1032.19f, 63.3033f };

Position const AgathaPreResetPos = { 1364.02f, 1028.54f, 55.9914f };

Position const AgathaResetPos = { 1364.02f, 1028.54f, 58.1319f };

enum QuestTheWarchiefCometh
{
    QUEST_THE_WARCHIEF_COMETH = 26965,

    NPC_LADY_SYLVANAS_WINDRUNNER_COMETH = 44365,
    NPC_AGATHA_COMETH = 44608,
    NPC_GRAND_EXECUTOR_MORTUUS = 44615,
    NPC_MALE_FALLEN_HUMAN = 44592,
    NPC_FEMALE_FALLEN_HUMAN = 44593,
    NPC_PORTAL_FROM_ORGRIMMAR = 44630,
    NPC_GARROSH_HELLSCREAM = 44629,
    NPC_HIGH_WARLORD_CROMUSH_COMETH = 44640,
    NPC_HELLSCREAM_ELITE_COMETH = 44636,
    NPC_QUEST_MONSTER_CREDIT = 44629,
    NPC_FORSAKEN_WARHORSE_UNPHASED = 73595,

    SPELL_RAISE_FORSAKEN_COMETH = 83173,
    SPELL_AIR_REVENANT_ENTRANCE = 55761,
    SPELL_SIMPLE_TELEPORT = 12980,
    SPELL_WELCOME_TO_SILVERPINE_CREDIT = 83384,

    EVENT_START_SCENE_COMETH = 1,
    EVENT_SUMMON_PORTAL_COMETH = 2,
    EVENT_SUMMON_GARROSH_COMETH = 3,
    EVENT_AGATHA_RAISE_FORSAKEN = 4, // Note: 4-8 are used
    EVENT_SCENE_TALK_COMETH = 9, // Note: 9-36 are used

    ACTION_START_SCENE_COMETH = 1,

    TALK_SYLVANAS_COMETH_0 = 0,
    TALK_SYLVANAS_COMETH_1 = 1,
    TALK_SYLVANAS_COMETH_2 = 2,
    TALK_SYLVANAS_COMETH_3 = 3,
    TALK_SYLVANAS_COMETH_4 = 4,
    TALK_SYLVANAS_COMETH_5 = 5,
    TALK_SYLVANAS_COMETH_6 = 6,
    TALK_SYLVANAS_COMETH_7 = 7,
    TALK_SYLVANAS_COMETH_8 = 8,
    TALK_SYLVANAS_COMETH_9 = 9,
    TALK_SYLVANAS_COMETH_10 = 10,
    TALK_GARROSH_COMETH_0 = 0,
    TALK_GARROSH_COMETH_1 = 1,
    TALK_GARROSH_COMETH_2 = 2,
    TALK_GARROSH_COMETH_3 = 3,
    TALK_GARROSH_COMETH_4 = 4,
    TALK_GARROSH_COMETH_5 = 5,
    TALK_GARROSH_COMETH_6 = 6,
    TALK_GARROSH_COMETH_7 = 7,
    TALK_GARROSH_COMETH_8 = 8,
    TALK_GARROSH_COMETH_9 = 9,
    TALK_GARROSH_COMETH_10 = 10,
    TALK_CROMUSH_COMETH_0 = 0,
    TALK_CROMUSH_COMETH_1 = 1,

    PATH_CROMUSH = 446402,
    PATH_GARROSH = 446290,

    POINT_AGATHA_PRE_RISE = 1,
    POINT_AGATHA_RISE = 2,
    POINT_AGATHA_PRE_RESET = 3,
    POINT_AGATHA_RESET = 4,

    ANIMKIT_GENERAL_1 = 609,
    ANIMKIT_SYLV_1 = 595,
    ANIMKIT_SYLV_2 = 606,
    ANIMKIT_GARROSH_1 = 662,
    ANIMKIT_GARROSH_2 = 595
};

class npc_silverpine_grand_executor_mortuus : public CreatureScript
{
public:
    npc_silverpine_grand_executor_mortuus() : CreatureScript("npc_silverpine_grand_executor_mortuus") { }

    struct npc_silverpine_grand_executor_mortuusAI : public ScriptedAI
    {
        npc_silverpine_grand_executor_mortuusAI(Creature* creature) : ScriptedAI(creature), _summons(me), _eventInProgress(false) {}

        void QuestAccept(Player* player, Quest const* quest)
        {
            if (quest->GetQuestId() == QUEST_THE_WARCHIEF_COMETH)
            {
                if (_eventInProgress)
                    return;
                DoAction(ACTION_START_SCENE_COMETH);
            }
        }

        void Reset() override
        {
            _events.Reset();
            _garroshGUID = 0;
            _cromushGUID = 0;
            _sylvanasGUID = 0;
            _agathaGUID = 0;
            _summons.DespawnAll();
            _eventInProgress = false;
        }

        void JustSummoned(Creature* summon) override
        {
            _summons.Summon(summon);

            switch (summon->GetEntry())
            {
            case NPC_GARROSH_HELLSCREAM:
                summon->CastSpell(summon, SPELL_SIMPLE_TELEPORT);
                _garroshGUID = summon->GetGUID();
                break;

            case NPC_HELLSCREAM_ELITE_COMETH:
                summon->CastSpell(summon, SPELL_SIMPLE_TELEPORT);
                break;

            default:
                break;
            }
        }

        void DoAction(int32 param) override
        {
            switch (param)
            {
            case ACTION_START_SCENE_COMETH:
            {
                _eventInProgress = true;

                if (Creature* sylvanas = me->FindNearestCreature(NPC_LADY_SYLVANAS_WINDRUNNER_COMETH, 100.0f))
                {
                    _sylvanasGUID = sylvanas->GetGUID();

                    if (Creature* agatha = me->FindNearestCreature(NPC_AGATHA_COMETH, 100.0f))
                    {
                        _agathaGUID = agatha->GetGUID();

                        _events.ScheduleEvent(EVENT_START_SCENE_COMETH, 250ms);
                    }
                }

                break;
            }

            default:
                break;
            }
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_START_SCENE_COMETH:
                    _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH, 1s);
                    _events.ScheduleEvent(EVENT_SUMMON_PORTAL_COMETH, 4s);
                    _events.ScheduleEvent(EVENT_SUMMON_GARROSH_COMETH, 7s + 500ms);
                    break;

                case EVENT_SUMMON_PORTAL_COMETH:
                    SummonPortalsFromOrgrimmar();
                    break;

                case EVENT_SUMMON_GARROSH_COMETH:
                    SummonGarroshAndHisEliteGuards();
                    break;

                case EVENT_SCENE_TALK_COMETH:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (sylvanas->IsAIEnabled)
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_0);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 1, 4s + 500ms);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 1:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        sylvanas->SetFacingTo(0.808979f);

                        if (sylvanas->IsAIEnabled)
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_1);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 2, 3s + 500ms);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 2:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        garrosh->GetMotionMaster()->MoveJump(GarroshJumpPos, 15.595897f, 15.595897f);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 3, 2s + 500ms);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 3:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        sylvanas->SetFacingTo(3.924652f);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 4, 1s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 4:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                            garrosh->SetFacingToObject(sylvanas);

                        garrosh->PlayOneShotAnimKitId(ANIMKIT_GARROSH_1);

                        if (garrosh->IsAIEnabled)
                            garrosh->AI()->Talk(TALK_GARROSH_COMETH_0);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 5, 12s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 5:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        garrosh->SetFacingTo(3.9444442f);

                        if (garrosh->IsAIEnabled)
                            garrosh->AI()->Talk(TALK_GARROSH_COMETH_1);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 6, 7s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 6:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        sylvanas->SetFacingTo(2.4260077f);

                        if (sylvanas->IsAIEnabled)
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_2);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 7, 5s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 7:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        sylvanas->SetFacingTo(3.7350047f);
                        sylvanas->PlayOneShotAnimKitId(ANIMKIT_SYLV_1);

                        if (sylvanas->IsAIEnabled)
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_3);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 8, 16s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 8:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (sylvanas->IsAIEnabled)
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_4);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 9, 4s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 9:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        if (garrosh->IsAIEnabled)
                            garrosh->AI()->Talk(TALK_GARROSH_COMETH_2);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 10, 3s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 10:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (sylvanas->IsAIEnabled)
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_5);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 11, 6s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 11:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (sylvanas->IsAIEnabled)
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_6);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 12, 6s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 12:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        sylvanas->PlayOneShotAnimKitId(ANIMKIT_SYLV_2);

                        if (sylvanas->IsAIEnabled)
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_7);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 13, 9s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 13:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (sylvanas->IsAIEnabled)
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_8);

                        _events.ScheduleEvent(EVENT_AGATHA_RAISE_FORSAKEN, 3s);
                    }
                    break;
                }

                case EVENT_AGATHA_RAISE_FORSAKEN:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, _agathaGUID))
                    {
                        agatha->GetMotionMaster()->MovePoint(POINT_AGATHA_PRE_RISE, AgathaPreRisePos, false);

                        _events.ScheduleEvent(EVENT_AGATHA_RAISE_FORSAKEN + 1, 2s + 500ms);
                    }
                    break;
                }

                case EVENT_AGATHA_RAISE_FORSAKEN + 1:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, _agathaGUID))
                    {
                        agatha->SetWalk(true);
                        agatha->GetMotionMaster()->MovePoint(POINT_AGATHA_RISE, AgathaRisePos, false);

                        _events.ScheduleEvent(EVENT_AGATHA_RAISE_FORSAKEN + 2, 6s);
                    }
                    break;
                }

                case EVENT_AGATHA_RAISE_FORSAKEN + 2:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, _agathaGUID))
                    {
                        agatha->CastSpell(agatha, SPELL_RAISE_FORSAKEN_COMETH);

                        _events.ScheduleEvent(EVENT_AGATHA_RAISE_FORSAKEN + 3, 10s);
                    }
                    break;
                }

                case EVENT_AGATHA_RAISE_FORSAKEN + 3:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, _agathaGUID))
                    {
                        agatha->GetMotionMaster()->MovePoint(POINT_AGATHA_PRE_RESET, AgathaPreResetPos, false);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 14, 750ms);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 14:
                {
                    if (Creature* cromush = ObjectAccessor::GetCreature(*me, _cromushGUID))
                    {
                        if (cromush->IsAIEnabled)
                            cromush->AI()->Talk(TALK_CROMUSH_COMETH_0);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 15, 3s + 500ms);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 15:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        garrosh->PlayOneShotAnimKitId(ANIMKIT_GENERAL_1);

                        if (garrosh->IsAIEnabled)
                            garrosh->AI()->Talk(TALK_GARROSH_COMETH_3);

                        if (Creature* agatha = ObjectAccessor::GetCreature(*me, _agathaGUID))
                            agatha->SetWalk(false);

                        _events.ScheduleEvent(EVENT_AGATHA_RAISE_FORSAKEN + 4, 12s);
                    }
                    break;
                }

                case EVENT_AGATHA_RAISE_FORSAKEN + 4:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, _agathaGUID))
                    {
                        agatha->GetMotionMaster()->MovePoint(POINT_AGATHA_RESET, AgathaResetPos, false);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 16, 1s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 16:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        sylvanas->PlayOneShotAnimKitId(ANIMKIT_GENERAL_1);

                        if (sylvanas->IsAIEnabled)
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_9);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 17, 10s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 17:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        if (garrosh->IsAIEnabled)
                            garrosh->AI()->Talk(TALK_GARROSH_COMETH_4);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 18, 6s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 18:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        if (garrosh->IsAIEnabled)
                            garrosh->AI()->Talk(TALK_GARROSH_COMETH_5);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 19, 6s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 19:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (sylvanas->IsAIEnabled)
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_10);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 20, 5s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 20:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        garrosh->SetFacingTo(5.51524f);

                        if (garrosh->IsAIEnabled)
                            garrosh->AI()->Talk(TALK_GARROSH_COMETH_6);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 21, 4s + 500ms);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 21:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        if (Creature* cromush = ObjectAccessor::GetCreature(*me, _cromushGUID))
                        {
                            garrosh->SetFacingToObject(cromush);

                            _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 22, 500ms);
                        }
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 22:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        garrosh->PlayOneShotAnimKitId(ANIMKIT_GARROSH_2);

                        if (garrosh->IsAIEnabled)
                            garrosh->AI()->Talk(TALK_GARROSH_COMETH_7);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 23, 14s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 23:
                {
                    if (Creature* cromush = ObjectAccessor::GetCreature(*me, _cromushGUID))
                    {
                        if (cromush->IsAIEnabled)
                            cromush->AI()->Talk(TALK_CROMUSH_COMETH_1);

                        if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                        {
                            cromush->SetFacingToObject(garrosh);

                            _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 24, 2s + 500ms);
                        }
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 24:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        garrosh->SetFacingTo(5.6199603f);
                        garrosh->PlayOneShotAnimKitId(ANIMKIT_GARROSH_2);

                        if (garrosh->IsAIEnabled)
                            garrosh->AI()->Talk(TALK_GARROSH_COMETH_8);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 25, 8s + 500ms);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 25:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        garrosh->CastSpell(garrosh, SPELL_WELCOME_TO_SILVERPINE_CREDIT, true);

                        garrosh->GetMotionMaster()->MovePath(PATH_GARROSH, false);

                        _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 26, 9s);
                    }
                    break;
                }

                case EVENT_SCENE_TALK_COMETH + 26:
                    DespawnGarroshAndHisEliteGuards();
                    _events.ScheduleEvent(EVENT_SCENE_TALK_COMETH + 27, 500ms);
                    break;

                case EVENT_SCENE_TALK_COMETH + 27:
                    _summons.DespawnAll();
                    Reset();
                    break;

                default:
                    break;
                }
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        void SummonPortalsFromOrgrimmar()
        {
            for (Position const& pos : OrgrimmarPortalPos)
                me->SummonCreature(NPC_PORTAL_FROM_ORGRIMMAR, pos, TEMPSUMMON_TIMED_DESPAWN, 300000);

            std::list<Creature*> orgrimmarPortals;
            GetCreatureListWithEntryInGrid(orgrimmarPortals, me, NPC_PORTAL_FROM_ORGRIMMAR, 100.0f);

            for (Creature* portal : orgrimmarPortals)
                portal->CastSpell(portal, SPELL_AIR_REVENANT_ENTRANCE);
        }

        void SummonGarroshAndHisEliteGuards()
        {
            for (Position const& pos : HellscreamElitePos)
                me->SummonCreature(NPC_HELLSCREAM_ELITE_COMETH, pos, TEMPSUMMON_TIMED_DESPAWN, 300000);

            me->SummonCreature(NPC_GARROSH_HELLSCREAM, GarroshPos, TEMPSUMMON_TIMED_DESPAWN, 300000);

            if (Creature* cromush = me->SummonCreature(NPC_HIGH_WARLORD_CROMUSH_COMETH, CromushPos, TEMPSUMMON_TIMED_DESPAWN, 300000))
            {
                _cromushGUID = cromush->GetGUID();

                cromush->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                cromush->GetMotionMaster()->MovePath(PATH_CROMUSH, false);
            }
        }

        void DespawnGarroshAndHisEliteGuards()
        {
            for (uint64 const& summonedCreature : _summons)
            {
                if (Creature* summon = ObjectAccessor::GetCreature(*me, summonedCreature))
                    summon->CastSpell(summon, SPELL_SIMPLE_TELEPORT);
            }
        }

    private:
        EventMap _events;
        uint64 _garroshGUID;
        uint64 _cromushGUID;
        uint64 _sylvanasGUID;
        uint64 _agathaGUID;
        SummonList _summons;
        bool _eventInProgress;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_silverpine_grand_executor_mortuusAI(creature);
    }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (auto ai = CAST_AI(npc_silverpine_grand_executor_mortuusAI, creature->AI()))
            ai->QuestAccept(player, quest);
        return true;
    }
};

enum RaiseForsakenCometh
{
    ACTION_RISE_DURING_RAISE = 1,
    ACTION_DESCEND_AFTER_RAISE = 2,

    POINT_BEING_RISEN = 1,

    ANIMKIT_RESET = 0,
    ANIMKIT_FALLEN_HUMAN = 721
};

// Raise Forsaken - 83173
class spell_silverpine_raise_forsaken : public SpellScriptLoader
{
public:
    spell_silverpine_raise_forsaken() : SpellScriptLoader("spell_silverpine_raise_forsaken_83173"){}

    class spell_silverpine_raise_forsaken_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_silverpine_raise_forsaken_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();

            if (Creature* fallenHuman = target->ToCreature())
            {
                if (fallenHuman->IsAIEnabled)
                    fallenHuman->AI()->DoAction(ACTION_RISE_DURING_RAISE);
            }
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();

            if (Creature* fallenHuman = target->ToCreature())
            {
                if (fallenHuman->IsAIEnabled)
                    fallenHuman->AI()->DoAction(ACTION_DESCEND_AFTER_RAISE);
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_silverpine_raise_forsaken_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_silverpine_raise_forsaken_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_silverpine_raise_forsaken_AuraScript();
    }
};

enum FallenHuman
{
    SPELL_FEIGNED = 80636,
    SPELL_FORSAKEN_TROOPER_MS_COMETH = 83149,

    EVENT_ASCEND = 1,
    EVENT_TRANSFORM_INTO_FORSAKEN = 2,
    EVENT_FACE_TOWARDS_SYLVANAS = 3,
    EVENT_EMOTE_TO_SYLVANAS = 4
};

// Fallen Human - 44592, 44593
struct npc_silverpine_fallen_human : public ScriptedAI
{
    npc_silverpine_fallen_human(Creature* creature) : ScriptedAI(creature), _transformDone(false) {}

    void Reset() override
    {
        _transformDone = false;
        _events.Reset();
    }

    void DoAction(int32 action) override
    {
        switch (action)
        {
        case ACTION_RISE_DURING_RAISE:
            me->SetAIAnimKitId(ANIMKIT_FALLEN_HUMAN);
            _events.ScheduleEvent(EVENT_ASCEND, 1s);
            break;

        case ACTION_DESCEND_AFTER_RAISE:
            me->SetWalk(false);
            me->SetAIAnimKitId(ANIMKIT_RESET);
            me->GetMotionMaster()->MoveFall();
            _events.ScheduleEvent(EVENT_TRANSFORM_INTO_FORSAKEN, 1s);
            break;

        default:
            break;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_ASCEND:
                me->SetWalk(true);
                me->GetMotionMaster()->MovePoint(POINT_BEING_RISEN, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 3.5f, false);
                break;

            case EVENT_TRANSFORM_INTO_FORSAKEN:
            {
                if (_transformDone)
                    return;

                DoCastSelf(SPELL_FORSAKEN_TROOPER_MS_COMETH);

                _transformDone = true;

                _events.ScheduleEvent(EVENT_FACE_TOWARDS_SYLVANAS, 1s + 500ms);
                break;
            }

            case EVENT_FACE_TOWARDS_SYLVANAS:
                me->SetFacingTo(0.706837f);
                _events.ScheduleEvent(EVENT_EMOTE_TO_SYLVANAS, 2s + 500ms);
                break;

            case EVENT_EMOTE_TO_SYLVANAS:
                me->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
                me->DespawnOrUnsummon(80000);
                break;

            default:
                break;
            }
        }
    }

private:
    EventMap _events;
    bool _transformDone;
};

enum SpellForsakenTrooperMasterScriptCometh
{
    SPELL_FORSAKEN_TROOPER_MALE_01_HC = 83150,
    SPELL_FORSAKEN_TROOPER_MALE_02_HC = 83163,
    SPELL_FORSAKEN_TROOPER_MALE_03_HC = 83164,
    SPELL_FORSAKEN_TROOPER_MALE_04_HC = 83165,
    SPELL_FORSAKEN_TROOPER_FEMALE_01_HC = 83152,
    SPELL_FORSAKEN_TROOPER_FEMALE_02_HC = 83166,
    SPELL_FORSAKEN_TROOPER_FEMALE_03_HC = 83167,
    SPELL_FORSAKEN_TROOPER_FEMALE_04_HC = 83168,

    DISPLAY_MALE_01_HC = 33978,
    DISPLAY_MALE_02_HC = 33980,
    DISPLAY_MALE_03_HC = 33979,
    DISPLAY_MALE_04_HC = 33981,
    DISPLAY_FEMALE_01_HC = 33982,
    DISPLAY_FEMALE_02_HC = 33983,
    DISPLAY_FEMALE_03_HC = 33984,
    DISPLAY_FEMALE_04_HC = 33985
};

// Forsaken Trooper Master Script (Forsaken High Command) - 83149
class spell_silverpine_forsaken_trooper_masterscript_high_command : public SpellScriptLoader
{
public:
    spell_silverpine_forsaken_trooper_masterscript_high_command() : SpellScriptLoader("spell_silverpine_forsaken_trooper_masterscript_high_command") { }

    class spell_silverpine_forsaken_trooper_masterscript_high_command_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_silverpine_forsaken_trooper_masterscript_high_command_SpellScript);

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_FORSAKEN_TROOPER_MALE_01_HC))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_FORSAKEN_TROOPER_MALE_02_HC))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_FORSAKEN_TROOPER_MALE_03_HC))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_FORSAKEN_TROOPER_MALE_04_HC))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_FORSAKEN_TROOPER_FEMALE_01_HC))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_FORSAKEN_TROOPER_FEMALE_02_HC))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_FORSAKEN_TROOPER_FEMALE_03_HC))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_FORSAKEN_TROOPER_FEMALE_04_HC))
                return false;

            return true;
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
            {
                caster->RemoveAura(SPELL_FEIGNED);

                uint32 spellId = SPELL_FORSAKEN_TROOPER_MALE_01_HC;
                switch (caster->GetDisplayId())
                {
                case DISPLAY_MALE_01_HC:
                    spellId = SPELL_FORSAKEN_TROOPER_MALE_01_HC;
                    break;
                case DISPLAY_MALE_02_HC:
                    spellId = SPELL_FORSAKEN_TROOPER_MALE_02_HC;
                    break;
                case DISPLAY_MALE_03_HC:
                    spellId = SPELL_FORSAKEN_TROOPER_MALE_03_HC;
                    break;
                case DISPLAY_MALE_04_HC:
                    spellId = SPELL_FORSAKEN_TROOPER_MALE_04_HC;
                    break;
                case DISPLAY_FEMALE_01_HC:
                    spellId = SPELL_FORSAKEN_TROOPER_FEMALE_01_HC;
                    break;
                case DISPLAY_FEMALE_02_HC:
                    spellId = SPELL_FORSAKEN_TROOPER_FEMALE_02_HC;
                    break;
                case DISPLAY_FEMALE_03_HC:
                    spellId = SPELL_FORSAKEN_TROOPER_FEMALE_03_HC;
                    break;
                case DISPLAY_FEMALE_04_HC:
                    spellId = SPELL_FORSAKEN_TROOPER_FEMALE_04_HC;
                    break;
                default:
                    break;
                }
                caster->CastSpell(caster, spellId, true);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_silverpine_forsaken_trooper_masterscript_high_command_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_silverpine_forsaken_trooper_masterscript_high_command_SpellScript();
    }
};

/*######
## npc_deathstalker_erland
######*/

enum eErland
{
    SAY_QUESTACCEPT     = 0,
    SAY_START           = 1,
    SAY_AGGRO           = 2,
    SAY_PROGRESS        = 3,
    SAY_LAST            = 4,

    SAY_RANE            = 0,
    SAY_RANE_ANSWER     = 5,
    SAY_MOVE_QUINN      = 6,

    SAY_QUINN           = 7,
    SAY_QUINN_ANSWER    = 0,
    SAY_BYE             = 8,

    QUEST_ESCORTING     = 435,
    NPC_RANE            = 1950,
    NPC_QUINN           = 1951
};

class npc_deathstalker_erland : public CreatureScript
{
public:
    npc_deathstalker_erland() : CreatureScript("npc_deathstalker_erland") { }

    struct npc_deathstalker_erlandAI : public npc_escortAI
    {
        npc_deathstalker_erlandAI(Creature* creature) : npc_escortAI(creature) { }

        void WaypointReached(uint32 waypointId)
        {
            Player* player = GetPlayerForEscort();
            if (!player)
                return;

            switch (waypointId)
            {
                case 1:
                    Talk(SAY_START, player);
                    break;
                case 10:
                    Talk(SAY_PROGRESS);
                    break;
                case 13:
                    Talk(SAY_LAST, player);
                    player->GroupEventHappens(QUEST_ESCORTING, me);
                    break;
                case 15:
                    if (Creature* rane = me->FindNearestCreature(NPC_RANE, 20.0f))
                        rane->AI()->Talk(SAY_RANE);
                    break;
                case 16:
                    Talk(SAY_RANE_ANSWER);
                    break;
                case 17:
                    Talk(SAY_MOVE_QUINN);
                    break;
                case 24:
                    Talk(SAY_QUINN);
                    break;
                case 25:
                    if (Creature* quinn = me->FindNearestCreature(NPC_QUINN, 20.0f))
                        quinn->AI()->Talk(SAY_QUINN_ANSWER);
                    break;
                case 26:
                    Talk(SAY_BYE);
                    break;
            }
        }

        void Reset() override { }

        void EnterCombat(Unit* who)
        {
            Talk(SAY_AGGRO, who);
        }
    };

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST_ESCORTING)
        {
            creature->AI()->Talk(SAY_QUESTACCEPT, player);

            if (npc_escortAI* pEscortAI = CAST_AI(npc_deathstalker_erland::npc_deathstalker_erlandAI, creature->AI()))
                pEscortAI->Start(true, false, player->GetGUID());
        }

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_deathstalker_erlandAI(creature);
    }
};

enum FenwickThatros
{
    SAY_FENWICK_AGGRO          = 0,

    SPELL_DRINK_HEALING_POTION = 54633,
    SPELL_POTION_OF_STRENGTH   = 80471,
    SPELL_ELIXIR_OF_AGILITY    = 80472,
    SPELL_TONIC_OF_HEALTH      = 80473,

    EVENT_BUFF_STRENGTH        = 1,
    EVENT_BUFF_AGILITY         = 2,
    EVENT_BUFF_STAMINA         = 3,
    EVENT_HEALING              = 4
};

struct npc_fenwick_thatros : public ScriptedAI
{
    npc_fenwick_thatros(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        half_hp_reached_once = false;
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) override
    {
        Talk(SAY_FENWICK_AGGRO);
        events.ScheduleEvent(EVENT_BUFF_STRENGTH, 3000);
    }

    void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override
    {
        if (!half_hp_reached_once && !HealthAbovePct(50))
        {
            half_hp_reached_once = true;
            me->CastSpell(me, SPELL_DRINK_HEALING_POTION);
            events.ScheduleEvent(EVENT_HEALING, 10000);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);
        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_BUFF_STRENGTH:
                    me->CastSpell(me, SPELL_POTION_OF_STRENGTH);
                    events.ScheduleEvent(EVENT_BUFF_AGILITY, 3000);
                    break;
                case EVENT_BUFF_AGILITY:
                    me->CastSpell(me, SPELL_ELIXIR_OF_AGILITY);
                    events.ScheduleEvent(EVENT_BUFF_STAMINA, 3000);
                    break;
                case EVENT_BUFF_STAMINA:
                    me->CastSpell(me, SPELL_TONIC_OF_HEALTH);
                    break;
                case EVENT_HEALING:
                    if (!HealthAbovePct(50))
                    {
                        me->CastSpell(me, SPELL_DRINK_HEALING_POTION);
                        events.ScheduleEvent(EVENT_HEALING, 10000);
                    }
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
private:
    EventMap events;
    bool half_hp_reached_once = false;
};

enum ForsakenBat
{
    TALK_BAT_ARRIVED_TO_ISLE = 0,
    TALK_BAT_GOING_HOME = 1,

    SPELL_BLIGHT_CONCOCTION = 83573,

    SPELL_GO_HOME = 83594,
    QUEST_ITERATING_UPON_SUCCESS = 26998,
    NPC_BAT_HANDLER_MAGGOTBREATH = 44825,
    NPC_FORSAKEN_BAT = 44821,

    ACTION_GO_HOME = 1,

    NPC_VILE_FIN_ORACLE = 1908,

    DATA_ITERATING_UPON_SUCCESS_REQ = 50
};

enum BatStages
{
    STAGE_ARRIVE = 0,
    STAGE_CIRCLE = 1,
    STAGE_GO_HOME = 2
};

enum BatPoints
{
    POINT_CIRCLE_END,
    POINT_HOME_END
};

enum BatEvents
{
    EVENT_CHECK_QUEST_DONE
};

enum BatFlightPoints
{
    POINT_ARRIVE_ISLE = 0,
    POINT_HOME = 1,
    POINT_FLIGHT_0 = 2000
};

static const Position FlightPath[] =
{
    {1214.8f,   414.792f,  61.0968f},
    {1221.35f,  323.231f,  61.0968f},
    {1189.93f,  281.372f,  61.0968f},
    {1100.24f,  212.868f,  61.0968f},
    {1015.84f,  188.21f,   63.0412f},
    {939.417f,  163.743f,  61.0968f},
    {874.302f,  114.092f,  61.0968f},
    {831.491f,  59.3177f,  56.5968f},
    {770.872f,  70.7726f,  56.5968f},
    {759.814f,  109.738f,  56.5968f},
    {803.745f,  181.882f,  56.5968f},
    {840.674f,  202.097f,  56.5968f},
    {846.651f,  232.344f,  56.5968f},
    {805.031f,  261.606f,  59.9579f},
    {759.528f,  265.283f,  59.9579f},
    {693.755f,  282.512f,  59.9579f},
    {658.036f,  357.233f,  59.9579f},
    {639.776f,  433.974f,  59.9579f},
    {676.128f,  470.752f,  59.9579f},
    {706.495f,  454.023f,  59.9579f},
    {713.505f,  380.88f,   59.9579f},
    {726.146f,  352.188f,  59.9579f},
    {758.299f,  321.101f,  59.9579f},
    {797.684f,  295.753f,  58.1801f},
    {882.036f,  252.597f,  52.1245f},
    {929.667f,  225.609f,  52.1245f},
    {987.259f,  219.582f,  52.1245f},
    {1030.56f,  276.193f,  52.1245f},
    {1058.35f,  308.229f,  49.3745f},
    {1121.81f,  372.755f,  56.4579f},
    {1167.09f,  438.865f,  58.4857f},
    {1197.37f,  451.757f,  64.569f},
    {1205.51f,  425.464f,  62.9479f},
    {931.068f,  352.101f,  81.027f},
    {952.535f,  571.493f,  108.303f},
    {970.028f,  664.392f,  110.83f},
    {1031.97f,  754.155f,  111.441f},
    {1234.44f,  880.16f,  97.9414f},
    {1345.66f,  970.281f,  75.1914f},
    {1415.9f,   1018.08f,  55.3025f},
    {1374.36f,  1029.12f,  67.8815f},
    {1289.08f,  1013.22f,  67.8815f},
    {1224.0f,   997.236f,  59.9371f},
    {1178.72f,  957.238f,  47.2426f},
    {1156.5f,   852.318f,  35.7704f},
    {1158.1f,   725.983f,  35.7704f},
    {1187.67f,  575.212f,  35.7704f},
    {1202.33f,  472.97f,   65.4648f}
};
static constexpr uint32 FLIGHT_POINT_COUNT = 48;

static const Position HomePos = { 1214.8f, 414.792f, 61.0968f };

// Forsaken Bat - 44821
struct npc_silverpine_forsaken_bat : public VehicleAI
{
    npc_silverpine_forsaken_bat(Creature* creature) : VehicleAI(creature) { Initialize(); }

    void Initialize()
    {
        SetupInitialActionBar();
        _stage = STAGE_ARRIVE;
        me->SetCanFly(true);
        me->SetDisableGravity(true);
    }

    void StartCircleWaypoint()
    {
        std::vector<Position> wp;
        wp.reserve(FLIGHT_POINT_COUNT + 1);

        for (uint32 i = 0; i < FLIGHT_POINT_COUNT; ++i)
        {
            Position p = FlightPath[i];
            p.m_positionZ += 5.0f;
            wp.push_back(p);
        }
        wp.push_back(wp.front());

        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MoveSplinePath(wp.data(), uint32(wp.size()), true, false, 0.0f, true);
    }

    void MoveToNextCirclePoint()
    {
        if (_stage == STAGE_GO_HOME)
            return;
        if (_flightIndex >= FLIGHT_POINT_COUNT)
            _flightIndex = 0;

        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MovePoint(POINT_FLIGHT_0 + _flightIndex, FlightPath[_flightIndex], false);
        _curPathIndex = _flightIndex;
        ++_flightIndex;
    }

    void PassengerBoarded(Unit* passenger, int8 /*seat*/, bool apply) override
    {
        if (apply)
        {
            if (Player* player = passenger->ToPlayer())
            {
                _playerGUID = player->GetGUID();
                if (player->GetQuestStatus(QUEST_ITERATING_UPON_SUCCESS) == QUEST_STATUS_INCOMPLETE)
                {
                    player->KilledMonsterCredit(NPC_BAT_HANDLER_MAGGOTBREATH);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->GetMotionMaster()->MovePoint(POINT_ARRIVE_ISLE, 1195.644f, 456.1104f, 93.333f, false);
                    _events.ScheduleEvent(EVENT_CHECK_QUEST_DONE, 500ms);
                }
            }
        }
        else
        {
            if (passenger->GetTypeId() == TYPEID_PLAYER && _stage != STAGE_GO_HOME)
                DoAction(ACTION_GO_HOME);
        }
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        MoveToNextCirclePoint();
        if (type != POINT_MOTION_TYPE)
            return;

        if (_stage == STAGE_GO_HOME && id != POINT_HOME_END)
            return;

        switch (id)
        {
        case POINT_ARRIVE_ISLE:
            _flightIndex = 0;
            if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
            {
                Talk(TALK_BAT_ARRIVED_TO_ISLE, player);
                player->VehicleSpellInitialize();
            }
            StartCircleWaypoint();
            _events.ScheduleEvent(EVENT_CHECK_QUEST_DONE, 500ms);
            break;

        case POINT_HOME_END:
            if (me->GetVehicleKit())
                me->GetVehicleKit()->RemoveAllPassengers();
            me->DespawnOrUnsummon();
            break;
        }

        if (id >= POINT_FLIGHT_0 && id < POINT_FLIGHT_0 + FLIGHT_POINT_COUNT)
            MoveToNextCirclePoint();
    }

    void UpdateAI(uint32 diff) override
    {
        VehicleAI::UpdateAI(diff);
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_CHECK_QUEST_DONE:
            {
                if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                {
                    if (player->GetReqKillOrCastCurrentCount(QUEST_ITERATING_UPON_SUCCESS, NPC_VILE_FIN_ORACLE) >= DATA_ITERATING_UPON_SUCCESS_REQ)
                    {
                        player->CastSpell(me, SPELL_GO_HOME, true);
                        return;
                    }
                }
                _events.ScheduleEvent(EVENT_CHECK_QUEST_DONE, 500ms);
                break;
            }
            }
        }
    }

    void DoAction(int32 param) override
    {
        if (param == ACTION_GO_HOME && _stage != STAGE_GO_HOME)
        {
            if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
            {
                _stage = STAGE_GO_HOME;
                _events.CancelEvent(EVENT_CHECK_QUEST_DONE);
                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MovePoint(POINT_HOME_END, 1415.9f, 1018.08f, 55.3025f, false);
                SetupFinishActionBar();
                player->VehicleSpellInitialize();
                Talk(TALK_BAT_GOING_HOME, player);
            }
        }
    }

    void SetupInitialActionBar() { me->m_spells[0] = SPELL_BLIGHT_CONCOCTION; me->m_spells[1] = 0; }
    void SetupFinishActionBar() { me->m_spells[0] = 0; me->m_spells[1] = 0; }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    uint8 _stage;
    uint8 _flightIndex = 0;
    uint8  _curPathIndex = 0;
};

// Go Home - 83594
class spell_silverpine_go_home : public SpellScriptLoader
{
public:
    spell_silverpine_go_home() : SpellScriptLoader("spell_silverpine_go_home") { }

    class spell_silverpine_go_home_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_silverpine_go_home_SpellScript);

        void HandleDummyEffect(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* hit = GetHitUnit();
            if (!caster || !hit) return;

            if (caster->GetTypeId() == TYPEID_PLAYER && hit->GetEntry() == NPC_FORSAKEN_BAT && hit->IsAIEnabled)
                hit->GetAI()->DoAction(ACTION_GO_HOME);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_silverpine_go_home_SpellScript::HandleDummyEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };
    SpellScript* GetSpellScript() const override
    {
        return new spell_silverpine_go_home_SpellScript();
    }
};

// 44951  quest 27097
class npc_agatha_44951 : public CreatureScript
{
public:
    npc_agatha_44951() : CreatureScript("npc_agatha_44951") { }

    enum eNPC
    {
        NPC_HILLSBRAD_REFUGEE1 = 44954,
        NPC_HILLSBRAD_REFUGEE2 = 44966,
        NPC_FORSAKEN = 44959,
        QUEST_RISE_FORSAKEN = 27097,
        QUEST_NO_ESCAPE = 27099,
        SPELL_DEATH_WALK = 85451,
        SPELL_NOTIFY_AGATHA = 83990,
        SPELL_RISE_FORSAKEN = 83993,
        SPELL_KILL_CREDIT = 83996,
        SPELL_DOOMHOWL = 84012,
        SPELL_UNHOLY_DARKNESS = 84013,
        SPELL_UNHOLY_SMITE = 84014,
        EVENT_CHAT_TO_PLAYER = 100,
        EVENT_CHECK_PLAYER,
        EVENT_CAST_AGGRO,
        EVENT_HEAL_COOLDOWN,
    };

    struct npc_agatha_44951AI : public VehicleAI
    {
        npc_agatha_44951AI(Creature* creature) : VehicleAI(creature) { Initialize(); }

        EventMap m_events;
        uint64 m_playerGUID;
        uint64 m_targetGUID;
        bool   m_doomhoulDone;
        bool   m_healCD;

        void Initialize()
        {
            m_playerGUID = NULL;
            m_targetGUID = NULL;
            m_doomhoulDone = false;
            m_events.Reset();
            m_events.ScheduleEvent(EVENT_CHAT_TO_PLAYER, 60000);
            m_events.ScheduleEvent(EVENT_CHECK_PLAYER, 2000);
            m_healCD = false;
        }

        void Reset() override
        {
            me->SetDisableGravity(true);
            me->SetWaterWalking(true);
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Player* player = summoner->ToPlayer())
            {
                m_playerGUID = player->GetGUID();
                me->SetFacingToObject(player);
                me->GetMotionMaster()->MoveFollow(player, 3.0f, M_PI);
                me->SetReactState(REACT_PASSIVE);
                me->CastSpell(me, SPELL_DEATH_WALK, true);
                me->AddAura(SPELL_DEATH_WALK, player);
            }
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            if ((caster->GetEntry() == NPC_HILLSBRAD_REFUGEE1 || caster->GetEntry() == NPC_HILLSBRAD_REFUGEE2) && spell->Id == SPELL_NOTIFY_AGATHA)
            {
                me->CastSpell(caster, SPELL_RISE_FORSAKEN, true);
                if (Player* player = sObjectAccessor->GetPlayer(*me, m_playerGUID))
                    player->CastSpell(player, SPELL_KILL_CREDIT);
            }
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (attacker->GetGUID() != m_targetGUID)
                if (Player* player = sObjectAccessor->GetPlayer(*me, m_playerGUID))
                    if (Unit* unit = player->GetVictim())
                        if (unit->GetGUID() != m_targetGUID)
                            EnterCombat(unit);
        }

        void EnterCombat(Unit* victim) override
        {
            m_doomhoulDone = false;
            m_targetGUID = victim->GetGUID();
            me->CastSpell(victim, SPELL_UNHOLY_SMITE);
            m_events.RescheduleEvent(EVENT_CAST_AGGRO, urand(1000, 4000));
        }

        void EnterEvadeMode() override { }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHAT_TO_PLAYER:
                {
                    if (Player* player = sObjectAccessor->GetPlayer(*me, m_playerGUID))
                        me->AI()->Talk(0);
                    m_events.ScheduleEvent(EVENT_CHAT_TO_PLAYER, 60000);
                    break;
                }
                case EVENT_CHECK_PLAYER:
                {
                    if (Player* player = sObjectAccessor->GetPlayer(*me, m_playerGUID))
                        if (player->IsAlive() || player->IsInWorld())
                            if (CheckQuestStatus(player))
                            {
                                if (!m_healCD && player->GetHealthPct() < 90.0f)
                                {
                                    me->CastSpell(player, SPELL_UNHOLY_DARKNESS);
                                    m_healCD = true;
                                    m_events.ScheduleEvent(EVENT_HEAL_COOLDOWN, 2000);
                                }

                                if (Unit* unit = player->GetVictim())
                                {
                                    if (unit->GetGUID() != m_targetGUID)
                                        EnterCombat(unit);
                                }
                                else if (player->IsInCombat())
                                {
                                    if (Unit* unit = player->GetSelectedUnit())
                                        if (unit->GetGUID() != m_targetGUID)
                                            EnterCombat(unit);
                                }

                                m_events.ScheduleEvent(EVENT_CHECK_PLAYER, 1000);
                                break;
                            }

                    me->DespawnOrUnsummon(10);
                    break;
                }
                case EVENT_CAST_AGGRO:
                {
                    if (Creature* target = sObjectAccessor->GetCreature(*me, m_targetGUID))
                        if (target->IsAlive())
                        {
                            if (target->GetEntry() == NPC_HILLSBRAD_REFUGEE1 || target->GetEntry() == NPC_HILLSBRAD_REFUGEE2)
                                if (!m_doomhoulDone)
                                {
                                    m_doomhoulDone = true;
                                    me->CastSpell(me, SPELL_DOOMHOWL);
                                    m_events.ScheduleEvent(EVENT_CAST_AGGRO, urand(2000, 3000));
                                    break;
                                }

                            me->CastSpell(target, SPELL_UNHOLY_SMITE);
                            m_events.ScheduleEvent(EVENT_CAST_AGGRO, urand(1000, 6000));
                        }
                    break;
                }
                case EVENT_HEAL_COOLDOWN:
                {
                    m_healCD = false;
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

        bool CheckQuestStatus(Player* player)
        {
            if (player->GetQuestStatus(QUEST_RISE_FORSAKEN) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_RISE_FORSAKEN) == QUEST_STATUS_COMPLETE)
                return true;
            else if (player->GetQuestStatus(QUEST_NO_ESCAPE) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_NO_ESCAPE) == QUEST_STATUS_COMPLETE)
                return true;

            return false;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_agatha_44951AI(creature);
    }
};

// 44954 //44966
class npc_hillsbrad_refugee_44966 : public CreatureScript
{
public:
    npc_hillsbrad_refugee_44966() : CreatureScript("npc_hillsbrad_refugee_44966") { }

    enum eNPC
    {
        NPC_AGATHA = 44951,
        SPELL_NOTIFY_AGATHA = 83990,
        SPELL_DOOMHOWL = 84012,
        SPELL_UNHOLY_SMITE = 84014,
        QUEST_RISE_FORSAKEN = 27097,
    };

    struct npc_hillsbrad_refugee_44966AI : public ScriptedAI
    {
        npc_hillsbrad_refugee_44966AI(Creature* creature) : ScriptedAI(creature) { }

        void JustSummoned(Creature* summon) override
        {
            uint32 rol = urand(0, 100);
            if (rol < 50)
            {
                summon->AI()->Talk(0);
                summon->DespawnOrUnsummon(10000);
            }
        }

        void JustDied(Unit* killer) override
        {
            if (Creature* agatha = me->FindNearestCreature(NPC_AGATHA, 30.0f))
            {
                me->CastSpell(agatha, SPELL_NOTIFY_AGATHA);
                uint32 m_forsakenSpell;
                if (me->GetGender() == GENDER_MALE)
                    m_forsakenSpell = RAND(83998, 83999, 84000, 84001);
                else
                    m_forsakenSpell = RAND(84002, 84003, 84004, 84005);
                me->CastSpell(me, m_forsakenSpell);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_hillsbrad_refugee_44966AI(creature);
    }
};

class npc_camera_waiting : public CreatureScript
{
public:
    npc_camera_waiting() : CreatureScript("npc_camera_waiting") { }

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new npc_camera_waitingAI(pCreature);
    }

    struct npc_camera_waitingAI : public ScriptedAI
    {
        npc_camera_waitingAI(Creature* c) : ScriptedAI(c) { }

        bool Cinematic;
        bool bSummoned;
        bool bSummoned2;
        uint8 Phase;
        uint32 CinematicTimer;
        uint64 DariusGUID;
        uint64 IvarGUID;
        uint64 TraitorGUID;
        uint64 PlayerGUID;

        void Reset() override
        {
            CinematicTimer = 5000;
            Cinematic = false;
            Phase = 0;
            DariusGUID = 0;
            IvarGUID = 0;
            TraitorGUID = 0;
            bSummoned = false;
            bSummoned2 = false;
        }

        void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply) override
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
            {
                if (apply)
                {
                    me->NearTeleportTo(1314.926880f, 1210.922729f, 58.504990f, 0.0f);
                    PlayerGUID = who->GetGUID();

                    Cinematic = true;
                }

                if (!apply)
                {
                    if (Creature* Darius = me->GetMap()->GetCreature(DariusGUID))
                        if (Creature* Ivar = me->GetMap()->GetCreature(IvarGUID))
                        {
                            Darius->DespawnOrUnsummon(1);
                            Ivar->DespawnOrUnsummon(1);
                        }

                    if (Creature* Traitor = me->GetMap()->GetCreature(TraitorGUID))
                    {
                        Traitor->DespawnOrUnsummon(1);
                    }
                }
            }
        }

        void SummonWorgens()
        {
            if (!bSummoned)
            {
                if (Creature* Ivar = me->SummonCreature(44884, 1302.08f, 1206.31f, 58.49f, 6.25f, TEMPSUMMON_MANUAL_DESPAWN, 0))
                    if (Creature* Darius = me->SummonCreature(44883, 1302.08f, 1206.31f, 58.49f, 6.25f, TEMPSUMMON_MANUAL_DESPAWN, 0))
                    {
                        DariusGUID = Darius->GetGUID();
                        IvarGUID = Ivar->GetGUID();
                        Darius->SetSpecialInvisibility(SPECIAL_VISIBILITY_PLAYER, PlayerGUID);
                        Ivar->SetSpecialInvisibility(SPECIAL_VISIBILITY_PLAYER, PlayerGUID);

                        Darius->GetMotionMaster()->MovePoint(0, 1313.26f, 1206.01f, 58.51f);
                        Ivar->GetMotionMaster()->MovePoint(1, 1308.82f, 1206.14f, 58.51f);
                        bSummoned = true;
                    }
            }
        }

        void SummonTraitor()
        {
            if (!bSummoned2)
            {
                if (Creature* Traitor = me->SummonCreature(79999, 1313.72f, 1212.17f, 58.49f, 4.68f, TEMPSUMMON_MANUAL_DESPAWN, 0))
                {
                    TraitorGUID = Traitor->GetGUID();
                    Traitor->SetSpecialInvisibility(SPECIAL_VISIBILITY_PLAYER, PlayerGUID);
                    bSummoned2 = true;
                }
            }
        }

        void DespawnTraitor()
        {
            std::list<Creature*> pCreatureList;
            Trinity::AllCreaturesOfEntryInRange checker(me, 44882, 20);
            Trinity::CreatureListSearcher<Trinity::AllCreaturesOfEntryInRange> searcher(me, pCreatureList, checker);
            me->VisitNearbyObject(20, searcher);

            for (std::list<Creature*>::iterator iter = pCreatureList.begin(); iter != pCreatureList.end(); ++iter)
            {
                if (Unit* summoner = (*iter)->GetTempSummoner())
                {
                    ObjectGuid pguid = summoner->GetGUID();
                    if (PlayerGUID == pguid)
                    {
                        (*iter)->DespawnOrUnsummon(1);
                        break;
                    }
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (Cinematic)
            {
                SummonWorgens();

                if (CinematicTimer <= diff)
                {
                    if (Creature* Darius = me->GetMap()->GetCreature(DariusGUID))
                        if (Creature* Ivar = me->GetMap()->GetCreature(IvarGUID))
                        {
                            switch (Phase)
                            {
                            case 0: Darius->SetFacingTo(3.05f); me->SetFacingTo(5.6f); CinematicTimer = 5000; ++Phase; break;
                            case 1: DespawnTraitor(); SummonTraitor(); Darius->AI()->Talk(0); CinematicTimer = 7000; ++Phase; break;
                            case 2: me->SetFacingTo(5.4f); Ivar->AI()->Talk(0); CinematicTimer = 9000; ++Phase; break;
                            case 3: me->SetFacingTo(5.6f); Darius->AI()->Talk(1); CinematicTimer = 8000; ++Phase; break;
                            case 4: Darius->AI()->Talk(2); CinematicTimer = 9000; ++Phase; break;
                            case 5: me->SetFacingTo(5.4f); Ivar->AI()->Talk(1); CinematicTimer = 4000; ++Phase; break;
                            case 6: Ivar->AI()->Talk(2); CinematicTimer = 2000; ++Phase; break;
                            case 7: Ivar->AI()->Talk(3); CinematicTimer = 7000; ++Phase; break;
                            case 8: me->SetFacingTo(5.6f); Darius->AI()->Talk(3); CinematicTimer = 6000; ++Phase; break;
                            case 9: me->SetFacingTo(5.4f); Ivar->AI()->Talk(4); CinematicTimer = 5000; ++Phase; break;
                            case 10: Ivar->GetMotionMaster()->MovePoint(0, 1311.8f, 1208.56f, 58.51f); CinematicTimer = 2500; Phase = 23; break;
                            case 23: Ivar->AI()->Talk(5); CinematicTimer = 4000; Phase = 11; break;
                            case 11: Ivar->AI()->Talk(6); CinematicTimer = 6000; ++Phase; break;
                            case 12: Ivar->AI()->Talk(7); CinematicTimer = 4000; ++Phase; break;
                            case 13:
                            {
                                if (Creature* Traitor = me->FindNearestCreature(79999, 20.0f, true))
                                {
                                    Traitor->EnterVehicle(Ivar, 0);
                                    TraitorGUID = Traitor->GetGUID();
                                }
                                CinematicTimer = 1500;
                                ++Phase;
                                break;
                            }
                            case 14:
                            {
                                Ivar->GetMotionMaster()->MovePoint(2, 1310.82f, 1206.05f, 58.51f);
                                CinematicTimer = 2500;
                                ++Phase;
                                break;
                            }
                            case 15: Ivar->SetFacingTo(3.06f); Ivar->AI()->Talk(8); CinematicTimer = 4000; ++Phase; break;
                            case 16:
                            {
                                if (Creature* Traitor = me->GetMap()->GetCreature(TraitorGUID))
                                {
                                    Traitor->AI()->Talk(2);
                                }
                                CinematicTimer = 6000;
                                ++Phase;
                            }break;
                            case 17:
                            {
                                if (Creature* Traitor = me->GetMap()->GetCreature(TraitorGUID))
                                {
                                    Ivar->AI()->Talk(9);
                                    Traitor->ExitVehicle();
                                    Traitor->Kill(Traitor, false);
                                    Traitor->DespawnOrUnsummon(5000);
                                }
                                CinematicTimer = 3000;
                                ++Phase;
                            }break;
                            case 18:
                            {
                                Ivar->SetFacingToObject(Darius);
                                Ivar->AI()->Talk(10);
                                Ivar->HandleEmoteCommand(66);
                                CinematicTimer = 7000;
                                ++Phase;
                            }break;
                            case 19: Ivar->GetMotionMaster()->MovePoint(3, 1298.61f, 1206.63f, 58.58f); CinematicTimer = 3000; ++Phase; break;
                            case 20: Darius->GetMotionMaster()->MovePoint(4, 1305.43f, 1206.1f, 58.5f); CinematicTimer = 3000; ++Phase; break;
                            case 21:
                            {
                                if (Player* player = me->GetMap()->GetPlayer(PlayerGUID))
                                    player->KilledMonsterCredit(44882);

                                CinematicTimer = 1000;
                                ++Phase;
                            }break;
                            case 22:
                            {
                                Ivar->DespawnOrUnsummon(1);
                                Darius->DespawnOrUnsummon(1);
                                if (Creature* Traitor = me->GetMap()->GetCreature(TraitorGUID))
                                {
                                    Traitor->DespawnOrUnsummon(1);
                                }
                                me->DespawnOrUnsummon(1);
                                return;
                            }
                            default: break;
                            }
                        }

                }
                else CinematicTimer -= diff;
            }
        }
    };
};

class npc_armoire : public CreatureScript
{
public:

    npc_armoire() : CreatureScript("npc_armoire") { }

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new npc_armoireAI(pCreature);
    }

    struct npc_armoireAI : public ScriptedAI
    {
        npc_armoireAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 CameraTimer;

        void Reset() override
        {
            CameraTimer = 3000;
        }

        void UpdateAI(uint32 diff) override
        {
            if (CameraTimer <= diff)
            {
                CameraTimer = 1000;
                if (Creature* Camera = me->FindNearestCreature(44893, 10.0f, true))
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                else
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }
            else CameraTimer -= diff;
        }
    };
};

class npc_deathstalker_rane_yorick : public CreatureScript
{
public:
    npc_deathstalker_rane_yorick() : CreatureScript("npc_deathstalker_rane_yorick") { }


    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_deathstalker_rane_yorickAI(creature);
    }

    struct npc_deathstalker_rane_yorickAI : public ScriptedAI
    {
        npc_deathstalker_rane_yorickAI(Creature* c) : ScriptedAI(c) { }

        ObjectGuid summonerGUID;
        ObjectGuid PlayerGUID;
        uint32 Timer;
        bool powiedz;

        void Reset() override
        {
            if (Unit* summoner = me->GetTempSummoner())
            {
                PlayerGUID = summoner->GetGUID();
                me->SetSpecialInvisibility(SPECIAL_VISIBILITY_PLAYER, PlayerGUID);
                Timer = 500;
                powiedz = true;
            }
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == WAYPOINT_MOTION_TYPE && id == 13)
                Talk(1);

            if (type == WAYPOINT_MOTION_TYPE && id == 14)
            {
                me->SetFacingTo(4.6f);
                me->CastSpell(me, 34189, true);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (Timer <= diff)
            {
                if (powiedz == true)
                {
                    powiedz = false;
                    me->GetMotionMaster()->MovePath(44882, false);
                    Talk(0);
                    Timer = 50000;
                }
            }
            else
                Timer -= diff;
        }
    };
};

class gobj_abandoned_outhouse : public GameObjectScript
{
public:
    gobj_abandoned_outhouse() : GameObjectScript("gobj_abandoned_outhouse") { }

    bool OnQuestAccept(Player* player, GameObject* /*go*/, Quest const* quest)
    {
        if (quest->GetQuestId() == 27045)
            if (Creature* yorick = player->FindNearestCreature(44882, 50.0f))
                yorick->UnsummonAllTotems();
            else
                player->SummonCreature(44882, 1300.516479f, 1189.810791f, 52.124302f, 2.57038f, TEMPSUMMON_MANUAL_DESPAWN, 0);

        return true;
    }
};

class npc_fenris_keep_camera_bunny1 : public CreatureScript
{
public:
    npc_fenris_keep_camera_bunny1() : CreatureScript("npc_fenris_keep_camera_bunny1") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_fenris_keep_camera_bunny1AI(creature);
    }

    struct npc_fenris_keep_camera_bunny1AI : public ScriptedAI
    {
        npc_fenris_keep_camera_bunny1AI(Creature* creature) : ScriptedAI(creature) { }

        bool CheckVehicle(ObjectGuid PlayerGUID)
        {
            std::list<Creature*> pCreatureList;
            Trinity::AllCreaturesOfEntryInRange checker(me, 45003, 20);
            Trinity::CreatureListSearcher<Trinity::AllCreaturesOfEntryInRange> searcher(me, pCreatureList, checker);
            me->VisitNearbyObject(4, searcher);

            for (std::list<Creature*>::iterator iter = pCreatureList.begin(); iter != pCreatureList.end(); ++iter)
            {
                if (Unit* summoner = (*iter)->GetTempSummoner())
                {
                    ObjectGuid pguid = summoner->GetGUID();
                    if (PlayerGUID == pguid)
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        void MoveInLineOfSight(Unit* who) override
        {
            Player* player = who ? who->ToPlayer() : nullptr;
            if (!player)
                return;
            if (player->GetQuestStatus(27097) == QUEST_STATUS_REWARDED && player->GetQuestStatus(27099) == QUEST_STATUS_INCOMPLETE)
            {
                if (player->IsWithinDistInMap(me, 3.0f))
                {
                    if (CheckVehicle(player->GetGUID()))
                        if (Creature* camera = player->SummonCreature(45003, who->GetPositionX(), who->GetPositionY(), who->GetPositionZ(), who->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0))
                        {
                            camera->GetMotionMaster()->MovePath(4500300, false);
                            player->EnterVehicle(camera, 0);
                        }
                }
            }
        }
    };
};

class npc_fenris_keep_camera_bunny2 : public CreatureScript
{
public:
    npc_fenris_keep_camera_bunny2() : CreatureScript("npc_fenris_keep_camera_bunny2") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_fenris_keep_camera_bunny2AI(creature);
    }

    struct npc_fenris_keep_camera_bunny2AI : public ScriptedAI
    {
        npc_fenris_keep_camera_bunny2AI(Creature* creature) : ScriptedAI(creature) { }


        bool CheckVehicle(ObjectGuid PlayerGUID)
        {
            std::list<Creature*> pCreatureList;
            Trinity::AllCreaturesOfEntryInRange checker(me, 45003, 20);
            Trinity::CreatureListSearcher<Trinity::AllCreaturesOfEntryInRange> searcher(me, pCreatureList, checker);
            me->VisitNearbyObject(4, searcher);

            for (std::list<Creature*>::iterator iter = pCreatureList.begin(); iter != pCreatureList.end(); ++iter)
            {
                if (Unit* summoner = (*iter)->GetTempSummoner())
                {
                    ObjectGuid pguid = summoner->GetGUID();
                    if (PlayerGUID == pguid)
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        void MoveInLineOfSight(Unit* who) override
        {
            Player* player = who ? who->ToPlayer() : nullptr;
            if (!player)
                return;

            if ((player->GetQuestStatus(27097) == QUEST_STATUS_REWARDED) && (player->GetQuestStatus(27099) == QUEST_STATUS_INCOMPLETE))
            {
                if (player->IsWithinDistInMap(me, 3.0f))
                {
                    if (CheckVehicle(player->GetGUID()))
                        if (Creature* camera = player->SummonCreature(45003, who->GetPositionX(), who->GetPositionY(), who->GetPositionZ(), who->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0))
                        {
                            camera->GetMotionMaster()->MovePath(4500301, false);
                            player->EnterVehicle(camera, 0);
                        }
                }
            }
        }
    };
};

class npc_fenris_keep_camera : public CreatureScript
{
public:

    npc_fenris_keep_camera() : CreatureScript("npc_fenris_keep_camera") { }

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new npc_fenris_keep_cameraAI(pCreature);
    }

    struct npc_fenris_keep_cameraAI : public ScriptedAI
    {
        npc_fenris_keep_cameraAI(Creature* creature) : ScriptedAI(creature) { }

        uint64 playerGUID;
        uint64 BloodfangGUID;
        uint64 CrowleyGUID;
        uint64 PhinGUID;
        uint64 BartoloGUID;
        uint64 DibbsGUID;
        uint64 MalebGUID;
        uint64 SmithersGUID;
        uint64 SophiaGUID;
        uint64 ArthuraGUID;
        uint32 Timer;
        uint32 phase;
        bool worgenysummoned;

        void Reset() override
        {
            worgenysummoned = false;
            Timer = 300;
            playerGUID = 0;
            BloodfangGUID = 0;
            CrowleyGUID = 0;
            PhinGUID = 0;
            BartoloGUID = 0;
            DibbsGUID = 0;
            MalebGUID = 0;
            SmithersGUID = 0;
            SophiaGUID = 0;
            ArthuraGUID = 0;
            phase = 0;

            if (Unit* summoner = me->GetTempSummoner())
                if (summoner->GetTypeId() == TYPEID_PLAYER)
                    playerGUID = summoner->GetGUID();
        }

        void SummonWorgens()
        {
            if (Player* player = me->GetMap()->GetPlayer(playerGUID))
                if (Creature* Bloodfang = player->SummonCreature(44990, 994.53f, 687.74f, 74.8984f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000))
                    if (Creature* Crowley = player->SummonCreature(44989, 994.562f, 691.186f, 74.8984f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000))
                        if (Creature* Phin = player->SummonCreature(44993, 1002.52f, 693.027f, 76.1922f, 3.1415f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000))
                            if (Creature* Bartolo = player->SummonCreature(44994, 1002.76f, 687.195f, 76.1922f, 3.1415f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000))
                                if (Creature* Dibbs = player->SummonCreature(44995, 1002.8f, 684.256f, 76.1922f, 3.1415f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000))
                                    if (Creature* Maleb = player->SummonCreature(44996, 1000.67f, 689.759f, 76.1922f, 3.1415f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000))
                                        if (Creature* Smithers = player->SummonCreature(44997, 1002.7f, 695.775f, 76.1922f, 3.1415f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000))
                                            if (Creature* Sophia = player->SummonCreature(45002, 1004.53f, 688.619f, 76.1922f, 3.1415f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000))
                                            {
                                                Timer = 9000;

                                                Bloodfang->SetSpecialInvisibility(SPECIAL_VISIBILITY_PLAYER, playerGUID);
                                                Crowley->SetSpecialInvisibility(SPECIAL_VISIBILITY_PLAYER, playerGUID);
                                                Phin->SetSpecialInvisibility(SPECIAL_VISIBILITY_PLAYER, playerGUID);
                                                Bartolo->SetSpecialInvisibility(SPECIAL_VISIBILITY_PLAYER, playerGUID);
                                                Dibbs->SetSpecialInvisibility(SPECIAL_VISIBILITY_PLAYER, playerGUID);
                                                Maleb->SetSpecialInvisibility(SPECIAL_VISIBILITY_PLAYER, playerGUID);
                                                Smithers->SetSpecialInvisibility(SPECIAL_VISIBILITY_PLAYER, playerGUID);
                                                Sophia->SetSpecialInvisibility(SPECIAL_VISIBILITY_PLAYER, playerGUID);

                                                BloodfangGUID = Bloodfang->GetGUID();
                                                CrowleyGUID = Crowley->GetGUID();
                                                PhinGUID = Phin->GetGUID();
                                                BartoloGUID = Bartolo->GetGUID();
                                                DibbsGUID = Dibbs->GetGUID();
                                                MalebGUID = Maleb->GetGUID();
                                                SmithersGUID = Smithers->GetGUID();
                                                SophiaGUID = Sophia->GetGUID();
                                                worgenysummoned = true;
                                            }

        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == WAYPOINT_MOTION_TYPE && id == 2)
            {
                if (Player* player = me->GetMap()->GetPlayer(playerGUID))
                {
                    SummonWorgens();
                }
            }

            if (type == WAYPOINT_MOTION_TYPE && id == 3)
            {

                if (Player* player = me->GetMap()->GetPlayer(playerGUID))
                {
                    player->ChangeSeat(1);
                    if (Creature* Crowley = me->GetMap()->GetCreature(CrowleyGUID))
                    {
                        me->SetFacingToObject(Crowley);
                    }
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!worgenysummoned)
                return;

            if (Timer <= diff)
            {
                switch (phase)
                {
                case 0:
                {
                    Timer = 3000;
                    ++phase;
                    break;
                }
                case 1:
                {
                    if (Creature* Crowley = me->GetMap()->GetCreature(CrowleyGUID))
                        if (Player* player = me->GetMap()->GetPlayer(playerGUID))
                        {
                            me->CastSpell(Crowley, 83768, false);
                            player->CastSpell(Crowley, 83768, false);
                            Crowley->AI()->Talk(0, player);
                        }
                    Timer = 5000;
                    ++phase;
                    break;
                }
                case 2:
                {
                    if (Creature* Crowley = me->GetMap()->GetCreature(CrowleyGUID))
                        if (Player* player = me->GetMap()->GetPlayer(playerGUID))
                        {
                            Crowley->AI()->Talk(1, player);
                        }
                    Timer = 2500;
                    ++phase;
                    break;
                }
                case 3:
                {
                    if (Creature* Crowley = me->GetMap()->GetCreature(CrowleyGUID))
                        if (Player* player = me->GetMap()->GetPlayer(playerGUID))
                        {
                            Crowley->AI()->Talk(2, player);
                        }
                    Timer = 4300;
                    ++phase;
                    break;
                }
                case 4:
                {
                    if (Creature* Crowley = me->GetMap()->GetCreature(CrowleyGUID))
                        if (Player* player = me->GetMap()->GetPlayer(playerGUID))
                        {
                            Crowley->AI()->Talk(3, player);
                        }
                    Timer = 5500;
                    ++phase;
                    break;
                }
                case 5:
                {
                    if (Creature* Crowley = me->GetMap()->GetCreature(CrowleyGUID))
                        if (Player* player = me->GetMap()->GetPlayer(playerGUID))
                        {
                            Crowley->AI()->Talk(4, player);
                        }
                    Timer = 6500;
                    ++phase;
                    break;
                }
                case 6:
                {
                    if (Creature* Crowley = me->GetMap()->GetCreature(CrowleyGUID))
                        if (Player* player = me->GetMap()->GetPlayer(playerGUID))
                            Crowley->AI()->Talk(5, player);
                    Timer = 9000;
                    ++phase;
                    break;
                }
                case 7:
                {
                    if (Creature* Maleb = me->GetMap()->GetCreature(MalebGUID))
                        if (Player* player = me->GetMap()->GetPlayer(playerGUID))
                        {
                            me->CastSpell(Maleb, 84103, false);
                            Maleb->AI()->Talk(0, player);
                        }
                    Timer = 9000;
                    ++phase;
                    break;
                }
                case 8:
                {
                    if (Creature* Maleb = me->GetMap()->GetCreature(MalebGUID))
                        if (Player* player = me->GetMap()->GetPlayer(playerGUID))
                        {
                            Maleb->AI()->Talk(1, player);
                            Timer = 4000;
                            ++phase;
                            break;
                        }
                }
                case 9:
                {
                    if (Player* player = me->GetMap()->GetPlayer(playerGUID))
                        if (Creature* Maleb = me->GetMap()->GetCreature(MalebGUID))
                            if (Creature* Smithers = me->GetMap()->GetCreature(SmithersGUID))
                                if (Creature* Dibbs = me->GetMap()->GetCreature(DibbsGUID))
                                    if (Creature* Bartolo = me->GetMap()->GetCreature(BartoloGUID))
                                        if (Creature* Sophia = me->GetMap()->GetCreature(SophiaGUID))
                                            if (Creature* Phin = me->GetMap()->GetCreature(PhinGUID))
                                            {
                                                Smithers->CastSpell(Smithers, 84098, false);
                                                Phin->CastSpell(Phin, 84094, false);
                                                Sophia->CastSpell(Sophia, 84099, false);
                                                Maleb->CastSpell(Maleb, 84097, false);
                                                Bartolo->CastSpell(Bartolo, 84095, false);
                                                Dibbs->CastSpell(Dibbs, 84096, false);
                                                Timer = 4500;
                                                ++phase;
                                                break;
                                            }
                }
                case 10:
                {
                    if (Player* player = me->GetMap()->GetPlayer(playerGUID))
                    {
                        player->ChangeSeat(0);
                        player->KilledMonsterCredit(44951);
                        for (Unit::ControlList::const_iterator itr = player->m_Controlled.begin(); itr != player->m_Controlled.end(); ++itr)
                            if ((*itr) && (*itr)->GetTypeId() == TYPEID_UNIT && (*itr)->GetEntry() == 44951)
                                (*itr)->ToCreature()->DespawnOrUnsummon(1);

                        if (Creature* Arthura = player->SummonCreature(44951, player->GetPositionX() - 2.0f, player->GetPositionY(), player->GetPositionZ(), 0.0f, TEMPSUMMON_MANUAL_DESPAWN, 0))
                            ArthuraGUID = Arthura->GetGUID();
                    }
                    Timer = 500;
                    ++phase;
                    break;
                }
                case 11:
                {
                    if (Creature* Arthura = me->GetMap()->GetCreature(ArthuraGUID))
                        if (Player* player = me->GetMap()->GetPlayer(playerGUID))
                        {
                            Arthura->AI()->Talk(3, player);
                            // Still need to handle Agatha¡¯s flight part ¨C not yet finished.
                            player->ExitVehicle();
                            player->EnterVehicle(Arthura, 0);
                            Arthura->DespawnOrUnsummon(1);

                        }
                    Timer = 1000;
                    ++phase;
                    break;
                }
                case 12:
                {
                    if (Creature* Arthura = me->GetMap()->GetCreature(ArthuraGUID))
                        if (Player* player = me->GetMap()->GetPlayer(playerGUID))
                        {
                            Arthura->AI()->Talk(2, player);
                            Arthura->SetSpeed(MOVE_FLIGHT, 2.0f);
                            player->ExitVehicle();
                            player->EnterVehicle(Arthura, 0);
                            me->DespawnOrUnsummon(1);
                            Timer = 1000;
                            ++phase;
                            break;
                        }
                }
                default:
                    break;
                }

            }
            else
                Timer -= diff;
        }
    };
};

/*######
## AddSC
######*/

void AddSC_silverpine_forest()
{
    new npc_deathstalker_erland();
    new creature_script<npc_fenwick_thatros>("npc_fenwick_thatros");

    new npc_silverpine_grand_executor_mortuus();
    new creature_script<npc_silverpine_fallen_human>("npc_silverpine_fallen_human");
    new spell_silverpine_forsaken_trooper_masterscript_high_command();
    new spell_silverpine_raise_forsaken();
    new spell_silverpine_go_home();
    new creature_script<npc_silverpine_forsaken_bat>("npc_silverpine_forsaken_bat");
    new npc_agatha_44951();
    new npc_hillsbrad_refugee_44966();
    new npc_camera_waiting();
    new npc_armoire();
    new npc_deathstalker_rane_yorick();
    new gobj_abandoned_outhouse();
    new npc_fenris_keep_camera();
    new npc_fenris_keep_camera_bunny1();
    new npc_fenris_keep_camera_bunny2();
}
