#pragma once

#ifndef HottModule_h__
#define HottModule_h__

#include "Mock_Arduino.h"
#include "HottModule_Interface.h"
#include "gmock/gmock.h"

#include <stdint.h>

class Mock_HottModule : public HottModule_Interface{

public:
Mock_HottModule();
~Mock_HottModule();

MOCK_METHOD0(init_BinMsg, void());
MOCK_METHOD0(initTxtMsg,void());
MOCK_METHOD0(createBinMessage,void());
MOCK_METHOD1(createTxtMessage,void(byte keyId));
MOCK_METHOD0(getBinMessageSize,int());
MOCK_METHOD0(getBinMessage,uint8_t*());
MOCK_METHOD0(getTxtMessage,uint8_t*());
MOCK_METHOD0(getTxtMessageSize,int());
MOCK_METHOD1(setCurrentPage, void(Page* _page));
MOCK_METHOD1(setDummyMessage, void(bool onOff));
MOCK_METHOD1(set_Alert,void(uint8_t alarm));
};

#endif