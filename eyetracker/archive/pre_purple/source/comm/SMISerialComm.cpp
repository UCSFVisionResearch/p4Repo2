/**
 * @file SMISerialComm.cpp
 * @brief SMI Serial Communication implementation
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include "app/AppCore.h"
#include "SMISerialComm.h"
#include "utils/Utils.h"
#include "suites/calibration/CalibrationStimulus.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CSMISerialComm::CSMISerialComm() {}

CSMISerialComm::~CSMISerialComm() {}

void CSMISerialComm::translate() 
{
	CString instruction = CString(message);
	CString command = instruction.Left(6);

	if (command.MakeUpper() == CString("ET_CAL")) {
		instruction = instruction.Right(instruction.GetLength() - 6).Trim();
		char buffer[10];
		sprintf(buffer, "%s", instruction);
		int type = etu::StringToInt(buffer, 0);
		((CCalibrationStimulus*)(g_pActiveStimulus))->setCalibrationType(type);
		bFirstCommandIgnored = true;
	} 
	else if (command.MakeUpper() == CString("ET_CHG")) {
		instruction = instruction.Right(instruction.GetLength() - 6).Trim();
		char buffer[10];
		sprintf(buffer, "%s", instruction);
		int point = etu::StringToInt(buffer, 0);
		if (point <10)
			((CCalibrationStimulus*)(g_pActiveStimulus))->setActiveCalibrationPoint(point);
	} 
	else if (command.MakeUpper() == CString("ET_FIN")) {
	} 
	else if (command.MakeUpper() == CString("ET_ACC")) {
		((CCalibrationStimulus*)(g_pActiveStimulus))->AcceptPoint();
	} 
	else if (command.MakeUpper() == CString("ET_PNT")) {
		/* Nothing to do */
	}
}

