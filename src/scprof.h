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
#ifndef SCPROF_H
#define SCPROF_H

#ifdef SC_ENABLE_PROFILER
#define SC_FXNUM_OBSERVER prof_sc_fxnum_observer::getInstance()
#define SC_FXNUM_FAST_OBSERVER prof_sc_fxnum_fast_observer::getInstance()
#define SC_UFXNUM_OBSERVER prof_sc_fxnum_observer::getInstance(true)
#define SC_UFXNUM_FAST_OBSERVER prof_sc_fxnum_fast_observer::getInstance(true)
#define SC_INCLUDE_FX 1
#define SC_ENABLE_OBSERVERS 1
#else
#define SC_FXNUM_OBSERVER 0
#define SC_FXNUM_FAST_OBSERVER 0
#define SC_UFXNUM_OBSERVER 0
#define SC_UFXNUM_FAST_OBSERVER 0
#endif

// g++ -DOUTPUT_FILE \"profile.json\"
#ifndef OUTPUT_FILE
#define OUTPUT_FILE "./profile.json"
#endif

#define _GETINFO uint64_t fline = __builtin_LINE(), const char *fname = __builtin_FILE()

#include "profile.h"
#include "observers.h"


#endif
