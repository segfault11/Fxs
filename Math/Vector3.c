#include "Vector3.h"
#include <memory.h>
#include <math.h>

void FxsVector3Make(FxsVector3* v, float x, float y, float z)
{
	v->x = x;
	v->y = y;
	v->z = z;
}

void FxsVector3MakeZero(FxsVector3* v)
{
    memset(v, 0, sizeof(FxsVector3));
}

void FxsVector3Copy(FxsVector3* dst, const FxsVector3* src)
{
	memcpy(dst, src, sizeof(FxsVector3));
}

void FxsVector3MakeWithScalar(FxsVector3* v, float s)
{
	v->x = s;
	v->y = s;
	v->z = s;
}

void FxsVector3IncrementWithScalar(FxsVector3* v, float s)
{
	v->x += s;
	v->y += s;
	v->z += s;
} 

void FxsVector3DecrementWithScalar(FxsVector3* v, float s)
{
	v->x -= s;
	v->y -= s;
	v->z -= s;
} 

void FxsVector3Increment(FxsVector3* v, const FxsVector3* i) 
{
	v->x += i->x;
	v->y += i->y;
	v->z += i->z;
}

void FxsVector3Decrement(FxsVector3* v, const FxsVector3* i) 
{
	v->x -= i->x;
	v->y -= i->y;
	v->z -= i->z;
}

void FxsVector3Add(
	FxsVector3* res, 
	const FxsVector3* a, 
	const FxsVector3* b
)
{
	res->x = a->x + b->x;
	res->y = a->y + b->y;
	res->z = a->z + b->z;
}

void FxsVector3Substract(
	FxsVector3* res, 
	const FxsVector3* a, 
	const FxsVector3* b
)
{
	res->x = a->x - b->x;
	res->y = a->y - b->y;
	res->z = a->z - b->z;
}

void FxsVector3AddScalar(
	FxsVector3* res,
	const FxsVector3* a,
	float s
)
{
	res->x = a->x + s;
	res->y = a->y + s;
	res->z = a->z + s;
}

void FxsVector3SubstractScalar(
	FxsVector3* res,
	const FxsVector3* a,
	float s
)
{
	res->x = a->x - s;
	res->y = a->y - s;
	res->z = a->z - s;
}

void FxsVector3Length(float* length, const FxsVector3* a)
{
	*length = sqrt(a->x*a->x + a->y*a->y + a->z*a->z);
}

int FxsVector3Normalize(FxsVector3* a)
{
	float l;
	FxsVector3Length(&l, a);

	if (l == 0.0f) 
	{
		return 0; 
	}

	a->x /= l;
	a->y /= l;
	a->z /= l;

	return 1;
}

void FxsVector3Dot(float* res, const FxsVector3* a, const FxsVector3* b)
{
	*res = a->x*b->x + a->y*b->y + a->z*b->z;
}

void FxsVector3Cross(FxsVector3* res, const FxsVector3* a, const FxsVector3* b)
{
	res->x = a->y*b->z - a->z*b->y;
	res->y = -(a->x*b->z - a->z*b->x);
	res->z = a->x*b->y - a->y*b->x;
}
