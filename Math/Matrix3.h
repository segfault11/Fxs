#ifndef MATRIX3_H
#define MATRIX3_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Vector3.h"

typedef struct
{ 
	float m11, m21, m31;
	float m12, m22, m32;
	float m13, m23, m33;
}
FxsMatrix3; 

/*
** Initializes the matrix m with its components.
*/ 
void FxsMatrix3Make(
	FxsMatrix3* m, 
	float m11, float m12, float m13,
	float m21, float m22, float m23, 
	float m31, float m32, float m33
); 

/*
** Copies the contents of matrix dst to the matrix src.
*/ 
void FxsMatrix3Copy(FxsMatrix3* dst, const FxsMatrix3* src);

/*
** Multiplies matrix a with matrix b and stores the result in res
*/ 
void FxsMatrix3Multiply(
	FxsMatrix3* res, 
	const FxsMatrix3* a, 
	const FxsMatrix3* b
);

/*
** Multiplies matrix a with vector b and stores the result in res
*/ 
void FxsMatrix3MultiplyVector3(
	FxsVector3* res,
	const FxsMatrix3* a,
	const FxsVector3* b
);


#ifdef __cplusplus
}
#endif

#endif /* end of include guard: MATRIX3_H */
