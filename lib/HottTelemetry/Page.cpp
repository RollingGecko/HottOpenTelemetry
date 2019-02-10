#include "Page.h"

#ifndef UNIT_TEST //ToDo: Make Testable
#include "MessageSender.h"

void Page::onKeyAction(byte keyId)
{
	switch (keyId)
	{
	case HOTT_KEY_RIGHT:
		if (nextPage != nullptr)
		{
			module->setCurrentPage(nextPage);
		}
		break;
	case HOTT_KEY_LEFT:
		if (previousPage != nullptr)
		{
			module->setCurrentPage(previousPage);
		}
		break;
	default:
		break;
	}
}

char * Page::invertChars(char * str)
{
	if (str == 0) return str;
	int len = strlen(str);
	//if ((len < cnt) && cnt > 0) len = cnt;
	for (int i = 0; i < len; i++) {
		str[i] = (byte)(0x80 + (byte)str[i]);
	}
	return str;
}


Page::Page(char * _title, HottModule* _module)
{
	pageTitle = _title;
	module = _module;
}

Page::~Page()
{

}

void Page::setPrevious(Page * _page)
{
	previousPage = _page;
}

void Page::setNext(Page * _page)
{
	nextPage = _page;
}

void Page::callPage()
{
	createPage();
}

void Page::callPage(byte keyId)
{
	onKeyAction(keyId);
	createPage();
}

void Page::createPage()
{
	printLine(0, 3, pageTitle);
	printLine(1, "Line 1 (int): ", 12);
	printLine(2, "Line 2(str): ", "Value", true);
	printLine(3, "Line 3(float): ", (float)12.1);
	//printLine(4, "Line 4: ", 12);
	printLine(5, "Line 5: ", 12);
	printLine(6, "Line 6: ", 12);
	printLine(7, "Line 7: ", 12);
}

void Page::defineModule(HottModule * _module)
{
	module = _module;
}

inline void Page::printLine(int _Line, const char * _Title, const char * _variable)
{
	snprintf((char *) &(module->hott_txt_msg->text[_Line][0]), 21, "%c%s%s", 62, _Title, _variable);
}
inline void Page::printLine(int _Line, const char * _Title, char * _variable, bool _invVariable)
{
	if (_invVariable) {
		_variable = invertChars(_variable);
	}
	snprintf((char *) &(module->hott_txt_msg->text[_Line][0]), 21, "%c%s%s", 62, _Title, _variable);
}

inline void Page::printLine(int _Line, const char* _Title, int _variable)
{
	snprintf((char *) &(module->hott_txt_msg->text[_Line][0]), 21, "%c%s%i", 62, _Title, _variable);
}

inline void Page::printLine(int _Line, const char * _Title, float _variable)
{
	char str_temp[10];
	
	/* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
	dtostrf(_variable, 4, 2, str_temp);
	printLine(_Line, _Title, str_temp);
}

inline void Page::printLine(int _Line, int _centerSpaces, const char * _Title)
{
	char* empty;
	empty = "     ";
	for (int i = 0; i <= _centerSpaces; i++)
	{
		empty[i] = 0x20;
	}
	snprintf((char*)&(module->hott_txt_msg->text[_Line][0]), 21, "%s%s", empty, _Title);
}


#endif