#pragma once

#include <Ancora.h>

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
  Ancora::Ref<BeatMapper> m_Mapper;
  Ancora::Ref<BeatMap> m_Map;


	Ancora::Ref<Ancora::Model3D> m_RedBox;
	Ancora::Ref<Ancora::Model3D> m_BlueBox;
	float m_Red = -20.0f;
	float m_Blue = -20.0f;
	float m_Speed;

  Ancora::Renderer3DSceneData m_SceneData;

  bool m_GameOver = false;
};
