/**
 * @file GratingsTrial.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CGratingsTrial* CGratingsTrial::clone() 
{
	CGratingsTrial* gt = new CGratingsTrial(phase, orientation, speed, leftShutterOpen, rightShutterOpen, leftEyeFixating, type);
	gt->locationCode = locationCode;
	return gt;
}

CGratingsTrial::CGratingsTrial(double dPhase, double dOrientation, double dSpeed, bool bLeftShutterOpen, bool bRightShutterOpen, bool bLeftEyeFixating, int iType) 
{
	phase = dPhase;
	orientation = dOrientation;
	speed = dSpeed;
	leftShutterOpen = bLeftShutterOpen;
	rightShutterOpen = bRightShutterOpen;
	leftEyeFixating = bLeftEyeFixating;
	type = iType;
	nPaletteItems=0;
	locationCode=0;
}

CGratingsTrial::~CGratingsTrial() {}

void CGratingsTrial::shuffleRandom(CGratingsTrial** list, int len) 
{
	if (!list) return;
	for (int i=0; i<len; i++) {
		//move down list, swapping what's left in the list
		int r = i+rand()%(len-i);
		//swap r and i
		CGratingsTrial *temp = list[i];
		list[i] = list[r];
		list[r] = temp;
	}
}

CGratingsTrial** CGratingsTrial::randomlyMix(CGratingsTrial** list1, int len1, CGratingsTrial** list2, int len2) 
{
	if (!list1 || !list2) return NULL;
	int i1=0, i2=0;
	int totallen = len1 + len2;
	CGratingsTrial** finallist = new CGratingsTrial* [totallen];
	while (i1+i2 < totallen) {
		//randomly add next point from either list, weighted by length of the lists
		if (rand()%totallen < len1) {
			//use list1
			if (i1 < len1) {
				finallist[i1+i2] = list1[i1];
				i1++;
			}
		} 
		else {
			//use list2
			if (i2 < len2) {
				finallist[i1+i2] = list2[i2];
				i2++;
			}
		}
	}
	delete [] list1;
	delete [] list2;
	return finallist;
}
