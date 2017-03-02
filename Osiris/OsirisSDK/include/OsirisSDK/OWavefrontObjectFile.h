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
	/**
	 \brief Class constructor.
	 \param filename Object file name.
	 */
	OWavefrontObjectFile(const char *filename);
	
	/**
	 \brief Class destructor.
	 */
	virtual ~OWavefrontObjectFile();

	/**
	 \brief Provides the object list from the file.
	 \param count Pointer to an integer where the object count will be written.
	 \return Vector with object names that can be used to load meshes.
	 */
	const char** objectList(int *count);
	
	/**
	 \brief Loads a given object into an OMesh class object, previously created.
	 \param objName Object name.
	 \param mesh OMesh class object in which the vertex data will be loaded.
	 */
	void loadMesh(const char* objName, OMesh* mesh);

protected:
	/**
	 \brief Scans the file for objecs.
	 */
	void loadObjectList();
	
	/**
	 \brief Reads the next line into the class internal buffer
	 */
	int readNextLine();
	
	/**
	 \brief Reads the next word on the line.
	 \return Returns the next word. If the end of line has been reached or the buffer overflowed, returns NULL.
	 */
	const char* readNextWord();
	
	/**
	 \brief Returns the current line of the file that is being processed.
	 */
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

