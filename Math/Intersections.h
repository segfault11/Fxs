/*
 * Various intersection test routines
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
#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Vector3.h"

int FxsRayDoesIntersectTriangle(
	const FxsVector3 *orig, 
	const FxsVector3 *dir, 
	float *t,
    float *u,
    float *v,
	const FxsVector3 *v0,
	const FxsVector3 *v1,
	const FxsVector3 *v2
);

/*
** Computes the intersection of a ray with a plane. The plane is provided
** in hessian normal form. Both, "dir" and "normal" are assumed to be 
** normalized.
*/ 
int FxsRayDoesIntersectPlane(
	const FxsVector3 *orig, 
	const FxsVector3 *dir, 
	float *t,
	float dist,
	const FxsVector3 *normal
);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: INTERSECTIONS_H */
