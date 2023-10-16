#include <GodDecayTop.h>
#include <GodDecay/Core/EnterPoint.h>

#include "EditorLayer.h"

namespace GodDecay 
{
	class GodDecayluntApp : public GodDecay::Application
	{
	public:
		GodDecayluntApp()
		{
			PushLayer(new EditorLayer("EngineEditorLayer"));
		}

		~GodDecayluntApp()
		{

		}
	};

	GodDecay::Application* CreateApplication()
	{
		return new GodDecayluntApp();
	}
}

