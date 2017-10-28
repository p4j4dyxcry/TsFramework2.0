#pragma once
#include "Code\Core\Develop.h"
#include "MemorySystem.h"

namespace TS
{
	class ReferenceCounter
	{
	private:
		std::mutex m_mutex;
		int m_referenceCount;
		int m_observerCount;
	public:
		ReferenceCounter()
			:m_referenceCount(0)
			,m_observerCount(0){}
		void Lock()
		{
			std::lock_guard<decltype(m_mutex)> lock(m_mutex);
		}
		void AddObserver()
		{
			Lock();
			++(m_observerCount);
		}

		void SubObserver()
		{
			Lock();
			--(m_observerCount);
		}

		void AddRef()
		{
			Lock();
			++(m_referenceCount);
			++(m_observerCount);
		}
		void SubRef()
		{
			Lock();
			--(m_referenceCount);
			--(m_observerCount);
		}
		bool IsRemovePointer()const
		{
			return m_referenceCount <= 0;
		}

		bool Invalid()const 
		{ 
			return m_observerCount <= 0; 
		}
	};

	template <typename T>
	class IPointer : public Object
	{		
	protected:
		T* m_nativePointer;
		std::function< void(T*) > m_deleter;

		virtual void DeletePointer()
		{
			if (m_nativePointer)
				m_deleter(m_nativePointer);
		}

	public:

		IPointer(T* pointer):
			m_nativePointer(pointer)
			, m_deleter([](T* p) {TS_DELETE(p);}) {}

		IPointer(T* pointer,const std::function<void(T*)>& deleter)
			:m_nativePointer(pointer)
			,m_deleter(deleter){}

		virtual T&  operator * () { return *m_nativePointer; }
		virtual T*& operator ->() { return m_nativePointer; }
		virtual const T&  operator * ()const { return *m_nativePointer; }
		virtual const T* operator ->()const { return m_nativePointer; }
		
		T* GetPointer(){ return m_nativePointer;}
		const T* GetPointer()const { return m_nativePointer; }
	};

	template<typename T>
	class UniquePtr : public IPointer<T>
	{
	private:
		TS_DISABLE_COPY(UniquePtr);

	public:
		UniquePtr(T* pointer): IPointer(pointer){}
		UniquePtr(T* pointer, std::function<void(T*)>& deleter)
			: IPointer(pointer, deleter) {}
		virtual ~UniquePtr()
		{
			DeletePointer();
		}
	};

	template <typename T>
	class IRefPtr : public  IPointer<T>
	{
	protected:
		ReferenceCounter* m_pRefCounter;
	public:
		IRefPtr(T* pointer) 
			: IRefPtr(pointer,nullptr) {}

		IRefPtr(T* pointer , ReferenceCounter * pRef)
			: IPointer(pointer)
			, m_pRefCounter(pRef){}

		IRefPtr(T* pointer, 
				ReferenceCounter * pRef ,
				const std::function<void(T*)>& deleter)
			: IPointer(pointer , deleter)
			, m_pRefCounter(pRef){}

		void AddObserver()
		{
			if (m_pRefCounter)
				m_pRefCounter->AddObserver();
		}

		void SubObserver()
		{
			if (m_pRefCounter)
				m_pRefCounter->SubObserver();
		}

		void AddRef()
		{
			if (m_pRefCounter)
				m_pRefCounter->AddRef();

		}
		void SubRef()
		{
			if (m_pRefCounter)
				m_pRefCounter->SubRef();
		}
		bool Invalid()const
		{
			return m_pRefCounter->Invalid();
		}
		void CopyRefCounterFromArgument(const IRefPtr& ptr)
		{
			m_deleter = ptr.m_deleter;
			m_pRefCounter = ptr.m_pRefCounter;
		}

	};

	template<typename T>
	class SharedPtr : public IRefPtr<T>
	{
	private:
		void Release()
		{
			if (m_pRefCounter == nullptr)
				return;

			m_pRefCounter->SubRef();
			if (m_pRefCounter->IsRemovePointer())
			{				
				DeletePointer();
			}
			if (m_pRefCounter->Invalid())
			{
				delete m_pRefCounter;
			}
		}

	public:
		SharedPtr() 
			: IRefPtr<T>(nullptr,nullptr){}

		SharedPtr(T *ptr) 
			: IRefPtr<T>(ptr, ptr != nullptr ? new ReferenceCounter() : nullptr)
		{
			if(m_pRefCounter)
				m_pRefCounter->AddRef();
		}
		SharedPtr(T* pointer, const std::function<void(T*)>& deleter)
			: IRefPtr(pointer 
					  ,pointer != nullptr ? new ReferenceCounter() : nullptr		
					  ,deleter)
		{
			if (m_pRefCounter)
				m_pRefCounter->AddRef();
		}


		SharedPtr(const SharedPtr &rhs) : IRefPtr<T>(rhs.m_nativePointer)
		{
			CopyRefCounterFromArgument(rhs);
			if (m_pRefCounter)
				m_pRefCounter->AddRef();
		}

		SharedPtr & operator = (const SharedPtr &rhs)
		{
			m_nativePointer = rhs.m_nativePointer;

			if (m_pRefCounter != nullptr)
				m_pRefCounter->Release();

			CopyRefCounterFromArgument(rhs);
			if (m_pRefCounter)
				m_pRefCounter->AddRef();

			return *this;
		}

		virtual ~SharedPtr()
		{
			Release();
		}
	};

	template<typename T>
	class WeakPtr : public IRefPtr<T>
	{
	public:
		WeakPtr() 
			: IRefPtr<T>(nullptr,nullptr){}

		WeakPtr(SharedPtr<T> &rhs) 
			: IRefPtr<T>(rhs)
		{
			CopyRefCounterFromArgument(rhs);
			m_pRefCounter->AddObserver();
		}

		WeakPtr(WeakPtr&rhs)
			: IRefPtr<T>(rhs)
		{
			CopyRefCounterFromArgument(rhs);
			m_pRefCounter->AddObserver();
		}

		WeakPtr & operator = (const WeakPtr &rhs)
		{
			m_nativePointer = rhs.m_nativePointer;

			if (m_pRefCounter != nullptr)
				m_pRefCounter->SubObserver();

			CopyRefCounterFromArgument(rhs);
			m_pRefCounter->AddObserver();

			return *this;
		}

		WeakPtr & operator = (const SharedPtr<T> &rhs)
		{
			m_nativePointer = rhs.m_nativePointer;

			if (m_pRefCounter != nullptr)
				m_pRefCounter->SubObserver();

			CopyRefCounterFromArgument(rhs);
			m_pRefCounter->AddObserver();

			return *this;
		}


		virtual ~WeakPtr()
		{
			if (m_pRefCounter == nullptr)
				return;

			if (m_pRefCounter != nullptr)
				m_pRefCounter->SubObserver();

			if (m_pRefCounter->Invalid())
			{
				delete m_pRefCounter;
			}
		}
	};
}

