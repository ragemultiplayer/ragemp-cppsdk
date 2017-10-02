#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <cstring>

#include "Entity.hpp"

#include "Pools.hpp"

#ifdef _WIN32
#define RAGE_API extern "C" __declspec(dllexport) 
#else
#define RAGE_API extern "C"
#endif

namespace rage
{
	class IPlugin
	{
	public:
		static constexpr int sdkver = 0;

		virtual uint32_t GetVersion() { return sdkver; }
		virtual void Unload() { }
	};
	
	class IEntityHandler;
	class IPlayerHandler;
	class IVehicleHandler;
	class IColshapeHandler;
	class ICheckpointHandler;
	class ITickHandler;

	class IEventHandler
	{
	public:
		virtual IEntityHandler *GetEntityHandler() { return nullptr; }
		virtual IPlayerHandler *GetPlayerHandler() { return nullptr; }
		virtual IVehicleHandler *GetVehicleHandler() { return nullptr; }
		virtual IColshapeHandler *GetColshapeHandler() { return nullptr; }
		virtual ICheckpointHandler *GetCheckpointHandler() { return nullptr; }
		virtual ITickHandler *GetTickHandler() { return nullptr; }
	};

	class IEntityHandler
	{
	public:
		virtual void OnEntityCreated(IEntity *entity) { }
		virtual void OnEntityDestroyed(IEntity *entity) { }
	};

	enum class exit_t : uint8_t
	{
		disconnect,
		timeout,
		kicked
	};

	class IPlayerHandler
	{
	public:
		virtual void OnPlayerJoin(IPlayer *player) { }
		virtual void OnPlayerQuit(IPlayer *player, exit_t exitType, const char *reason) { }
		virtual void OnPlayerSpawn(IPlayer *player) { }
		virtual void OnPlayerCommand(IPlayer *player, const std::u16string& command) { }
		virtual void OnPlayerChat(IPlayer *player, const std::u16string& text) { }
		virtual void OnPlayerEnterVehicle(IPlayer *player, IVehicle *vehicle, uint8_t seatId) { }
		virtual void OnPlayerEnteredVehicle(IPlayer *player, IVehicle *vehicle, uint8_t seatId) { }
		virtual void OnPlayerExitVehicle(IPlayer *player, IVehicle *vehicle) { }
		virtual void OnPlayerLeftVehicle(IPlayer *player, IVehicle *vehicle) { }
		virtual void OnPlayerDeath(IPlayer *player, hash_t reason, IPlayer *killer) { }
		virtual void OnPlayerRemoteEvent(IPlayer *player, const std::string& eventName, const args_t& args) { } // to be done
	};

	class IVehicleHandler
	{
	public:
		virtual IVehicleHandler *GetVehicleHandler() { return this; }
		virtual void OnVehicleDeath(IVehicle *vehicle, hash_t hash, IPlayer *killer) { }
	};

	class IColshapeHandler
	{
	public:
		virtual void OnPlayerEnterColshape(IPlayer *player, IColshape *colshape) { }
		virtual void OnPlayerExitColshape(IPlayer *player, IColshape *colshape) { }
	};

	class ICheckpointHandler
	{
	public:
		virtual void OnPlayerEnterCheckpoint(IPlayer *player, ICheckpoint *checkpoint) { }
		virtual void OnPlayerExitCheckpoint(IPlayer *player, ICheckpoint *checkpoint) { }
	};

	class ITickHandler
	{
	public:
		virtual void Tick() { }
	};

	class IMultiplayer
	{
	public:
		virtual void AddEventHandler(IEventHandler *handler) = 0;

		virtual const IPlayerPool& GetPlayerPool() = 0;
		virtual const IVehiclePool& GetVehiclePool() = 0;
		virtual const IColshapePool& GetColshapePool() = 0;
		virtual const ICheckpointPool& GetCheckpointPool() = 0;
		virtual const IMarkerPool& GetMarkerPool() = 0;
		virtual const IBlipPool& GetBlipPool() = 0;
		virtual const IPickupPool& GetPickupPool() = 0;
		virtual const IObjectPool& GetObjectPool() = 0;
	};


	using initializeFunc_t = IPlugin*(
	#ifdef WIN32
	__cdecl
#endif
		*) (IMultiplayer *multiplayer);
}
