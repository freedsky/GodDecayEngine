#pragma once


/// <summary>
/// 抽象时间类，在引擎后期会在各个系统使用不同种类的时间系统
/// 并且时间与平台差异无关
/// </summary>
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