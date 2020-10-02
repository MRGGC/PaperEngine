#pragma once

#if defined(_MSC_VER)
	//  Microsoft 
	#define EXPORT __declspec(dllexport)
	#define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
	//  GCC
	#define EXPORT __attribute__((visibility("default")))
	#define IMPORT
#else
	//  Hope for the Best
	#define EXPORT
	#define IMPORT
	#pragma warning Unknown dynamic link import/export semantics.
#endif

#if defined(_MSC_VER)
	#define DEBUG_BREAK __debugbreak()
#else
	#define DEBUG_BREAK raise(SIGTRAP)
#endif

#ifdef PENGINE_BUILD_SO
	#define PAPER_API EXPORT
#else
	#define PAPER_API IMPORT
#endif

#ifdef PAPER_ENABLE_ASSERTS
	#define PAPER_ASSERT(x, ...) { if(!(x)) { PAPER_CLIENT_ERROR("Assertation Error {0}", __VA_ARGS__); DEBUG_BREAK; } }
	#define PAPER_CORE_ASSERT(x, ...) { if(!(x)) { PAPER_CORE_ERROR("Assertation Error {0}", __VA_ARGS__); DEBUG_BREAK; } }
#else
	#define PAPER_ASSERT(x, ...)
	#define PAPER_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define PAPER_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)