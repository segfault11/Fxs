#ifndef VECTOR2_H
#define VECTOR2_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	float x, y;
}
FxsVector2;

/*
** Initializes the vector v with its components
*/ 
void FxsVector2Make(FxsVector2* v, float x, float y);

/* 
** Initializes the vector to zero
*/
void FxsVector2MakeZero(FxsVector2* v);

/*
** Initializes the vector with a scalar
*/ 
void FxsVector2MakeWithScalar(FxsVector2* v, float s);

/*
** Copies contents of vector src to vector dst
*/ 
void FxsVector2Copy(FxsVector2* dst, const FxsVector2* src); 

/*
** Increments the vector with a scalar
*/ 
void FxsVector2IncrementWithScalar(FxsVector2* v, float s);

/*
** Increments the vector with another vector
*/ 
void FxsVector2Increment(FxsVector2* v, const FxsVector2* i);

/*
** Increments the vector with a scalar
*/ 
void FxsVector2DecrementWithScalar(FxsVector2* v, float s);

/*
** Increments the vector with another vector
*/ 
void FxsVector2Decrement(FxsVector2* v, const FxsVector2* i);

/*
** Adds vector a and b and stores the result in rest
*/ 
void FxsVector2Add(
	FxsVector2* res,
	const FxsVector2* a,
	const FxsVector2* b
);

/*
** Substracts vector a and b and stores the result in rest
*/ 
void FxsVector2Substract(
	FxsVector2* res,
	const FxsVector2* a,
	const FxsVector2* b
);

/*
** Adds a scalar s to vector a and stores the result in res
*/ 
void FxsVector2AddScalar(
	FxsVector2* res,
	const FxsVector2* a,
	float s
);

/*
** Substracts a scalar s to vector a and stores the result in res
*/ 
void FxsVector2SubstractScalar(
	FxsVector2* res,
	const FxsVector2* a,
	float s
);

/*
** Copies contents of vector src to vector dst
*/ 
void FxsVector2Copy(FxsVector2* dst, const FxsVector2* src); 
/*
** Computes the length of vector a and stores the result in length
*/ 
void FxsVector2Length(float* length, const FxsVector2* a);

/*
** Normalizes the vector v. Returns 0 if the vector cannot be normalized, i.e.
** has length zero
*/ 
int FxsVector2Normalize(FxsVector2* a);

/*
** Computes the dot product between vector a and vector b and stores the result
** in res
*/ 
void FxsVector2Dot(float* res, const FxsVector2* a, const FxsVector2* b);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: VECTOR2_H */
