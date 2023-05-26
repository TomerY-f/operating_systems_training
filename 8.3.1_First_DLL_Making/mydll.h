#pragma once 

#ifdef DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

extern "C" //Transform linkage requirements to C instead of C++.
{
	DECLDIR void Share(); //DLL shared function will be implemented in share()/
	void Keep(); //DLL will keep those code to himself.
}