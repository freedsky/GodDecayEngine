#pragma once

#include "GodDecayTop.h"

namespace GodDecay 
{
	class CameraContorller : public ScriptableEntity
	{
	public:
		virtual void OnCreate() override
		{

		}

		virtual void OnDestroy() override
		{

		}

		virtual void OnUpdate(float delteTime) override
		{
			auto& transform = GetComponent<TransformComponent>().Transform;
			static float speed = 2.0f;

			if (Input::IsKeyPressed(GODDECAY_KEY_A))
			{
				transform[3][0] -= speed * delteTime;
			}
			if (Input::IsKeyPressed(GODDECAY_KEY_D))
			{
				transform[3][0] += speed * delteTime;
			}
			if (Input::IsKeyPressed(GODDECAY_KEY_W))
			{
				transform[3][1] += speed * delteTime;
			}
			if (Input::IsKeyPressed(GODDECAY_KEY_S))
			{
				transform[3][1] -= speed * delteTime;
			}
		}
	};
}