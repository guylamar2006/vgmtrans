/*
* VGMTrans (c) 2002-2024
 * Licensed under the zlib license,
 * refer to the included LICENSE.txt file
 */
#include "WhittakerSnesInstr.h"
#include "SNESDSP.h"
#include <spdlog/fmt/fmt.h>

// ****************
// WhittakerSnesInstrSet
// ****************

WhittakerSnesInstrSet::WhittakerSnesInstrSet(RawFile *file,
                                 WhittakerSnesVersion ver,
                                 uint32_t offset,
                                 uint32_t spcDirAddr,
                                 const std::string &name) :
    VGMInstrSet(WhittakerSnesFormat::name, file, offset, 0, name), version(ver),
    spcDirAddr(spcDirAddr),
    konamiTuningTableAddress(0),
    konamiTuningTableSize(0) {
}

WhittakerSnesInstrSet::~WhittakerSnesInstrSet() {
}

bool WhittakerSnesInstrSet::parseHeader() {
  return true;
}

bool WhittakerSnesInstrSet::parseInstrPointers() {
  uint8_t instr_max = 0x7f;
  if (version == NINSNES_HUMAN) {
    instr_max = 0x200 / WhittakerSnesInstr::expectedSize(version);
  }

  usedSRCNs.clear();
  for (int instr = 0; instr <= instr_max; instr++) {
    uint32_t instrItemSize = WhittakerSnesInstr::expectedSize(version);
    uint32_t addrInstrHeader = dwOffset + (instrItemSize * instr);
    if (addrInstrHeader + instrItemSize > 0x10000) {
      return false;
    }

    // skip blank slot
    if (version != NINSNES_EARLIER) {
      // Kirby Super Star
      if (readByte(addrInstrHeader) == 0xff && readByte(addrInstrHeader + 1) == 0xff
        && readByte(addrInstrHeader + 2) == 0xff &&
        readByte(addrInstrHeader + 3) == 0xff && readByte(addrInstrHeader + 4) == 0xff
        && readByte(addrInstrHeader + 5) == 0xff) {
        continue;
      }

      // Bubsy in Claws Encounters of the Furred Kind
      if (readByte(addrInstrHeader) == 0 && readByte(addrInstrHeader + 1) == 0
        && readByte(addrInstrHeader + 2) == 0 &&
        readByte(addrInstrHeader + 3) == 0 && readByte(addrInstrHeader + 4) == 0
        && readByte(addrInstrHeader + 5) == 0) {
        continue;
      }
    }

    uint8_t srcn = readByte(addrInstrHeader);

    uint32_t offDirEnt = spcDirAddr + (srcn * 4);
    if (offDirEnt + 4 > 0x10000) {
      break;
    }

    uint16_t addrSampStart = readShort(offDirEnt);
    uint16_t addrLoopStart = readShort(offDirEnt + 2);

    if (addrSampStart == 0x0000 && addrLoopStart == 0x0000 ||
        addrSampStart == 0xffff && addrLoopStart == 0xffff) {
      // example: Lemmings - Stage Clear (00 00 00 00)
      // example: Yoshi's Island - Bowser (ff ff ff ff)
      continue;
    }
    if (!WhittakerSnesInstr::isValidHeader(this->rawFile(), version, addrInstrHeader, spcDirAddr, false)) {
      break;
    }
    if (!WhittakerSnesInstr::isValidHeader(this->rawFile(), version, addrInstrHeader, spcDirAddr, true)) {
      continue;
    }

    if (version == NINSNES_EARLIER || version == NINSNES_STANDARD) {
      if (addrSampStart < offDirEnt + 4) {
        continue;
      }
    }

    std::vector<uint8_t>::iterator itrSRCN = find(usedSRCNs.begin(), usedSRCNs.end(), srcn);
    if (itrSRCN == usedSRCNs.end()) {
      usedSRCNs.push_back(srcn);
    }

    WhittakerSnesInstr *newInstr = new WhittakerSnesInstr(
      this, version, addrInstrHeader, instr >> 7, instr & 0x7f,
      spcDirAddr, fmt::format("Instrument {}", instr));
    newInstr->konamiTuningTableAddress = konamiTuningTableAddress;
    newInstr->konamiTuningTableSize = konamiTuningTableSize;
    aInstrs.push_back(newInstr);
  }
  if (aInstrs.size() == 0) {
    return false;
  }

  std::sort(usedSRCNs.begin(), usedSRCNs.end());
  SNESSampColl *newSampColl = new SNESSampColl(WhittakerSnesFormat::name, this->rawFile(), spcDirAddr, usedSRCNs);
  if (!newSampColl->loadVGMFile()) {
    delete newSampColl;
    return false;
  }

  return true;
}

// *************
// WhittakerSnesInstr
// *************

WhittakerSnesInstr::WhittakerSnesInstr(VGMInstrSet *instrSet,
                           WhittakerSnesVersion ver,
                           uint32_t offset,
                           uint32_t theBank,
                           uint32_t theInstrNum,
                           uint32_t spcDirAddr,
                           const std::string &name) :
    VGMInstr(instrSet, offset, WhittakerSnesInstr::expectedSize(ver), theBank, theInstrNum, name), version(ver),
    spcDirAddr(spcDirAddr),
    konamiTuningTableAddress(0),
    konamiTuningTableSize(0) {
}

WhittakerSnesInstr::~WhittakerSnesInstr() {
}

bool WhittakerSnesInstr::loadInstr() {
  uint8_t srcn = readByte(dwOffset);
  uint32_t offDirEnt = spcDirAddr + (srcn * 4);
  if (offDirEnt + 4 > 0x10000) {
    return false;
  }

  uint16_t addrSampStart = readShort(offDirEnt);

  WhittakerSnesRgn *rgn = new WhittakerSnesRgn(this, version, dwOffset, konamiTuningTableAddress, konamiTuningTableSize);
  rgn->sampOffset = addrSampStart - spcDirAddr;
  addRgn(rgn);

  return true;
}

bool WhittakerSnesInstr::isValidHeader(RawFile *file,
                                 WhittakerSnesVersion version,
                                 uint32_t addrInstrHeader,
                                 uint32_t spcDirAddr,
                                 bool validateSample) {
  
  auto instrItemSize = WhittakerSnesInstr::expectedSize(version);

  if (addrInstrHeader + instrItemSize > 0x10000) {
    return false;
  }

  std::vector<uint8_t> instrHeader(instrItemSize);
  file->readBytes(addrInstrHeader, instrItemSize, instrHeader.data());

  if (std::all_of(instrHeader.cbegin(), instrHeader.cend(),
                  [](const uint8_t b) { return b == 0x00 || b == 0xFF; })) {
    return false;
  }

  const uint8_t &srcn = instrHeader[0];
  const uint8_t &adsr1 = instrHeader[1];
  const uint8_t &adsr2 = instrHeader[2];
  const uint8_t &gain = instrHeader[3];

  if (srcn >= 0x80 || (adsr1 == 0 && gain == 0)) {
    return false;
  }

  uint32_t addrDIRentry = spcDirAddr + (srcn * 4);
  if (!SNESSampColl::isValidSampleDir(file, addrDIRentry, validateSample)) {
    return false;
  }

  uint16_t srcAddr = file->readShort(addrDIRentry);
  uint16_t loopStartAddr = file->readShort(addrDIRentry + 2);
  if (srcAddr > loopStartAddr || (loopStartAddr - srcAddr) % 9 != 0) {
    return false;
  }

  return true;
}

uint32_t WhittakerSnesInstr::expectedSize(WhittakerSnesVersion version) {
  if (version == NINSNES_EARLIER) {
    return 5;
  }
  else {
    return 6;
  }
}

// ***********
// WhittakerSnesRgn
// ***********

WhittakerSnesRgn::WhittakerSnesRgn(WhittakerSnesInstr *instr,
                       WhittakerSnesVersion ver,
                       uint32_t offset,
                       uint16_t konamiTuningTableAddress,
                       uint8_t konamiTuningTableSize) :
    VGMRgn(instr, offset, WhittakerSnesInstr::expectedSize(ver)), version(ver) {
  uint8_t srcn = readByte(offset);
  uint8_t adsr1 = readByte(offset + 1);
  uint8_t adsr2 = readByte(offset + 2);
  uint8_t gain = readByte(offset + 3);
  int16_t pitch_scale;
  if (version == NINSNES_EARLIER) {
    pitch_scale = (int8_t) readByte(offset + 4) * 256;
  }
  else {
    pitch_scale = getShortBE(offset + 4);
  }

  const double pitch_fixer = 4286.0 / 4096.0;
  double fine_tuning;
  double coarse_tuning;
  fine_tuning = modf((log(pitch_scale * pitch_fixer / 256.0) / log(2.0)) * 12.0, &coarse_tuning);

  // normalize
  if (fine_tuning >= 0.5) {
    coarse_tuning += 1.0;
    fine_tuning -= 1.0;
  }
  else if (fine_tuning <= -0.5) {
    coarse_tuning -= 1.0;
    fine_tuning += 1.0;
  }

  addSampNum(srcn, offset, 1);
  addChild(offset + 1, 1, "ADSR1");
  addChild(offset + 2, 1, "ADSR2");
  addChild(offset + 3, 1, "GAIN");
  if (version == NINSNES_EARLIER) {
    addUnityKey(96 - (int) (coarse_tuning), offset + 4, 1);
    addFineTune((int16_t) (fine_tuning * 100.0), offset + 4, 1);
  }
  else if (version == NINSNES_KONAMI && konamiTuningTableAddress != 0) {
    uint16_t addrTuningTableCoarse = konamiTuningTableAddress;
    uint16_t addrTuningTableFine = konamiTuningTableAddress + konamiTuningTableSize;

    int8_t coarse_tuning;
    uint8_t fine_tuning;
    if (srcn < konamiTuningTableSize) {
      coarse_tuning = readByte(addrTuningTableCoarse + srcn);
      fine_tuning = readByte(addrTuningTableFine + srcn);
    }
    else {
      coarse_tuning = 0;
      fine_tuning = 0;
    }

    double fine_tune_real = fine_tuning / 256.0;
    fine_tune_real += log(4045.0 / 4096.0) / log(2) * 12; // -21.691 cents

    unityKey = 71 - coarse_tuning;
    fineTune = (int16_t) (fine_tune_real * 100.0);

    addChild(offset + 4, 2, "Tuning (Unused)");
  }
  else {
    addUnityKey(96 - (int) (coarse_tuning), offset + 4, 1);
    addFineTune((int16_t) (fine_tuning * 100.0), offset + 5, 1);
  }
  snesConvADSR<VGMRgn>(this, adsr1, adsr2, gain);
}

WhittakerSnesRgn::~WhittakerSnesRgn() {}

bool WhittakerSnesRgn::loadRgn() {
  return true;
}
