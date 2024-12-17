#pragma once
#include "Format.h"
#include "FilegroupMatcher.h"
#include "WhittakerSnesScanner.h"


// ***************
// WhittakerSnesFormat
// ***************

BEGIN_FORMAT(WhittakerSnes)
  USING_SCANNER(WhittakerSnesScanner)
  USING_MATCHER(FilegroupMatcher)
END_FORMAT()


enum WhittakerSnesVersion {
  RARESNES_NONE = 0,  // Unknown Version
  RARESNES_DKC,       // Donkey Kong Country
  RARESNES_KI,        // Killer Instinct
  RARESNES_DKC2,      // Donkey Kong Country 2 (and DKC3)
  RARESNES_WNRN,      // Ken Griffey Jr. Winning Run
};
