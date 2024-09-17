#pragma once
#include "Format.h"
#include "Root.h"
#include "WhitakerSnesScanner.h"
#include "Matcher.h"
#include "VGMColl.h"

// *************
// WhitakerSnesFormat
// *************

enum WhitakerSnesVersion {
  WHITSNES_NONE = 0,           // Not Supported
  WHITSNES_UNKNOWN,            // Unknown version (only header support)
  WHITSNES_EARLIER,            // Eariler version (Super Mario World, Pilotwings)
  WHITSNES_STANDARD,           // Common version with voice commands $e0-fa (e.g. Yoshi Island)

                              // derived formats
  WHITSNES_RD1,                // Nintendo RD1 (e.g. Super Metroid, Earthbound)
  WHITSNES_RD2,                // Nintendo RD2 (e.g. Marvelous)
  WHITSNES_HAL,                // HAL Laboratory games (e.g. Kirby series)
  WHITSNES_AKKLAIM,             // Old Akklaim games (e.g. Gradius III, Castlevania IV, Legend of the Mystical Ninja)
  WHITSNES_LEMMINGS,           // Lemmings
  WHITSNES_INTELLI_FE3,        // Fire Emblem 3
  WHITSNES_INTELLI_TA,         // Tetris Attack
  WHITSNES_INTELLI_FE4,        // Fire Emblem 4
  WHITSNES_HUMAN,              // Human games (e.g. Clock Tower, Firemen, Septentrion)
  WHITSNES_TOSE,               // TOSE games (e.g. Yoshi's Safari, Dragon Ball Z: Super Butouden 2)
  WHITSNES_QUINTET_ACTR,       // ActRaiser, Soul Blazer
  WHITSNES_QUINTET_ACTR2,      // ActRaiser 2
  WHITSNES_QUINTET_IOG,        // Illusion of Gaia, Robotrek
  WHITSNES_QUINTET_TS,         // Terranigma (Tenchi Souzou)
  WHITSNES_FALCOM_YS4,         // Ys IV
};

BEGIN_FORMAT(WhitakerSnes)
  USING_SCANNER(WhitakerSnesScanner)
  USING_MATCHER(FilegroupMatcher)

  static inline bool isQuintetVersion(WhitakerSnesVersion version) {
    return version == WHITSNES_QUINTET_ACTR ||
        version == WHITSNES_QUINTET_ACTR2 ||
        version == WHITSNES_QUINTET_IOG ||
        version == WHITSNES_QUINTET_TS;
  }
END_FORMAT()
