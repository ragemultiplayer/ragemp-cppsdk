#include <iostream>

#include "../rage.hpp"

namespace gm
{
	class EventHandler
		: public rage::IEventHandler,
		public rage::IPlayerHandler,
		public rage::ITickHandler
	{
	public:
		virtual rage::ITickHandler *GetTickHandler() { return this; }
		virtual rage::IPlayerHandler *GetPlayerHandler() { return this; }

		virtual void OnPlayerJoin(rage::IPlayer *player) { std::cout << "Player: " << player->GetId() << std::endl; }

		virtual void Tick() { std::cout << "tick!!!!" << std::endl; }
	};
}

RAGE_API rage::IPlugin *InitializePlugin(rage::IMultiplayer *mp)
{
	mp->AddEventHandler(new gm::EventHandler);
	return new rage::IPlugin;
}

