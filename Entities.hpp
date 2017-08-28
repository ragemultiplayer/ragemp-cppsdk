#pragma once

#ifdef GetProp
#undef GetProp
#endif
#ifdef SetProp
#undef SetProp
#endif

namespace rage
{
	struct rgba_t
	{
		union
		{
			uint32_t intvalue;
			uint8_t rgba[4];
		};
	};

	struct rgb_t
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	struct paintInfo_t
	{
		uint8_t type;
		uint8_t colour;
	};

	struct headBlend_t
	{
		uint8_t m_shape[3];
		uint8_t m_skin[3];
		float m_shapeMix;
		float m_skinMix;
		float m_thirdMix;
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

	enum class blipMode_t : uint8_t
	{
		Global,
		Streamed
	};

	struct arg_t
	{
	public:
		enum class val_t : uint8_t
		{
			Int,
			Float,
			String,
			Boolean,
			Null,

			Entity
		};

		arg_t() : type(val_t::Null) { }
		arg_t(bool b) : type(val_t::Boolean) { v.b = b; }
		arg_t(int i) : type(val_t::Int) { v.i = i; }
		arg_t(float f) : type(val_t::Float) { v.f = f; }
		arg_t(const std::string& str) : type(val_t::String) { v.str = new char[str.length()]; memcpy(v.str, str.c_str(), str.length() * sizeof(std::string::traits_type::char_type)); }
		arg_t(rage::IEntity *entity) : type(val_t::Entity) { v.entity = entity; }

		val_t GetType() { return type; }
		bool IsNull() const { return type == val_t::Null; }
		bool IsBoolean() const { return type == val_t::Boolean; }
		bool IsInt() const { return type == val_t::Int; }
		bool IsFloat() const { return type == val_t::Float; }
		bool IsString() const { return type == val_t::String; }
		bool IsEntity() const { return type == val_t::Entity; }

		bool Boolean() const { return (type == val_t::Boolean) ? v.b : false; }
		int Int() const { return (type == val_t::Int) ? v.i : 0; }
		float Float() const { return (type == val_t::Float) ? v.f : 0.0f; }
		const char *String() const { return (type == val_t::String) ? v.str : ""; }
		rage::IEntity *Entity() const { return (type == val_t::Entity) ? v.entity : nullptr; }

		~arg_t() { if (type == val_t::String && v.str) delete[] v.str; }

	private:
		union
		{
			bool b;
			int i;
			float f;
			char *str;
			rage::IEntity *entity;
		} v;

		val_t type;
	};

	struct args_t
	{
	public:
		args_t(arg_t *data, size_t len)
			: m_data(data), m_len(len) { }

		size_t Length() const { return this->m_len; }
		const arg_t& operator[](int id) const { if (id >= this->m_len) return arg_t{}; return this->m_data[id]; }

	private:
		size_t m_len;
		arg_t *m_data;
	};

	using hash_t = uint32_t;

	class IPlayer;
	class IVehicle;
	class IColshape;
	class ICheckpoint;
	class IMarker;
	class IBlip;
	class IPickup;

	using Entity =
#ifdef IS_MP
		multiplayer::Entity;
#else
		IEntity;
#endif

	class IPlayer
		: public Entity
	{
	public:
		virtual void Kick(const char *reason) = 0;
		virtual void Ban(const char *reason) = 0;
		virtual void OutputChatBox(const std::u16string& text) = 0;
		virtual void Notify(const std::u16string& text) = 0;
	private:
		virtual void _Call(const std::string& eventName, const arg_t *arguments = nullptr, size_t count = 0) = 0;
		virtual void _Invoke(uint64_t nativeHash, const arg_t *arguments = nullptr, size_t count = 0) = 0;
	public:
		virtual void Spawn(const vector3& pos, float heading) = 0;
		virtual void PlayAnimation(const std::string& dict, const std::string& name, float speed = 8.f, int flags = 0) = 0;
		virtual void PlayScenario(const std::string& scenario) = 0;
		virtual void StopAnimation() = 0;

		virtual const clothData_t& GetClothes(uint8_t id) = 0;
		virtual void SetClothes(uint8_t id, const clothData_t& clothes) = 0;

		virtual const propData_t& GetProp(uint8_t id) = 0;
		virtual void SetProp(uint8_t id, const propData_t& prop) = 0;

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

		virtual const std::string& GetKickReason() = 0;

		virtual std::string GetIp() = 0;

		virtual bool IsJumping() = 0;
		virtual bool IsInCover() = 0;
		virtual bool IsEnteringVehicle() = 0;
		virtual bool IsLeavingVehicle() = 0;
		virtual bool IsClimbing() = 0;
		virtual std::string GetActionString() = 0;

		virtual IVehicle *GetVehicle() = 0;
		virtual void PutIntoVehicle(IVehicle *vehicle, int8_t seatId) = 0;
		virtual void RemoveFromVehicle() = 0;
		virtual int8_t GetSeat() = 0;

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

		virtual uint32_t GetWeapon() = 0;
		virtual void GiveWeapon(hash_t hash, uint16_t ammo) = 0;
		virtual void GiveWeapons(std::vector<std::pair<hash_t, uint16_t>> weapons) = 0;

		virtual const std::string& GetSerial() = 0;

	public:
		template<typename ...Args>
		void Call(const std::string& eventName, Args&&... args)
		{
			const int count = sizeof...(Args);

			if (count == 0)
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

			if (count == 0)
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
		virtual float GetRadius() = 0;

		virtual blipMode_t GetMode() = 0;

		virtual void RouteFor(const std::vector<rage::IPlayer*>& players, uint8_t colour, float scale) = 0;
		virtual void UnrouteFor(const std::vector<rage::IPlayer*>& players) = 0;

		virtual uint8_t GetColour() = 0;
		virtual void SetColour(uint8_t colour) = 0;

		virtual uint8_t GetAlpha() = 0;
		virtual void SetAlpha(uint8_t alpha) = 0;

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

}