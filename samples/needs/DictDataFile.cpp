#pragma once
#include "Script.h"

#include "Misc.h"

vector<string> split(const string& s, char delim) {
	vector<string> result;
	stringstream ss(s);
	string item;

	while (getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}



DictDataFile::DictDataFile(const char* filename)
	: BaseDataFile(filename)
{
}

const char* DictDataFile::get(const char* key)
{
	string keyString = string(key);

	if (cache.find(keyString) == cache.end())
	{
		return "";
	}

	return cache.at(keyString).c_str();
}

int DictDataFile::getInt(const char* key, int fallback)
{
	string value = get(key);
	if (value.empty())
	{
		return fallback;
	}

	return stoi(value);
}

float DictDataFile::getFloat(const char* key, float fallback)
{
	string value = get(key);
	if (value.empty())
	{
		return fallback;
	}

	return stof(value);
}

bool DictDataFile::getBool(const char* key)
{
	const int value = getInt(key);
	return value != -999 && value != 0;
}

Vector3 DictDataFile::getVector3(const char* key)
{
	string value = get(key);
	if (value.empty())
	{
		return Misc::toVector3(0, 0, 0);
	}

	vector<string> parts = split(value, ',');
	return Misc::toVector3(
		stof(parts.at(0)),
		stof(parts.at(1)),
		stof(parts.at(2))
	);
}

vector<const char*>* DictDataFile::getList(const char* key)
{
	if (lists.find(string(key)) == lists.end())
	{
		vector<const char*>* list = new vector<const char*>();
		string value = get(key);
		if (value.empty())
		{
			return list;
		}

		stringstream ss(value);
		string item;

		while (getline(ss, item, ','))
		{
			char* itemStr = new char[item.length()];
			strcpy(itemStr, item.c_str());
			list->push_back(itemStr);
		}

		return list;
	}

	return lists.at(string(key));
}

void DictDataFile::set(const char* key, string value)
{
	if (value.empty())
	{
		map<string, string>::iterator item = cache.find(string(key));
		if (item != cache.end())
		{
			hasChanges = true;
			cache.erase(item);
		}
	}
	else
	{
		if (value.compare(cache[string(key)]))
		{
			cache[string(key)] = string(value);
			hasChanges = true;
		}
	}
}

void DictDataFile::set(const char* key, const char* value)
{
	set(key, string(value));
}

void DictDataFile::set(const char* key, int value)
{
	set(key, to_string(value));
}

void DictDataFile::set(const char* key, bool value)
{
	set(key, to_string(value));
}

void DictDataFile::set(const char* key, float value)
{
	set(key, to_string(value));
}

void DictDataFile::set(const char* key, Vector3 value)
{
	//set(key, to_string(value));
}

void DictDataFile::set(const char* key, vector<const char*>* list)
{
	stringstream value("");

	for (vector<const char*>::iterator itr = list->begin(); itr != list->end(); itr++)
	{
		if (itr == prev(list->end()))
		{
			value << *itr;
		}
		else
		{
			value << *itr << ",";
		}
	}

	set(key, value.str().c_str());
}

bool DictDataFile::handleLineInput(string* line)
{
	if (line->empty())
	{
		return true;
	}

	vector<string> parts = split(*line, '=');
	if (parts.size() != 2)
	{
		return false;
	}

	string key = parts.at(0);
	string value = parts.at(1);
	(cache)[key] = value;
	return true;
}

bool DictDataFile::writeData()
{
	std::ofstream file;
	file.open(filename);
	if (!file.is_open())
	{
		return false;
	}

	for (map<string, string>::iterator itr = cache.begin(); itr != cache.end(); itr++)
	{
		string line = string(itr->first).append("=").append(itr->second).append("\n");
		file << line.c_str();
	}

	file.close();
	return true;
}

const char* DictDataFile::replace(const char* original, char character, char replacement)
{
	char* str = strdup(original);
	int len = strlen(str);
	for (int i = 0; i < len; i++)
	{
		if (str[i] == character)
		{
			str[i] = replacement;
		}
	}

	return str;
}

const char* DictDataFile::formatToLineBreaks(const char* str, char delimeter)
{
	return replace(str, delimeter, '\n');
}