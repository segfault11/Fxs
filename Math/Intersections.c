#include "Intersections.h"

#define VEC2ARR(ARR, VEC) ARR[0] = VEC->x; ARR[1] = VEC->y; ARR[2] = VEC->z; 

extern int intersect_triangle(double orig[3], double dir[3], double vert0[3],
	double vert1[3], double vert2[3], double *t, double *u, double *v);

int FxsRayDoesIntersectTriangle(
	const FxsVector3 *orig, 
	const FxsVector3 *dir, 
	float *t,
    float *u,
    float *v,
	const FxsVector3 *v0,
	const FxsVector3 *v1,
	const FxsVector3 *v2
)
{
	double dorig[3], ddir[3], dv0[3], dv1[3], dv2[3];
	double dt, du, dv;
	int res = 0;

	VEC2ARR(dorig, orig); 
	VEC2ARR(ddir, dir); 
	VEC2ARR(dv0, v0); 
	VEC2ARR(dv1, v1); 
	VEC2ARR(dv2, v2); 

	res = intersect_triangle(dorig, ddir, dv0, dv1, dv2, &dt, &du, &dv);

	*t = (float)dt;
	*u = (float)du;
	*v = (float)dv;

	return res;
}
