
#pragma once

#if defined(LON_BUILD)
#define LONAPI __declspec(dllexport)
#else 
#define LONAPI __declspec(dllimport)
#endif 