#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OStringDefs.h"
#include "OsirisSDK/OTrashBin.h"
#include "OsirisSDK/OResourceManagerInterface.h"

class OMeshGeometry;

/**
 @brief Geometry manager for meshes.
 */
class OAPI OGeometryManager : public OResouceManagerInterface<OMeshGeometry>
{
public:
	/**
	 @brief Class default constructor.
	 */
	OGeometryManager();

	/**
	 @brief Deleted copy constructor.
	 */
	OGeometryManager(const OGeometryManager& aOther) = delete;

	/**
	 @brief Move constructor.
	 */
	OGeometryManager(OGeometryManager&& aOther);

	/**
	 @brief Class destructor.
	 */
	virtual ~OGeometryManager();

	/**
	 @brief Delete copy assignment operator.
	 */
	OGeometryManager& operator=(const OGeometryManager& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	OGeometryManager& operator=(OGeometryManager&& aOther);

	/**
	 @brief Mesh geometry file type. 
	 */
	enum class FileType {
		WavefrontObjectFile /**< Wavefront object file. */
	};

	/**
	 @brief Register mesh file.
	 @param aFileType File type.
	 @param aFileName Full or relative path of the mesh file.
	 @param aFileID File ID.
	 */
	void registerFile(FileType aFileType, const OString& aFilename, const OString& aFileID);

	/**
	 @brief Unregister mesh file.
	 @param aFileID File ID.
	 */
	void unRegisterFile(const OString& aFileID);

	/**
	 @brief Loads mesh geometry from file.
	 @param aFileID File ID.
	 @param aObjectName The object name in the mesh file.
	 @param aKey The geometry search key.
	 @return A pointer to the mesh geometry resource. 
	 */
	ResourcePtr loadFromFile(const OString& aFileID, const OString& aObjectName, const OString& aKey);


	// OResourceManagerInterface
	virtual void forEach(IterationCallbackFn aCallbackFn) override;
	virtual void forEach(IterationConstCallbackFn aCallbackFn) const override;
	virtual ResourcePtr& fetchResource(const OString& aKey) override;
	virtual void purge() override;

private:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */
};

inline OGeometryManager::OGeometryManager(OGeometryManager&& aOther)
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
}

