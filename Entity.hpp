#pragma once

namespace rage
{
	struct rgba_t
	{
		rgba_t(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			: intvalue((a << 24) + (b << 16) + (g << 8) + r) { }

		explicit rgba_t(uint32_t val)
			: intvalue(val) { }

		rgba_t()
			: intvalue(0) { }

		union
		{
			uint32_t intvalue;
			uint8_t rgba[4];
		};
	};

	using entityId_t = uint16_t;
	using dimensionId_t = uint32_t;

	template <typename T>
	struct _vector3
	{
		T x;
		T y;
		T z;
	};

	using vector3 = _vector3<float>;

	struct quat_t
	{
		float x;
		float y;
		float z;
		float w;
	};
	
	struct vector2
	{
		float x;
		float y;
	};

	enum class entity_t : uint8_t
	{
		Player,
		Vehicle,
		Object,
		Pickup,
		Blip,
		Checkpoint,
		Marker,
		Colshape,
		TextLabel,
	};

	class IEntity;

#pragma pack(push, 1)
	struct clothData_t
	{
		uint8_t drawableId;
		uint8_t textureId;
		uint8_t paletteId;
	};

	struct headBlend_t
	{
		uint8_t m_shape[3];
		uint8_t m_skin[3];
		float m_shapeMix;
		float m_skinMix;
		float m_thirdMix;
	};

	struct propData_t
	{
		uint8_t drawableId;
		uint8_t textureId;
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
			Vector3,
			Object,
			Null,

			Entity
		};

		arg_t() : type(val_t::Null), v{} { }
		arg_t(bool b) : type(val_t::Boolean), v { b } { }
		arg_t(int i) : type(val_t::Int), v { i } { }
		arg_t(float f) : type(val_t::Float), v { f } { }
		arg_t(const std::string& str) : type(val_t::String), v { new char[str.length() + 1] } { memcpy(v.str, str.c_str(), str.length()); v.str[str.length()] = 0; }
		arg_t(entity_t entityType, entityId_t id, rage::IEntity *entity) : type(val_t::Entity), v{ entityType, id, entity } { }
		arg_t(const arg_t& r) : type(val_t::Null) { *this = r; }

		void SetNull() { DeleteString(); type = val_t::Null; }
		void SetBoolean(bool b) { DeleteString(); type = val_t::Boolean; v.b = b; }
		void SetInteger(int i) { DeleteString(); type = val_t::Int; v.i = i; }
		void SetFloat(float f) { DeleteString(); type = val_t::Float; v.f = f; }
		void SetString(const std::string& str) { DeleteString(); type = val_t::String; v.str = new char[str.length() + 1]; memcpy(v.str, str.c_str(), str.length()); v.str[str.length()] = 0; }
		void SetJson(const std::string& str) { DeleteString(); type = val_t::Object; v.str = new char[str.length() + 1]; memcpy(v.str, str.c_str(), str.length()); v.str[str.length()] = 0; }
		void SetEntity(entity_t entityType, entityId_t id, IEntity *entity) { DeleteString(); type = val_t::Entity; v.entity = { entityType, id, entity }; }
		void SetVector3(const vector3& vec) { DeleteString(); type = val_t::Vector3; v.vector = vec; }

		val_t GetType() const { return type; }
		bool IsNull() const { return type == val_t::Null; }
		bool IsBoolean() const { return type == val_t::Boolean; }
		bool IsInt() const { return type == val_t::Int; }
		bool IsFloat() const { return type == val_t::Float; }
		bool IsString() const { return type == val_t::String; }
		bool IsEntity() const { return type == val_t::Entity; }

		bool Boolean() const { return (type == val_t::Boolean) ? v.b : false; }
		int Int() const { return (type == val_t::Int) ? v.i : 0; }
		float Float() const { return (type == val_t::Float) ? v.f : 0.0f; }
		const char *String() const { return (type == val_t::String && v.str) ? v.str : ""; }
		const char *Object() const { return (type == val_t::Object && v.str) ? v.str : ""; }
		const vector3& Vector3() const { if (type == val_t::Vector3) { return v.vector; } else { static vector3 ret; return ret; } }

		rage::IEntity *Entity() const { return (type == val_t::Entity) ? v.entity.ptr : nullptr; }
		entityId_t EntityId() const { return (type == val_t::Entity) ? v.entity.id : 0xFFFF; }
		entity_t EntityType() const { return (type == val_t::Entity) ? v.entity.type : static_cast<entity_t>(-1); }

		arg_t& operator=(const arg_t& r) { DeleteString(); if (r.GetType() != val_t::String) { this->v.entity = r.v.entity; type = r.GetType(); } else { this->SetString(r.String()); } return *this; }
		arg_t& operator=(arg_t&& r) { DeleteString(); this->v.str = r.v.str; type = r.GetType(); r.type = val_t::Null; r.v.str = nullptr; return *this; }

		~arg_t() { DeleteString(); }

	private:
		bool OwnsAString() { return (type == val_t::Object || type == val_t::String); }
		void DeleteString() { if (this->OwnsAString() && v.str) { delete[] v.str; v.str = nullptr; } }

		union _Value
		{
			bool b;
			int i;
			float f;
			char *str;

			vector3 vector;

			struct _EntityData
			{
				entity_t type;
				entityId_t id;

				IEntity *ptr;
			} entity;

			_Value() { memset(this, 0, sizeof(_Value)); }
			_Value(bool _b) : b { _b } { }
			_Value(int _i) : i { _i } { }
			_Value(float _f) : f { _f } { }
			_Value(char *_str) : str{ _str } { }
			_Value(const vector3& _vec) : vector{ _vec } { }
			_Value(entity_t entityType, entityId_t id, IEntity *ptr) : entity{ entityType, id, ptr } { }
		} v;

		val_t type;
	};
#pragma pack(pop)

	struct args_t
	{
	public:
		args_t(arg_t *data, size_t len)
			: m_data(data), m_len(len) { }

		size_t Length() const { return this->m_len; }
		const arg_t& operator[](size_t id) const { if (id >= this->m_len) { static arg_t ar{}; return ar; } return this->m_data[id]; }

		auto begin() const { return m_data; }
		auto end() const { return &m_data[this->m_len]; }

	private:
		size_t m_len;
		arg_t *m_data;
	};
	
	class IEntity
	{
	public:
		virtual entityId_t GetId() = 0;
		virtual entity_t GetType() = 0;

		virtual void Destroy() = 0;

		virtual dimensionId_t GetDimension() = 0;
		virtual void SetDimension(dimensionId_t dimension) = 0;

		virtual const vector3& GetPosition() = 0;
		virtual void SetPosition(const vector3& pos) = 0;

		virtual const vector3& GetRotation() = 0;
		virtual void SetRotation(const vector3& rot) = 0;

		virtual uint32_t GetModel() = 0;
		virtual void SetModel(uint32_t model) = 0;

		virtual const vector3& GetVelocity() = 0;

		virtual uint8_t GetAlpha() = 0;
		virtual void SetAlpha(uint8_t alpha) = 0;

	private:
		virtual void* GetExternalValue() = 0;
		virtual void SetExternalValue(void *val) = 0;

	public:
		// todo

	public:
		template<class T>
		T* External() { return reinterpret_cast<T*>(this->GetExternalValue()); }

		template<class T>
		void External(T *val) { this->SetExternalValue(reinterpret_cast<void*>(val)); }
		
		virtual const arg_t& GetVariable(const std::string& key) = 0;
		virtual void SetVariable(const std::string& key, const arg_t& arg) = 0;
		virtual void SetVariables(const std::vector<std::pair<const std::string, const arg_t&>>& vars) = 0;
		virtual bool HasVariable(const std::string& key) = 0;
	};

	template<class T>
	class IPool
	{
	private:
		virtual const T** Data() const = 0;
	public:
		virtual T *GetAt(entityId_t id) const = 0;
		virtual entityId_t Length() const = 0;
		virtual entityId_t Count() const = 0;

		virtual std::vector<T*> GetInRange(vector3 position, float range, dimensionId_t dimension = 0xFFFFFFFF) const = 0;
		virtual std::vector<T*> GetInDimension(dimensionId_t dimension) const = 0;

		class iterator
		{
		public:
			typedef iterator self_type;
			typedef T* value_type;
			typedef T*& reference;
			typedef T** pointer;
			typedef std::forward_iterator_tag iterator_category;
			typedef int difference_type;
			iterator(pointer ptr, uint32_t rest) : ptr_(ptr), rest_(rest - 1) { if (*ptr == nullptr) while (*(++ptr_) == nullptr && --rest_ != -1); }
			self_type operator++() { self_type i = *this; while (*(++ptr_) == nullptr && --rest_ != -1); return i; }
			self_type operator++(int junk) { while (*(++ptr_) == nullptr && --rest_ != -1); return *this; }
			reference operator*() { return *ptr_; }
			pointer operator->() { return ptr_; }
			bool operator==(const self_type& rhs) { return rest_ == -1 ? false : ptr_ == rhs.ptr_; }
			bool operator!=(const self_type& rhs) { return rest_ == -1 ? false : ptr_ != rhs.ptr_; }
			auto operator-(const self_type& rhs) { return ptr_ - rhs.ptr_; }

		private:
			pointer ptr_;
			int32_t rest_;
		};

		iterator begin() const
		{
			return iterator(const_cast<T**>(this->Data()), this->Length());
		}

		iterator end() const
		{
			return iterator(const_cast<T**>(this->Data()) + this->Length(), 0);
		}

		T* operator[](entityId_t id) const { return this->GetAt(id); }
		// todo
	};
}
