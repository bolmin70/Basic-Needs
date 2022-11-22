#pragma once

#include "BaseDataFile.h"
#include "DictDataFile.h"

class DataFiles
{
public:
	static const char* DataFilesPath;
	static const char* getDataFilePath(const char* filename);
	static bool load();
	static DictDataFile* Lang;
	static DictDataFile* TutorialFlags;
	static DictDataFile* GeneralMeta;
	static DictDataFile* AI;
	static DictDataFile* Savegame;
};
