#include <Ancora.h>
#include <Ancora/Core/EntryPoint.h>

#include "MenuLayer.h"

class BeatSaber : public Ancora::Application
{
public:
	BeatSaber()
	{
		PushLayer(new MenuLayer());
	}

	~BeatSaber()
	{
	}

};

Ancora::Application* Ancora::CreateApplication()
{
	return new BeatSaber();
}
