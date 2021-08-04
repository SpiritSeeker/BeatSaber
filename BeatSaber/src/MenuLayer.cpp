#include "MenuLayer.h"

MenuLayer::MenuLayer()
  : Layer("MenuLayer")
{
  auto& window = Ancora::Application::Get().GetWindow();
  CreateCamera(window.GetWidth(), window.GetHeight());

  Ancora::Random::Init();
}

void MenuLayer::OnAttach()
{
  m_Level.Init();
}

void MenuLayer::OnDetach()
{
}

void MenuLayer::OnUpdate(Ancora::Timestep ts)
{
  m_FPS = 1 / ts;

  if (m_Level.IsGameOver())
    m_State = GameState::GameOver;

  m_Camera->SetView({ 0.0f, 2.0f, 0.0f }, { 0.0f, 1.0f, -10.0f });

  switch (m_State)
  {
    case GameState::Play:
    {
      m_Level.OnUpdate(ts);
      break;
    }
  }

  Ancora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.11f, 1.0f });
  Ancora::RenderCommand::Clear();

  m_Level.OnRender();
}

void MenuLayer::OnImGuiRender()
{
}

void MenuLayer::OnEvent(Ancora::Event& e)
{
	Ancora::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Ancora::WindowResizeEvent>(AE_BIND_EVENT_FN(MenuLayer::OnWindowResize));
	dispatcher.Dispatch<Ancora::MouseButtonPressedEvent>(AE_BIND_EVENT_FN(MenuLayer::OnMouseButtonPressed));
}

bool MenuLayer::OnMouseButtonPressed(Ancora::MouseButtonPressedEvent& e)
{
	if (m_State == GameState::GameOver)
		m_Level.Reset();

	m_State = GameState::Play;
	return false;
}

bool MenuLayer::OnWindowResize(Ancora::WindowResizeEvent& e)
{
	CreateCamera(e.GetWidth(), e.GetHeight());
	return false;
}

void MenuLayer::CreateCamera(uint32_t width, uint32_t height)
{
  float aspectRatio = (float)width / (float)height;
  float fov = 45.0f;
  float nearPlane = 1.0f;
  float farPlane = 1000.0f;

  m_Camera = Ancora::CreateRef<Ancora::PerspectiveCamera>(fov, aspectRatio, nearPlane, farPlane);
  m_Level.UpdateCamera(m_Camera);
}
