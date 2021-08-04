#pragma once

#include <Ancora.h>
#include <filesystem>
#include "BeatMap.h"

enum class Difficulty {
  None = 0, Easy, Normal, Hard, Expert, ExpertPlus
};

struct BeatMapData {
  std::string FilePath;
  double NoteJumpSpeed;
};

struct BeatData {
  std::string SongName;
  uint32_t BPM;
  std::unordered_map<Difficulty, BeatMapData> BeatMaps;
  std::string FolderPath;
  std::string CoverPath;
  std::string MusicPath;
};

class BeatMapper
{
public:
  BeatMapper(const std::string& filepath);
  ~BeatMapper() = default;

  const BeatData& GetBeatData() { return m_BeatData; }
  uint32_t GetBPM() { return m_BeatData.BPM; }
  const std::string& GetCover() { return m_BeatData.CoverPath; }
  const std::string& GetMusic() { return m_BeatData.MusicPath; }
  Ancora::Ref<BeatMap> GetBeatMap(Difficulty difficulty);
private:
  std::string ReadFile(const std::string& filepath);
  void ReadInfoFile(const std::string& filepath);
private:
  struct BeatData m_BeatData;
  std::unordered_map<Difficulty, Ancora::Ref<BeatMap>> m_BeatMaps;
};
