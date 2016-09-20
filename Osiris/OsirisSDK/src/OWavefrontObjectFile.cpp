#include <stdio.h>

#include "OsirisSDK/OWavefrontObjectFile.h"
#include "OsirisSDK/OException.h"

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH
#define OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH	128
#endif

using namespace std;

/**
 \brief Class constructor.
 \param filename Object file name.
 */
OWavefrontObjectFile::OWavefrontObjectFile(const char *filename) :
	_filename(filename),
	_currLine(0),
	_wordptr(NULL)
{
#ifdef WIN32
	fopen_s(&_fp, filename, "rb");
#else
	_fp = fopen(filename, "rb");
#endif
	if (!_fp) {
		char errString[OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH];
		snprintf(errString, OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH, "Error opening file: '%s'.", filename);
		throw OException(errString);
	}
	memset(_lineBuffer, 0, OWAVEFRONTOBJECTFILE_LINEBUFFER);
	loadObjectList();
}


/**
 \brief Class destructor.
 */
OWavefrontObjectFile::~OWavefrontObjectFile()
{
	if (_fp != NULL) fclose(_fp);
}

/**
 \brief Provides the object list from the file.
 \param count Pointer to an integer where the object count will be written.
 \return Vector with object names that can be used to load meshes.
 */
const char ** OWavefrontObjectFile::objectList(int * count)
{
	*count = _objVec.size();
	if (*count == 0) return NULL;
	return &_objVec[0];
}

/**
 \brief Loads a given object into an OMesh class object, previously created.
 \param objName Object name.
 \param mesh OMesh class object in which the vertex data will be loaded.
 */
void OWavefrontObjectFile::loadMesh(const char * objName, OMesh * mesh)
{
	const char *firstWord, *valueWord;
	map<string, long>::iterator it;
	float dval[3];
	GLuint ival[3];
	int i;
	char errString[OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH];
	
	/* try and find the */
	it = _objMap.find(objName);
	if (it == _objMap.end()) {
		snprintf(errString, OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH, "Unable to locate object '%s' in file '%s'.", 
			objName, _filename.c_str());
		throw OException(errString);
	}

	if (fseek(_fp, it->second, SEEK_SET) != 0) {
		snprintf(errString, OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH, "Unable to read proper file position for object '%s' in file '%s'.",
			objName, _filename.c_str());
		throw OException(errString);
	}

	while (readNextLine() == 0) {
		firstWord = readNextWord();
		if (!firstWord || !strcmp(firstWord, "#")) {
			/* ignore empty lines or comments */
			continue;
		} else if (!strcmp(firstWord, "o")) {
			/* reached next object, time to stop */
			break;
		} else if (!strcmp(firstWord, "v")) {
			/* reading vertices */
			for (i = 0; i < 3; i++) {
				if ((valueWord = readNextWord())) dval[i] = (float)atof(valueWord);
				else break;
			}
			if (i != 3) {
				snprintf(errString, OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH, "Missing vertex values on %s:%u",
					_filename.c_str(), currLine());
				throw OException(errString);
			}
			mesh->addVertexData(dval[0], dval[1], dval[2]);
		} else if (!strcmp(firstWord, "f")) {
			/* reading indexes */
			for (i = 0; i < 3; i++) {
				if ((valueWord = readNextWord())) {
					/* for now we only take vertex indexes, ignoring the rest */
					char tempBuf[32];
					const char *p;
					if ((p = strchr(valueWord, '/')) != NULL) {
						#ifdef WIN32
						strncpy_s(tempBuf, 32, valueWord, (rsize_t)(p - valueWord));
						#else
						strncpy(tempBuf, valueWord, (size_t)(p - valueWord));
						#endif
						ival[i] = (GLuint)atoi(tempBuf);
					} else {
						ival[i] = (GLuint)atoi(valueWord);
					}
				} else break;
			}
			if (i != 3) {
				snprintf(errString, OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH, "Missing index values on %s:%d",
					_filename.c_str(), currLine());
				throw OException(errString);
			}
			mesh->addIndexData(ival[0], ival[1], ival[2]);
		}
	}
}

void OWavefrontObjectFile::loadObjectList()
{
	const char *firstWord, *objectName;
	char errString[OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH];

	_objVec.clear();
	_objMap.clear();

	while (readNextLine() == 0) {
		firstWord = readNextWord();
		if (strcmp(firstWord, "o") != 0) continue;

		if ((objectName = readNextWord()) == NULL) {
			snprintf(errString, OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH, "Parse error at line %u.", currLine());
			throw OException(errString);
		}

		_objMap[objectName] = ftell(_fp);
	}

	/* building object vector */
	map<string, long>::iterator it;
	for (it = _objMap.begin(); it != _objMap.end(); it++) _objVec.push_back(it->first.c_str());
}

/**
 \brief Reads the next line into the class internal buffer
 */
int OWavefrontObjectFile::readNextLine()
{
	if (feof(_fp) || !fgets(_lineBuffer, OWAVEFRONTOBJECTFILE_LINEBUFFER, _fp)) {
		_wordptr = NULL;
		return 1;
	}
	_wordptr = _lineBuffer;
	_endOfLine = false;
	_currLine++;
	return 0;
}

/**
 \brief Reads the next word on the line.
 \return Returns the next word. If the end of line has been reached or the buffer overflowed, returns NULL.
 */
const char * OWavefrontObjectFile::readNextWord()
{
	char *p = _wordptr;

	/* if end of line has been reached, point to the beginning of the line and return NULL. */
	if (_endOfLine) {
		_wordptr = _lineBuffer;
		_endOfLine = false;
		return NULL;
	}

	/* if no space is found or end of line chars, means end of line */
	if ((p = strchr(_wordptr, ' ')) == NULL && ((p = strchr(_wordptr, '\n')) != NULL || (p = strchr(_wordptr, '\r')) != NULL || (p = strchr(_wordptr, '\0')) != NULL) ) {
		_endOfLine = true;
	}

	*_wordBuffer = '\0';

#ifdef WIN32
	strncpy_s(_wordBuffer, (rsize_t)OWAVEFRONTOBJECTFILE_WORDBUFFER, _wordptr, (size_t)(p-_wordptr));
#else
	strncpy(_wordBuffer, _wordptr, (size_t)(p - _wordptr));
#endif
	if (!_endOfLine) _wordptr = p + 1;
	return _wordBuffer;
}

/**
 \brief Returns the current line of the file that is being processed.
 */
unsigned int OWavefrontObjectFile::currLine()
{
	return _currLine;
}

