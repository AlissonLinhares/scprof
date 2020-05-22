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
#ifndef PROFILE_H
#define PROFILE_H

#include "sysc/datatypes/fx/sc_fix.h"

#include <sstream>
#include <iostream>
#include <cstdlib>
#include <map>
#include <set>
#include <cstring>
#include <bitset>
#include <type_traits>
#include <algorithm>
#include <fstream>
#include <stdint.h>

using namespace std;
using namespace sc_dt;

class SCProf {
public:
	union IEEE754DP {
		double value;

		struct {
			uint64_t m : 52;
			uint64_t e : 11;
			unsigned s : 1;
		} __attribute__((__packed__)) bin;

		IEEE754DP(const double &value) : value(value) {

		}

		const int exp() const {
			return bin.e - 1023;
		}
	};

	struct tProfile {
		uintptr_t addr;
		uint64_t count;
		uint64_t nzeros;
		uint64_t nneg;
		bitset<64> bits;
		bitset<64> bitmap;
		uint64_t changes[64];
		uint64_t freq[128];
		int size;
		bool isUnsigned;
		int iwl;

		tProfile(uintptr_t addr, int size, bool isUnsigned) : addr(addr),
					count(0), nzeros(0), nneg(0), bitmap(0), size(size),
					isUnsigned(isUnsigned), iwl(size) {

			for (int i = 0; i < 64; i++) {
				bits[i] = 0;
				changes[i] = 0;
			}
		}

		const void genJSON(const int line, ofstream &json) {
			json << "{";
			json << "\"line\":" << line;
			json << ",\"count\":" << count;
			json << ",\"utilization\":" << getUtilization();
			json << ",\"variation\":" << getVariation();
			json << ",\"zeros\":" << nzeros;
			json << ",\"neg\":" << nneg;
			json << ",\"iwl\":" << iwl;
			json << ",\"wl\":" << size;
			json << ",\"signed\":" << (isUnsigned == 0);
			json << ",\"changes\": [" << " ";

			for (int i = size - 1; i >= 0; i--) {
				json << changes[i];
				if (i != 0)
					json << ", ";
			}

			json << "],\"bitmap\": [" << " ";
			for (int i = size - 1; i >= 0; i--) {
				json << bitmap[i];
				if (i != 0)
					json << ", ";
			}

			json << "],\"freq\": [" << " ";
			for (int i = 0; i < 128; i++) {
				json << freq[i];
				if (i != 127)
					json << ", ";
			}

			json << "]}";
		}

		const double getUtilization() const {
			return bitmap.count() * 100.0 / size;
		}

		const double getVariation() const {
			uint64_t totalChanges = 0;

			for (int i = 0; i < size; i++) {
				if (changes[i] > 0)
					totalChanges += changes[i];
			}

			return (totalChanges * 100.0) / (size * count);
		}
	};

public:
	static SCProf *getInstance() {
		static SCProf *sc = 0;
		if (sc == 0)
			sc = new SCProf();
		return sc;
	}

	const char*& profile(const char* &value, _GETINFO) {
		return value;
	}

	template<typename T> tProfile* profBitStream(
			T &value, uintptr_t addr, bool isUnsigned, _GETINFO) {
		int len = value.wl();
		string bin = value.range(0, len-1).to_string();
		tProfile *profile;

		map<int, map<uintptr_t, tProfile*>> &ptr = history[fname];
		if (ptr[fline].count(addr) == 0) {
			profile = new tProfile(addr, len, isUnsigned);
			ptr[fline][addr] = profile;
		} else {
			profile = ptr[fline][addr];
		}

		profile->iwl = value.iwl();
		reverse(bin.begin(), bin.end());
		bitset<64> bits(bin);
		analyze(bits, profile, len);

		IEEE754DP v(value.to_double());
		int exp = v.exp();
		if (exp > -64 && exp < 64)
			profile->freq[exp + 64]++;

		return profile;
	}

	template<typename T, typename enable_if<is_base_of<sc_fxnum,T>::value>::type* = nullptr>
	T& profile(T &value, bool isUnsigned, _GETINFO) {
		profBitStream(value, (uintptr_t)(&value), isUnsigned, fline, fname);
		return value;
	}

	template<typename T, typename enable_if<is_base_of<sc_fxnum_fast,T>::value>::type* = nullptr>
	T& profile(T& value, bool isUnsigned, _GETINFO) {
		profBitStream(value, (uintptr_t)(&value), isUnsigned, fline, fname);
		return value;
	}

	template<typename T, typename enable_if<is_base_of<sc_int_base,T>::value>::type* = nullptr>
	T& profile(T& value, _GETINFO) {
		long long v = value.to_int64();
		profile(v, fline, fname, value.length(), (uintptr_t)(&value));
		return value;
	}

	template<typename T, typename enable_if<is_base_of<sc_signed,T>::value>::type* = nullptr>
	T& profile(T& value, _GETINFO) {
		long long v = value.to_int64();
		profile(v, fline, fname, value.length(), (uintptr_t)(&value));
		return value;
	}

	template<typename T, typename enable_if<is_base_of<sc_uint_base,T>::value>::type* = nullptr>
	T& profile(T& value, _GETINFO) {
		unsigned long long v = value.to_uint64();
		profile(v, fline, fname, value.length(), (uintptr_t)(&value));
		return value;
	}

	template<typename T, typename enable_if<is_base_of<sc_unsigned,T>::value>::type* = nullptr>
	T& profile(T& value, _GETINFO) {
		unsigned long long v = value.to_uint64();
		profile(v, fline, fname, value.length(), (uintptr_t)(&value));
		return value;
	}

	template<typename T, typename enable_if<is_integral<T>::value>::type* = nullptr>
	T& profile(T &value, _GETINFO, int size = sizeof(T)*8, uintptr_t addr = 0) {
		tProfile *profile;

		if (addr == 0)
			addr = (uintptr_t)(&value);

		map<int, map<uintptr_t, tProfile*>> &ptr = history[fname];
		if (ptr[fline].count(addr) == 0) {
			profile = new tProfile(addr, size, is_unsigned<T>::value);
			ptr[fline][addr] = profile;
		} else {
			profile = ptr[fline][addr];
		}

		analyze(value, profile, size);
		return value;
	}

	const double& profile(const double& value, _GETINFO) {
		profile(*(uint64_t*)(&value), fline, fname, 64, (uintptr_t)(&value));
		return value;
	}

	double& profile(double& value, _GETINFO) {
		profile(*(uint64_t*)(&value), fline, fname, 64, (uintptr_t)(&value));
		return value;
	}

	const float& profile(const float& value, _GETINFO) {
		profile(*(uint32_t*)(&value), fline, fname, 32, (uintptr_t)(&value));
		return value;
	}

	float& profile(float& value, _GETINFO) {
		profile(*(uint32_t*)(&value), fline, fname, 32, (uintptr_t)(&value));
		return value;
	}

	const bool& profile(const bool &value, _GETINFO) {
		profile(value, fline, fname, 1, (uintptr_t)(&value));
		return value;
	}

	bool& profile(bool& value, _GETINFO) {
		profile(value, fline, fname, 1, (uintptr_t)(&value));
		return value;
	}

	static void genJSON() {
		bool ffile = true;
		const auto history = getInstance()->history;

		ofstream json;
		json.open(OUTPUT_FILE);
		json << "{";
		json << "\"files\": [";

		for (auto file : history) {
			if (ffile)
				ffile = false;
			else
				json << ",";

			string str = file.first;

			json << "{\"path\": \"" << str.c_str() << "\",";
			json << "\"entry\": [";

			bool fentry = true;
			for (auto data : file.second) {
				for (auto addr : data.second) {
					if (fentry)
						fentry = false;
					else
						json << ",";

					((tProfile*) addr.second)->genJSON(data.first, json);
				}
			}

			json << "]}";
		}

		json << "]}";
		json.close();
	}

	static void genLog() {
		const auto history = getInstance()->history;

		for (auto file : history) {
			string str = file.first;

			printf("\n < %s >\n", str.c_str());
			printf(" LINE      | COUNT    | UTIL.  | VARI.  | ZEROS    | NEG      | S | IWL | WL  | MSB                          BIT MASK                         LSB | HIST. \n" );

			for (auto data : file.second) {
				for (auto addr : data.second) {
					tProfile *prof = addr.second;

					string bitmap = prof->bitmap.to_string();
					for (int i = 0; i < 64 - prof->size; i++)
						bitmap[i] = ' ';

					string dot = ".";
					bitmap.insert(prof->iwl + 64 - prof->size, dot);

					printf("  %-8u | %8llu | %6.2f | %6.2f | %-8llu | %-8llu | %1u | %3d | %3d | %s | ",
							data.first,
							(long long unsigned int) (prof->count),
							prof->getUtilization(),
							prof->getVariation(),
							(long long unsigned int) (prof->nzeros),
							(long long unsigned int) (prof->nneg),
							(prof->isUnsigned == 0),
							prof->iwl,
							prof->size,
							bitmap.c_str());

					for (int i = prof->size - 1; i >= 0; i--)
						cout << prof->changes[i] << " ";
					cout << endl;
				}
			}
		}
	}

private:
	SCProf() {
		static bool hasCallbackFunction = false;

		if (!hasCallbackFunction) {
    		atexit(SCProf::onExit);
			hasCallbackFunction = true;
		}
	}

	virtual ~SCProf() {

	}

	static void onExit() {
		genJSON();
		genLog();
	}

	void analyze(const bitset<64> value, tProfile *profile, int size) {
		int nzeros = 0;
		profile->count++;
		profile->size = size;

		int prefix = size;
		if (value.any()) {
			while (value[prefix-1] == 1)
				prefix--;
		}

		for (int i = 0; i < size; i++) {
			if (profile->count > 1)
				profile->changes[i] += (profile->bits[i] != (bool) value[i]);

			profile->bits[i] = (bool) value[i];

			if (value[i] == 1) {
				if (!profile->isUnsigned) {
					if (i > prefix)
						continue;

					if (i+1 < prefix)
						profile->bitmap[i+1] = 1;
				}

				profile->bitmap[i] = 1;
			} else {
				nzeros++;
			}
		}

		profile->nzeros += (nzeros == size);
		profile->nneg += profile->bits[size-1];  // msb
	}

	template<typename T, typename enable_if<is_integral<T>::value>::type* = nullptr>
	void analyze(T value, tProfile *profile) {
		analyze(bitset<64>(value), profile, sizeof(T)*8);
	}

	void analyze(bool value, tProfile *profile) {
		analyze(bitset<64>(value), profile, 1);
	}

private:
	map<string,map<int, map<uintptr_t, tProfile*>>> history;
};

#endif
