/**
 * @file Ruler.h
 * @brief 
 */

#ifndef RULER_H
#define RULER_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CRuler
{
protected:
	int iNumRings;
	bool bBox;

public:
	CString defaultFile;
	
	CRuler();
	~CRuler();
	
	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);

	int getNumRings() {return iNumRings;}
	void setNumRings(int i) {iNumRings=i;}

	bool getBox() {return bBox;}
	void setBox(bool b) {bBox = b;}
};

#endif
