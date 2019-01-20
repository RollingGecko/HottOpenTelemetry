#pragma once


template <class T>
class Value
{
private:
	T value = 0;
	const char* valueName;
public:
	Value();
	Value(const char* _name);
	~Value();
	void setValue(T _value);
	const char* getName();
	T getValue();
};

template <class T>
Value<T>::Value(const char* _name)
{
	valueName = _name;
}

template <class T>
Value<T>::~Value()
{
}

template<class T>
 void Value<T>::setValue(T _value)
 {
	 value = _value;
}

template <class T>
const char * Value<T>::getName()
{
	return valueName;
}

template<class T>
T  Value<T>::getValue()
{
	return value ;
}






