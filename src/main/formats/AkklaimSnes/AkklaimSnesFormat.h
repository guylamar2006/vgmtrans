#pragma once
#include "Format.h"
#include "Matcher.h"
#include "AkklaimSnesScanner.h"


// **************
// AkklaimSnesFormat
// **************

BEGIN_FORMAT(AkklaimSnes)
  USING_SCANNER(AkklaimSnesScanner)
  USING_MATCHER(FilegroupMatcher)
END_FORMAT()

// Akklaim SNES is used by a few games, such as Krusty's Super Fun House
// Experiment to add support.

enum AkklaimSnesVersion: uint8_t {
  AKKLAIMSNES_NONE = 0,              // Unknown Version
  AKKLAIMSNES_V1,                    // Krusty's Super Fun House
};

enum AkklaimSnesMinorVersion: uint8_t {
  AKKLAIMSNES_NOMINORVERSION = 0,    // Unknown Minor Version
  AKKLAIMSNES_V1_KFH,                // Krusty's Super Fun House
};
