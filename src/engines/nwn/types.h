/* xoreos - A reimplementation of BioWare's Aurora engine
 *
 * xoreos is the legal property of its developers, whose names can be
 * found in the AUTHORS file distributed with this source
 * distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 */

/** @file engines/nwn/types.h
 *  Basic NWN type definitions.
 */

#ifndef ENGINES_NWN_TYPES_H
#define ENGINES_NWN_TYPES_H

#include "common/types.h"

namespace Engines {

namespace NWN {

enum ObjectType {
	kObjectTypeNone         = 0     ,
	kObjectTypeCreature     = 1 << 0,
	kObjectTypeItem         = 1 << 1,
	kObjectTypeTrigger      = 1 << 2,
	kObjectTypeDoor         = 1 << 3,
	kObjectTypeAreaOfEffect = 1 << 4,
	kObjectTypeWaypoint     = 1 << 5,
	kObjectTypePlaceable    = 1 << 6,
	kObjectTypeStore        = 1 << 7,
	kObjectTypeEncounter    = 1 << 8,
	kObjectTypeAll          = 0x7FFF
};

enum Script {
	kScriptAcquireItem       = 0,
	kScriptUnacquireItem        ,
	kScriptActivateItem         ,
	kScriptEnter                ,
	kScriptExit                 ,
	kScriptCutsceneAbort        ,
	kScriptHeartbeat            ,
	kScriptModuleLoad           ,
	kScriptModuleStart          ,
	kScriptPlayerChat           ,
	kScriptPlayerDeath          ,
	kScriptPlayerDying          ,
	kScriptPlayerEquipItem      ,
	kScriptPlayerUnequipItem    ,
	kScriptPlayerLevelUp        ,
	kScriptPlayerRest           ,
	kScriptPlayerRespan         ,
	kScriptUserdefined          ,
	kScriptUsed                 ,
	kScriptClick                ,
	kScriptOpen                 ,
	kScriptClosed               ,
	kScriptDamaged              ,
	kScriptDeath                ,
	kScriptDisarm               ,
	kScriptLock                 ,
	kScriptUnlock               ,
	kScriptAttacked             ,
	kScriptSpellCastAt          ,
	kScriptTrapTriggered        ,
	kScriptDialogue             ,
	kScriptDisturbed            ,
	kScriptEndRound             ,
	kScriptBlocked              ,
	kScriptNotice               ,
	kScriptRested               ,
	kScriptSpawn                ,
	kScriptFailToOpen           ,
	kScriptMAX
};

enum Animation {
	kAnimationLoopingPause               =   0,
	kAnimationLoopingPause2              =   1,
	kAnimationLoopingListen              =   2,
	kAnimationLoopingMeditate            =   3,
	kAnimationLoopingWorship             =   4,
	kAnimationLoopingLookFar             =   5,
	kAnimationLoopingSitChair            =   6,
	kAnimationLoopingSitCross            =   7,
	kAnimationLoopingTalkNormal          =   8,
	kAnimationLoopingTalkPleading        =   9,
	kAnimationLoopingTalkForceful        =  10,
	kAnimationLoopingTalkLaughing        =  11,
	kAnimationLoopingGetLow              =  12,
	kAnimationLoopingGetMid              =  13,
	kAnimationLoopingPauseTired          =  14,
	kAnimationLoopingPauseDrunk          =  15,
	kAnimationLoopingDeadFront           =  16,
	kAnimationLoopingDeadBack            =  17,
	kAnimationLoopingConjure1            =  18,
	kAnimationLoopingConjure2            =  19,
	kAnimationLoopingSpasm               =  20,
	kAnimationLoopingCustom1             =  21,
	kAnimationLoopingCustom2             =  22,
	kAnimationLoopingCustom3             =  23,
	kAnimationLoopingCustom4             =  24,
	kAnimationLoopingCustom5             =  25,
	kAnimationLoopingCustom6             =  26,
	kAnimationLoopingCustom7             =  27,
	kAnimationLoopingCustom8             =  28,
	kAnimationLoopingCustom9             =  29,
	kAnimationLoopingCustom10            =  30,
	kAnimationFireForgetHeadTurnLeft     = 100,
	kAnimationFireForgetHeadTurnRight    = 101,
	kAnimationFireForgetPauseScratchHead = 102,
	kAnimationFireForgetPauseBored       = 103,
	kAnimationFireForgetSalute           = 104,
	kAnimationFireForgetBow              = 105,
	kAnimationFireForgetSteal            = 106,
	kAnimationFireForgetGreeting         = 107,
	kAnimationFireForgetTaunt            = 108,
	kAnimationFireForgetVictory1         = 109,
	kAnimationFireForgetVictory2         = 110,
	kAnimationFireForgetVictory3         = 111,
	kAnimationFireForgetRead             = 112,
	kAnimationFireForgetDrink            = 113,
	kAnimationFireForgetDodgeSide        = 114,
	kAnimationFireForgetDodgeDuck        = 115,
	kAnimationFireForgetSpasm            = 116,
	kAnimationPlaceableActivate          = 200,
	kAnimationPlaceableDeactivate        = 201,
	kAnimationPlaceableOpen              = 202,
	kAnimationPlaceableClose             = 203,
	kAnimationDoorClose                  = 204,
	kAnimationDoorOpen1                  = 205,
	kAnimationDoorOpen2                  = 206,
	kAnimationDoorDestroy                = 207
};

enum TalkAnimation {
	kTalkAnimationDefault        =  0,
	kTalkAnimationTaunt          = 28,
	kTalkAnimationGreeting       = 29,
	kTalkAnimationListen         = 30,
	kTalkAnimationWorship        = 33,
	kTalkAnimationSalute         = 34,
	kTalkAnimationBow            = 35,
	kTalkAnimationSteal          = 37,
	kTalkAnimationNormal         = 38,
	kTalkAnimationPleading       = 39,
	kTalkAnimationForceful       = 40,
	kTalkAnimationLaugh          = 41,
	kTalkAnimationVictoryFighter = 44,
	kTalkAnimationVictoryMage    = 45,
	kTalkAnimationVictoryThief   = 46,
	kTalkAnimationLookFar        = 48,
	kTalkAnimationDrink          = 70,
	kTalkAnimationRead           = 71,
	kTalkAnimationNone           = 88
};

enum SFFSounds {
	kSSFAttack            =  0,
	kSSFBattlecry1        =  1,
	kSSFBattlecry2        =  2,
	kSSFBattlecry3        =  3,
	kSSFHealMe            =  4,
	kSSFHelp              =  5,
	kSSFEnemiesSighted    =  6,
	kSSFFlee              =  7,
	kSSFTaunt             =  8,
	kSSFGuardMe           =  9,
	kSSFHold              = 10,
	kSSFAttackGrunt1      = 11,
	kSSFAttackGrunt2      = 12,
	kSSFAttackGrunt3      = 13,
	kSSFPainGrunt1        = 14,
	kSSFPainGrunt2        = 15,
	kSSFPainGrunt3        = 16,
	kSSFNearDeath         = 17,
	kSSFDeath             = 18,
	kSSFPoisoned          = 19,
	kSSFSpellFailed       = 20,
	kSSFWeaponIneffective = 21,
	kSSFFollowMe          = 22,
	kSSFLookHere          = 23,
	kSSFGroupParty        = 24,
	kSSFMoveOver          = 25,
	kSSFPickLock          = 26,
	kSSFSearch            = 27,
	kSSFGoStealthy        = 28,
	kSSFCanDo             = 29,
	kSSFCannotDo          = 30,
	kSSFTaskComplete      = 31,
	kSSFEncumbered        = 32,
	kSSFSelected          = 33,
	kSSFHello             = 34,
	kSSFYes               = 35,
	kSSFNo                = 36,
	kSSFStop              = 37,
	kSSFRest              = 38,
	kSSFBored             = 39,
	kSSFGoodbye           = 40,
	kSSFThankYou          = 41,
	kSSFLaugh             = 42,
	kSSFCuss              = 43,
	kSSFCheer             = 44,
	kSSFSomethingToSay    = 45,
	kSSFGoodIdea          = 46,
	kSSFBadIdea           = 47,
	kSSFThreaten          = 48
};

enum GameDifficulty {
	kDifficultyVeryEasy  = 0, // Not in the options dialog?!?
	kDifficultyEasy      = 1,
	kDifficultyNormal    = 2,
	kDifficultyDifficult = 3,
	kDifficultyCoreRules = 4  // "Very difficult"
};

enum AssociateType {
	kAssociateTypeNone            = 0,
	kAssociateTypeHenchman        = 1,
	kAssociateTypeAnimalCompanion = 2,
	kAssociateTypeFamiliar        = 3,
	kAssociateTypeSummoned        = 4,
	kAssociateTypeDominated       = 5
};

enum Ability {
	kAbilityStrength     = 0,
	kAbilityDexterity    = 1,
	kAbilityConstitution = 2,
	kAbilityIntelligence = 3,
	kAbilityWisdom       = 4,
	kAbilityCharisma     = 5,
	kAbilityMAX
};

enum Alignment {
	kAlignmentAll     = 0,
	kAlignmentNeutral = 1,
	kAlignmentLawful  = 2,
	kAlignmentChaotic = 3,
	kAlignmentGood    = 4,
	kAlignmentEvil    = 5
};

static const uint32 kActionInvalid     = 0xFFFF;
static const uint32 kObjectTypeInvalid = 0x7FFF;

static const uint32 kGenderNone   =   4;
static const uint32 kRaceInvalid  =  28;
static const uint32 kClassInvalid = 255;

Alignment getAlignmentLawChaos(uint8 lawChaos);
Alignment getAlignmentGoodEvil(uint8 goodEvil);

} // End of namespace NWN

} // End of namespace Engines

#endif // ENGINES_NWN_TYPES_H
