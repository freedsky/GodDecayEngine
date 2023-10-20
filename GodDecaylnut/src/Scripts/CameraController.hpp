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
			auto& transform = GetComponent<TransformComponent>().Translation;
			static float speed = 2.0f;

			if (Input::IsKeyPressed(GODDECAY_KEY_A))
			{
				transform.x -= speed * delteTime;
			}
			if (Input::IsKeyPressed(GODDECAY_KEY_D))
			{
				transform.x += speed * delteTime;
			}
			if (Input::IsKeyPressed(GODDECAY_KEY_W))
			{
				transform.y += speed * delteTime;
			}
			if (Input::IsKeyPressed(GODDECAY_KEY_S))
			{
				transform.y -= speed * delteTime;
			}
		}
	};
}