#ifndef MATRIX2_H
#define MATRIX2_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Vector2.h"

typedef struct
{ 
	float m11, m21;
	float m12, m22;
}
FxsMatrix2; 

/*
** Initializes the matrix m with its components.
*/ 
void FxsMatrix2Make(
	FxsMatrix2* m, 
	float m11, float m12,
	float m21, float m22
); 

/*
** Copies the contents of matrix dst to the matrix src.
*/ 
void FxsMatrix2Copy(FxsMatrix2* dst, const FxsMatrix2* src);

/*
** Multiplies matrix a with matrix b and stores the result in res
*/ 
void FxsMatrix2Multiply(
	FxsMatrix2* res, 
	const FxsMatrix2* a, 
	const FxsMatrix2* b
);

/*
** Multiplies matrix a with vector b and stores the result in res
*/ 
void FxsMatrix2MultiplyVector2(
	FxsVector2* res,
	const FxsMatrix2* a,
	const FxsVector2* b
);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: MATRIX2_H */
