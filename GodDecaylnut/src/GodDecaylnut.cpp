#include <GodDecayTop.h>
#include <GodDecay/Core/EnterPoint.h>

#include "EditorLayer.h"

namespace GodDecay
{
	class GodDecaylnut : public Application
	{
	public:
		GodDecaylnut()
		{
			PushLayer(new EditorLayer("Engine_Editor"));
		}

		~GodDecaylnut()
		{

		}
	};

	Application* CreateApplication()
	{
		return new GodDecaylnut();
	}
}


