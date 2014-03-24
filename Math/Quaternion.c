#include "Quaternion.h"
#include <memory.h>
#include <math.h>

void FxsQuaternionMake(FxsQuaternion* v, float x, float y, float z, float w)
{
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
}

void FxsQuaternionCopy(FxsQuaternion* dst, const FxsQuaternion* src)
{
	memcpy(dst, src, sizeof(FxsQuaternion));
} 

int FxsQuaternionMakeWithAxis(FxsQuaternion* q, const FxsVector3* axis)
{
    float l = axis->x*axis->x + axis->y*axis->y + axis->z*axis->z;
    
    if (l > 1.0f)
    {
        return 0;
    }
    
    q->x = axis->x;
    q->y = axis->y;
    q->z = axis->z;
    q->w = -sqrt(1.0 - l);

    return 1;
}