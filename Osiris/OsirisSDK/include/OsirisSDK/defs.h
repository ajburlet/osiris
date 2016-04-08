#pragma once

#pragma warning (disable : 4251)

#ifdef WIN32
#	ifdef OSIRISSDK_EXPORTS
#		define OAPI __declspec(dllexport)
#	else
#		define OPAI __declspec(dllimport)
#	endif
#else
#	define OAPI
#endif
