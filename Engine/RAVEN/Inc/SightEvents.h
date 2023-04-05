#pragma once

#include "Event.h"

namespace RAVEN
{
	class SightComponent;

	class OnSightEnterEvent : public Event
	{
	public:
		OnSightEnterEvent(SightComponent* other)
			: Event(EventId::SightEnter)
			, other(other)
		{}

		SightComponent* other = nullptr;
	};

	class OnSightStayEvent : public Event
	{
	public:
		OnSightStayEvent(SightComponent* other)
			: Event(EventId::SightStay)
			, other(other)
		{}

		SightComponent* other = nullptr;
	};

	class OnSightExitEvent : public Event
	{
	public:
		OnSightExitEvent(SightComponent* other)
			: Event(EventId::SightExit)
			, other(other)
		{}

		SightComponent* other = nullptr;
	};
}