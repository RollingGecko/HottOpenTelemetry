
#ifdef UNIT_TEST
#include "SensorAlarm.h"
#include "Mock_HottModule.h"
#include <gtest/gtest.h>
#include <stdint.h>

#define GTEST

//Testing Class SensorAlarm

//Fixture for Class Sensor Alarm

class SensorAlarmClassTest : public testing::Test 
{
public:
	SensorAlarm<uint8_t>* minAlarm;
	SensorAlarm<uint8_t>* maxAlarm;
	Mock_HottModule* module;
	SensorAlarmClassTest() {
		minAlarm = new SensorAlarm<uint8_t>("MinAlarm", module, 0x02);
		minAlarm = new SensorAlarm<uint8_t>("MinAlarm", module, 0x02, true);
	}
	~SensorAlarmClassTest() {}

};

TEST_F(SensorAlarmClassTest, toggleAlarmTone) {
	bool previousSetting = minAlarm->getAlarmToneSetting();
	minAlarm->toggleAlarmTone();	
	EXPECT_NE(previousSetting, minAlarm->getAlarmToneSetting());
	minAlarm->toggleAlarmTone();
	EXPECT_EQ(previousSetting, minAlarm->getAlarmToneSetting());
}


//Main Test runner
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#endif