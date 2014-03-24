#ifndef QUATERNION_H
#define QUATERNION_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Vector3.h"

/*
** Declaration of a quaternion.
** x, y, z are the axis of rotation, w is the angle of rotation.
*/ 
typedef struct 
{
 	float x, y, z, w;
}
FxsQuaternion;

/*
** Initializes a quaternion with its components.
*/ 
void FxsQuaternionMake(FxsQuaternion* q, float x, float y, float z, float w);

/*
**  Makes a _unit_ quaternion, given only an axis. the axis is expected
**  to have length <= 1.0. Then the last component of the quaternion is
**  computed as w = -sqrt(1.0 - length(axis)^2). returns 0 in case of failure.
*/
int FxsQuaternionMakeWithAxis(FxsQuaternion* q, const FxsVector3* axis);

/*
** Copies contents of quaternion src to quaternion dst
*/ 
void FxsQuaternionCopy(FxsQuaternion* dst, const FxsQuaternion* src);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: QUATERNION_H */
