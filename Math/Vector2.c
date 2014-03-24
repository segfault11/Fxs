#include "Vector2.h"
#include <memory.h>
#include <math.h>

void FxsVector2Make(FxsVector2* v, float x, float y)
{
	v->x = x;
	v->y = y;
}

void FxsVector2MakeZero(FxsVector2* v)
{
	memset(v, 0, sizeof(FxsVector2)*2);
}

void FxsVector2MakeWithScalar(FxsVector2* v, float s)
{
	v->x = s;
	v->y = s;
}

void FxsVector2IncrementWithScalar(FxsVector2* v, float s)
{
	v->x += s;
	v->y += s;
} 

void FxsVector2DecrementWithScalar(FxsVector2* v, float s)
{
	v->x -= s;
	v->y -= s;
} 

void FxsVector2Increment(FxsVector2* v, const FxsVector2* i) 
{
	v->x += i->x;
	v->y += i->y;
}

void FxsVector2Decrement(FxsVector2* v, const FxsVector2* i) 
{
	v->x -= i->x;
	v->y -= i->y;
}

void FxsVector2Add(
	FxsVector2* res, 
	const FxsVector2* a, 
	const FxsVector2* b
)
{
	res->x = a->x + b->x;
	res->y = a->y + b->y;
}

void FxsVector2Substract(
	FxsVector2* res, 
	const FxsVector2* a, 
	const FxsVector2* b
)
{
	res->x = a->x - b->x;
	res->y = a->y - b->y;
}

void FxsVector2AddScalar(
	FxsVector2* res,
	const FxsVector2* a,
	float s
)
{
	res->x = a->x + s;
	res->y = a->y + s;
}

void FxsVector2Copy(FxsVector2* dst, const FxsVector2* src)
{
	memcpy(dst, src, sizeof(FxsVector2));
} 

void FxsVector2Length(float* length, const FxsVector2* a)
{
	*length = sqrt(a->x*a->x + a->y*a->y);
}

int FxsVector2Normalize(FxsVector2* a)
{
	float l;
	FxsVector2Length(&l, a);

	if (l == 0.0f) 
	{
		return 0; 
	}

	a->x /= l;
	a->y /= l;

	return 1;
}

void FxsVector2Dot(float* res, const FxsVector2* a, const FxsVector2* b)
{
	*res = a->x*b->x + a->y*b->y;
}
