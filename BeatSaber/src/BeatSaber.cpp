#include <Ancora.h>
#include <Ancora/Core/EntryPoint.h>

class BeatSaber : public Ancora::Application
{
public:
	BeatSaber()
	{
	}

	~BeatSaber()
	{
	}

};

Ancora::Application* Ancora::CreateApplication()
{
	return new BeatSaber();
}
