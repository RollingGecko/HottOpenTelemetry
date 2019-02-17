
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Mock_HottModule.h"
#include "Value.h"
#include "SensorAlarm.h"
#include <stdint.h>

using ::testing::_;

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
		maxAlarm = new SensorAlarm<uint8_t>("MaxAlarm", module, 0x02, true);
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

TEST_F(SensorAlarmClassTest, triggerAlarmOnce){
	EXPECT_CALL(*module, set_Alert(_))
		.Times(1);
	EXPECT_TRUE(minAlarm->triggerAlarm());
}



//Main Test runner
int main(int argc, char **argv) {
	 ::testing::GTEST_FLAG(throw_on_failure) = true;
	::testing::InitGoogleMock(&argc, argv);
	//  ::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#endif