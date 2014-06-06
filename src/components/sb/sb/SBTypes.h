#ifndef _SBTYPES_H_
#define _SBTYPES_H_

#ifdef WIN32

	#ifdef SB_EXPORTS
		#define SBAPI __declspec(dllexport)
	#else
		#define SBAPI __declspec(dllimport)
	#endif

#else

	#if __GNUC__ >= 4
		#ifdef SB_EXPORTS
			#define SBAPI __attribute__ ((visibility ("default")))
		#else
			#define SBAPI 
		#endif
	#else
		#define SBAPI 
	#endif
#endif

#ifdef __ANDROID__
#define LINK_VHMSG_CLIENT		(1)
#define USE_WSP 1
#elif defined(__native_client__)
#define USE_WSP 0
#else
#define LINK_VHMSG_CLIENT		(1)
#define USE_WSP 1
#endif

#ifdef __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    #ifndef SB_IPHONE
    #define SB_IPHONE
    #endif
#else
    #ifndef SB_MAC
    #define SB_MAC
    #endif
#endif
#endif


#endif

