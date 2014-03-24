#include "Matrix4.h"
#include <memory.h>
#include <math.h>

static void Matrix4MakePerspective(
	FxsMatrix4* m,
	float l, float r,
	float b, float t,
	float n, float f
)
{
	memset(m, 0, sizeof(FxsMatrix4)); 

	m->m11 = 2.0*n/(r-l);
	m->m13 = (r + l)/(r - l);

	m->m22 = 2.0*n/(t - b);
	m->m23 = (t + b)/(t - b);
	
	m->m33 = -(f + n)/(f - n);
	m->m34 = -2.0*f*n/(f - n);

	m->m43 = -1.0;
}

void FxsMatrix4Make(
	FxsMatrix4* m, 
	float m11, float m12, float m13, float m14, 
	float m21, float m22, float m23, float m24, 
	float m31, float m32, float m33, float m34, 
	float m41, float m42, float m43, float m44
) 
{
	m->m11 = m11;
	m->m12 = m12;
	m->m13 = m13;	
	m->m14 = m14;	

	m->m21 = m21;
	m->m22 = m22;
	m->m23 = m23;	
	m->m24 = m24;	

	m->m31 = m31;
	m->m32 = m32;
	m->m33 = m33;	
	m->m34 = m34;	

	m->m41 = m41;
	m->m42 = m42;
	m->m43 = m43;	
	m->m44 = m44;	
} 

void FxsMatrix4MakeZero(FxsMatrix4* m)
{
	memset(m, 0, sizeof(FxsMatrix4));
}

void FxsMatrix4MakeTranslation(FxsMatrix4* m, float x, float y, float z)
{
	FxsMatrix4MakeZero(m);

	m->m11 = 1.0;
	m->m22 = 1.0;
	m->m33 = 1.0;
	m->m44 = 1.0;
	
	m->m14 = x;
	m->m24 = y;
	m->m34 = z;
}

void FxsMatrix4MakeScale(FxsMatrix4* m, float sx, float sy, float sz)
{
    FxsMatrix4MakeZero(m);
    m->m11 = sx;
    m->m22 = sy;
    m->m33 = sz;
    m->m44 = 1.0;
}


void FxsMatrix4MakePerspective(
	FxsMatrix4* m,
	float fovy,
	float aspect,
	float near,
	float far
)
{
    float t = tanf(fovy/2.0);
    float h = near*t;
    float w = h*aspect;
    
    Matrix4MakePerspective(m, -w, w, -h, h, near, far);
}

void FxsMatrix4MakeWorldToCamera(
	FxsMatrix4* m,
	const FxsVector3* eye,
	const FxsVector3* focus,
	const FxsVector3* up
)
{
	FxsVector3 u, v, n;
	float d;

	FxsVector3Substract(&n, eye, focus);
	FxsVector3Normalize(&n);

	FxsVector3Cross(&u, up, &n);
	FxsVector3Normalize(&u);

	FxsVector3Cross(&v, &n, &u);

	m->m11 = u.x;
	m->m12 = u.y;
	m->m13 = u.z;
	FxsVector3Dot(&d, &u, eye);
	m->m14 = -d;

	m->m21 = v.x;
	m->m22 = v.y;
	m->m23 = v.z;
	FxsVector3Dot(&d, &v, eye);
	m->m24 = -d;

	m->m31 = n.x;
	m->m32 = n.y;
	m->m33 = n.z;
	FxsVector3Dot(&d, &n, eye);
	m->m34 = -d;

	m->m41 = 0.0; 
	m->m42 = 0.0;
	m->m43 = 0.0;
	m->m44 = 1.0;
}

void FxsMatrix4MakeCameraToWorld(
	FxsMatrix4* m,
	const FxsVector3* eye,
	const FxsVector3* focus,
	const FxsVector3* up
)
{
	FxsVector3 u, v, n;

	FxsVector3Substract(&n, eye, focus);
	FxsVector3Normalize(&n);

	FxsVector3Cross(&u, up, &n);
	FxsVector3Normalize(&u);

	FxsVector3Cross(&v, &n, &u);

	m->m11 = u.x;
	m->m21 = u.y;
	m->m31 = u.z;
	m->m41 = 0.0;

	m->m12 = v.x;
	m->m22 = v.y;
	m->m32 = v.z;
	m->m42 = 0.0;

	m->m13 = u.x;
	m->m23 = u.y;
	m->m33 = u.z;
	m->m43 = 0.0;

	m->m14 = eye->x;
	m->m24 = eye->y;
	m->m34 = eye->z;
	m->m44 = 1.0;
}

void FxsMatrix4MakeRotationWithQuaternion(FxsMatrix4* m, const FxsQuaternion* q)
{
	FxsMatrix4MakeZero(m);
	m->m44 = 1.0;
	
	m->m11 = 1.0 - 2.0*q->y*q->y - 2.0*q->z*q->z;
	m->m12 = 2.0*q->x*q->y - 2.0*q->z*q->w;
	m->m13 = 2.0*q->x*q->z + 2.0*q->y*q->w;

	m->m21 = 2.0*q->x*q->y + 2.0*q->z*q->w;
	m->m22 = 1.0 - 2.0*q->x*q->x - 2.0*q->z*q->z;
	m->m23 = 2.0*q->y*q->z - 2.0*q->x*q->w;

	m->m31 = 2.0*q->x*q->z - 2.0*q->y*q->w;
	m->m32 = 2.0*q->y*q->z + 2.0*q->x*q->w;
	m->m33 = 1.0 - 2.0*q->x*q->x - 2.0*q->y*q->y;
}


void FxsMatrix4Copy(FxsMatrix4* dst, const FxsMatrix4* src)
{
	memcpy(dst, src, sizeof(FxsMatrix4));
}

void FxsMatrix4Multiply(
	FxsMatrix4* res, 
	const FxsMatrix4* a, 
	const FxsMatrix4* b
)
{
	res->m11 = a->m11*b->m11 + a->m12*b->m21 + a->m13*b->m31 + a->m14*b->m41;
	res->m12 = a->m11*b->m12 + a->m12*b->m22 + a->m13*b->m32 + a->m14*b->m42;
	res->m13 = a->m11*b->m13 + a->m12*b->m23 + a->m13*b->m33 + a->m14*b->m43;
	res->m14 = a->m11*b->m14 + a->m12*b->m24 + a->m13*b->m34 + a->m14*b->m44;

	res->m21 = a->m21*b->m11 + a->m22*b->m21 + a->m23*b->m31 + a->m24*b->m41;
	res->m22 = a->m21*b->m12 + a->m22*b->m22 + a->m23*b->m32 + a->m24*b->m42;
	res->m23 = a->m21*b->m13 + a->m22*b->m23 + a->m23*b->m33 + a->m24*b->m43;
	res->m24 = a->m21*b->m14 + a->m22*b->m24 + a->m23*b->m34 + a->m24*b->m44;

	res->m31 = a->m31*b->m11 + a->m32*b->m21 + a->m33*b->m31 + a->m34*b->m41;
	res->m32 = a->m31*b->m12 + a->m32*b->m22 + a->m33*b->m32 + a->m34*b->m42;
	res->m33 = a->m31*b->m13 + a->m32*b->m23 + a->m33*b->m33 + a->m34*b->m43;
	res->m34 = a->m31*b->m14 + a->m32*b->m24 + a->m33*b->m34 + a->m34*b->m44;

	res->m41 = a->m41*b->m11 + a->m42*b->m21 + a->m43*b->m31 + a->m44*b->m41;
	res->m42 = a->m41*b->m12 + a->m42*b->m22 + a->m43*b->m32 + a->m44*b->m42;
	res->m43 = a->m41*b->m13 + a->m42*b->m23 + a->m43*b->m33 + a->m44*b->m43;
	res->m44 = a->m41*b->m14 + a->m42*b->m24 + a->m43*b->m34 + a->m44*b->m44;
}

void FxsMatrix4MultiplyVector4(
	FxsVector4* res,
	const FxsMatrix4* a,
	const FxsVector4* b
)
{
	res->x = a->m11*b->x + a->m12*b->y + a->m13*b->z + a->m14*b->w;
	res->y = a->m21*b->x + a->m22*b->y + a->m23*b->z + a->m24*b->w;
	res->z = a->m31*b->x + a->m32*b->y + a->m33*b->z + a->m34*b->w;
	res->w = a->m41*b->x + a->m42*b->y + a->m43*b->z + a->m44*b->w;
}

void FxsMatrix4MultiplyVector3(
	FxsVector4* res,
	const FxsMatrix4* a,
	const FxsVector3* b
)
{
	res->x = a->m11*b->x + a->m12*b->y + a->m13*b->z + a->m14;
	res->y = a->m21*b->x + a->m22*b->y + a->m23*b->z + a->m24;
	res->z = a->m31*b->x + a->m32*b->y + a->m33*b->z + a->m34;
	res->w = a->m41*b->x + a->m42*b->y + a->m43*b->z + a->m44;
}

#define SWAP(x, y, t) t = x; y = x; x = t;

void FxsMatrix4Transpose(FxsMatrix4* m)
{
    float t;
    
    SWAP(m->m12, m->m21, t)
    SWAP(m->m13, m->m31, t)
    SWAP(m->m14, m->m41, t)

    SWAP(m->m23, m->m32, t)
    SWAP(m->m24, m->m42, t)

    SWAP(m->m34, m->m43, t)
}

/*
**  stolen from the mesa opengl implementation
*/
void FxsMatrix4Invert(FxsMatrix4* data)
{
    float inv[16], det;
    float* m = (float*)data;

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
    det = 1.0/det;

    for (int i = 0; i < 16; i++)
    {
        m[i] = inv[i] * det;
    }

}
