#pragma once
#include "Format.h"
#include "NDSScanner.h"
#include "VGMColl.h"

// *********
// NDSFormat
// *********

BEGIN_FORMAT(NDS)
  USING_SCANNER(NDSScanner)
END_FORMAT()


