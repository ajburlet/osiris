#pragma once

#include <string>
#include <vector>
#include <map>
#include <stdio.h>

#include "defs.h"
#include "OMesh.h"

#ifndef OWAVEFRONTOBJECTFILE_LINEBUFFER
#define OWAVEFRONTOBJECTFILE_LINEBUFFER		256
#endif

#ifndef OWAVEFRONTOBJECTFILE_WORDBUFFER
#define OWAVEFRONTOBJECTFILE_WORDBUFFER		64
#endif

/**
 \brief Class that parses an Waveffront Object file and loads vertex information on a OMesh
        class object.
 */
class OAPI OWavefrontObjectFile
{
public:
	OWavefrontObjectFile(const char *filename);
	virtual ~OWavefrontObjectFile();

	const char** objectList(int *count);
	void loadMesh(const char* objName, OMesh* mesh);

protected:
	void loadObjectList();
	int readNextLine();
	const char* readNextWord();
	unsigned int currLine();

private:
	std::string _filename;
	FILE *_fp;
	std::vector<const char*> _objVec;
	std::map<std::string, long> _objMap;

	unsigned int _currLine;
	char _lineBuffer[OWAVEFRONTOBJECTFILE_LINEBUFFER];
	char _wordBuffer[OWAVEFRONTOBJECTFILE_WORDBUFFER];
	char* _wordptr;
	bool _endOfLine;
};

