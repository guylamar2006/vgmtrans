#pragma once
#include "VGMInstrSet.h"
#include "VGMSampColl.h"
#include "VGMRgn.h"
#include "WhitakerSnesFormat.h"

// ****************
// WhitakerSnesInstrSet
// ****************

class WhitakerSnesInstrSet:
    public VGMInstrSet {
 public:
  WhitakerSnesInstrSet(RawFile *file,
                  WhitakerSnesVersion ver,
                  uint32_t offset,
                  uint32_t spcDirAddr,
                  const std::string &name = "WhitakerSnesInstrSet");
  virtual ~WhitakerSnesInstrSet();

  virtual bool parseHeader();
  virtual bool parseInstrPointers();

  WhitakerSnesVersion version;

  uint16_t akklaimTuningTableAddress;
  uint8_t akklaimTuningTableSize;

 protected:
  uint32_t spcDirAddr;
  std::vector<uint8_t> usedSRCNs;
};

// *************
// WhitakerSnesInstr
// *************

class WhitakerSnesInstr
    : public VGMInstr {
 public:
  WhitakerSnesInstr(VGMInstrSet *instrSet,
               WhitakerSnesVersion ver,
               uint32_t offset,
               uint32_t theBank,
               uint32_t theInstrNum,
               uint32_t spcDirAddr,
               const std::string &name = "WhitakerSnesInstr");
  virtual ~WhitakerSnesInstr();

  virtual bool loadInstr();

  static bool isValidHeader
      (RawFile *file, WhitakerSnesVersion version, uint32_t addrInstrHeader, uint32_t spcDirAddr, bool validateSample);
  static uint32_t expectedSize(WhitakerSnesVersion version);

  WhitakerSnesVersion version;

  uint16_t akklaimTuningTableAddress;
  uint8_t akklaimTuningTableSize;

 protected:
  uint32_t spcDirAddr;
};

// ***********
// WhitakerSnesRgn
// ***********

class WhitakerSnesRgn
    : public VGMRgn {
 public:
  WhitakerSnesRgn(WhitakerSnesInstr *instr,
             WhitakerSnesVersion ver,
             uint32_t offset,
             uint16_t akklaimTuningTableAddress = 0,
             uint8_t akklaimTuningTableSize = 0);
  virtual ~WhitakerSnesRgn();

  virtual bool loadRgn();

  WhitakerSnesVersion version;
};
