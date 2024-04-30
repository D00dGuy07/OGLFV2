#pragma once

#include "UI/Surface.h"

#include <unordered_map>
#include <iostream>

namespace UI
{
	namespace EasingFunctions
	{
		double easeLinear(double x);
			   
		double easeInSine(double x);
		double easeOutSine(double x);
		double easeInOutSine(double x);
			   
		double easeInQuad(double x);
		double easeOutQuad(double x);
		double easeInOutQuad(double x);
			   
		double easeInCubic(double x);
		double easeOutCubic(double x);
		double easeInOutCubic(double x);
			   
		double easeInQuart(double x);
		double easeOutQuart(double x);
		double easeInOutQuart(double x);
			   
		double easeInQuint(double x);
		double easeOutQuint(double x);
		double easeInOutQuint(double x);
			   
		double easeInExpo(double x);
		double easeOutExpo(double x);
		double easeInOutExpo(double x);
			   
		double easeInCirc(double x);
		double easeOutCirc(double x);
		double easeInOutCirc(double x);
			   
		double easeInBack(double x);
		double easeOutBack(double x);
		double easeInOutBack(double x);
			   
		double easeInElastic(double x);
		double easeOutElastic(double x);
		double easeInOutElastic(double x);
			   
		double easeInBounce(double x);
		double easeOutBounce(double x);
		double easeInOutBounce(double x);
	}

	enum class EasingType
	{
		Linear,

		InSine,
		OutSine,
		InOutSine,

		InQuad,
		OutQuad,
		InOutQuad,

		InCubic,
		OutCubic,
		InOutCubic,

		InQuart,
		OutQuart,
		InOutQuart,

		InQuint,
		OutQuint,
		InOutQuint,

		InExpo,
		OutExpo,
		InOutExpo,

		InCirc,
		OutCirc,
		InOutCirc,

		InBack,
		OutBack,
		InOutBack,

		InElastic,
		OutElastic,
		InOutElastic,

		InBounce,
		OutBounce,
		InOutBounce,
	};

	template<EasingType easingType, typename T, typename J = double>
	class Animator
	{
	public:

		Animator(T* value, T goal, Surface* surface, double duration, double delay = 0.0, 
			std::optional<std::function<void()>> callback = std::nullopt)
			: m_Value(value), m_Difference(goal - *value), m_Start(*value), m_Duration(duration), m_Timer(delay), m_Callback(callback)
		{
			m_Connection = surface->ConnectTimedUpdate([&, surface](double deltaTime)
				{
					m_Timer -= deltaTime;
					if (m_Timer < 0.0)
					{
						if (m_IsWaiting)
						{
							m_Timer = m_Duration;
							m_IsWaiting = false;
						}
						else
						{
							m_Connection->Disconnect();
							m_IsDone = true;
							if (m_Callback)
								m_Callback->operator()();
							return;
						}
					}

					if (!m_IsWaiting)
					{
						double animationProgress = (m_Duration - m_Timer) / m_Duration;
						*m_Value = m_Start + m_Difference * static_cast<J>(Ease(animationProgress));
						surface->ShouldRender = true;
					}
				}
			);
		}

		~Animator()
		{ m_Connection->Disconnect(); }

		inline bool IsDone() const { return m_IsDone; }

	private:

		double m_Timer;
		double m_Duration;

		bool m_IsWaiting = true;
		bool m_IsDone = false;
		std::optional<std::function<void()>> m_Callback;

		T m_Difference;
		T m_Start;
		T* m_Value;

		std::shared_ptr<EventConnection<double>> m_Connection;

		double Ease(double x)
		{
			switch (easingType)
			{
				using namespace EasingFunctions;
			case EasingType::Linear:
				return easeLinear(x);
			case EasingType::InSine:
				return easeInSine(x);
			case EasingType::OutSine:
				return easeOutSine(x);
			case EasingType::InOutSine:
				return easeInOutSine(x);
			case EasingType::InQuad:
				return easeInQuad(x);
			case EasingType::OutQuad:
				return easeOutQuad(x);
			case EasingType::InOutQuad:
				return easeInOutQuad(x);
			case EasingType::InCubic:
				return easeInCubic(x);
			case EasingType::OutCubic:
				return easeOutCubic(x);
			case EasingType::InOutCubic:
				return easeInOutCubic(x);
			case EasingType::InQuart:
				return easeInQuart(x);
			case EasingType::OutQuart:
				return easeOutQuart(x);
			case EasingType::InOutQuart:
				return easeInOutQuart(x);
			case EasingType::InQuint:
				return easeInQuint(x);
			case EasingType::OutQuint:
				return easeOutQuint(x);
			case EasingType::InOutQuint:
				return easeInOutQuint(x);
			case EasingType::InExpo:
				return easeInExpo(x);
			case EasingType::OutExpo:
				return easeOutExpo(x);
			case EasingType::InOutExpo:
				return easeInOutExpo(x);
			case EasingType::InCirc:
				return easeInCirc(x);
			case EasingType::OutCirc:
				return easeOutCirc(x);
			case EasingType::InOutCirc:
				return easeInOutCirc(x);
			case EasingType::InBack:
				return easeInBack(x);
			case EasingType::OutBack:
				return easeOutBack(x);
			case EasingType::InOutBack:
				return easeInOutBack(x);
			case EasingType::InElastic:
				return easeInElastic(x);
			case EasingType::OutElastic:
				return easeOutElastic(x);
			case EasingType::InOutElastic:
				return easeInOutElastic(x);
			case EasingType::InBounce:
				return easeInBounce(x);
			case EasingType::OutBounce:
				return easeOutBounce(x);
			case EasingType::InOutBounce:
				return easeInOutBounce(x);
			default:
				return 0.0;
			}
		}
	};

	template<typename T, typename J = double>
	class Spring
	{
	public:
		Spring(T* value, T goal, Surface* surface, double constant, double damping)
			: m_Value(value), Goal(goal), m_Surface(surface), Constant(constant), Damping(damping) 
		{
			m_TimedCallback = [&](double deltaTime)
			{
				T springForce = (Goal - *m_Value) * static_cast<J>(Constant);
				T dampingForce = m_Velocity * static_cast<J>(Damping);
				m_Velocity += (springForce - dampingForce) * static_cast<J>(deltaTime);
				*m_Value += m_Velocity * static_cast<J>(deltaTime);
				m_Surface->ShouldRender = true;
			};
		}

		T Goal;
		double Constant;
		double Damping;

		void Enable()
		{
			if (m_Connection != nullptr)
				return;
			m_Connection = m_Surface->ConnectTimedUpdate(m_TimedCallback);
		}

		void Disable()
		{
			if (m_Connection == nullptr)
				return;
			m_Connection->Disconnect();
			m_Connection = nullptr;
		}

	private:

		T* m_Value;
		T m_Velocity = T();
		Surface* m_Surface;

		std::function<void(double)> m_TimedCallback;
		std::shared_ptr<EventConnection<double>> m_Connection;
	};

	class AnimationScheduler
	{
	public:
		
		template<EasingType easingType, typename T, typename J = double>
		static void CreateAnimation(T* value, T goal, Surface* surface, double duration, double delay = 0.0,
			std::optional<std::function<void()>> callback = std::nullopt)
		{
			auto existingAnimation = s_Animations.find(reinterpret_cast<void*>(value));
			if (existingAnimation != s_Animations.end())
				delete (* existingAnimation).second;

			s_Animations[value] = reinterpret_cast<BaseAnimatorPointer*>(new AnimatorPointer(
				new Animator<easingType, T, J>(value, goal, surface, duration, delay, callback)));
		}

		template<EasingType easingType, typename T, typename J = double>
		static void CreateAnimation(T* value, T goal, Surface* surface, float duration, float delay = 0.0f,
			std::optional<std::function<void()>> callback = std::nullopt)
		{
			CreateAnimation<easingType, T, J>(
				value, goal, surface, static_cast<float>(duration), static_cast<double>(delay), callback);
		}

		// This function does not need to be called very often, it shouldn't be too bad in terms of performance
		// but it's also not a big deal that the finished animations are cleaned up instantly. All this function does is
		// prevent a memory leak so waiting a few seconds or so between calls makes sense.
		static void CleanupFinishedAnimations()
		{
			if (s_Animations.empty())
				return;

			for (auto it = s_Animations.begin(); it != s_Animations.end();)
			{
				if (it->second->CanDestroy())
				{
					delete it->second;
					it = s_Animations.erase(it);
				}
				else
					it++;
			}
		}
		
	private:
		struct BaseAnimatorPointer
		{
			virtual bool CanDestroy() { return true; };
			virtual ~BaseAnimatorPointer() {}
		};

		template<typename T>
		struct AnimatorPointer : public BaseAnimatorPointer
		{
			T* Pointer;

			AnimatorPointer(T* pointer)
				: Pointer(pointer) {}

			virtual bool CanDestroy() override { return Pointer->IsDone(); }

			virtual ~AnimatorPointer() override
			{
				Pointer->~T();
			}
		};

		static std::unordered_map<void*, BaseAnimatorPointer*> s_Animations;
	};
}