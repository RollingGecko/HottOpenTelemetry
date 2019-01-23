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
	inline void printLine(uint8_t _Line, const char* _Title, uint8_t _variable);
	//inline void printLine(uint8_t _Line, const char* _Title, float _variable); //ToDo: Überladung klappt nicht. Aber erst nach Umstellung auf uint8_t
	inline void printLine(uint8_t _Line, const char* _Title, const char* _variable);
	inline void printLine(uint8_t _Line, uint8_t _centerSpaces, const char* _Title);
	inline void printLine(uint8_t _Line, const char * _Title, char * _variable, bool _invVariable);


protected:	
	virtual void onKeyAction(char keyId);
	char * invertChars(char *str);


public:
	Page(char * _title, HottModule* _module);
	virtual ~Page();
	void setPrevious(Page* _page);
	void setNext(Page* _page);
	virtual void callPage();
	virtual void callPage(char keyId);
};

#endif // Page_h__