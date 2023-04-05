#pragma once



namespace RAVEN
{

	class AppState;

	struct AppConfig
	{
		std::wstring appName = L"RAVEN";
		uint32_t windowWidth = 1280;
		uint32_t windowHeight = 720;
		uint32_t debugDrawLimit = 100000;
		bool fullScreen = false;
	};

	class App
	{
	public:

		template<class StateType>
		void AddState(std::string stateName)
		{
			static_assert(
				std::is_base_of_v<AppState, StateType>,
				"App - 'StateType' must derive from 'AppState'."
				);

			auto [iter, success] = mAppStates.try_emplace(std::move(stateName), nullptr);
			ASSERT(success, "App - Already have a state named '%s'", stateName.c_str());
			
			auto& ptr = iter->second;
			ptr = std::make_unique<StateType>();
			if (mCurrentState == nullptr)
			{
				LOG("App - Starting state: %s", iter->first.c_str());
				mCurrentState = ptr.get();
			}
		}

		void ChangeState(const std::string& stateName);

		void Run(AppConfig appConfig);
		void Quit();

	private:

		using AppStateMap = std::map<std::string, std::unique_ptr<AppState>>;


		AppStateMap mAppStates;
		AppState* mCurrentState = nullptr;
		AppState* mNextState = nullptr;
		bool mRunning = false;
	};
}