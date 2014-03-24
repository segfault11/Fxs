#ifndef VECTOR3_H
#define VECTOR3_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	float x, y, z;
}
FxsVector3;

/*
** Initializes the vector v with its components
*/ 
void FxsVector3Make(FxsVector3* v, float x, float y, float z);

/*
** Initializes the vector to zero
*/
void FxsVector3MakeZero(FxsVector3* v);

/*
** Initializes the vector with a scalar
*/ 
void FxsVector3MakeWithScalar(FxsVector3* v, float s);

/*
** Copies contents of vector src to vector dst
*/ 
void FxsVector3Copy(FxsVector3* dst, const FxsVector3* src); 

/*
** Increments the vector with a scalar
*/ 
void FxsVector3IncrementWithScalar(FxsVector3* v, float s);

/*
** Increments the vector with another vector
*/ 
void FxsVector3Increment(FxsVector3* v, const FxsVector3* i);

/*
** Increments the vector with a scalar
*/ 
void FxsVector3DecrementWithScalar(FxsVector3* v, float s);

/*
** Increments the vector with another vector
*/ 
void FxsVector3Decrement(FxsVector3* v, const FxsVector3* i);

/*
** Adds vector a and b and stores the result in rest
*/ 
void FxsVector3Add(
	FxsVector3* res, 
	const FxsVector3* a, 
	const FxsVector3* b
);

/*
** Substracts vector a and b and stores the result in rest
*/ 
void FxsVector3Substract(
	FxsVector3* res, 
	const FxsVector3* a, 
	const FxsVector3* b
);

/*
** Adds a scalar s to vector a and stores the result in res
*/ 
void FxsVector3AddScalar(
	FxsVector3* res,
	const FxsVector3* a,
	float s
);

/*
** Substracts a scalar s to vector a and stores the result in res
*/ 
void FxsVector3SubstractScalar(
	FxsVector3* res,
	const FxsVector3* a,
	float s
);

/*
** Computes the length of vector a and stores the result in length
*/ 
void FxsVector3Length(float* length, const FxsVector3* a);

/*
** Normalizes the vector v. Returns 0 if the vector cannot be normalized, i.e.
** has length zero
*/ 
int FxsVector3Normalize(FxsVector3* a);

/*
** Computes the dot product between vector a and vector b and stores the result
** in res
*/ 
void FxsVector3Dot(float* res, const FxsVector3* a, const FxsVector3* b);

/*
** Computes the cross product between vector a and vector b and stores the 
** result in res
*/ 
void FxsVector3Cross(FxsVector3* res, const FxsVector3* a, const FxsVector3* b);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: VECTOR3_H */
