/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010-2011 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

/** @file common/threads.h
 *  Threading system helpers.
 */

#ifndef COMMON_THREADS_H
#define COMMON_THREADS_H

namespace Common {

void initThreads();

bool isMainThread();
void enforceMainThread();

} // End of namespace Common

#endif // COMMON_THREADS_H