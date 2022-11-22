#pragma once

#include "script.h"

#include <iostream>
#include <iterator>
#include <map>

#include <vector>

using namespace std;

class DictDataFile : public BaseDataFile
{
private:
	map<string, string> cache;
	map<string, vector<const char*>*> lists;

public:
	DictDataFile(const char* filename);

	const char* get(const char* key);
	int getInt(const char* key, int fallback = -999);
	float getFloat(const char* key, float fallback = -999);
	bool getBool(const char* key);
	Vector3 getVector3(const char* key);
	vector<const char*>* getList(const char* key);
	void set(const char* key, string value);
	void set(const char* key, const char* value);
	void set(const char* key, int value);
	void set(const char* key, bool value);
	void set(const char* key, float value);
	void set(const char* key, Vector3 coords);
	void set(const char* key, vector<const char*>* list);

	static const char* replace(const char* str, char character, char replacement);
	static const char* formatToLineBreaks(const char* str, char delimeter = '@');

protected:
	bool handleLineInput(string* line);
	bool writeData();
};