#pragma once


namespace TS
{
	class ReferenceCounter : IMutex
	{
	private:
		int m_referenceCount;
		int m_observerCount;
	public:
		ReferenceCounter()
			:m_referenceCount(0)
			,m_observerCount(0){}

	    void AddObserver();

	    void SubObserver();

	    void AddRef();

	    void SubRef();

	    bool IsRemovePointer() const;

	    bool Invalid() const;
	};

	template <typename T>
	class IPointer : public Object
	{		
	protected:
		T* m_nativePointer;
		std::function< void(T*) > m_deleter;

	protected:
		virtual void DeletePointer(){if ( m_nativePointer )m_deleter(m_nativePointer);}

	public:
		IPointer(T* pointer):
			m_nativePointer(pointer)
			, m_deleter([](T* p) {TS_DELETE(p); p = nullptr; }) {}

		IPointer(T* pointer,const std::function<void(T*)>& deleter)
			:m_nativePointer(pointer)
			,m_deleter(deleter){}

		virtual T&  operator * () { return *m_nativePointer; }
		virtual T*& operator ->() { return m_nativePointer; }
		virtual const T&  operator * ()const { return *m_nativePointer; }
		virtual const T* operator ->()const { return m_nativePointer; }
		
		T* GetPointer(){ return m_nativePointer;}
		const T* GetPointer()const { return m_nativePointer; }

        bool operator==(const IPointer& rhs){ return m_nativePointer == rhs.m_nativePointer; }

        bool operator!=(const IPointer& rhs){ return !(*this == rhs);}

        bool operator==(const nullptr_t& null){ return m_nativePointer == null; }

        bool operator!=(const nullptr_t& null){ return !(*this == null);}

        operator bool()const{return (*this != nullptr);}

        bool IsNull()const{ return *this == nullptr;}
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
	public:
		SharedPtr() 
			: IRefPtr<T>(nullptr,nullptr){}

		SharedPtr(T *ptr) 
			: IRefPtr<T>(ptr, ptr != nullptr ? TS_NEW(ReferenceCounter)() : nullptr)
		{
			if(m_pRefCounter)
				m_pRefCounter->AddRef();
		}
		SharedPtr(T* pointer, const std::function<void(T*)>& deleter)
			: IRefPtr(pointer 
					  ,pointer != nullptr ? TS_NEW(ReferenceCounter)() : nullptr
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
                Release();

			CopyRefCounterFromArgument(rhs);
			if (m_pRefCounter != nullptr)
				m_pRefCounter->AddRef();

			return *this;
		}

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
				TS_DELETE(m_pRefCounter);
			}
		}

		virtual ~SharedPtr()
		{
			Release();
		}
	};

	template<typename T>
	class WeakPtr : public IRefPtr<T>
	{
	private:
        void Release()
        {
            if (m_pRefCounter == nullptr)
                return;

            if (m_pRefCounter != nullptr)
                m_pRefCounter->SubObserver();

            if (m_pRefCounter->Invalid())
            {
                TS_DELETE(m_pRefCounter);
            }
        }
	public:
		WeakPtr() 
			: IRefPtr<T>(nullptr,nullptr){}

		WeakPtr(SharedPtr<T> &rhs) 
			: IRefPtr<T>(rhs)
		{
			CopyRefCounterFromArgument(rhs);
			if(m_pRefCounter != nullptr)
				m_pRefCounter->AddObserver();
		}

		WeakPtr(WeakPtr&rhs)
			: IRefPtr<T>(rhs)
		{
			CopyRefCounterFromArgument(rhs);
			if (m_pRefCounter != nullptr)
				m_pRefCounter->AddObserver();
		}

		WeakPtr & operator = (const WeakPtr &rhs)
		{
			m_nativePointer = rhs.m_nativePointer;

			if (m_pRefCounter != nullptr)
                Release();

			CopyRefCounterFromArgument(rhs);
			if (m_pRefCounter != nullptr)
				m_pRefCounter->AddObserver();

			return *this;
		}

		WeakPtr & operator = (const SharedPtr<T> &rhs)
		{
			m_nativePointer = rhs.m_nativePointer;

			if (m_pRefCounter != nullptr)
                Release();

			CopyRefCounterFromArgument(rhs);
			if (m_pRefCounter != nullptr)
				m_pRefCounter->AddObserver();

			return *this;
		}

		virtual ~WeakPtr()
		{
            Release();
		}
	};

}

