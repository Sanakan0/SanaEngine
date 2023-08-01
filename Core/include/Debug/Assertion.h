#pragma once
#ifdef NDEBUG
#include "cstdio"
#include "cstdlib"
#define SANA_ASSERT(_EXPR) do {if (!(_EXPR)){printf("SANA_ASSERT! aborted " #_EXPR);abort();} } while (0)
#else
#include <assert.h>
#define SANA_ASSERT(_EXPR) assert(_EXPR)
#endif

//#define SANA_ASSERT(_EXPR) ((void)(_EXPR)) //if disabled