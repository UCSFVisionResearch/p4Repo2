/**
 * @file GratingsTrial.h
 * @brief 
 */

#ifndef GRATINGSTRIAL_H
#define GRATINGSTRIAL_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CGratingsTrial 
{
public:
	CGratingsTrial(double dPhase, double dOrientation, double dSpeed, bool bLeftShutterOpen, bool bRightShutterOpen, bool bLeftEyeFixating, int iType);
	~CGratingsTrial();
	static void shuffleRandom(CGratingsTrial** list, int len);
	static CGratingsTrial** randomlyMix(CGratingsTrial** list1, int len1, CGratingsTrial** list2, int len2);
	CGratingsTrial* clone();

	double phase;
	double orientation;
	double speed;
	bool leftShutterOpen;
	bool rightShutterOpen;
	bool leftEyeFixating;
	int type;
	int nPaletteItems;
	int locationCode;//0=default/unused, 1=top left, 2=top right, 3=bottom left, 4=bottom right

	const static int TYPE_SQUAREWAVE = 0;
	const static int TYPE_SINEWAVE = 1;
	const static int TYPE_GABOR_BOTH = 2;
	const static int TYPE_GABOR_LEFT = 3;
	const static int TYPE_GABOR_RIGHT = 4;
	const static int TYPE_AUTORFMAP = 5;
};

#endif // GRATINGSTRIAL_H
