#pragma once

#include "script.h"

using namespace std;

#include <iostream>
#include <iterator>
#include <map>

class BaseDataFile
{
protected:
	const char* filename;
	bool hasChanges;

public:
	BaseDataFile(const char* filename);
	const char* getFilename();
	virtual bool load();
	virtual bool save(bool force = false);
	virtual bool isTouched();

protected:
	virtual bool handleLineInput(string* line) = 0;
	virtual bool writeData() = 0;

};