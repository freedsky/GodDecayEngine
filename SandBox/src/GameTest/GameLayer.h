#pragma once
#include <GodDecay/Layer.h>
#include <GodDecay/Logger.h>

/// <summary>
/// ≤‚ ‘¿‡
/// </summary>
class GameLayer : public GodDecay::Layer
{
public:
	GameLayer(std::string name);
	~GameLayer();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpDate() override;
	void OnEvents(const GodDecay::Event& e) override;
private:

};

