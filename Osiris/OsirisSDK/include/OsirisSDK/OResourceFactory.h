#pragma once

#include <memory>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OStringDefs.h"
#include "OsirisSDK/OResourceManager.h"
#include "OsirisSDK/OResourceManagerDefs.h"
#include "OsirisSDK/OGraphicsAllocators.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OGeometry.h"
#include "OsirisSDK/OTexture.h"
#include "OsirisSDK/OMaterial.h"
#include "OsirisSDK/OMaterialSet.h"

class OMesh;
class ORenderComponents;

/**
 @brief Geometry manager for meshes.
 */
class OAPI OResourceFactory : public OMemoryManagedObject<OGraphicsAllocators::Default> 
{
public:
	using GeometryManager = OResourceManager<OGeometry>;
	using TextureManager = OResourceManager<OTexture>;
	using MaterialManager = OResourceManager<OMaterial>;
	using MaterialSetManager = OResourceManager<OMaterialSet>;
	using ResourcePtr = GeometryManager::ResourcePtr;

public:
	/**
	 @brief Class default constructor.
	 */
	OResourceFactory();

	/**
	 @brief Move constructor.
	 */
	OResourceFactory(OResourceFactory&& aOther);

	/**
	 @brief Class destructor.
	 */
	virtual ~OResourceFactory();

	/**
	 @brief Move assignment operator.
	 */
	OResourceFactory& operator=(OResourceFactory&& aOther);

	/**
	 * @brief Geometry manager.
	 */
	GeometryManager& geometryManager();

	/**
	 * @brief Texture manager.
	 */
	TextureManager& textureManager();

	/**
	 * @brief Material manager.
	 */
	MaterialManager& materialManager();

	/**
	 * @brief Material set manager.
	 */
	MaterialSetManager& materialSetManager();

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
	void loadFromFile(const OString& aFileID, const OString& aObjectName,
		const OString& aKey, ORenderComponents& aRenderComponents);
	void loadFromFile(const OString& aFileID, const OString& aObjectName,
		const OString& aKey, OMesh& aMesh);

private:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	std::unique_ptr<Impl> _impl;

	struct MeshResources {
		ResourcePtr geometry;
		MaterialSetManager::ResourcePtr materials;
	};
	MeshResources loadMeshResources(const OString& aFileID, const OString& aObjectName, const OString& aKey);
	/**
	 @endcond
	 */
};



