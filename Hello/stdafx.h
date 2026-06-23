// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define LOGPRINT_NL(x) \
	DbgPrint("%s\n", x);

#define CALL_FUNC(t, x, ...) \
	((t(*)(...))x)(__VA_ARGS__)

#include <xtl.h>
#include <xboxmath.h>
#include <xbox.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unordered_map>

#include <xkelib.h>
#include "Detour.h"

#include "Structs.h"



// TODO: reference additional headers your program requires here
