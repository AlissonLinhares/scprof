/* ---------------------------------------------------------------------------- *
 * Copyright (C) 2019-2020 Alisson Linhares de Carvalho.                        *
 * All rights reserved.                                                         *
 *                                                                              *
 * This file is part of the SCProf.                                             *
 *                                                                              *
 * The SCProf is free software: you can redistribute it and/or modify it        *
 * under the terms of the GNU Lesser General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or            *
 * (at your option) any later version.                                          *
 *                                                                              *
 * The SCProf is distributed in the hope that it will be useful, but            *
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY   *
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for  *
 * more details.                                                                *
 *                                                                              *
 * You should have received a copy of the GNU General Public License along with *
 * the SCProf. If not, see <http://www.gnu.org/licenses/>.                      *
 * ---------------------------------------------------------------------------- */
#ifndef OBSERVERS_H
#define OBSERVERS_H

#include "profile.h"

class prof_sc_fxnum_observer : public sc_fxnum_observer {
public:
	static sc_fxnum_observer* getInstance(bool isUnsigned = false, _GETINFO) {

		static map<string,map<uint64_t, sc_fxnum_observer*>> history;
		map<uint64_t, sc_fxnum_observer*> &ptr = history[fname];

		if (ptr.count(fline) == 0)
			ptr[fline] = new prof_sc_fxnum_observer(fline, fname, isUnsigned);

		return ptr[fline];
	}

	prof_sc_fxnum_observer(_GETINFO, bool isUnsigned = false) :
			sc_fxnum_observer(), fline(fline), fname(fname),
			isUnsigned(isUnsigned) {
	}

	inline void construct(const sc_fxnum& value) {
		profile(value);
	}

	inline void read(const sc_fxnum& value) {
		profile(value);
	}

	inline void write(const sc_fxnum& value) {
		profile(value);
	}

protected:
	const void profile(const sc_fxnum& value) const {
		SCProf::getInstance()->profBitStream(value, (uintptr_t)(&value),
			isUnsigned, fline, fname);
	}

private:
	uint64_t fline;
	const char* fname;
	bool isUnsigned;
};


class prof_sc_fxnum_fast_observer : public sc_fxnum_fast_observer {
public:
	static sc_fxnum_fast_observer* getInstance(bool isUnsigned = false,
			_GETINFO) {

		static map<string,map<uint64_t, sc_fxnum_fast_observer*>> history;
		map<uint64_t, sc_fxnum_fast_observer*> &ptr = history[fname];

		if (ptr.count(fline) == 0)
			ptr[fline] = new prof_sc_fxnum_fast_observer(fline, fname, isUnsigned);

		return ptr[fline];
	}

	prof_sc_fxnum_fast_observer(_GETINFO, bool isUnsigned = false) :
		sc_fxnum_fast_observer(), fline(fline), fname(fname),
		isUnsigned(isUnsigned) {
	}

	inline void construct(const sc_fxnum_fast& value) {
		profile(value);
	}

	inline void read(const sc_fxnum_fast& value) {
		profile(value);
	}

	inline void write(const sc_fxnum_fast& value) {
		profile(value);
	}

protected:
	const void profile(const sc_fxnum_fast& value) const {
		SCProf::getInstance()->profBitStream(value, (uintptr_t)(&value),
			isUnsigned, fline, fname);
	}

private:
	uint64_t fline;
	const char* fname;
	bool isUnsigned;
};

#endif
