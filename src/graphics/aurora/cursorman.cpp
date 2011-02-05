/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010-2011 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

/** @file graphics/aurora/cursorman.cpp
 *  The Aurora cursor manager.
 */

#include "common/util.h"
#include "common/error.h"

#include "graphics/aurora/cursorman.h"
#include "graphics/aurora/cursor.h"

#include "graphics/graphics.h"

DECLARE_SINGLETON(Graphics::Aurora::CursorManager)

namespace Graphics {

namespace Aurora {

CursorManager::CursorManager() : _hidden(false), _currentCursor(0) {
}

CursorManager::~CursorManager() {
	for (CursorMap::iterator cursor = _cursors.begin(); cursor != _cursors.end(); ++cursor)
		delete cursor->second;
}

void CursorManager::clear() {
	Common::StackLock lock(_mutex);

	set();

	for (CursorMap::iterator cursor = _cursors.begin(); cursor != _cursors.end(); ++cursor)
		delete cursor->second;

	_cursors.clear();
}

void CursorManager::set(const Common::UString &name) {
	if (!name.empty())
		_currentCursor = get(name);
	else
		_currentCursor = 0;

	if (!_hidden) {
		set(_currentCursor);
		GfxMan.showCursor(_currentCursor == 0);
	}
}

void CursorManager::set(Cursor *cursor) {
	GfxMan.setCursor(cursor);
}

Cursor *CursorManager::get(const Common::UString &name) {
	CursorMap::iterator cursor = _cursors.find(name);
	if (cursor != _cursors.end())
		return cursor->second;

	return _cursors.insert(std::make_pair(name, new Cursor(name))).first->second;
}

void CursorManager::hideCursor() {
	_hidden = true;

	GfxMan.setCursor();
	GfxMan.showCursor(false);
}

void CursorManager::showCursor() {
	_hidden = false;

	if (_currentCursor)
		GfxMan.setCursor(_currentCursor);
	else
		GfxMan.showCursor(true);
}

} // End of namespace Aurora

} // End of namespace Graphics