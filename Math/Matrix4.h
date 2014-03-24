#ifndef MATRIX4_H
#define MATRIX4_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

typedef struct
{ 
	float m11, m21, m31, m41;
	float m12, m22, m32, m42;
	float m13, m23, m33, m43;
	float m14, m24, m34, m44;
}
FxsMatrix4; 

/*
** Initializes the matrix m with its components.
*/ 
void FxsMatrix4Make(
	FxsMatrix4* m, 
	float m11, float m12, float m13, float m14, 
	float m21, float m22, float m23, float m24, 
	float m31, float m32, float m33, float m34, 
	float m41, float m42, float m43, float m44
); 

/*
** Initializes the matrix to zero
*/ 
void FxsMatrix4MakeZero(FxsMatrix4* m);

/*
** Initializes the matrix as a translation matrix.
*/ 
void FxsMatrix4MakeTranslation(FxsMatrix4* m, float x, float y, float z);

/*
** Initialize the matrix as a scale matrix
*/
void FxsMatrix4MakeScale(FxsMatrix4* m, float sx, float sy, float sz);

/*
** Initializes the matrix to a world to camera matrix
*/ 
void FxsMatrix4MakeWorldToCamera(
	FxsMatrix4* m,
	const FxsVector3* eye,
	const FxsVector3* focus,
	const FxsVector3* up
);

/*
** Initializes the matrix to a camera to world matrix 
*/ 
void FxsMatrix4MakeCameraToWorld(
	FxsMatrix4* m,
	const FxsVector3* eye,
	const FxsVector3* focus,
	const FxsVector3* up
);

/*
** Initializes the camera to a perspective projection matrix.
*/ 
void FxsMatrix4MakePerspective(
	FxsMatrix4* m,
	float fovy,
	float aspect,
	float near,
	float far
);

/*
** Initializes the matrix as a rotation matrix using a quaternion.
*/ 
void FxsMatrix4MakeRotationWithQuaternion(FxsMatrix4* m, const FxsQuaternion* q);

/*
** Copies the contents of matrix dst to the matrix src.
*/ 
void FxsMatrix4Copy(FxsMatrix4* dst, const FxsMatrix4* src);

/*
** Multiplies matrix a with matrix b and stores the result in res
*/ 
void FxsMatrix4Multiply(
	FxsMatrix4* res, 
	const FxsMatrix4* a, 
	const FxsMatrix4* b
);

/*
** Multiplies matrix a with vector b and stores the result in res
*/ 
void FxsMatrix4MultiplyVector4(
	FxsVector4* res,
	const FxsMatrix4* a,
	const FxsVector4* b
);

/*
** Multiplies matrix a with a 3D vector b and stores the result in res
*/ 
void FxsMatrix4MultiplyVector3(
	FxsVector4* res,
	const FxsMatrix4* a,
	const FxsVector3* b
);

/*
**  Inverts the matrix.
*/
void FxsMatrix4Invert(FxsMatrix4* m);

/*
**  Transposes the matrix.
*/
void FxsMatrix4Transpose(FxsMatrix4* m);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: MATRIX4_H */
