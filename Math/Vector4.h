#ifndef VECTOR4_H
#define VECTOR4_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	float x, y, z, w;
}
FxsVector4;

/*
** Initializes the vector v with its components
*/ 
void FxsVector4Make(FxsVector4* v, float x, float y, float z, float w);

/*
** Initializes the vector v to zero
*/
void FxsVector4MakeZero(FxsVector4* v);

/*
** Initializes the vector with a scalar
*/ 
void FxsVector4MakeWithScalar(FxsVector4* v, float s);

/*
** Copies contents of vector src to vector dst
*/ 
void FxsVector4Copy(FxsVector4* dst, const FxsVector4* src); 

/*
** Increments the vector with a scalar
*/ 
void FxsVector4IncrementWithScalar(FxsVector4* v, float s);

/*
** Increments the vector with another vector
*/ 
void FxsVector4Increment(FxsVector4* v, const FxsVector4* i);

/*
** Increments the vector with a scalar
*/ 
void FxsVector4DecrementWithScalar(FxsVector4* v, float s);

/*
** Increments the vector with another vector
*/ 
void FxsVector4Decrement(FxsVector4* v, const FxsVector4* i);

/*
** Adds vector a and b and stores the result in rest
*/ 
void FxsVector4Add(
	FxsVector4* res, 
	const FxsVector4* a, 
	const FxsVector4* b
);

/*
** Substracts vector a and b and stores the result in rest
*/ 
void FxsVector4Substract(
	FxsVector4* res, 
	const FxsVector4* a, 
	const FxsVector4* b
);

/*
** Adds a scalar s to vector a and stores the result in res
*/ 
void FxsVector4AddScalar(
	FxsVector4* res,
	const FxsVector4* a,
	float s
);

/*
** Substracts a scalar s to vector a and stores the result in res
*/ 
void FxsVector4SubstractScalar(
	FxsVector4* res,
	const FxsVector4* a,
	float s
);

/*
** Computes the length of vector a and stores the result in length
*/ 
void FxsVector4Length(float* length, const FxsVector4* a);

/*
** Normalizes the vector v. Returns 0 if the vector cannot be normalized, i.e.
** has length zero
*/ 
int FxsVector4Normalize(FxsVector4* a);

/*
** Computes the dot product between vector a and vector b and stores the result
** in res
*/ 
void FxsVector4Dot(float* res, const FxsVector4* a, const FxsVector4* b);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: VECTOR4_H */
