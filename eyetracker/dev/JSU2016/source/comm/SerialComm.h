/**
 * @file SerialComm.h
 * @brief Serial communication object
 */

#ifndef SERIALCOMM_H
#define SERIALCOMM_H

/*****************************************************************************
 * Classes
 *****************************************************************************/

/**
 * @brief Generic serial communication object.
 */
class CSerialComm {
public:
	CSerialComm();
	virtual ~CSerialComm();

	void init(bool bListen, DWORD baudRate, int port);
	void listen();
	void transmit(const char* msg, int nCount);
	virtual void translate();
	void stop();

protected:
	bool bActive;
	char message[511];
	int msgIndex;
	HANDLE hComm;
	bool bFirstCommandIgnored;
};

#endif // SERIALCOMM_H
