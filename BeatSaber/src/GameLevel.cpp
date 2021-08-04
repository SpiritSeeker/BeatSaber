#include "GameLevel.h"

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

	m_SecondsPerBeat = 60.0 / m_Mapper->GetBPM();

	double moveDistance = 18000.0 / m_Mapper->GetBPM();
	double highThreshold = 2700 / moveDistance;
	double lowThreshold = highThreshold / 2;
	double njs = m_Mapper->GetBeatData().BeatMaps.at(Difficulty::Expert).NoteJumpSpeed;
	if (njs < highThreshold)
	{
		if (njs < lowThreshold)
			m_HalfJumpDuration = 4;
		else
			m_HalfJumpDuration = 2;
	}
	else
		m_HalfJumpDuration = 1;

	m_HalfJumpDistance = njs * moveDistance * m_HalfJumpDuration / 300;
	m_HalfJumpDuration *= m_SecondsPerBeat;
	m_Speed = m_HalfJumpDistance / m_HalfJumpDuration;
	m_Time = -m_HalfJumpDuration;

  m_SceneData.DirLight = Ancora::Light::CreateDirectionalLight(glm::vec3(0.0f, -1.0f, -2.0f));
}

void GameLevel::OnUpdate(Ancora::Timestep ts)
{
	m_Time += ts;

	UpdateBoxPositions(ts);
	GetNewBoxes();
}

void GameLevel::UpdateBoxPositions(Ancora::Timestep ts)
{
	if (!m_RedPositions.empty())
	{
		for (int i = 0; i < m_RedPositions.size(); i++)
			m_RedPositions[i].z += ts * m_Speed;
		while (!m_RedPositions.empty())
		{
			if (m_RedPositions.front().z > m_HalfJumpDistance)
			{
				m_RedPositions.pop_front();
				m_RedTransforms.pop_front();
			}
			else
				break;
		}
	}

	if (!m_BluePositions.empty())
	{
		for (int i = 0; i < m_BluePositions.size(); i++)
			m_BluePositions[i].z += ts * m_Speed;
		while (!m_BluePositions.empty())
		{
			if (m_BluePositions.front().z > m_HalfJumpDistance)
			{
				m_BluePositions.pop_front();
				m_BlueTransforms.pop_front();
			}
			else
				break;
		}
	}
}

void GameLevel::GetNewBoxes()
{
	auto notes = m_Map->GetNotes();

	if (m_Index < notes.size())
	{
		while ((notes[m_Index].Time * m_SecondsPerBeat <= m_Time))
		{
			if (notes[m_Index].Type == NoteType::Red)
			{
				m_RedTransforms.push_back(GetTransform(notes[m_Index].Direction));
				m_RedPositions.push_back({ m_XScale * (notes[m_Index].LineIndex - 1.5), m_YScale * notes[m_Index].LineLayer, -m_HalfJumpDistance });
			}
			else if (notes[m_Index].Type == NoteType::Blue)
			{
				m_BlueTransforms.push_back(GetTransform(notes[m_Index].Direction));
				m_BluePositions.push_back({ m_XScale * (notes[m_Index].LineIndex - 1.5), m_YScale * notes[m_Index].LineLayer, -m_HalfJumpDistance });
			}
			m_Index++;
			if (m_Index >= notes.size())
				break;
		}
	}
}

void GameLevel::OnRender()
{
  Ancora::Renderer3D::BeginScene(m_SceneData);

  // Ancora::Renderer3D::DrawModel(m_RedBox, glm::translate(glm::mat4(1.0f), glm::vec3({ -1.0f, 0.0f, -15.0f })) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 0.0f, -1.0f, 0.0f })) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 0.0f, 0.0f, -1.0f })) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
  // Ancora::Renderer3D::DrawModel(m_BlueBox, glm::translate(glm::mat4(1.0f), glm::vec3({ 1.0f, 0.0f, -15.0f })) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 0.0f, -1.0f, 0.0f })) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 0.0f, 0.0f, -1.0f })) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
	// Ancora::Renderer3D::DrawModel(m_BlueBox, glm::translate(glm::mat4(1.0f), glm::vec3({ -1.5f, -1.0f, -10.0f })) * GetTransform(NoteDirection::Up));
	if (!m_RedPositions.empty())
	{
		for (int i = 0; i < m_RedPositions.size(); i++)
			Ancora::Renderer3D::DrawModel(m_RedBox, glm::translate(glm::mat4(1.0f), m_RedPositions[i]) * m_RedTransforms[i]);
	}
	if (!m_BluePositions.empty())
	{
		for (int i = 0; i < m_BluePositions.size(); i++)
			Ancora::Renderer3D::DrawModel(m_BlueBox, glm::translate(glm::mat4(1.0f), m_BluePositions[i]) * m_BlueTransforms[i]);
	}

  Ancora::Renderer3D::EndScene();
}

glm::mat4 GameLevel::GetTransform(NoteDirection direction)
{
	switch (direction)
	{
		case NoteDirection::Up:
			return glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 0.0f, 0.0f, 1.0f })) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 1.0f, 0.0f, 0.0f })) * glm::scale(glm::mat4(1.0f), glm::vec3(0.4f));
		case NoteDirection::Down:
			return glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 0.0f, 0.0f, -1.0f })) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 1.0f, 0.0f, 0.0f })) * glm::scale(glm::mat4(1.0f), glm::vec3(0.4f));
		case NoteDirection::Left:
			return glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3({ 0.0f, 0.0f, 1.0f })) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 1.0f, 0.0f, 0.0f })) * glm::scale(glm::mat4(1.0f), glm::vec3(0.4f));
		case NoteDirection::Right:
			return glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 1.0f, 0.0f, 0.0f })) * glm::scale(glm::mat4(1.0f), glm::vec3(0.4f));
		case NoteDirection::UpperLeft:
			return glm::rotate(glm::mat4(1.0f), glm::radians(135.0f), glm::vec3({ 0.0f, 0.0f, 1.0f })) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 1.0f, 0.0f, 0.0f })) * glm::scale(glm::mat4(1.0f), glm::vec3(0.4f));
		case NoteDirection::UpperRight:
			return glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3({ 0.0f, 0.0f, 1.0f })) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 1.0f, 0.0f, 0.0f })) * glm::scale(glm::mat4(1.0f), glm::vec3(0.4f));
		case NoteDirection::LowerLeft:
			return glm::rotate(glm::mat4(1.0f), glm::radians(135.0f), glm::vec3({ 0.0f, 0.0f, -1.0f })) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 1.0f, 0.0f, 0.0f })) * glm::scale(glm::mat4(1.0f), glm::vec3(0.4f));
		case NoteDirection::LowerRight:
			return glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3({ 0.0f, 0.0f, -1.0f })) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ 1.0f, 0.0f, 0.0f })) * glm::scale(glm::mat4(1.0f), glm::vec3(0.4f));
		case NoteDirection::Dot:
			return glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3({ -1.0f, 0.0f, 0.0f })) * glm::scale(glm::mat4(1.0f), glm::vec3(0.4f));
		default:
			return glm::scale(glm::mat4(1.0f), glm::vec3(0.4f));
	}
}

void GameLevel::Reset()
{
  m_GameOver = false;
}
