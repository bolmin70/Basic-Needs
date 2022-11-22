#include "Script.h"

#include <iostream>
#include <iterator>
#include <map>

BaseDataFile::BaseDataFile(const char* filename)
{
	this->filename = filename;
	this->hasChanges = false;
}

const char* BaseDataFile::getFilename()
{
	return filename;
}


bool BaseDataFile::load()
{
	ifstream file;
	string input;
	int count = 0;

	
	file.open(filename);;
	if (!file.is_open())
	{
		return false;
	}

	
	while (!file.eof())
	{
		std::getline(file, input);
		if (!handleLineInput(&input))
		{
			
			return false;
		}

		count++;
	}

	file.close();

	return true;
}

bool BaseDataFile::save(bool force)
{
	if (!hasChanges && !force)
	{
		return true;
	}


	if (!writeData())
	{
	
		return false;
	}

	hasChanges = false;

	return true;
}

bool BaseDataFile::isTouched()
{
	return hasChanges;
}