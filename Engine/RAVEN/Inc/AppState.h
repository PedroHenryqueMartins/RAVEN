#pragma once

namespace RAVEN
{

	class AppState
	{
	public:
		virtual ~AppState() = default;

		virtual void Initialize() {}
		virtual void Termiante() {}
		
		virtual void Update(float deltaTime) {}
		virtual void Render() {}
		virtual void DebugUI() {}
	};
}