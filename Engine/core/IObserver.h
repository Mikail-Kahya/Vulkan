#pragma once
#include <memory>
#include <vector>
#include "Events.h"

namespace mk 
{
	class ISubject;
	class GameObject;

	struct ObserveEvent final : public IEvent
	{
		ObserveEvent()				= default;
		~ObserveEvent() override	= default;
		ObserveEvent(const ObserveEvent& other)					= delete;
		ObserveEvent(ObserveEvent&& other) noexcept				= delete;
		ObserveEvent& operator=(const ObserveEvent& other)		= delete;
		ObserveEvent& operator=(ObserveEvent&& other) noexcept	= delete;
	};

	struct StopObserveEvent final : public IEvent
	{
		StopObserveEvent()				= default;
		~StopObserveEvent() override	= default;
		StopObserveEvent(const StopObserveEvent& other)					= delete;
		StopObserveEvent(StopObserveEvent&& other) noexcept				= delete;
		StopObserveEvent& operator=(const StopObserveEvent& other)		= delete;
		StopObserveEvent& operator=(StopObserveEvent&& other) noexcept	= delete;
	};

	struct ObjectDestroyEvent final : public IEvent
	{
		ObjectDestroyEvent()				= default;
		~ObjectDestroyEvent() override	= default;
		ObjectDestroyEvent(const ObjectDestroyEvent& other)					= delete;
		ObjectDestroyEvent(ObjectDestroyEvent&& other) noexcept				= delete;
		ObjectDestroyEvent& operator=(const ObjectDestroyEvent& other)		= delete;
		ObjectDestroyEvent& operator=(ObjectDestroyEvent&& other) noexcept	= delete;
	};

	class IObserver
	{
	public:
		IObserver() = default;
		virtual ~IObserver();
		virtual void OnNotify(ISubject* subjectPtr, IEvent* event);

	private:
		std::vector<ISubject*> m_Subjects{};
	};

	class ISubject
	{
	public:
		ISubject() = default;
		virtual ~ISubject();

		void AddObserver(IObserver* observerPtr);
		void RemoveObserver(IObserver* observerPtr);

	protected:
		void Notify(const std::unique_ptr<IEvent>& event);

	private:
		std::vector<IObserver*> m_Observers{};
	};
}