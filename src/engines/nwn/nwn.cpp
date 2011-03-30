/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010-2011 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

/** @file engines/nwn/nwn.cpp
 *  Engine class handling Neverwinter Nights.
 */

#include "common/util.h"
#include "common/filelist.h"
#include "common/filepath.h"
#include "common/stream.h"
#include "common/configman.h"

#include "aurora/error.h"
#include "aurora/resman.h"
#include "aurora/talkman.h"

#include "sound/sound.h"

#include "events/events.h"

#include "graphics/graphics.h"

#include "graphics/aurora/fontman.h"
#include "graphics/aurora/fps.h"

#include "graphics/aurora/cursorman.h"

#include "engines/aurora/util.h"
#include "engines/aurora/resources.h"
#include "engines/aurora/model.h"

#include "engines/nwn/nwn.h"
#include "engines/nwn/modelloader.h"
#include "engines/nwn/charstore.h"
#include "engines/nwn/console.h"
#include "engines/nwn/module.h"

#include "engines/nwn/gui/legal.h"
#include "engines/nwn/gui/main/main.h"

namespace Engines {

namespace NWN {

const NWNEngineProbe kNWNEngineProbe;

const Common::UString NWNEngineProbe::kGameName = "Neverwinter Nights";

Aurora::GameID NWNEngineProbe::getGameID() const {
	return Aurora::kGameIDNWN;
}

const Common::UString &NWNEngineProbe::getGameName() const {
	return kGameName;
}

bool NWNEngineProbe::probe(const Common::UString &directory, const Common::FileList &rootFiles) const {
	// Don't accidentally trigger on NWN2
	if (rootFiles.contains(".*/nwn2.ini", true))
		return false;
	if (rootFiles.contains(".*/nwn2main.exe", true))
		return false;

	// If either the ini file or a binary is found, this should be a valid path

	if (rootFiles.contains(".*/nwn.ini", true))
		return true;

	if (rootFiles.contains(".*/(nw|nwn)main.exe", true))
		return true;

	if (rootFiles.contains(".*/(nw|nwn)main", true))
		return true;

	return false;
}

bool NWNEngineProbe::probe(Common::SeekableReadStream &stream) const {
	return false;
}

Engines::Engine *NWNEngineProbe::createEngine() const {
	return new NWNEngine;
}


NWNEngine::NWNEngine() : _hasXP1(false), _hasXP2(false), _hasXP3(false), _fps(0) {
}

NWNEngine::~NWNEngine() {
}

void NWNEngine::run(const Common::UString &target) {
	_baseDirectory = target;

	init();
	if (EventMan.quitRequested())
		return;

	status("Successfully initialized the engine");

	CursorMan.hideCursor();
	CursorMan.set("default", true);

	playIntroVideos();
	if (EventMan.quitRequested())
		return;

	CursorMan.showCursor();

	if (ConfigMan.getBool("showfps", false)) {
		_fps = new Graphics::Aurora::FPS(FontMan.get("fnt_galahad14"));
		_fps->show();
	}

	mainMenuLoop();

	deinit();
}

void NWNEngine::init() {
	initConfig();
	checkConfig();

	if (EventMan.quitRequested())
		return;

	Common::UString localCharDir =
		Common::FilePath::findSubDirectory(_baseDirectory, "localvault", true);

	CharStore.addDirectory(localCharDir);

	initResources();

	if (EventMan.quitRequested())
		return;

	initCursors();

	if (EventMan.quitRequested())
		return;

	initGameConfig();
}

void NWNEngine::initResources() {
	status("Setting base directory");
	ResMan.registerDataBaseDir(_baseDirectory);
	indexMandatoryDirectory("", 0, 0, 0);

	status("Adding extra archive directories");
	ResMan.addArchiveDir(Aurora::kArchiveBIF, "data");
	ResMan.addArchiveDir(Aurora::kArchiveERF, "nwm");
	ResMan.addArchiveDir(Aurora::kArchiveERF, "modules");
	ResMan.addArchiveDir(Aurora::kArchiveERF, "hak");
	ResMan.addArchiveDir(Aurora::kArchiveERF, "texturepacks");

	status("Loading main KEY");
	indexMandatoryArchive(Aurora::kArchiveKEY, "chitin.key", 0);

	status("Loading expansions and patch KEYs");

	// Base game patch
	indexOptionalArchive(Aurora::kArchiveKEY, "patch.key", 1);

	// Expansion 1: Shadows of Undrentide (SoU)
	_hasXP1 = indexOptionalArchive(Aurora::kArchiveKEY, "xp1.key", 2);
	indexOptionalArchive(Aurora::kArchiveKEY, "xp1patch.key", 3);

	// Expansion 2: Hordes of the Underdark (HotU)
	_hasXP2 = indexOptionalArchive(Aurora::kArchiveKEY, "xp2.key", 4);
	indexOptionalArchive(Aurora::kArchiveKEY, "xp2patch.key", 5);

	// Expansion 3: Kingmaker (resources also included in the final 1.69 patch)
	_hasXP3 = indexOptionalArchive(Aurora::kArchiveKEY, "xp3.key", 6);
	indexOptionalArchive(Aurora::kArchiveKEY, "xp3patch.key", 7);

	status("Loading GUI textures");
	indexMandatoryArchive(Aurora::kArchiveERF, "gui_32bit.erf"   , 10);
	indexOptionalArchive (Aurora::kArchiveERF, "xp1_gui.erf"     , 11);
	indexOptionalArchive (Aurora::kArchiveERF, "xp2_gui.erf"     , 12);

	status("Indexing extra sound resources");
	indexMandatoryDirectory("ambient"   , 0, 0, 20);
	status("Indexing extra music resources");
	indexMandatoryDirectory("music"     , 0, 0, 21);
	status("Indexing extra movie resources");
	indexMandatoryDirectory("movies"    , 0, 0, 22);
	status("Indexing extra image resources");
	indexMandatoryDirectory("portraits" , 0, 0, 23);
	status("Indexing extra talktables");
	indexOptionalDirectory ("tlk"       , 0, 0, 25);
	status("Indexing databases");
	indexOptionalDirectory ("database"  , 0, 0, 26);

	status("Indexing override files");
	indexOptionalDirectory("override", 0, 0, 1000);

	if (EventMan.quitRequested())
		return;

	status("Loading main talk table");
	TalkMan.addMainTable("dialog");

	registerModelLoader(new NWNModelLoader);

	FontMan.setFormat(Graphics::Aurora::kFontFormatTexture);
}

void NWNEngine::initCursors() {
	CursorMan.add("gui_mp_defaultd" , "default"  , true);
	CursorMan.add("gui_mp_defaultu" , "default"  , false);

	CursorMan.add("gui_mp_actiond"  , "action"   , true);
	CursorMan.add("gui_mp_actionu"  , "action"   , false);
	CursorMan.add("gui_mp_attackd"  , "attack"   , true);
	CursorMan.add("gui_mp_attacku"  , "attack"   , false);
	CursorMan.add("gui_mp_created"  , "create"   , true);
	CursorMan.add("gui_mp_createu"  , "create"   , false);
	CursorMan.add("gui_mp_disarmd"  , "disarm"   , true);
	CursorMan.add("gui_mp_disarmu"  , "disarm"   , false);
	CursorMan.add("gui_mp_doord"    , "door"     , true);
	CursorMan.add("gui_mp_dooru"    , "door"     , false);
	CursorMan.add("gui_mp_examined" , "examine"  , true);
	CursorMan.add("gui_mp_examineu" , "examine"  , false);
	CursorMan.add("gui_mp_followd"  , "follow"   , true);
	CursorMan.add("gui_mp_followu"  , "follow"   , false);
	CursorMan.add("gui_mp_heald"    , "heal"     , true);
	CursorMan.add("gui_mp_healu"    , "heal"     , false);
	CursorMan.add("gui_mp_killd"    , "kill"     , true);
	CursorMan.add("gui_mp_killu"    , "kill"     , false);
	CursorMan.add("gui_mp_lockd"    , "lock"     , true);
	CursorMan.add("gui_mp_locku"    , "lock"     , false);
	CursorMan.add("gui_mp_magicd"   , "magic"    , true);
	CursorMan.add("gui_mp_magicu"   , "magic"    , false);
	CursorMan.add("gui_mp_pickupd"  , "pickup"   , true);
	CursorMan.add("gui_mp_pickupu"  , "pickup"   , false);
	CursorMan.add("gui_mp_pushpind" , "pushpin"  , true);
	CursorMan.add("gui_mp_pushpinu" , "pushpin"  , false);
	CursorMan.add("gui_mp_talkd"    , "talk"     , true);
	CursorMan.add("gui_mp_talku"    , "talk"     , false);
	CursorMan.add("gui_mp_transd"   , "trans"    , true);
	CursorMan.add("gui_mp_transu"   , "trans"    , false);
	CursorMan.add("gui_mp_used"     , "use"      , true);
	CursorMan.add("gui_mp_useu"     , "use"      , false);
	CursorMan.add("gui_mp_walkd"    , "walk"     , true);
	CursorMan.add("gui_mp_walku"    , "walk"     , false);

	CursorMan.add("gui_mp_noactiond", "noaction" , true);
	CursorMan.add("gui_mp_noactionu", "noaction" , false);
	CursorMan.add("gui_mp_noatckd"  , "noattack" , true);
	CursorMan.add("gui_mp_noatcku"  , "noattack" , false);
	CursorMan.add("gui_mp_nocreatd" , "nocreate" , true);
	CursorMan.add("gui_mp_nocreatu" , "nocreate" , false);
	CursorMan.add("gui_mp_nodisarmd", "nodisarm" , true);
	CursorMan.add("gui_mp_nodisarmu", "nodisarm" , false);
	CursorMan.add("gui_mp_noexamd"  , "noexamine", true);
	CursorMan.add("gui_mp_noexamu"  , "noexamine", false);
	CursorMan.add("gui_mp_noheald"  , "noheal"   , true);
	CursorMan.add("gui_mp_nohealu"  , "noheal"   , false);
	CursorMan.add("gui_mp_nokilld"  , "nokill"   , true);
	CursorMan.add("gui_mp_nokillu"  , "nokill"   , false);
	CursorMan.add("gui_mp_nolockd"  , "nolock"   , true);
	CursorMan.add("gui_mp_nolocku"  , "nolock"   , false);
	CursorMan.add("gui_mp_nomagicd" , "nomagic"  , true);
	CursorMan.add("gui_mp_nomagicu" , "nomagic"  , false);
	CursorMan.add("gui_mp_notalkd"  , "notalk"   , true);
	CursorMan.add("gui_mp_notalku"  , "notalk"   , false);
	CursorMan.add("gui_mp_noused"   , "nouse"    , true);
	CursorMan.add("gui_mp_nouseu"   , "nouse"    , false);
	CursorMan.add("gui_mp_nowalkd"  , "nowalk"   , true);
	CursorMan.add("gui_mp_nowalku"  , "nowalk"   , false);
}

void NWNEngine::initConfig() {
	ConfigMan.setInt(Common::kConfigRealmDefault, "texturepack"  ,   1);
	ConfigMan.setInt(Common::kConfigRealmDefault, "difficulty"   ,   0);
	ConfigMan.setInt(Common::kConfigRealmDefault, "tooltipdelay" , 100);

	ConfigMan.setBool(Common::kConfigRealmDefault, "largefonts", false);
}

void NWNEngine::initGameConfig() {
	ConfigMan.setBool(Common::kConfigRealmGameTemp, "NWN_hasXP1", _hasXP1);
	ConfigMan.setBool(Common::kConfigRealmGameTemp, "NWN_hasXP2", _hasXP2);
	ConfigMan.setBool(Common::kConfigRealmGameTemp, "NWN_hasXP3", _hasXP3);

	ConfigMan.setString(Common::kConfigRealmGameTemp, "NWN_extraModuleDir",
		Common::FilePath::findSubDirectory(_baseDirectory, "modules", true));
}

void NWNEngine::checkConfig() {
	checkConfigInt("texturepack"  ,   0,    3,   1);
	checkConfigInt("difficulty"   ,   0,    3,   0);
	checkConfigInt("tooltipdelay" , 100, 2700, 100);
}

void NWNEngine::deinit() {
	CharacterStore::destroy();

	delete _fps;
}

void NWNEngine::playIntroVideos() {
	playVideo("atarilogo");
	playVideo("biowarelogo");
	playVideo("wotclogo");
	playVideo("fge_logo_black");
	playVideo("nwnintro");
}

void NWNEngine::playMenuMusic() {
	if (SoundMan.isPlaying(_menuMusic))
		return;

	_menuMusic = _hasXP2 ?
		playSound("mus_x2theme"   , Sound::kSoundTypeMusic, true) :
		playSound("mus_theme_main", Sound::kSoundTypeMusic, true);
}

void NWNEngine::stopMenuMusic() {
	SoundMan.stopChannel(_menuMusic);
}

void NWNEngine::mainMenuLoop() {
	playMenuMusic();

	// Start sound
	playSound("gui_prompt", Sound::kSoundTypeSFX);

	// Create and fade in the legal billboard
	Legal *legal = new Legal;

	Console console;
	Module module(console);

	console.setModule(&module);

	while (!EventMan.quitRequested()) {
		GUI *mainMenu = new MainMenu(module);

		EventMan.flushEvents();
		if (legal) {
			// Fade in, show and fade out the legal billboard
			legal->fadeIn();
			mainMenu->show();
			legal->show();

			delete legal;
			legal = 0;
		} else
			mainMenu->show();

		mainMenu->run();
		mainMenu->hide();

		delete mainMenu;

		if (EventMan.quitRequested())
			break;

		stopMenuMusic();

		module.run();
		if (EventMan.quitRequested())
			break;

		playMenuMusic();
		console.hide();
		module.clear();
	}

	console.setModule();

	stopMenuMusic();
}

} // End of namespace NWN

} // End of namespace Engines
