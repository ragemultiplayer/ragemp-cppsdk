#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>

#include <functional>

#include <locale>
#include <codecvt>

#include "Entity.hpp"

#include "Pools.hpp"

#ifdef _WIN32
#define RAGE_API extern "C" __declspec(dllexport) 
#else
#define RAGE_API extern "C"
#endif

namespace rage
{
	using removeEventHandler_t = std::function<void(IPlayer*, const args_t&)>;
	
	struct time_t
	{
		union
		{
			struct
			{
				uint8_t hour;
				uint8_t minute;
				uint8_t second;
			} a;
			uint32_t i;
		};
	};

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
	/*class IMarkerHandler;
	class IPickupHandler;*/
	class IBlipHandler;
	class IStreamerHandler;
	class ITickHandler;

	class IEventHandler
	{
	public:
		virtual IEntityHandler *GetEntityHandler() { return nullptr; }
		virtual IPlayerHandler *GetPlayerHandler() { return nullptr; }
		virtual IVehicleHandler *GetVehicleHandler() { return nullptr; }
		virtual IColshapeHandler *GetColshapeHandler() { return nullptr; }
		virtual ICheckpointHandler *GetCheckpointHandler() { return nullptr; }
		/*virtual IMarkerHandler *GetMarkerHandler() { return nullptr; }
		virtual IPickupHandler *GetPickupHandler() { return nullptr; }*/
		virtual IBlipHandler *GetBlipHandler() { return nullptr; }
		virtual IStreamerHandler *GetStreamerHandler() { return nullptr; }
		virtual ITickHandler *GetTickHandler() { return nullptr; }
	};

	class IEntityHandler
	{
	public:
		virtual void OnEntityCreated(IEntity *entity) { }
		virtual void OnEntityDestroyed(IEntity *entity) { }
		virtual void OnEntityModelChange(IEntity *entity, hash_t oldModel) { }
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
		virtual void OnPlayerReady(IPlayer *player) { }
		virtual void OnPlayerQuit(IPlayer *player, exit_t exitType, const char *reason) { }

		virtual void OnPlayerCommand(IPlayer *player, const std::u16string& command) { }
		virtual void OnPlayerChat(IPlayer *player, const std::u16string& text) { }

		virtual void OnPlayerDeath(IPlayer *player, hash_t reason, IPlayer *killer) { }
		virtual void OnPlayerSpawn(IPlayer *player) { }
		virtual void OnPlayerDamage(IPlayer *player, float healthLoss, float armorLoss) { }
		virtual void OnPlayerWeaponChange(IPlayer *player, hash_t oldWeapon, hash_t newWeapon) { }

		virtual void OnPlayerRemoteEvent(IPlayer *player, uint64_t eventNameHash, const args_t& args) { } // xxhash64

		virtual void OnPlayerStartEnterVehicle(IPlayer *player, IVehicle *vehicle, uint8_t seatId) { }
		virtual void OnPlayerEnterVehicle(IPlayer *player, IVehicle *vehicle, uint8_t seatId) { }
		virtual void OnPlayerStartExitVehicle(IPlayer *player, IVehicle *vehicle) { }
		virtual void OnPlayerExitVehicle(IPlayer *player, IVehicle *vehicle) { }
	};

	class IVehicleHandler
	{
	public:
		virtual void OnVehicleDeath(IVehicle *vehicle, hash_t hash, IPlayer *killer) { }
		virtual void OnVehicleSirenToggle(IVehicle *vehicle, bool toggle) { }
		virtual void OnVehicleHornToggle(IVehicle *vehicle, bool toggle) { }
		virtual void OnTrailerAttached(IVehicle *vehicle, IVehicle *trailer) { }
		virtual void OnVehicleDamage(IVehicle *vehicle, float bodyHealthLoss, float engineHealthLoss) { }
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

	/*class IMarkerHandler
	{
	public:
	};

	class IPickupHandler
	{
	public:
	};*/

	class IBlipHandler
	{
	public:
		virtual void OnPlayerCreateWaypoint(rage::IPlayer *player, const vector3& position) { }
		virtual void OnPlayerReachWaypoint(rage::IPlayer *player) { }
	};

	class IStreamerHandler
	{
	public:
		virtual void OnPlayerStreamIn(IPlayer *player, IPlayer *forplayer) { }
		virtual void OnPlayerStreamOut(IPlayer *player, IPlayer *forplayer) { }
	};

	class ITickHandler
	{
	public:
		virtual void Tick() { }
	};

	class IWorld
	{
	public:
		virtual time_t GetTime() = 0;
		virtual void SetTime(time_t time) = 0;
		virtual const std::string& GetWeather() = 0;
		virtual void SetWeather(const std::string& weather) = 0;
		virtual void SetWeatherTransition(const std::string& weather, float time = 0.5f) = 0;

		virtual void RequestIpl(const std::string& ipl) = 0;
		virtual void RemoveIpl(const std::string& ipl) = 0;

		virtual bool AreTrafficLightsLocked() = 0;
		virtual void LockTrafficLights(bool toggle) = 0;

		virtual int GetTrafficLightsState() = 0;
		virtual void SetTrafficLightsState(int state) = 0;
	};

	class IConfig
	{
	public:
		virtual int GetInt(const std::string& key, int def = 0) = 0;
		virtual std::string GetString(const std::string& key, const std::string& def = "") = 0;
	};

	class IMultiplayer
	{
	public:
		virtual void AddEventHandler(IEventHandler *handler) = 0;

		virtual void AddRemoteEventHandler(const std::string& eventName, removeEventHandler_t handler) = 0;

		virtual IPlayerPool& GetPlayerPool() = 0;
		virtual IVehiclePool& GetVehiclePool() = 0;
		virtual IColshapePool& GetColshapePool() = 0;
		virtual ICheckpointPool& GetCheckpointPool() = 0;
		virtual IMarkerPool& GetMarkerPool() = 0;
		virtual IBlipPool& GetBlipPool() = 0;
		virtual IPickupPool& GetPickupPool() = 0;
		virtual IObjectPool& GetObjectPool() = 0;
		virtual ITextLabelPool& GetLabelPool() = 0;
		virtual IWorld& GetWorld() = 0;
		virtual IConfig& GetConfig() = 0;
	};


	using initializeFunc_t = IPlugin*(
#ifdef _WIN32
	__cdecl
#endif
		*) (IMultiplayer *multiplayer);
}
