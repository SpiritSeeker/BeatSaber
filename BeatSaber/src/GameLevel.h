#pragma once

#include <Ancora.h>

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
	Ancora::Ref<Ancora::Model3D> m_RedBox;
	Ancora::Ref<Ancora::Model3D> m_BlueBox;
	float m_Red = -20.0f;
	float m_Blue = -20.0f;
	float m_Speed = 10.0f;

  Ancora::Ref<Ancora::CubeMap> m_CubeMap;

  Ancora::Renderer3DSceneData m_SceneData;

  bool m_GameOver = false;
};
