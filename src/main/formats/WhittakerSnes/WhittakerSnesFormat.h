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
  WHITTSNES_NONE = 0,  // Unknown Version
  WHITTSNES_DTVB2,     // Dream TV
  WHITTSNES_KOFF,      // Kick Off
  WHITTSNES_KSFH1,     // Krusty's Super Fun House Version 1'
  WHITTSNES_KSFH2,     // Krusty's Super Fun House Version 2'
  WHITTSNES_GODSE,     // Gods (Ending)
  WHITTSNES_TSCRN,     // Gods (Title Screen/In Game)
};
