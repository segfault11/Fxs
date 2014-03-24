#include "Matrix3.h"
#include <memory.h>

void FxsMatrix3Make(
	FxsMatrix3* m, 
	float m11, float m12, float m13,
	float m21, float m22, float m23, 
	float m31, float m32, float m33
) 
{
	m->m11 = m11;
	m->m12 = m12;
	m->m13 = m13;	

	m->m21 = m21;
	m->m22 = m22;
	m->m23 = m23;	

	m->m31 = m31;
	m->m32 = m32;
	m->m33 = m33;	
} 


void FxsMatrix3Copy(FxsMatrix3* dst, const FxsMatrix3* src)
{
	memcpy(dst, src, sizeof(FxsMatrix3));
}

void FxsMatrix3Multiply(
	FxsMatrix3* res, 
	const FxsMatrix3* a, 
	const FxsMatrix3* b
)
{
	res->m11 = a->m11*b->m11 + a->m12*b->m21 + a->m13*b->m31;
	res->m12 = a->m11*b->m12 + a->m12*b->m22 + a->m13*b->m32;
	res->m13 = a->m11*b->m13 + a->m12*b->m23 + a->m13*b->m33;

	res->m21 = a->m21*b->m11 + a->m22*b->m21 + a->m23*b->m31;
	res->m22 = a->m21*b->m12 + a->m22*b->m22 + a->m23*b->m32;
	res->m23 = a->m21*b->m13 + a->m22*b->m23 + a->m23*b->m33;

	res->m31 = a->m31*b->m11 + a->m32*b->m21 + a->m33*b->m31;
	res->m32 = a->m31*b->m12 + a->m32*b->m22 + a->m33*b->m32;
	res->m33 = a->m31*b->m13 + a->m32*b->m23 + a->m33*b->m33;
}

void FxsMatrix3MultiplyVector3(
	FxsVector3* res,
	const FxsMatrix3* a,
	const FxsVector3* b
)
{
	res->x = a->m11*b->x + a->m12*b->y + a->m13*b->z;
	res->y = a->m21*b->x + a->m22*b->y + a->m23*b->z;
	res->z = a->m31*b->x + a->m32*b->y + a->m33*b->z;
}
