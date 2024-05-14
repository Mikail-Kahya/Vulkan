#pragma once

namespace mk
{
	struct IEvent
	{
		IEvent()			= default;
		virtual ~IEvent()	= default;
		IEvent(const IEvent& other)					= delete;
		IEvent(IEvent&& other) noexcept				= delete;
		IEvent& operator=(const IEvent& other)		= delete;
		IEvent& operator=(IEvent&& other) noexcept	= delete;
	};
}
