#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <stdlib.h>	
#include "Vector3D.hpp"

const double 	PI 			= 3.1415926535897932384;
const double 	TWO_PI 		= 6.2831853071795864769;
const double 	PI_ON_180 	= 0.0174532925199432957;
const double 	invPI 		= 0.3183098861837906715;
const double 	invTWO_PI 	= 0.1591549430918953358;

const double	E			= 2.7182818284590452353;

const double 	kEpsilon 	= 0.001; 
const double	kHugeValue	= 200000000;

//colors
const Vector3D white(1, 1, 1);										// white
const Vector3D brown(0.71, 0.40, 0.16);								// brown
const Vector3D darkGreen(0.0, 0.41, 0.41);							// darkGreen
const Vector3D green(0, 0.6, 0.3);									// green
const Vector3D lightGreen(0.65, 1, 0.30);							// light green
const Vector3D orange(1, 0.75, 0);									// orange
const Vector3D red(1,0,0);											// red
const Vector3D lightBlue(0.75,0.75,1);								// light blue
const Vector3D blue(0.0,0.0,1);										// blue
const Vector3D darkBlue(0.25,0.25,1);								// dark blue
const Vector3D darkYellow(0.61, 0.61, 0);							// dark yellow
const Vector3D yellow(1, 1, 0);										// yellow
const Vector3D lightPurple(0.65, 0.3, 1);							// light purple
const Vector3D darkPurple(0.5, 0, 1);								// dark purple
const Vector3D darkGrey(0.25, 0.25, 0.25);							// dark grey
const Vector3D grey(0.5, 0.5, 0.5);									// grey
const Vector3D lightGrey(0.75, 0.75, 0.75);							// light grey
const Vector3D black(0, 0, 0);										// black

const float 	invRAND_MAX = 1.0 / (float)RAND_MAX;

#endif
