#pragma once

namespace GodDecay 
{
	enum class TimeType
	{
		None = 0,
		DeltaTime = 1
	};

	class Time 
	{
	public:
		Time(TimeType type) : timeType(type){}
		virtual ~Time() = default;

		virtual float GetTime() = 0;
		
		inline const TimeType GetTimeType() const { return timeType; }
		inline const void SetTimeType(TimeType type) { timeType = type; }
	private:
		TimeType timeType;
	};
}