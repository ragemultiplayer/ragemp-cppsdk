#pragma once

#ifdef GetProp
#undef GetProp
#endif
#ifdef SetProp
#undef SetProp
#endif

namespace rage
{

	struct rgb_t
	{
		rgb_t(uint8_t r, uint8_t g, uint8_t b)
			: intvalue((b << 16) + (g << 8) + r) { }

		explicit rgb_t(uint32_t val)
			: intvalue(val) { }

		rgb_t()
			: intvalue(0) { }

		union
		{
			uint32_t intvalue;
			uint8_t rgba[3];
		};
	};

	struct paintInfo_t
	{
		uint8_t type;
		uint8_t colour;
	};

	enum class material_t : uint8_t
	{
		Color,
		RGB,
		Paint
	};

	enum class colshape_t : uint8_t
	{
		Sphere,
		Tube,
		Circle,
		Polygon,
		Cuboid,
		Rectangle
	};

	using hash_t = uint32_t;

	class IPlayer;
	class IVehicle;
	class IColshape;
	class ICheckpoint;
	class IMarker;
	class IBlip;
	class IPickup;

	struct headOverlay_t
	{
		uint8_t index;
		float opacity;
		uint8_t colorId;
		uint8_t secondaryColorId;
	};

	using Entity =
		IEntity;

	class IPlayer
		: public Entity
	{
	public:

		virtual void Kick(const char *reason = nullptr) = 0;
		virtual void Ban(const char *reason = nullptr) = 0;
		virtual void OutputChatBox(const std::string& text) = 0;
		virtual void Notify(const std::string& text) = 0;
	public:
		virtual void _Call(const std::string& eventName, const arg_t *arguments = nullptr, size_t count = 0) = 0;
		virtual void _CallHash(uint64_t eventNameHash, const arg_t *arguments = nullptr, size_t count = 0) = 0; // xxhash64
		virtual void _Invoke(uint64_t nativeHash, const arg_t *arguments = nullptr, size_t count = 0) = 0;
	public:
		virtual void Spawn(const vector3& pos, float heading) = 0;
		virtual void PlayAnimation(const std::string& dict, const std::string& name, float speed = 8.f, int flags = 0) = 0;
		virtual void PlayScenario(const std::string& scenario) = 0;
		virtual void StopAnimation() = 0;

		virtual const clothData_t& GetClothes(uint8_t id) = 0;
		virtual void SetClothes(uint8_t id, const clothData_t& clothes) = 0;

		virtual void SetClothes(const std::vector<std::pair<uint8_t, const clothData_t>>& clothes) = 0;

		virtual const propData_t& GetProp(uint8_t id) = 0;
		virtual void SetProp(uint8_t id, const propData_t prop) = 0;

		virtual void SetProp(const std::vector<std::pair<uint8_t, const propData_t>>& clothes) = 0;

		virtual void SetCustomization(bool gender, 
			const rage::headBlend_t& headBlend,
			uint8_t eyeColor, uint8_t hairColor, uint8_t hightlightColor, 
			const std::vector<float>& faceFeatures,
			const std::map<int, headOverlay_t>& headOverlays,
			const std::vector<std::pair<uint32_t, uint32_t>> decorations) = 0;

		virtual uint32_t GetDecoration(uint32_t collection) = 0;
		virtual void RemoveDecoration(uint32_t collection, uint32_t overlay) = 0;
		virtual void SetDecoration(uint32_t collection, uint32_t overlay) = 0;
		virtual void SetDecorations(std::vector<std::pair<uint32_t, uint32_t>> decorations) = 0;
		
		virtual void ClearDecorations() = 0;

		virtual void EnableVoiceTo(IPlayer *target) = 0;
		virtual void DisableVoiceTo(IPlayer *target) = 0;

		virtual std::vector<IPlayer*> GetVoiceListeners() = 0;
		
		virtual void Eval(const std::string& code) = 0;

		virtual const std::string& GetName() = 0;
		virtual void SetName(const std::string& name) = 0;

		virtual bool IsAiming() = 0;

		virtual float GetHeading() = 0;
		virtual void SetHeading(float heading) = 0;

		virtual float GetMoveSpeed() = 0;

		virtual float GetHealth() = 0;
		virtual void SetHealth(float armour) = 0;

		virtual float GetArmour() = 0;
		virtual void SetArmour(float armour) = 0;

		virtual const vector3& GetAimingAt() = 0;

		virtual int GetPing() = 0;
		virtual float GetPacketLoss() = 0;

		virtual const std::string& GetKickReason() = 0;

		virtual std::string GetIp() = 0;

		virtual bool IsJumping() = 0;
		virtual bool IsInCover() = 0;
		virtual bool IsEnteringVehicle() = 0;
		virtual bool IsLeavingVehicle() = 0;
		virtual bool IsClimbing() = 0;
		virtual bool IsOnLadder() = 0;
		virtual bool IsReloading() = 0;
		virtual bool IsInMelee() = 0;
		virtual std::string GetActionString() = 0;

		// Vehicle
		virtual IVehicle *GetVehicle() = 0;
		virtual void PutIntoVehicle(IVehicle *vehicle, int8_t seatId) = 0;
		virtual void RemoveFromVehicle() = 0;
		virtual int8_t GetSeat() = 0;

		// Customization
		virtual uint8_t GetEyeColour() = 0;
		virtual void SetEyeColour(uint8_t colour) = 0;

		virtual uint8_t GetHairColour() = 0;
		virtual uint8_t GetHairHighlightColour() = 0;
		virtual void SetHairColour(uint8_t colour, uint8_t highlightColour) = 0;

		virtual float GetFaceFeature(uint8_t id) = 0;
		virtual void SetFaceFeature(uint8_t id, float scale) = 0;

		virtual const headBlend_t& GetHeadBlend() = 0;

		virtual void SetHeadBlend(int shapeFirstID, int shapeSecondID, int shapeThirdID, int skinFirstID, int skinSecondID, int skinThirdID,
			float shapeMix, float skinMix, float thirdMix) = 0;

		virtual void UpdateHeadBlend(float shapeMix, float skinMix, float thirdMix) = 0;

		virtual headOverlay_t GetHeadOverlay(uint8_t overlayId) = 0;
		virtual void SetHeadOverlay(uint8_t overlayId, headOverlay_t overlay) = 0;

		// Weapons
		virtual uint32_t GetCurrentWeapon() = 0;
		virtual void SetCurrentWeapon(uint32_t weapon) = 0;

		virtual uint16_t GetCurrentWeaponAmmo() = 0;
		virtual void SetCurrentWeaponAmmo(uint16_t ammo) = 0;

		virtual uint16_t GetWeaponAmmo(uint32_t weaponHash) = 0;
		virtual void SetWeaponAmmo(hash_t hash, uint16_t ammo) = 0;

		virtual std::map<hash_t, uint16_t> GetWeapons() = 0;

		virtual void GiveWeapon(hash_t hash, uint16_t ammo) = 0;
		virtual void GiveWeapons(std::vector<std::pair<hash_t, uint16_t>> weapons) = 0;

		virtual void RemoveWeapon(hash_t hash) = 0;
		virtual void RemoveWeapons(const std::vector<hash_t>& hash) = 0;
		virtual void RemoveAllWeapons() = 0;

		//
		virtual bool IsStreamed(IPlayer *player) = 0;
		virtual std::vector<IPlayer*> GetStreamed() = 0;

		virtual const std::string& GetSerial() = 0;
		virtual const std::string& GetSocialClubName() = 0;

		virtual void RemoveObject(uint32_t model, const vector3& position, float radius) = 0;

	public:
		template<typename ...Args>
		void Call(const std::string& eventName, Args&&... args)
		{
			const int count = sizeof...(Args);

			if constexpr(count == 0)
				this->_Call(eventName);
			else
			{
				arg_t arguments[count] = { arg_t(std::forward<Args>(args))... };
				this->_Call(eventName, arguments, count);
			}
		}

		template<typename ...Args>
		void Invoke(uint64_t hash, Args&&... args)
		{
			const int count = sizeof...(Args);

			if constexpr(count == 0)
				this->_Invoke(hash);
			else
			{
				arg_t arguments[count] = { arg_t(std::forward<Args>(args))... };
				this->_Invoke(hash, arguments, count);
			}
		}
	};

	class IVehicle
		: public Entity
	{
	public:
		virtual bool IsSirenActive() = 0;
		virtual void SetSirenActive(bool toggle) = 0;

		virtual bool IsHornActive() = 0;

		virtual bool AreHighbeamsActive() = 0;
		virtual void SetHighbeamsActive(bool toggle) = 0;

		virtual bool AreLightsActive() = 0;
		virtual void SetLightsActive(bool toggle) = 0;

		virtual bool IsEngineActive() = 0;
		virtual void SetEngineActive(bool toggle) = 0;

		virtual bool AreTaxiLightsActive() = 0;
		virtual void SetTaxiLightsActive(bool toggle) = 0;

		virtual bool IsRocketBoostActive() = 0;
		virtual bool IsBrakeActive() = 0;
		virtual float GetSteerAngle() = 0;
		virtual float GetGasPedalState() = 0;

		virtual float GetEngineHealth() = 0;
		virtual float GetBodyHealth() = 0;

		virtual rage::IPlayer *GetOccupant(uint8_t seat) = 0;
		virtual std::vector<rage::IPlayer*> GetOccupants() = 0;
		virtual void SetOccupant(uint8_t seat, rage::IPlayer *player) = 0;

		virtual bool IsLocked() = 0;
		virtual void Lock(bool toggle) = 0;

		virtual bool IsDead() = 0;

		virtual void Explode() = 0;
		virtual void Spawn(const vector3& pos, float heading) = 0;

		virtual uint8_t GetMod(uint8_t id) = 0;
		virtual void SetMod(uint8_t id, uint8_t mod) = 0;

		virtual bool AreNeonsEnabled() = 0;
		virtual void EnableNeons(bool toggle) = 0;

		virtual void SetNeonsColour(uint8_t r, uint8_t g, uint8_t b) = 0;
		virtual const rage::rgb_t& GetNeonsColour() = 0;

		virtual void Repair() = 0;

		virtual rage::rgb_t GetColourRGB(uint8_t id) = 0;
		virtual uint8_t GetColour(uint8_t id) = 0;
		virtual uint8_t GetPaint(uint8_t id) = 0;

		virtual void SetColourRGB(const rage::rgb_t& primary, const rage::rgb_t& secondary) = 0;
		virtual void SetColour(uint8_t primary, uint8_t secondary) = 0;
		virtual void SetPaint(const rage::paintInfo_t& primary, const rage::paintInfo_t& secondary) = 0;

		virtual material_t GetMaterialType() = 0;

		virtual const std::string& GetNumberPlate() = 0;
		virtual void SetNumberPlate(const std::string& numberPlate) = 0;

		virtual bool IsStreamed(IPlayer *player) = 0;
		virtual std::vector<IPlayer*> GetStreamed() = 0;

		virtual uint8_t GetLivery() = 0;
		virtual void SetLivery(uint8_t livery) = 0;

		virtual uint8_t GetWheelColor() = 0;
		virtual void SetWheelColor(uint8_t color) = 0;

		virtual uint8_t GetWheelType() = 0;
		virtual void SetWheelType(uint8_t type) = 0;

		virtual uint8_t GetNumberPlateType() = 0;
		virtual void SetNumberPlateType(uint8_t type) = 0;

		virtual uint8_t GetPearlescentColor() = 0;
		virtual void SetPearlescentColor(uint8_t color) = 0;

		virtual uint8_t GetWindowTint() = 0;
		virtual void SetWindowTint(uint8_t tint) = 0;

		virtual uint8_t GetDashboardColor() = 0;
		virtual void SetDashboardColor(uint8_t color) = 0;

		virtual uint8_t GetTrimColor() = 0;
		virtual void SetTrimColor(uint8_t type) = 0;

		virtual bool GetExtra(uint8_t id) = 0;
		virtual void SetExtra(uint8_t id, bool state) = 0;

		virtual float GetMovableState() = 0;

		virtual const quat_t& GetQuaternion() = 0;
		virtual float GetHeading() = 0;

		virtual rage::IVehicle *GetTrailer() = 0;
		virtual rage::IVehicle *GetTraileredBy() = 0;
	};

	class IColshape
		: public Entity
	{
	public:
		virtual bool IsPointWithin(const vector3& pos) = 0;
		virtual colshape_t GetShapeType() = 0;
	};

	class ICheckpoint
		: public Entity
	{
	public:
		virtual const rage::rgba_t& GetColour() = 0;
		virtual void SetColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;

		virtual vector3& GetDirection() = 0;
		virtual void SetDirection(const vector3& dir) = 0;

		virtual float GetRadius() = 0;
		virtual void SetRadius(float radius) = 0;

		virtual bool IsVisible() = 0;
		virtual void SetVisible(bool toggle) = 0;

		virtual void ShowFor(const std::vector<rage::IPlayer*>& players) = 0;
		virtual void HideFor(const std::vector<rage::IPlayer*>& players) = 0;
	};

	class IMarker
		: public Entity
	{
	public:
		virtual const rage::rgba_t& GetColour() = 0;
		virtual void SetColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;

		virtual vector3& GetDirection() = 0;
		virtual void SetDirection(const vector3& dir) = 0;

		virtual float GetScale() = 0;
		virtual void SetScale(float scale) = 0;

		virtual bool IsVisible() = 0;
		virtual void SetVisible(bool toggle) = 0;

		virtual void ShowFor(const std::vector<rage::IPlayer*>& players) = 0;
		virtual void HideFor(const std::vector<rage::IPlayer*>& players) = 0;
	};

	class IBlip
		: public Entity
	{
	public:
		virtual float GetDrawDistance() = 0;
		virtual void SetDrawDistance(float distance) = 0;

		virtual short GetBlipRotation() = 0;
		virtual void SetRotation(short rotation) = 0;

		virtual bool IsShortRange() = 0;
		virtual void SetShortRange(bool toggle) = 0;

		virtual void RouteFor(const std::vector<rage::IPlayer*>& players, uint8_t colour, float scale) = 0;
		virtual void UnrouteFor(const std::vector<rage::IPlayer*>& players) = 0;

		virtual uint8_t GetColour() = 0;
		virtual void SetColour(uint8_t colour) = 0;

		//virtual uint8_t GetAlpha() = 0;
		//virtual void SetAlpha(uint8_t alpha) = 0;

		virtual float GetScale() = 0;
		virtual void SetScale(float scale) = 0;

		virtual const std::string& GetName() = 0;
		virtual void SetName(const std::string& name) = 0;
	};

	class IPickup
		: public Entity
	{
	public:
		// todo
	};

	class IObject
		: public Entity
	{
	public:
	};

	class ITextLabel
		: public Entity
	{
	public:
		virtual rgba_t GetColor() = 0;
		virtual void SetColor(const rgba_t color) = 0;

		virtual const std::string& GetText() = 0;
		virtual void SetText(const std::string& text) = 0;

		virtual bool GetLOS() = 0;
		virtual void SetLOS(bool toggle) = 0;

		virtual float GetDrawDistance() = 0;
		virtual void SetDrawDistance(float distance) = 0;

		virtual uint8_t GetFont() = 0;
		virtual void SetFont(uint8_t font) = 0;
	};
}
