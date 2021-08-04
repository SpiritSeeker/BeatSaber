#include "GameLevel.h"
#include <glm/gtc/matrix_transform.hpp>

void GameLevel::Init()
{
	#ifdef AE_PLATFORM_LINUX
		m_RedBox = Ancora::ModelLoader::LoadModel("BeatSaber/assets/models/box_red_arrow.fbx");
		m_BlueBox = Ancora::ModelLoader::LoadModel("BeatSaber/assets/models/box_blue_arrow.fbx");
	#elif AE_PLATFORM_WINDOWS
		m_RedBox = Ancora::ModelLoader::LoadModel("assets/models/box_red_arrow.fbx");
		m_BlueBox = Ancora::ModelLoader::LoadModel("assets/models/box_blue_arrow.fbx");
	#else
		AE_ERROR("Only Linux and Windows supported!");
	#endif

	m_Mapper = Ancora::CreateRef<BeatMapper>("assets/beatmaps/Legends Never Die");
	m_Map = m_Mapper->GetBeatMap(Difficulty::ExpertPlus);

  m_SceneData.DirLight = Ancora::Light::CreateDirectionalLight(glm::vec3(0.0f, -1.0f, -2.0f));
}

void GameLevel::OnUpdate(Ancora::Timestep ts)
{
	m_Red += ts * m_Speed;
	m_Blue += ts * m_Speed;
	if (m_Red > 10)
	{
		m_Red = -20.0f;
		m_Blue = -20.0f;
	}
}

void GameLevel::OnRender()
{
  Ancora::Renderer3D::BeginScene(m_SceneData);

  Ancora::Renderer3D::DrawModel(m_RedBox, glm::translate(glm::mat4(1.0f), glm::vec3({ -1.0f, 0.0f, m_Red })) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 0.0f, -1.0f, 0.0f })) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 0.0f, 0.0f, -1.0f })) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
  Ancora::Renderer3D::DrawModel(m_BlueBox, glm::translate(glm::mat4(1.0f), glm::vec3({ 1.0f, 0.0f, m_Blue })) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 0.0f, -1.0f, 0.0f })) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 0.0f, 0.0f, -1.0f })) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));

  Ancora::Renderer3D::EndScene();
}

void GameLevel::Reset()
{
  m_GameOver = false;
}
