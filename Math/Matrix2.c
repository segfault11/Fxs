#include "Matrix2.h"
#include <memory.h>

void FxsMatrix2Make(
	FxsMatrix2* m, 
	float m11, float m12,
	float m21, float m22
) 
{
	m->m11 = m11;
	m->m12 = m12;

	m->m21 = m21;
	m->m22 = m22;
} 


void FxsMatrix2Copy(FxsMatrix2* dst, const FxsMatrix2* src)
{
	memcpy(dst, src, sizeof(FxsMatrix2));
}

void FxsMatrix2Multiply(
	FxsMatrix2* res, 
	const FxsMatrix2* a, 
	const FxsMatrix2* b
)
{
	res->m11 = a->m11*b->m11 + a->m12*b->m21;
	res->m12 = a->m11*b->m12 + a->m12*b->m22;

	res->m21 = a->m21*b->m11 + a->m22*b->m21;
	res->m22 = a->m21*b->m12 + a->m22*b->m22;
}

void FxsMatrix2MultiplyVector3(
	FxsVector2* res,
	const FxsMatrix2* a,
	const FxsVector2* b
)
{
	res->x = a->m11*b->x + a->m12*b->y;
	res->y = a->m21*b->x + a->m22*b->y;
}
