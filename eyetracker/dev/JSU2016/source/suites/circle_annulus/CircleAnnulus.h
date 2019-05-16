/**
 * @file CircleAnnulus.h
 * @brief 
 */

#ifndef CIRCLE_ANNULUS_H
#define CIRCLE_ANNULUS_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>

/*****************************************************************************
 * Typedefs and Structs
 *****************************************************************************/

typedef struct myColorStruct {
	float red;
	float green;
	float blue;
	char listString[100];
} ColorListItem;

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CCircleAnnulus 
{
protected:
	int iIntertrialDuration, iTrialDuration;
	int blankPeriod;
	int iRepeats;
	
	float fXPosition, fYPosition;
	CString unitTag;

	float circleRadiusMin, circleRadiusMax, circleRadiusStep;
	float annulusWidthMin, annulusWidthMax, annulusWidthStep;
	ColorListItem* pColorList;
	ColorListItem* pSecondaryColorList;
	int iNumColors, iNumSecondaryColors;
	int iColorListLength, iSecondaryColorListLength;
	
	int iReceptiveField;
	int iShape;
	bool bOnOff;

public:
	static const int SHAPES_CIRCLES = 0;
	static const int SHAPES_ANNULI = 1;
	static const int SHAPES_CIRCLE_ANNULI = 2;
	
	static const int FIELD_RIGHT_EYE = 0;
	static const int FIELD_LEFT_EYE = 1;
	static const int FIELD_INTERLEAVED = 2;
	static const int FIELD_MANUAL = 3;

	//CString defaultFile;
	CCircleAnnulus();
	~CCircleAnnulus();

	void addColor(float red, float green, float blue);
	void removeColor(int i);
	void clearColors();
	CString getColorString(int i);
	ColorListItem getColor(int i);
	void addSecondaryColor(float red, float green, float blue);
	void removeSecondaryColor(int i);
	void clearSecondaryColors();
	CString getSecondaryColorString(int i);
	ColorListItem getSecondaryColor(int i);
	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);        

	int getIntertrialDuration() { return iIntertrialDuration; }
	void setIntertrialDuration(int i) { iIntertrialDuration = i; }
	
	int getTrialDuration() { return iTrialDuration; }
	void setTrialDuration(int i) { iTrialDuration = i; }

	int getRepeats() { return iRepeats; }
	void setRepeats(int i) { iRepeats = i; }

	float getXPosition() { return fXPosition; }
	void setXPosition(float f) { fXPosition = f; }

	float getYPosition() { return fYPosition; }
	void setYPosition(float f) { fYPosition = f; }

	CString getUnitTag() { return unitTag; }
	void setUnitTag(CString cstr) { unitTag = cstr; }
	
	float getCircleRadiusMin() { return circleRadiusMin; }
	void setCircleRadiusMin(float f) { circleRadiusMin  = f; }

	float getCircleRadiusMax() { return circleRadiusMax; }
	void setCircleRadiusMax(float f) { circleRadiusMax  = f; }

	float getCircleRadiusStep() { return circleRadiusStep; }
	void setCircleRadiusStep(float f) { circleRadiusStep  = f; }
	
	float getAnnulusWidthMin() { return annulusWidthMin; }
	void setAnnulusWidthMin(float f) { annulusWidthMin  = f; }

	float getAnnulusWidthMax() { return annulusWidthMax; }
	void setAnnulusWidthMax(float f) { annulusWidthMax  = f; }

	float getAnnulusWidthStep() { return annulusWidthStep; }
	void setAnnulusWidthStep(float f) { annulusWidthStep  = f; }

	int getBlankPeriod() { return blankPeriod; }
	void setBlankPeriod(int i) { blankPeriod = i; }

	bool getOnOff() { return bOnOff; }
	void setOnOff(bool b) { bOnOff = b; }

	int getField() { return iReceptiveField; }
	void setField(int i) { iReceptiveField = i; }

	int getShape() { return iShape; }
	void setShape(int i) { iShape = i; }

	int getNumSecondaryColors() { return iNumSecondaryColors; }

	int getNumColors() { return iNumColors; }
};

#endif
