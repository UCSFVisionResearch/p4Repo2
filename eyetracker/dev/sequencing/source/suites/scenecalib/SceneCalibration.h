/**
 * @file SceneCalibration.h
 * @brief 
 */

#ifndef SCENE_CALIBRATION_H
#define SCENE_CALIBRATION_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CSceneCalibration 
{
protected:
	CString strImagePath;
	float xPosition, yPosition;
		
public:
	CString defaultFile;

	CSceneCalibration();
	~CSceneCalibration();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);

	void setImagePath(CString str) { strImagePath = str; }
	CString getImagePath() { return strImagePath; }

	void setXPosition(float f) { xPosition = f; }
	float getXPosition() { return xPosition; }

	void setYPosition(float f) { yPosition = f; }
	float getYPosition() { return yPosition;}
};

#endif
