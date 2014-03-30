/*
 * Abstraction of a number
 * Copyright (C) 2014 Arno Luebke
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
#ifndef NUMBER_H
#define NUMBER_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct FxsNumber_* FxsNumberPtr;

FxsNumberPtr FxsNumberCreateWithInt(int i);
FxsNumberPtr FxsNumberCreateWithFloat(float f);

int FxsNumberAsInt(FxsNumberPtr n);
float FxsNumberAsFloat(FxsNumberPtr n);
const char* FxsNumberAsString(FxsNumberPtr n);

void FxsNumberDestroy(FxsNumberPtr* n);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: NUMBER_H */
