#pragma once

#include "OMeshFile.h"
#include "OWavefrontMaterialFile.h"


/**
 * @brief Implementação modular da interface OMeshFile baseada num Tokenizer autónomo.
 */
class OAPI OObjMeshFile : public OMeshFile
{
public:
    OObjMeshFile(const OString& aFilename);
    virtual ~OObjMeshFile() override;

    virtual void loadMesh(const OString& aObjName, RawData& aRawData) override;

private:
    struct Impl;
    Impl* _impl = nullptr;
};
