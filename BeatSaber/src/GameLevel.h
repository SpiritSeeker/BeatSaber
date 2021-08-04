#pragma once

#include <Ancora.h>
#include <glm/gtc/matrix_transform.hpp>

#include "BeatMapper.h"

class GameLevel
{
public:
  void Init();

  void OnUpdate(Ancora::Timestep ts);
  void OnRender();

  bool IsGameOver() const { return m_GameOver; }
  void Reset();

  void UpdateCamera(const Ancora::Ref<Ancora::PerspectiveCamera>& camera) { m_SceneData.Camera = camera; }
private:
  void UpdateBoxPositions(Ancora::Timestep ts);
  void GetNewBoxes();

  glm::mat4 GetTransform(NoteDirection direction);
private:
  Ancora::Ref<BeatMapper> m_Mapper;
  Ancora::Ref<BeatMap> m_Map;

  double m_SecondsPerBeat;
  double m_HalfJumpDistance;
  double m_HalfJumpDuration;
	double m_Speed;

  double m_XScale = 1.2;
  double m_YScale = 1.0;

	Ancora::Ref<Ancora::Model3D> m_RedBox;
	Ancora::Ref<Ancora::Model3D> m_BlueBox;
  std::deque<glm::mat4> m_RedTransforms;
  std::deque<glm::mat4> m_BlueTransforms;
  std::deque<glm::vec3> m_RedPositions;
  std::deque<glm::vec3> m_BluePositions;

  Ancora::Renderer3DSceneData m_SceneData;

  double m_Time;
  int m_Index = 0;

  bool m_GameOver = false;
};
