#include "Projections.h"


void FxsPointProjectOntoLine(
	const FxsVector3 *point,
	FxsVector3 *projPoint,
	const FxsVector3 *origin,
	const FxsVector3 *direction
)
{
	FxsVector3 u;
	float a, b, c;

	FxsVector3Substract(&u, point, origin);
	FxsVector3Dot(&a, &u, direction);
	FxsVector3Dot(&b, direction, direction);
	c = a / b;

	projPoint->x = origin->x + c * direction->x;
	projPoint->y = origin->y + c * direction->y;
	projPoint->z = origin->z + c * direction->z;
}
