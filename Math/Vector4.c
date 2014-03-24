#include "Vector4.h"
#include <math.h>
#include <memory.h>

void FxsVector4Make(FxsVector4* v, float x, float y, float z, float w)
{
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
}

void FxsVector4MakeZero(FxsVector4* v)
{
    memset(v, 0, sizeof(FxsVector4));
}

void FxsVector4Copy(FxsVector4* dst, const FxsVector4* src)
{
	memcpy(dst, src, sizeof(FxsVector4));
} 

void FxsVector4MakeWithScalar(FxsVector4* v, float s)
{
	v->x = s;
	v->y = s;
	v->z = s;
	v->w = s;
}

void FxsVector4IncrementWithScalar(FxsVector4* v, float s)
{
	v->x += s;
	v->y += s;
	v->z += s;
	v->w += s;
} 

void FxsVector4DecrementWithScalar(FxsVector4* v, float s)
{
	v->x -= s;
	v->y -= s;
	v->z -= s;
	v->w -= s;
} 

void FxsVector4Increment(FxsVector4* v, const FxsVector4* i) 
{
	v->x += i->x;
	v->y += i->y;
	v->z += i->z;
	v->w += i->w;
}

void FxsVector4Decrement(FxsVector4* v, const FxsVector4* i) 
{
	v->x -= i->x;
	v->y -= i->y;
	v->z -= i->z;
	v->w -= i->w;
}

void FxsVector4Add(
	FxsVector4* res, 
	const FxsVector4* a, 
	const FxsVector4* b
)
{
	res->x = a->x + b->x;
	res->y = a->y + b->y;
	res->z = a->z + b->z;
	res->w = a->w + b->w;
}

void FxsVector4Substract(
	FxsVector4* res, 
	const FxsVector4* a, 
	const FxsVector4* b
)
{
	res->x = a->x - b->x;
	res->y = a->y - b->y;
	res->z = a->z - b->z;
	res->w = a->w - b->w;
}

void FxsVector4AddScalar(
	FxsVector4* res,
	const FxsVector4* a,
	float s
)
{
	res->x = a->x + s;
	res->y = a->y + s;
	res->z = a->z + s;
	res->w = a->w + s;
}

void FxsVector4SubstractScalar(
	FxsVector4* res,
	const FxsVector4* a,
	float s
)
{
	res->x = a->x - s;
	res->y = a->y - s;
	res->z = a->z - s;
	res->w = a->w - s;
}

void FxsVector4Length(float* length, const FxsVector4* a)
{
	*length = sqrt(a->x*a->x + a->y*a->y + a->z*a->z + a->w*a->w);
}

int FxsVector4Normalize(FxsVector4* a)
{
	float l;
	FxsVector4Length(&l, a);

	if (l == 0.0f) 
	{
		return 0; 
	}

	a->x /= l;
	a->y /= l;
	a->z /= l;
	a->w /= l;

	return 1;
}

void FxsVector4Dot(float* res, const FxsVector4* a, const FxsVector4* b)
{
	*res = a->x*b->x + a->y*b->y + a->z*b->z + a->w*b->w; 
}
