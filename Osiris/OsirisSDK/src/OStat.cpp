#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OString.hpp"
#include "OsirisSDK/OStat.h"

#ifdef WIN32
#define STAT _stat
#else
#define STAT stat
#endif

bool OStat::FileExists(const OString& aFilePath)
{
    struct STAT st;
    return (STAT(aFilePath.cString(), &st) == 0);
}

OStat OStat::Get(const OString& aFilePath)
{
    struct STAT st;
    if (STAT(aFilePath.cString(), &st) != 0) {
        throw OException(OString::Fmt("Unable to fetch file info: %s.", aFilePath.cString()));
    }
}

OStat::OStat()
= default;

OStat::~OStat()
= default;