#pragma once

#include "Event.h"

namespace RAVEN
{
	class ColliderComponent;

	class OnCollisionEnterEvent : public Event
	{
	public:
		OnCollisionEnterEvent(ColliderComponent* other)
			: Event(EventId::CollisionEnter)
			, other(other)
		{}

		ColliderComponent* other = nullptr;
	};

	class OnCollisionStayEvent : public Event
	{
	public:
		OnCollisionStayEvent(ColliderComponent* other)
			: Event(EventId::CollisionStay)
			, other(other)
		{}

		ColliderComponent* other = nullptr;
	};

	class OnCollisionExitEvent : public Event
	{
	public:
		OnCollisionExitEvent(ColliderComponent* other)
			: Event(EventId::CollisionExit)
			, other(other)
		{}

		ColliderComponent* other = nullptr;
	};
}