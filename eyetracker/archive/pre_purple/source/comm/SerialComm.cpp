/**
 * @file SerialComm.cpp
 * @brief Generic Serial Communication implementation
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include "SerialComm.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CSerialComm::CSerialComm() {}

CSerialComm::~CSerialComm() {}

/** Open serial communication. */
void CSerialComm::init(bool bListen, DWORD baudRate, int port) 
{
	char portStr[10];
	sprintf(portStr, "COM%d\0", port);
	hComm = CreateFile(portStr, 
						GENERIC_READ | GENERIC_WRITE, 
						0, 
						0, 
						OPEN_EXISTING, 
						0, 
						NULL); //0
	if (hComm == INVALID_HANDLE_VALUE) {
		/* Error when initializing SMISerialComm for calibration; ignore. */
		return;
	}

	DCB dcb;
	ZeroMemory(&dcb, sizeof(dcb));
	if (!GetCommState(hComm, &dcb)) {     // get current DCB
		DWORD err = GetLastError();
		char buf[1024];
		sprintf(buf, "Error in getting comm state: %d\0", err);
		AfxMessageBox(buf);
		return;
	}

	/* Update DCB rate. */
	dcb.BaudRate = baudRate;
	dcb.StopBits = ONESTOPBIT;
	dcb.Parity = NOPARITY;
	dcb.ByteSize = 8;

	/* Set new state. */
	if (!SetCommState(hComm, &dcb)) {
		AfxMessageBox("could not set comm state");
	}

	if (hComm == INVALID_HANDLE_VALUE) {
		AfxMessageBox("Failed to open communications port");
	} 
	else {
		bFirstCommandIgnored = false;
		bActive = true;
		if (bListen) listen();
	}
}

void CSerialComm::listen() {
	DWORD dwRead;
	char chRead;
	bool bResult;

	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;

	if (!SetCommTimeouts(hComm, &timeouts)) {
		AfxMessageBox("error setting comm timeouts");
	} 
	else {
		msgIndex = 0;
		while (bActive) {
			bool bRead = true;
			do {
				bResult = ReadFile(hComm, &chRead, 1, &dwRead, NULL);
				if (bResult && dwRead > 0) {
					/* process byte */
					if (chRead == 10 || chRead == 13) { // end of command
						/* translate command, and wait for next command */
						translate();
						memset(&message, 0, 511);
						msgIndex = 0;
						bRead = false;
					} 
					else {
						/* save byte in command buffer */
						if (msgIndex < 510) {
							message[msgIndex] = chRead;
							msgIndex++;
							message[msgIndex] = 0;
						}
					}
				}				
				Sleep(10);
			} 
			while (bActive && bRead);
		}
	}
}

// this function should never be called -- all subclasses should over ride
void CSerialComm::translate() {
}


void CSerialComm::transmit(char* msg, int nCount) 
{
	DWORD dwWritten;
	bool bResult = WriteFile(hComm, msg, nCount, &dwWritten, NULL);
}


void CSerialComm::stop() 
{
	if (bActive) {
		CloseHandle(hComm);
		bActive = false;
	}
}