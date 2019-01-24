#pragma once

#ifndef Page_h__
#define Page_h__


#include "Arduino.h"
#include "HottModule.h"

class HottModule;

class Page
{
private:
	const char* pageTitle;
	HottModule * module;
	Page* previousPage = 0;
	Page* nextPage = 0;
	void createPage();
	void defineModule(HottModule* _module);
	inline void printLine(int _Line, const char* _Title, int _variable);
	inline void printLine(int _Line, const char* _Title, float _variable);
	inline void printLine(int _Line, const char* _Title, const char* _variable);
	inline void printLine(int _Line, int _centerSpaces, const char* _Title);
	inline void printLine(int _Line, const char * _Title, char * _variable, bool _invVariable);


protected:	
	virtual void onKeyAction(uint8_t keyId);
	char * invertChars(char *str);


public:
	Page(char * _title, HottModule* _module);
	virtual ~Page();
	void setPrevious(Page* _page);
	void setNext(Page* _page);
	virtual void callPage();
	virtual void callPage(uint8_t keyId);
};

#endif // Page_h__