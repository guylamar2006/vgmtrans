#pragma once
#include "VGMInstrSet.h"
#include "VGMSampColl.h"
#include "VGMRgn.h"
#include "WhittakerSnesFormat.h"

// ****************
// WhittakerSnesInstrSet
// ****************

class WhittakerSnesInstrSet:
    public VGMInstrSet {
 public:
  WhittakerSnesInstrSet(RawFile *file,
                  WhittakerSnesVersion ver,
                  uint32_t offset,
                  uint32_t spcDirAddr,
                  const std::string &name = "WhittakerSnesInstrSet");
  virtual ~WhittakerSnesInstrSet();

  virtual bool parseHeader();
  virtual bool parseInstrPointers();

  WhittakerSnesVersion version;

  uint16_t konamiTuningTableAddress;
  uint8_t konamiTuningTableSize;

 protected:
  uint32_t spcDirAddr;
  std::vector<uint8_t> usedSRCNs;
};

// *************
// WhittakerSnesInstr
// *************

class WhittakerSnesInstr
    : public VGMInstr {
 public:
  WhittakerSnesInstr(VGMInstrSet *instrSet,
               WhittakerSnesVersion ver,
               uint32_t offset,
               uint32_t theBank,
               uint32_t theInstrNum,
               uint32_t spcDirAddr,
               const std::string &name = "WhittakerSnesInstr");
  virtual ~WhittakerSnesInstr();

  virtual bool loadInstr();

  static bool isValidHeader
      (RawFile *file, WhittakerSnesVersion version, uint32_t addrInstrHeader, uint32_t spcDirAddr, bool validateSample);
  static uint32_t expectedSize(WhittakerSnesVersion version);

  WhittakerSnesVersion version;

  uint16_t konamiTuningTableAddress;
  uint8_t konamiTuningTableSize;

 protected:
  uint32_t spcDirAddr;
};

// ***********
// WhittakerSnesRgn
// ***********

class WhittakerSnesRgn
    : public VGMRgn {
 public:
  WhittakerSnesRgn(WhittakerSnesInstr *instr,
             WhittakerSnesVersion ver,
             uint32_t offset,
             uint16_t konamiTuningTableAddress = 0,
             uint8_t konamiTuningTableSize = 0);
  virtual ~WhittakerSnesRgn();

  virtual bool loadRgn();

  WhittakerSnesVersion version;
};
