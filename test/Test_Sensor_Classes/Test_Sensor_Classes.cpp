
#ifdef UNIT_TEST
#include "Mock_HottModule.h"
#include "Value.h"
#include "SensorAlarm.h"
#include <gtest/gtest.h>
#include <stdint.h>

#define GTEST

// #define GTEST

//Testing Class Value

//Fixture for testing class Value
class ValueClassTest :public testing::Test {
protected:
	
	const char* valueName = "TestValue";
	Value<uint8_t>* val;

	ValueClassTest() {
		
		val = new Value<uint8_t>(valueName);
		//Value<uint8_t> value(valueName);
	}
	~ValueClassTest() {
	
	}
};
TEST(ValueClass, ConstructorByName) {
	const char name[] = "ValueClassName";
	Value<uint8_t> val(name);

	EXPECT_STREQ(name, val.getName());
}

TEST_F(ValueClassTest, getName) {
	EXPECT_STREQ(valueName, val->getName());
}

TEST_F(ValueClassTest, set_uint8Value) {
	val->setValue(100);
	EXPECT_EQ(100, val->getValue());
}

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