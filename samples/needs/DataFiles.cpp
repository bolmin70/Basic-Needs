#include "Script.h"

#include <iostream>
#include <iterator>
#include <map>

const char* DataFiles::DataFilesPath = "Needs";

DictDataFile* DataFiles::Lang = NULL;
DictDataFile* DataFiles::TutorialFlags = NULL;
DictDataFile* DataFiles::GeneralMeta = NULL;
DictDataFile* DataFiles::AI = NULL;
DictDataFile* DataFiles::Savegame = NULL;

const char* DataFiles::getDataFilePath(const char* filename)
{
	return (new string(string(DataFilesPath).append("/").append(filename).c_str()))->c_str();
}

bool DataFiles::load()
{
	Lang = new DictDataFile(getDataFilePath("Lang.dat"));
	if (!Lang->load())
	{
		return false;
	}

	GeneralMeta = new DictDataFile(getDataFilePath("GeneralMeta.dat"));
	if (!GeneralMeta->load())
	{
		return false;
	}

	AI = new DictDataFile(getDataFilePath("AI.dat"));
	if (!AI->load())
	{
		return false;
	}

	Savegame = new DictDataFile(getDataFilePath("Savegame.dat"));
	if (!Savegame->load())
	{
		return false;
	}

	TutorialFlags = new DictDataFile(getDataFilePath("TutorialFlags.dat"));
	if (!TutorialFlags->load())
	{
		
	}

	return true;
}