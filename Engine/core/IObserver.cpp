#include "IObserver.h"
#include <algorithm>

using namespace mk;

IObserver::~IObserver()
{
	for (ISubject* subjectPtr : m_Subjects)
		subjectPtr->RemoveObserver(this);
}

void IObserver::OnNotify(ISubject* subjectPtr, IEvent* event)
{
	if (dynamic_cast<ObserveEvent*>(event))
		subjectPtr->AddObserver(this);
	else if (	dynamic_cast<StopObserveEvent*>(event) ||
				dynamic_cast<ObjectDestroyEvent*>(event))
		subjectPtr->RemoveObserver(this);
}

ISubject::~ISubject()
{
	const std::unique_ptr<ObjectDestroyEvent> event{};
	for (IObserver* observerPtr : m_Observers)
		observerPtr->OnNotify(this, event.get());
}

void ISubject::AddObserver(IObserver* observerPtr)
{
	const auto foundIter = std::find(m_Observers.begin(), m_Observers.end(), observerPtr);
	if (foundIter == m_Observers.end())
	{
		const std::unique_ptr<ObserveEvent> event{};
		m_Observers.emplace_back(observerPtr);
		observerPtr->OnNotify(this, event.get());
	}
}

void ISubject::RemoveObserver(IObserver* observerPtr)
{
	const auto foundIter = std::find(m_Observers.begin(), m_Observers.end(), observerPtr);
	if (foundIter == m_Observers.end())
	{
		const std::unique_ptr<ObserveEvent> event{};
		m_Observers.erase(foundIter);
		observerPtr->OnNotify(this, event.get());
	}
}

void ISubject::Notify(const std::unique_ptr<IEvent>& event)
{
	for (IObserver* observerPtr : m_Observers)
		observerPtr->OnNotify(this, event.get());
}
