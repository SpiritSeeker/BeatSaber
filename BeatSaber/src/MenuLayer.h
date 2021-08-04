#pragma once

#include <Ancora.h>
#include "GameLevel.h"

class MenuLayer : public Ancora::Layer
{
public:
  MenuLayer();
  virtual ~MenuLayer() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;

  void OnUpdate(Ancora::Timestep ts) override;
  virtual void OnImGuiRender() override;
  void OnEvent(Ancora::Event& e) override;
private:
  void CreateCamera(uint32_t width, uint32_t height);
  bool OnMouseButtonPressed(Ancora::MouseButtonPressedEvent& e);
  bool OnWindowResize(Ancora::WindowResizeEvent& e);
private:
  Ancora::Ref<Ancora::PerspectiveCamera> m_Camera;
  GameLevel m_Level;
  int m_FPS;

  float m_Time = 0.0f;

  enum class GameState
  {
    Play = 0, Menu = 1, GameOver = 2
  };

  GameState m_State = GameState::Play;
};
