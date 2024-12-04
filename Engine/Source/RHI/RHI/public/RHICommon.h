#pragma once

namespace GameEngine
{
	namespace Render::HAL
	{
		/*
		* Represents the native render hardware object
		* E.g. ID3D12Device*, IDXGISwapChain*, ID3D12DescriptorHeap*, etc.
		*/
		class RenderNativeObject
		{
		public:
			RenderNativeObject() = default;
			RenderNativeObject(uint64_t i) : m_Value(i) { }
			RenderNativeObject(void* p) : m_Value(p) { }
			~RenderNativeObject() = default;

		public:
			// Redefining C-type cast
			template<typename T> operator T* () const { return static_cast<T*>(std::get<void*>(m_Value)); }

		public:
			uint64_t GetInt() const { return std::get<uint64_t>(m_Value); }
			void* GetPtr() const { return std::get<void*>(m_Value); }

		protected:
			std::variant<uint64_t, void*> m_Value;
		};

		/*
		* Parent class for any RHI classes. Incapsulates the native object
		*/
		class RenderBackendResource
		{
		public:
			// Non-copyable and non-movable
			RenderBackendResource(const RenderBackendResource&) = delete;
			RenderBackendResource(const RenderBackendResource&&) = delete;
			RenderBackendResource& operator=(const RenderBackendResource&) = delete;
			RenderBackendResource& operator=(const RenderBackendResource&&) = delete;

		protected:
			RenderBackendResource() = default;
			virtual ~RenderBackendResource() = default;

		public:
			// We will use manual reference counting to use the custom ComPtr-like interface
			virtual uint64_t AddRef() = 0;
			virtual uint64_t Release() = 0;

			// Returns a native object or interface, for example ID3D12Device*, or nullptr if the requested interface is unavailable.
			virtual RenderNativeObject GetNativeObject() = 0;
		};


		/*
		* Modified copy of Microsoft::WRL::ComPtr<T>
		*/
		template <typename T>
		class RefCountPtr
		{
		public:
			using InterfaceType = T;

		protected:
			InterfaceType* ptr;

			// We want to be able to access the ptr member without additional problems
			template<class U> friend class RefCountPtr;

			void InternalAddRef() const noexcept
			{
				if (ptr != nullptr)
				{
					ptr->AddRef();
				}
			}

			uint64_t InternalRelease() noexcept
			{
				uint64_t ref = 0;
				T* temp = ptr;

				if (temp != nullptr)
				{
					ptr = nullptr;
					ref = temp->Release();
				}

				return ref;
			}

		public:

			RefCountPtr() noexcept : ptr(nullptr)
			{
			}

			RefCountPtr(std::nullptr_t) noexcept : ptr(nullptr)
			{
			}

			template<class U>
			RefCountPtr(U* other) noexcept : ptr(other)
			{
				InternalAddRef();
			}

			RefCountPtr(const RefCountPtr& other) noexcept : ptr(other.ptr)
			{
				InternalAddRef();
			}

			// copy ctor that allows to instanatiate class when U* is convertible to InterfaceType*
			template<class U> requires std::convertible_to<U*, InterfaceType*>
			RefCountPtr(const RefCountPtr<U>& other) noexcept :
				ptr(other.ptr)

			{
				InternalAddRef();
			}

			RefCountPtr(RefCountPtr&& other) noexcept : ptr(nullptr)
			{
				if (this != reinterpret_cast<RefCountPtr*>(&reinterpret_cast<unsigned char&>(other)))
				{
					Swap(other);
				}
			}

			// Move ctor that allows instantiation of a class when U* is convertible to InterfaceType*
			template<class U> requires std::convertible_to<U*, InterfaceType*>
			RefCountPtr(RefCountPtr<U>&& other) noexcept :
				ptr(other.ptr)
			{
				other.ptr = nullptr;
			}

			~RefCountPtr() noexcept
			{
				InternalRelease();
			}

			RefCountPtr& operator=(std::nullptr_t) noexcept
			{
				InternalRelease();
				return *this;
			}

			RefCountPtr& operator=(InterfaceType* other) noexcept
			{
				if (ptr != other)
				{
					RefCountPtr(other).Swap(*this);
				}
				return *this;
			}

			template <typename U>
			RefCountPtr& operator=(U* other) noexcept
			{
				RefCountPtr(other).Swap(*this);
				return *this;
			}

			RefCountPtr& operator=(const RefCountPtr& other) noexcept
			{
				if (ptr != other.ptr)
				{
					RefCountPtr(other).Swap(*this);
				}
				return *this;
			}

			template<class U>
			RefCountPtr& operator=(const RefCountPtr<U>& other) noexcept
			{
				RefCountPtr(other).Swap(*this);
				return *this;
			}

			RefCountPtr& operator=(RefCountPtr&& other) noexcept
			{
				RefCountPtr(static_cast<RefCountPtr&&>(other)).Swap(*this);
				return *this;
			}

			template<class U>
			RefCountPtr& operator=(RefCountPtr<U>&& other) noexcept
			{
				RefCountPtr(static_cast<RefCountPtr<U>&&>(other)).Swap(*this);
				return *this;
			}

			void Swap(RefCountPtr&& r) noexcept
			{
				T* tmp = ptr;
				ptr = r.ptr;
				r.ptr = tmp;
			}

			void Swap(RefCountPtr& r) noexcept
			{
				T* tmp = ptr;
				ptr = r.ptr;
				r.ptr = tmp;
			}

			[[nodiscard]] InterfaceType* Get() const noexcept
			{
				return ptr;
			}

			operator InterfaceType* () const
			{
				return ptr;
			}

			InterfaceType* operator->() const noexcept
			{
				return ptr;
			}

			InterfaceType** operator&()
			{
				return &ptr;
			}

			[[nodiscard]] InterfaceType* const* GetAddressOf() const noexcept
			{
				return &ptr;
			}

			[[nodiscard]] InterfaceType** GetAddressOf() noexcept
			{
				return &ptr;
			}

			[[nodiscard]] InterfaceType** ReleaseAndGetAddressOf() noexcept
			{
				InternalRelease();
				return &ptr;
			}

			InterfaceType* Detach() noexcept
			{
				InterfaceType* ptr = ptr;
				ptr = nullptr;
				return ptr;
			}

			// Set the pointer while keeping the object's reference count unchanged
			void Attach(InterfaceType* other)
			{
				if (ptr != nullptr)
				{
					auto ref = ptr->Release();
					(void)ref;

					// Attaching to the same object only works if duplicate references are being coalesced. Otherwise
					// re-attaching will cause the pointer to be released and may cause a crash on a subsequent dereference.
					assert(ref != 0 || ptr != other);
				}

				ptr = other;
			}

			// Create a wrapper around a raw object while keeping the object's reference count unchanged
			static RefCountPtr<InterfaceType> Create(InterfaceType* other)
			{
				RefCountPtr<InterfaceType> Ptr;
				Ptr.Attach(other);
				return Ptr;
			}

			uint64_t Reset()
			{
				return InternalRelease();
			}
		};    // RefCountPtr

		// Is used by RefCountPtr
		template<class T>
		class RefCounter : public T
		{
		private:
			std::atomic<uint64_t> m_RefCount = 0;

		public:
			virtual uint64_t AddRef() override
			{
				return ++m_RefCount;
			}

			virtual uint64_t Release() override
			{
				uint64_t result = --m_RefCount;
				if (result == 0) {
					delete this;
				}
				return result;
			}
		};
	}
}