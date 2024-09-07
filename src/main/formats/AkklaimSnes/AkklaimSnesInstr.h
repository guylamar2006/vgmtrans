#pragma once
#include "VGMInstrSet.h"
#include "VGMRgn.h"
#include "AkklaimSnesFormat.h"

// ****************
// AkklaimSnesInstrSet
// ****************

class AkklaimSnesInstrSet:
    public VGMInstrSet {
 public:
  static constexpr uint32_t DRUMKIT_PROGRAM = (0x7F << 7);

  AkklaimSnesInstrSet(RawFile *file,
                   AkklaimSnesVersion ver,
                   uint32_t spcDirAddr,
                   uint16_t addrTuningTable,
                   uint16_t addrADSRTable,
                   uint16_t addrDrumKitTable,
                   const std::string &name = "AkklaimoSnesInstrSet");
  ~AkklaimSnesInstrSet() override;

  bool parseHeader() override;
  bool parseInstrPointers() override;

  AkklaimSnesVersion version;

 protected:
  uint32_t spcDirAddr;
  uint16_t addrTuningTable;
  uint16_t addrADSRTable;
  uint16_t addrDrumKitTable;
  std::vector<uint8_t> usedSRCNs;
};

// *************
// AkklaimSnesInstr
// *************

class AkklaimSnesInstr
    : public VGMInstr {
 public:
  AkklaimoSnesInstr(VGMInstrSet *instrSet,
                AkklaimSnesVersion ver,
                uint8_t srcn,
                uint32_t spcDirAddr,
                uint16_t addrTuningTable,
                uint16_t addrADSRTable,
                const std::string &name = "AkklaimSnesInstr");
  ~AkklaimSnesInstr() override;

  bool loadInstr() override;

  AkklaimSnesVersion version;

 protected:
  uint32_t spcDirAddr;
  uint16_t addrTuningTable;
  uint16_t addrADSRTable;
};

// *************
// AkklaimSnesDrumKit
// *************

class AkklaimSnesDrumKit
  : public VGMInstr {
public:
  AkklaimSnesDrumKit(VGMInstrSet *instrSet,
                  AkklaimSnesVersion ver,
                  uint32_t programNum,
                  uint32_t spcDirAddr,
                  uint16_t addrTuningTable,
                  uint16_t addrADSRTable,
                  uint16_t addrDrumKitTable,
                  const std::string &name = "AkklaimoSnesDrumKit");
  ~AkklaimSnesDrumKit() override;

  bool loadInstr() override;

  AkklaimSnesVersion version;

protected:
  uint32_t spcDirAddr;
  uint16_t addrTuningTable;
  uint16_t addrADSRTable;
  uint16_t addrDrumKitTable;
};

// ***********
// AkklaimSnesRgn
// ***********

class AkklaimSnesRgn
    : public VGMRgn {
 public:
  AkklaimSnesRgn(VGMInstr *instr,
              AkklaimSnesVersion ver,
              uint16_t addrTuningTable);
  ~AkklaimSnesRgn() override;

  bool initializeRegion(uint8_t srcn,
                        uint32_t spcDirAddr,
                        uint16_t addrADSRTable);
  bool loadRgn() override;

  AkklaimSnesVersion version;
};

// ***********
// AkklaimSnesDrumKitRgn
// ***********

class AkklaimiSnesDrumKitRgn
  : public AkklaimSnesRgn {
public:
  // We need some space to move for unityKey, since we might need it to go
  // less than the current note, so we add this to all the percussion notes.
  // This value can be anything from 0 to 127, but being near the middle of
  // the range gives it a decent chance of not going out of range in either
  // direction.
  static constexpr uint8_t KEY_BIAS = 60;

  AkklaimSnesDrumKitRgn(AkklaimSnesDrumKit *instr,
                     AkklaimSnesVersion ver,
                     uint16_t addrTuningTable);
  ~AkklaimSnesDrumKitRgn() override;

  bool initializePercussionRegion(uint8_t srcn,
                                  uint32_t spcDirAddr,
                                  uint16_t addrADSRTable,
                                  uint16_t addrDrumKitTable);
};
