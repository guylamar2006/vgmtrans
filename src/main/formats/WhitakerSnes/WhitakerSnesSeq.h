#pragma once
#include "VGMMultiSectionSeq.h"
#include "SeqTrack.h"
#include "WhitakerSnesFormat.h"
#include "WhitakerSnesScanner.h"

enum WhitakerSnesSeqEventType {
  //start enum at 1 because if map[] look up fails, it returns 0, and we don't want that to get confused with a legit event
  EVENT_UNKNOWN0 = 1,
  EVENT_UNKNOWN1,
  EVENT_UNKNOWN2,
  EVENT_UNKNOWN3,
  EVENT_UNKNOWN4,
  EVENT_NOP,
  EVENT_NOP1,
  EVENT_END,
  EVENT_NOTE_PARAM,
  EVENT_NOTE,
  EVENT_TIE,
  EVENT_REST,
  EVENT_PERCUSSION_NOTE,
  EVENT_PROGCHANGE,
  EVENT_PAN,
  EVENT_PAN_FADE,
  EVENT_VIBRATO_ON,
  EVENT_VIBRATO_OFF,
  EVENT_MASTER_VOLUME,
  EVENT_MASTER_VOLUME_FADE,
  EVENT_TEMPO,
  EVENT_TEMPO_FADE,
  EVENT_GLOBAL_TRANSPOSE,
  EVENT_TRANSPOSE,
  EVENT_TREMOLO_ON,
  EVENT_TREMOLO_OFF,
  EVENT_VOLUME,
  EVENT_VOLUME_FADE,
  EVENT_CALL,
  EVENT_VIBRATO_FADE,
  EVENT_PITCH_ENVELOPE_TO,
  EVENT_PITCH_ENVELOPE_FROM,
  EVENT_PITCH_ENVELOPE_OFF,
  EVENT_TUNING,
  EVENT_ECHO_ON,
  EVENT_ECHO_OFF,
  EVENT_ECHO_PARAM,
  EVENT_ECHO_VOLUME_FADE,
  EVENT_PITCH_SLIDE,
  EVENT_PERCCUSION_PATCH_BASE,

  // Nintendo RD2:
      EVENT_RD2_PROGCHANGE_AND_ADSR,

  // Akklaim:
      EVENT_AKKLAIM_LOOP_START,
  EVENT_AKKLAIM_LOOP_END,
  EVENT_AKKLAIM_ADSR_AND_GAIN,

  // Lemmings:
      EVENT_LEMMINGS_NOTE_PARAM,

  // Intelligent Systems:
  // Fire Emblem 3 & 4:
      EVENT_INTELLI_NOTE_PARAM,
  EVENT_INTELLI_ECHO_ON,
  EVENT_INTELLI_ECHO_OFF,
  EVENT_INTELLI_LEGATO_ON,
  EVENT_INTELLI_LEGATO_OFF,
  EVENT_INTELLI_JUMP_SHORT_CONDITIONAL,
  EVENT_INTELLI_JUMP_SHORT,
  EVENT_INTELLI_FE3_EVENT_F5,
  EVENT_INTELLI_WRITE_APU_PORT,
  EVENT_INTELLI_FE3_EVENT_F9,
  EVENT_INTELLI_DEFINE_VOICE_PARAM,
  EVENT_INTELLI_LOAD_VOICE_PARAM,
  EVENT_INTELLI_ADSR,
  EVENT_INTELLI_GAIN_SUSTAIN_TIME_AND_RATE,
  EVENT_INTELLI_GAIN_SUSTAIN_TIME,
  EVENT_INTELLI_GAIN,
  EVENT_INTELLI_FE4_EVENT_FC,
  EVENT_INTELLI_TA_SUBEVENT,
  EVENT_INTELLI_FE4_SUBEVENT,

  // Quintet:
  EVENT_QUINTET_TUNING,
  EVENT_QUINTET_ADSR,
};

class WhitakerSnesTrackSharedData {
 public:
  WhitakerSnesTrackSharedData();

  virtual void resetVars();

  uint8_t spcNoteDuration;
  uint8_t spcNoteDurRate;
  uint8_t spcNoteVolume;
  int8_t spcTranspose;
  uint16_t loopReturnAddress;
  uint16_t loopStartAddress;
  uint8_t loopCount;

  // Akklaim:
  uint16_t akklaimLoopStart;
  uint8_t akklaimLoopCount;
};

class WhitakerSnesSeq:
    public VGMMultiSectionSeq {
 public:
  WhitakerSnesSeq(RawFile *file,
             WhitakerSnesVersion ver,
             uint32_t offset,
             uint8_t percussion_base = 0,
             const std::vector<uint8_t> &theVolumeTable = std::vector<uint8_t>(),
             const std::vector<uint8_t> &theDurRateTable = std::vector<uint8_t>(),
             std::string theName = "WhitakerSnes Seq");
  virtual ~WhitakerSnesSeq();

  virtual bool parseHeader();
  virtual void resetVars();
  virtual bool readEvent(long stopTime);

  double getTempoInBPM();
  double getTempoInBPM(uint8_t tempo);

  uint16_t convertToAPUAddress(uint16_t offset);
  uint16_t getShortAddress(uint32_t offset);

  WhitakerSnesVersion version;
  uint8_t STATUS_END;
  uint8_t STATUS_NOTE_MIN;
  uint8_t STATUS_NOTE_MAX;
  uint8_t STATUS_PERCUSSION_NOTE_MIN;
  uint8_t STATUS_PERCUSSION_NOTE_MAX;
  std::map<uint8_t, WhitakerSnesSeqEventType> EventMap;

  WhitakerSnesTrackSharedData sharedTrackData[8];

  std::vector<uint8_t> volumeTable;
  std::vector<uint8_t> durRateTable;
  std::vector<uint8_t> panTable;

  uint8_t spcPercussionBase;
  uint8_t sectionRepeatCount;

  // Akklaim:
  uint16_t akklaimBaseAddress;

  // Intelligent Systems:
  std::vector<uint8_t> intelliDurVolTable;
  bool intelliUseCustomNoteParam;
  bool intelliUseCustomPercTable;
  uint16_t intelliVoiceParamTable;
  uint8_t intelliVoiceParamTableSize;

  // Quintet
  uint8_t quintetBGMInstrBase;
  uint16_t quintetAddrBGMInstrLookup;

  // Falcom:
  uint16_t falcomBaseOffset;

protected:
  VGMHeader *header;

 private:
  void loadEventMap();
  void loadStandardVcmdMap(uint8_t statusByte);

  uint8_t spcPercussionBaseInit;
};

class WhitakerSnesSection
    : public VGMSeqSection {
 public:
  WhitakerSnesSection(WhitakerSnesSeq *parentFile, uint32_t offset = 0, uint32_t length = 0);

  virtual bool parseTrackPointers();

  uint16_t convertToApuAddress(uint16_t offset);
  uint16_t getShortAddress(uint32_t offset);
};

class WhitakerSnesTrack
    : public SeqTrack {
 public:
  WhitakerSnesTrack
      (WhitakerSnesSection *parentSection, uint32_t offset = 0, uint32_t length = 0, const std::string &theName = "WhitakerSnes Track");

  virtual void resetVars();
  virtual bool readEvent();

  uint16_t convertToApuAddress(uint16_t offset);
  uint16_t getShortAddress(uint32_t offset);
  void getVolumeBalance(uint16_t pan, double &volumeLeft, double &volumeRight);
  uint8_t readPanTable(uint16_t pan);
  int8_t calculatePanValue(uint8_t pan, double &volumeScale, bool &reverseLeft, bool &reverseRight);

  WhitakerSnesSection *parentSection;
  WhitakerSnesTrackSharedData *shared;
  bool available;
};
