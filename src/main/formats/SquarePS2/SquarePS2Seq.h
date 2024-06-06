#pragma once
#include "VGMSeq.h"
#include "SeqTrack.h"
#include "SquarePS2Format.h"

class BGMSeq : public VGMSeq {
 public:
  BGMSeq(RawFile *file, uint32_t offset);
  ~BGMSeq() override = default;

  bool GetHeaderInfo() override;
  bool GetTrackPointers() override;
  uint32_t GetID() const override { return assocWDID; }

 protected:
  unsigned short seqID;
  unsigned short assocWDID;
};


class BGMTrack : public SeqTrack {
 public:
  BGMTrack(BGMSeq *parentSeq, uint32_t offset = 0, uint32_t length = 0);

  bool ReadEvent() override;
};