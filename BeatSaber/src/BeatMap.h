#pragma once

#include <Ancora.h>

enum class NoteType {
  Red = 0, Blue = 1, Invalid = 2
};

enum class NoteDirection {
  Up = 0, Down, Left, Right, UpperLeft, UpperRight, LowerLeft, LowerRight, Dot
};

struct NoteBox {
  double Time;
  uint32_t LineIndex;
  uint32_t LineLayer;
  NoteType Type;
  NoteDirection Direction;
};

class BeatMap
{
public:
  BeatMap(const std::string& filepath);
  ~BeatMap() = default;

  const std::vector<NoteBox>& GetNotes() { return m_Notes; }
private:
  void GenerateNotes(const std::string& source);
  std::string ReadFile(const std::string& filepath);
private:
  std::vector<NoteBox> m_Notes;
};
