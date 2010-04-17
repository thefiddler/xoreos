/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

/** @file engines/nwn2/nwn2.cpp
 *  Engine class handling Neverwinter Nights 2
 */

#include "engines/nwn2/nwn2.h"

#include "common/util.h"
#include "common/filelist.h"

#include "aurora/resman.h"
#include "aurora/error.h"

namespace NWN2 {

static const std::string kGameName = "Neverwinter Nights 2";

const NWN2EngineProbe kNWN2EngineProbe;

Aurora::GameID NWN2EngineProbe::getGameID() const {
	return Aurora::kGameIDNWN2;
}

const std::string &NWN2EngineProbe::getGameName() const {
	return kGameName;
}

bool NWN2EngineProbe::probe(const std::string &directory, const Common::FileList &rootFiles) const {
	// If either the ini file or the binary is found, this should be a valid path
	if (rootFiles.contains(".*/nwn2.ini", true))
		return true;
	if (rootFiles.contains(".*/nwn2main.exe", true))
		return true;

	return false;
}

Engines::Engine *NWN2EngineProbe::createEngine() const {
	return new NWN2Engine;
}


NWN2Engine::NWN2Engine() {
}

NWN2Engine::~NWN2Engine() {
}

void NWN2Engine::run(const std::string &directory) {
	_baseDirectory = directory;

	init();

	status("Successfully initialized the engine");

	// Play atarilogo.bik
	// Play oeilogo.bik
	// Play wotclogo.bik
	// Play nvidialogo.bik
	// Play legal.bik

	// Menu
}

void NWN2Engine::init() {
	ResMan.registerDataBaseDir(_baseDirectory);

	warning("TODO: NWN2's resource stuff");
}

} // End of namespace NWN2
