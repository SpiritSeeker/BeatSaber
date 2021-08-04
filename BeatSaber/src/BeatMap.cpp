#include "BeatMap.h"

#include <fstream>

BeatMap::BeatMap(const std::string& filepath)
{
  std::string source = ReadFile(filepath);
  GenerateNotes(source);
}

void BeatMap::GenerateNotes(const std::string& source)
{
  const char* noteToken = "\"_notes\"";
  const char* timeToken = "\"_time\":";
  const char* indexToken = "\"_lineIndex\":";
  const char* layerToken = "\"_lineLayer\":";
  const char* typeToken = "\"_type\":";
  const char* cutToken = "\"_cutDirection\":";

  size_t pos = source.find(noteToken, 0);
  size_t start = source.find_first_of("[", pos);
  size_t end = source.find_first_of("]", start);
  size_t noteStart = source.find_first_of("{", start);
  while (noteStart < end) {
    size_t timeStart = source.find_first_of(timeToken, noteStart) + strlen(timeToken);
    size_t timeEnd = source.find_first_of(",", timeStart);
    std::string timeStr = source.substr(timeStart, timeEnd - timeStart);
    NoteBox note;
    note.Time = std::stod(timeStr);
    std::cout << std::stod(timeStr) << std::endl;

    size_t indexStart = source.find_first_of(indexToken, timeEnd) + strlen(indexToken);
    size_t indexEnd = source.find_first_of(",", indexStart);
    std::string indexStr = source.substr(indexStart, indexEnd - indexStart);
    note.LineIndex = std::stoi(indexStr);
    std::cout << std::stoi(indexStr) << std::endl;

    size_t layerStart = source.find_first_of(layerToken, indexEnd) + strlen(layerToken);
    size_t layerEnd = source.find_first_of(",", layerStart);
    std::string layerStr = source.substr(layerStart, layerEnd - layerStart);
    note.LineLayer = std::stoi(layerStr);
    std::cout << std::stoi(layerStr) << std::endl;

    size_t typeStart = source.find_first_of(typeToken, layerEnd) + strlen(typeToken);
    size_t typeEnd = source.find_first_of(",", typeStart);
    std::string typeStr = source.substr(typeStart, typeEnd - typeStart);
    int type = std::stoi(typeStr);
    if (type < 2)
      note.Type = (NoteType)type;
    else
      note.Type = NoteType::Invalid;
    std::cout << static_cast<std::underlying_type<NoteType>::type>(note.Type) << std::endl;

    size_t cutStart = source.find_first_of(cutToken, typeEnd) + strlen(cutToken);
    size_t cutEnd = source.find_first_of("}", cutStart);
    std::string cutStr = source.substr(cutStart, cutEnd - cutStart);
    int cut = std::stoi(cutStr);
    note.Direction = (NoteDirection)cut;
    std::cout << static_cast<std::underlying_type<NoteDirection>::type>(note.Direction) << std::endl;

    if (note.Type != NoteType::Invalid)
      m_Notes.push_back(note);
    std::cout << "\n";
    noteStart = source.find_first_of("{", cutEnd);
  }
}

std::string BeatMap::ReadFile(const std::string& filepath)
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
