#ifndef __MATRIX_H_
#define __MATRIX_H_

typedef float MATRIX3[9];

typedef float MATRIX4[16];

/**
@brief performs a dot product to these two matricies
@param the multiplier
@param the multiplicant
@returns a dot product
*/

float matrix4_dot( MATRIX4 a,
				   MATRIX4 b
				 );
#endif