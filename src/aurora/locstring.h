/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

/** @file aurora/locstring.h
 *  Handling BioWare's localized strings.
 */

#ifndef AURORA_LOCSTRING_H
#define AURORA_LOCSTRING_H

#include <string>

#include "common/types.h"

#include "aurora/types.h"

namespace Common {
	class SeekableReadStream;
}

namespace Aurora {

/** A localized string. */
class LocString {
public:
	static const int kStringCount = 23;

	LocString();
	~LocString();

	void clear();

	/** Return the string ID / StrRef. */
	uint32 getID() const;
	/** Set the string ID / StrRef. */
	void setID(uint32 id);

	/** Does the LocString have a string of this language? */
	bool hasString(Language language) const;

	/** Get the string of that language. */
	const std::string &getString(Language language) const;
	/** Set the string of that language. */
	void setString(Language language, const std::string &str);

	/** Get the first available string. */
	const std::string &getFirstString() const;

	/** Read a string out of a stream. */
	void readString(Language language, Common::SeekableReadStream &stream);
	/** Read a LocSubString (substring of a LocString in game data) out of a stream. */
	void readLocSubString(Common::SeekableReadStream &stream);
	/** Read a LocString out of a stream. */
	void readLocString(Common::SeekableReadStream &stream, uint32 id, uint32 count);
	/** Read a LocString out of a stream. */
	void readLocString(Common::SeekableReadStream &stream);

private:
	uint32 _id; ///< The string's ID / StrRef. */

	std::string _strings[kStringCount]; /** The localized strings. */
};

} // End of namespace Aurora

#endif // AURORA_LOCSTRING_H