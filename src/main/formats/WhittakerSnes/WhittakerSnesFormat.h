#pragma once
#include "Format.h"
#include "WhittakerSnesScanner.h"
#include "FilegroupMatcher.h"

// *************
// WhittakerSnesFormat
// *************

enum WhittakerSnesVersion {
  WHITTSNES_NONE = 0,  // Unknown Version
  WHITTSNES_DTVB2,     // Dream TV (Beta 2)
  WHITTSNES_KOFF,      // Kick Off
  WHITTSNES_KSFH1,     // Krusty's Super Fun House (version 1)
  WHITTSNES_KSFH2,     // Krusty's Super Fun House (version 2)
  WHITTSNES_GODS1,     // Gods (Ending)
  WHITTSNES_GODS2,     // Gods (Title Screen & In-Game)
  WHITTSNES_WCR1       // World Class Rugby (Outside of a game)
  WHITTSNES_BMAN       // Batman: Revenge of the Joker
  WHITTSNES_GODS3      // Gods (Opening)
  WHITTSNES_DTV1       // Dream TV
  WHITTSNES_LEM2A      // Lemmings 2: The Tribes (Cavelem Tribe)
  WHITTSNES_WCR2       // World Class Rugby (During a game)
  WHITTSNES_SSF2       // Super SWIV/Firepower 2000
  WHITTSNES_LEM2B      // Apocalypse II (Opening Logo) Lemmings 2: The Tribes (Opening Logo)
  WHITTSNES_LEM2C      // Lemmings 2: The Tribes (Beach Tribe)
  WHITTSNES_LEM2D      // Lemmings 2: The Tribes (Title Screen)
  WHITTSNES_LEM2E      // Lemmings 2: The Tribes (Most common build used) Riddick Bowe Boxing (Japanese version)
  WHITTSNES_APO2       // Apocalypse II (In-Game)
  WHITTSNES_KO3B1      // Kick Off 3 (Beta) (In-Game)
  WHITTSNES_KO3B2      // Kick Off 3 (Beta) (Title Screen) Lawnmower Man/Virtual Wars (US Beta)
  WHITTSNES_PPHHB      // Porky Pig's Haunted Holiday (Beta)
  WHITTSNES_SHAQ       // Shaq Fu
  WHITTSNES_ESOC       // Elite Soccer/World Cup Striker (Japanese version) (Title Screen)
  WHITTSNES_RBBOX      // Riddick Bowe Boxing (US version)
  WHITTSNES_CHAV       // Chavez
  WHITTSNES_LMVW       // Lawnmower Man/Virtual Wars (In-Game)
  WHITTSNES_ESEU       // Elite Soccer/World Cup Striker (European version) (In-Game)
  WHITTSNES_BCARS      // Battle Cars
  WHITTSNES_WCSJP      // World Cup Striker (Japanese version) (In-Game)
  WHITTSNES_WCSEU      // World Cup Striker (European version) (Title Screen)
  WHITTSNES_LMVW2      // Lawnmower Man/Virtual Wars (Driving)
  WHITTSNES_MJORD      // Michael Jordan: Chaos in the Windy City
};

BEGIN_FORMAT(WhittakerSnes)
  USING_SCANNER(WhittakerSnesScanner)
  USING_MATCHER(FilegroupMatcher)
END_FORMAT()
