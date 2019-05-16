/**
 * @file SMISerialComm.h
 * @brief SMI serial comm type
 */

#ifndef SMISERIALCOMM_H
#define SMISERIALCOMM_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

/**
 * @brief Derived class to use the SMI serial.
 */
class CSMISerialComm : public CSerialComm {
public:
	CSMISerialComm();
	virtual ~CSMISerialComm();
	
	virtual void translate();
	
};

#endif // SMISERIALCOMM_H
