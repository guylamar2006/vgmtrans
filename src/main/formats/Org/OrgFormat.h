#pragma once
#include "Format.h"
#include "OrgScanner.h"


// *********
// OrgFormat
// *********

BEGIN_FORMAT(Org)
  USING_SCANNER(OrgScanner)
END_FORMAT()
