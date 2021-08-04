#include "BeatMapper.h"

BeatMapper::BeatMapper(const std::string& filepath)
{
  m_BeatData = BeatData();
  if (filepath.back() != '/')
    m_BeatData.FolderPath = filepath + '/';
  else
    m_BeatData.FolderPath = filepath;

  std::string infoPath = m_BeatData.FolderPath + "info.dat";
  ReadInfoFile(infoPath);
}

Ancora::Ref<BeatMap> BeatMapper::GetBeatMap(Difficulty difficulty)
{
  if (m_BeatMaps.find(difficulty) != m_BeatMaps.end())
    return m_BeatMaps[difficulty];

  Ancora::Ref<BeatMap> beatMap = Ancora::CreateRef<BeatMap>(m_BeatData.FolderPath + m_BeatData.BeatMaps[difficulty].FilePath);
  m_BeatMaps[difficulty] = beatMap;
  return beatMap;
}

void BeatMapper::ReadInfoFile(const std::string& filepath)
{
  std::string source = ReadFile(filepath);
  std::cout << "Info File: " << filepath << std::endl;

  const char* songNameToken = "\"_songName\"";
  size_t pos = source.find(songNameToken, 0);
  size_t start = source.find_first_of("\"", pos + strlen(songNameToken)) + 1;
  size_t end = source.find_first_of("\"", start);
  m_BeatData.SongName = source.substr(start, end - start);
  std::cout << "  Name: " << m_BeatData.SongName << std::endl;

  const char* bpmToken = "\"_beatsPerMinute\"";
  pos = source.find(bpmToken, end);
  start = source.find_first_of(":", pos + strlen(bpmToken)) + 1;
  end = source.find_first_of(",", start);
  m_BeatData.BPM = stoi(source.substr(start, end - start));
  std::cout << "  BPM: " << m_BeatData.BPM << std::endl;

  const char* songFileToken = "\"_songFilename\"";
  pos = source.find(songFileToken, end);
  start = source.find_first_of("\"", pos + strlen(songFileToken)) + 1;
  end = source.find_first_of("\"", start);
  m_BeatData.MusicPath = source.substr(start, end - start);
  std::cout << "  Song Filename: " << m_BeatData.MusicPath << std::endl;

  const char* coverFileToken = "\"_coverImageFilename\"";
  pos = source.find(coverFileToken, end);
  start = source.find_first_of("\"", pos + strlen(coverFileToken)) + 1;
  end = source.find_first_of("\"", start);
  m_BeatData.CoverPath = source.substr(start, end - start);
  std::cout << "  Cover Path: " << m_BeatData.CoverPath << "\n" << std::endl;

  const char* difficultyToken = "\"_difficulty\"";
  const char* fileNameToken = "\"_beatmapFilename\"";
  const char* njsToken = "\"_noteJumpMovementSpeed\"";
  pos = source.find(difficultyToken, end);
  while (pos != std::string::npos)
  {
    start = source.find_first_of("\"", pos + strlen(difficultyToken)) + 1;
    end = source.find_first_of("\"", start);
    std::string difficultyStr = source.substr(start, end - start);
    pos = source.find(fileNameToken, end);
    start = source.find_first_of("\"", pos + strlen(fileNameToken)) + 1;
    end = source.find_first_of("\"", start);
    std::string fileName = source.substr(start, end - start);
    pos = source.find(njsToken, start);
    start = source.find_first_of(":", pos + strlen(njsToken)) + 1;
    end = source.find_first_of(",", start);
    double njs = stod(source.substr(start, end - start));

    if (difficultyStr == "Easy")
    {
      m_BeatData.BeatMaps[Difficulty::Easy].FilePath = fileName;
      m_BeatData.BeatMaps[Difficulty::Easy].NoteJumpSpeed = njs;
    }
    if (difficultyStr == "Normal")
    {
      m_BeatData.BeatMaps[Difficulty::Normal].FilePath = fileName;
      m_BeatData.BeatMaps[Difficulty::Normal].NoteJumpSpeed = njs;
    }
    if (difficultyStr == "Hard")
    {
      m_BeatData.BeatMaps[Difficulty::Hard].FilePath = fileName;
      m_BeatData.BeatMaps[Difficulty::Hard].NoteJumpSpeed = njs;
    }
    if (difficultyStr == "Expert")
    {
      m_BeatData.BeatMaps[Difficulty::Expert].FilePath = fileName;
      m_BeatData.BeatMaps[Difficulty::Expert].NoteJumpSpeed = njs;
    }
    if (difficultyStr == "ExpertPlus")
    {
      m_BeatData.BeatMaps[Difficulty::ExpertPlus].FilePath = fileName;
      m_BeatData.BeatMaps[Difficulty::ExpertPlus].NoteJumpSpeed = njs;
    }

    std::cout << "  " << difficultyStr << ": " << fileName << ", " << njs << std::endl;
    pos = source.find(difficultyToken, end);
  }
  std::cout << "\n";
}

std::string BeatMapper::ReadFile(const std::string& filepath)
{
  std::string result;
  std::ifstream in(filepath, std::ios::in | std::ios::binary);
  if (in)
  {
    in.seekg(0, std::ios::end);
    result.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&result[0], result.size());
    in.close();
  }
  else
    std::cout << "Error: Could not open file " << filepath << std::endl;

  return result;
}
