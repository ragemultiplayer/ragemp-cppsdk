#pragma once

#include "Entities.hpp"

namespace rage
{
	class IPlayerPool
		: public IPool<IPlayer>
	{
	public:
		virtual void Broadcast(const std::u16string& message) const = 0;
		virtual void BroadcastInRange(const std::u16string& message, const vector3& position, float range, dimensionId_t dimension = 0xFFFFFFFF) const = 0;
		virtual void BroadcastInDimension(const std::u16string& message, dimensionId_t dimension) const = 0;

	private:
		virtual void _Call(const std::string& eventName, const arg_t *arguments = nullptr, size_t count = 0) const = 0;
		virtual void _CallInRange(const vector3& position, float range, dimensionId_t dimension, const std::string& eventName, const arg_t *arguments = nullptr, size_t count = 0) const = 0;
		virtual void _CallInDimension(dimensionId_t dimension, const std::string& eventName, const arg_t *arguments = nullptr, size_t count = 0) const = 0;

		virtual void _Invoke(uint64_t nativeHash, const arg_t *arguments = nullptr, size_t count = 0) const = 0;
		virtual void _InvokeInRange(const vector3& position, float range, dimensionId_t dimension, uint64_t nativeHash, const arg_t *arguments = nullptr, size_t count = 0) const = 0;
		virtual void _InvokeInDimension(dimensionId_t dimension, uint64_t nativeHash,const arg_t *arguments = nullptr, size_t count = 0) const = 0;

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
		void CallInRange(const vector3& position, float range, dimensionId_t dimension, const std::string& eventName, Args&&... args)
		{
			const int count = sizeof...(Args);

			if (count == 0)
				this->_CallInRange(position, range, dimension, eventName);
			else
			{
				arg_t arguments[count] = { arg_t(std::forward<Args>(args))... };
				this->_CallInRange(position, range, dimension, eventName, arguments, count);
			}
		}

		template<typename ...Args>
		void CallInDimension(dimensionId_t dimension, const std::string& eventName, Args&&... args)
		{
			const int count = sizeof...(Args);

			if (count == 0)
				this->_CallInDimension(dimension, eventName);
			else
			{
				arg_t arguments[count] = { arg_t(std::forward<Args>(args))... };
				this->_CallInDimension(dimension, eventName, arguments, count);
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

		template<typename ...Args>
		void InvokeInRange(const vector3& position, float range, dimensionId_t dimension, uint64_t hash, Args&&... args)
		{
			const int count = sizeof...(Args);

			if (count == 0)
				this->_InvokeInRange(position, range, dimension, hash);
			else
			{
				arg_t arguments[count] = { arg_t(std::forward<Args>(args))... };
				this->_InvokeInRange(position, range, dimension, hash, arguments, count);
			}
		}

		template<typename ...Args>
		void InvokeInDimension(dimensionId_t dimension, uint64_t hash, Args&&... args)
		{
			const int count = sizeof...(Args);

			if (count == 0)
				this->_InvokeInDimension(dimension, hash);
			else
			{
				arg_t arguments[count] = { arg_t(std::forward<Args>(args))... };
				this->_InvokeInDimension(dimension, hash, arguments, count);
			}
		}
	};

	class IVehiclePool
		: public IPool<IVehicle>
	{
	public:
		virtual IVehicle *New(uint32_t model, const vector3& position, float heading = 0.f, dimensionId_t dimension = 0) const = 0;
	};

	class IColshapePool
		: public IPool<IColshape>
	{
	public:
		virtual IColshape *NewCircle(const vector2& position, float radius, dimensionId_t dimension = 0) const = 0;
		virtual IColshape *NewSphere(const vector3& position, float radius, dimensionId_t dimension = 0) const = 0;
		virtual IColshape *NewTube(const vector3& position, float radius, float height, dimensionId_t dimension = 0) const = 0;
		virtual IColshape *NewRectangle(const vector2& position, const vector2& size, dimensionId_t dimension = 0) const = 0;
		virtual IColshape *NewCube(const vector3& position, const vector3& size, dimensionId_t dimension = 0) const = 0;
	};

	class ICheckpointPool
		: public IPool<ICheckpoint>
	{
	public:
		virtual ICheckpoint *New(uint8_t type, const vector3& position, const vector3& nextPos, float radius, const rgba_t& colour, bool visible = true, dimensionId_t dimension = 0) const = 0;
	};

	class IMarkerPool
		: public IPool<IMarker>
	{
	public:
		virtual IMarker *New(uint32_t model, const vector3& position, const vector3& rotation, const vector3& direction, float scale, rage::rgba_t color, bool visible = true, dimensionId_t dimension = 0) const = 0;
	};

	class IBlipPool
		: public IPool<IBlip>
	{
	public:
		virtual IBlip *NewStreamed(uint32_t sprite, const vector3& position, float streamRange, dimensionId_t dimension = 0) const = 0;
		virtual IBlip *New(uint32_t sprite, const vector3& position, dimensionId_t dimension = 0) const = 0;
	};

	class IPickupPool
		: public IPool<IPickup>
	{
	public:
		// todo
	};

	class IObjectPool
		: public IPool<IObject>
	{
	public:
		virtual IObject *New(uint32_t model, const vector3& position, const vector3& rotation, dimensionId_t dimension = 0) const = 0;
	};
}