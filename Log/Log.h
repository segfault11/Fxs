/*
 * A simple logger
 * Copyright (C) 2014 Arno in Wolde Luebke
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct FxsLog* FxsLogPtr;

FxsLogPtr FxsLogCreate(unsigned int entryLen);
void FxsLogPrintf(FxsLogPtr log, const char *format, ...);
unsigned int FxsLogGetNumEntries(FxsLogPtr log);
const char* FxsLogGetEntry(FxsLogPtr log, unsigned int entry);
void FxsLogDestroy(FxsLogPtr *log);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: LOG_H */
