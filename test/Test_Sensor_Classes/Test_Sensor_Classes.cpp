#include <gtest/gtest.h>
//#include <gmock/gmock.h>

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

class SensorAlarmClassTest_Basic : public testing::Test 
{
public:
	SensorAlarm<uint8_t>* minAlarm;
	SensorAlarm<uint8_t>* maxAlarm;
	//Mock_HottModule* module = new Mock_HottModule();
	Mock_HottModule module;
	SensorAlarmClassTest_Basic() {
		minAlarm = new SensorAlarm<uint8_t>("MinAlarm", &module, 0x02);
		maxAlarm = new SensorAlarm<uint8_t>("MaxAlarm", &module, 0x01, true);
	}
	~SensorAlarmClassTest_Basic() {}

};

TEST_F(SensorAlarmClassTest_Basic, toggleAlarmTone) {
	bool previousSetting = minAlarm->getAlarmToneSetting();
	minAlarm->toggleAlarmTone();	
	EXPECT_NE(previousSetting, minAlarm->getAlarmToneSetting());
	minAlarm->toggleAlarmTone();
	EXPECT_EQ(previousSetting, minAlarm->getAlarmToneSetting());
}

TEST_F(SensorAlarmClassTest_Basic, triggerAlarmOnce){
	EXPECT_CALL(module, set_Alert(0x02));
	minAlarm->triggerAlarm();
}

class SensorAlarmClassTest_uint8_t : public testing::Test
{
public:
	SensorAlarm<uint8_t>* minAlarm;
	SensorAlarm<uint8_t>* maxAlarm;
	//Mock_HottModule* module = new Mock_HottModule();
	Mock_HottModule module;
	SensorAlarmClassTest_uint8_t() {
		minAlarm = new SensorAlarm<uint8_t>("MinAlarm", &module, 0x02);
		maxAlarm = new SensorAlarm<uint8_t>("MaxAlarm", &module, 0x01, true);
	}
	~SensorAlarmClassTest_uint8_t() {}
};

TEST_F(SensorAlarmClassTest_uint8_t, checkMinValue) {
	minAlarm->setValue(4);
	//Expect that method returns true if checkAlarm is positive
	// is < then
	EXPECT_CALL(module, set_Alert(0x02));
	EXPECT_EQ(true, minAlarm->checkAlarm(3)); 
	//is = then
	EXPECT_CALL(module, set_Alert(0x02));
	EXPECT_EQ(true, minAlarm->checkAlarm(4));
	//is > then
	EXPECT_EQ(false, minAlarm->checkAlarm(5));
}

TEST_F(SensorAlarmClassTest_uint8_t, checkMaxValue) {
	maxAlarm->setValue(10);
	//Expect that method returns true if checkAlarm is positive
	// is > then
	EXPECT_CALL(module, set_Alert(0x01));
	EXPECT_EQ(true, maxAlarm->checkAlarm(11));
	//is = then
	EXPECT_CALL(module, set_Alert(0x01));
	EXPECT_EQ(true, maxAlarm->checkAlarm(10));
	//is < then
	EXPECT_EQ(false, maxAlarm->checkAlarm(9));
}

TEST_F(SensorAlarmClassTest_uint8_t, checkMinValue_atBoundary) {
	minAlarm->setValue(0);
	//Expect that method returns true if checkAlarm is positive
	// is < then
	EXPECT_CALL(module, set_Alert(0x02));
	EXPECT_EQ(true, minAlarm->checkAlarm(-1));
	//is = then
	EXPECT_CALL(module, set_Alert(0x02));
	EXPECT_EQ(true, minAlarm->checkAlarm(0));
	//is > then
	EXPECT_EQ(false, minAlarm->checkAlarm(1));
}

TEST_F(SensorAlarmClassTest_uint8_t, checkMaxValue_atBoundary) {
	maxAlarm->setValue(255);
	//Expect that method returns true if checkAlarm is positive
	// is > then
	EXPECT_CALL(module, set_Alert(0x01));
	EXPECT_EQ(true, maxAlarm->checkAlarm(256));
	//is = then
	EXPECT_CALL(module, set_Alert(0x01));
	EXPECT_EQ(true, maxAlarm->checkAlarm(255));
	//is < then
	EXPECT_EQ(false, maxAlarm->checkAlarm(254));
}


//Main Test runner
int main(int argc, char **argv) {
	 ::testing::GTEST_FLAG(throw_on_failure) = true;
	::testing::InitGoogleMock(&argc, argv);
	//  ::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
