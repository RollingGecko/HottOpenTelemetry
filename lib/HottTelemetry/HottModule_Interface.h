#ifndef HottModule_Interface_h__
#define HottModule_Interface_h__

#include "Module.h"
#ifdef UNIT_TEST
#include "Mock_Arduino.h"
#endif
#include "Page.h"

class Page;

class HottModule_Interface : public Module
{
	
public:
	// HottModule_Interface();
	// virtual ~HottModule_Interface();
	
	virtual void init_BinMsg() = 0;
	virtual void initTxtMsg() = 0;
	virtual void createBinMessage() = 0;
	virtual void createTxtMessage(byte keyId) = 0;
	virtual int getBinMessageSize() = 0;
	virtual uint8_t* getBinMessage() = 0;
	virtual uint8_t* getTxtMessage() = 0;
	virtual int getTxtMessageSize() = 0;
	virtual void setCurrentPage(Page* _page) = 0;
	virtual void setDummyMessage(bool onOff) = 0;
	virtual void set_Alert(uint8_t alarm)=0;
};

#endif