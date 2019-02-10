#ifdef UNIT_TEST

#include "Value.h"
#include <gtest/gtest.h>

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

//Main Test runner
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#endif