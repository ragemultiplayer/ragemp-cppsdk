#pragma once

namespace rage
{
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

	template <typename T>
	struct _vector2
	{
		T x;
		T y;
	};

	using vector2 = _vector2<float>;

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
	};

	struct clothData_t {};
	struct propData_t {};

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
	};

	template<class T>
	class IPool
	{
	private:
		virtual const T** Data() const = 0;
	public:
		virtual T *GetAt(entityId_t id) const = 0;
		virtual entityId_t Length() const = 0;

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