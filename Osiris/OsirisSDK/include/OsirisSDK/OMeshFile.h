#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/OFile.h"
#include "OsirisSDK/OMeshRawData.h"

#ifndef OMESHFILE_OBJNAMEARRAY_BLOCKSIZE
#define OMESHFILE_OBJNAMEARRAY_BLOCKSIZE	4
#endif

template <typename T, class Allocator> class OList;
class OMaterial;
/**
 @brief Base mesh file interface.
 */
class OAPI OMeshFile : public OFile
{
public:
	using Allocator = OSystemMemoryAllocator<OMemoryManagerScope::Default>;
	using RawData = OMeshRawData;

	/**
	 @brief Class constructor.
	 @param aFilename Mesh geometry file name.
	 */
	OMeshFile(const OString& aFilename) : OFile(aFilename, Mode::Read) {}

	/**
	 @brief Class destructor.
	 */
	virtual ~OMeshFile() = default;

	/**
	 @brief Loads a given object into an OMesh class object, previously created.
	 @param aObjName Object name.
	 @param aGeometry Geometry rawdata output.
	 */
	virtual void loadMesh(const OString& aObjName, RawData& aRawData) = 0;
};

