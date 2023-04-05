#pragma once

#include "TypeIds.h"

namespace RAVEN
{
	class Event
	{
	public:
		uint32_t GetTypeID() const { return mTypeId; }

	protected:
		Event(uint32_t typeId)
			: mTypeId(typeId)
		{}	

	private:
		uint32_t mTypeId;
	};
}