#pragma once
#include "VGMInstrSet.h"
#include "VGMSampColl.h"
#include "VGMRgn.h"

// ****************
// WhittakerSnesInstrSet
// ****************

class WhittakerSnesInstrSet:
    public VGMInstrSet {
 public:
  WhittakerSnesInstrSet(RawFile *file, uint32_t offset, uint32_t spcDirAddr, const std::string &name = "WhittakerSnesInstrSet");
  WhittakerSnesInstrSet(RawFile *file,
                   uint32_t offset,
                   uint32_t spcDirAddr,
                   const std::map<uint8_t, int8_t> &instrUnityKeyHints,
                   const std::map<uint8_t, int16_t> &instrPitchHints,
                   const std::map<uint8_t, uint16_t> &instrADSRHints,
                   const std::string &name = "WhittakerSnesInstrSet");
  virtual ~WhittakerSnesInstrSet();

  virtual void Initialize();
  virtual bool parseHeader();
  virtual bool parseInstrPointers();

  const std::vector<uint8_t> &getAvailableInstruments();

 protected:
  uint32_t spcDirAddr;
  uint8_t maxSRCNValue;
  std::vector<uint8_t> availInstruments;
  std::map<uint8_t, int8_t> instrUnityKeyHints;
  std::map<uint8_t, int16_t> instrPitchHints;
  std::map<uint8_t, uint16_t> instrADSRHints;

  void ScanAvailableInstruments();
};

// *************
// WhittakerSnesInstr
// *************

class WhittakerSnesInstr
    : public VGMInstr {
 public:
  WhittakerSnesInstr(VGMInstrSet *instrSet,
                uint32_t offset,
                uint32_t theBank,
                uint32_t theInstrNum,
                uint32_t spcDirAddr,
                int8_t transpose = 0,
                int16_t pitch = 0,
                uint16_t adsr = 0x8FE0,
                const std::string &name = "WhittakerSnesInstr");
  virtual ~WhittakerSnesInstr();

  virtual bool loadInstr();

 protected:
  uint32_t spcDirAddr;
  int8_t transpose;
  int16_t pitch;
  uint16_t adsr;
};

// ***********
// WhittakerSnesRgn
// ***********

class WhittakerSnesRgn
    : public VGMRgn {
 public:
  WhittakerSnesRgn(WhittakerSnesInstr *instr, uint32_t offset, int8_t transpose = 0, int16_t pitch = 0, uint16_t adsr = 0x8FE0);
  virtual ~WhittakerSnesRgn();

  virtual bool loadRgn();

 protected:
  int8_t transpose;
  int16_t pitch;
  uint16_t adsr;
};
