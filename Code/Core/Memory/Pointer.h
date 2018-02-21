#pragma once


namespace TS
{
    struct IDeleter : Object
    {
        virtual void Delete() = 0;
    };

    template <typename P>
    struct DefaultDeleter : IDeleter
    {
    public:
        void Delete()override
        {
            TS_DELETE(m_pointer);
        }
        DefaultDeleter(P* p)
            :m_pointer(p) {}
        P* m_pointer;
    };

    template <typename P ,typename D>
    struct CustomDeleter : IDeleter
    {
    public:
        void Delete()override
        {
            m_deleter(m_pointer);
        }
        CustomDeleter(P* p, D t)
            :m_pointer(p), m_deleter(t){}
        D m_deleter;
        P* m_pointer;
    };

    /**
	 * \brief 参照カウンタを定義するクラス
	 */
	class ReferenceCounter : IMutex
	{
	private:
		int m_referenceCount;
		int m_observerCount;
	public:
		ReferenceCounter()
			:m_referenceCount(0)
			,m_observerCount(0){}

	    /**
	     * \brief 監視カウンタを増加
	     */
	    void AddObserver();

	    /**
	     * \brief 監視カウンタを減少
	     */
	    void SubObserver();

	    /**
	     * \brief 参照カウンタを増加
	     */
	    void AddRef();

        /**
        * \brief 参照カウンタを減少
        */
	    void SubRef();

	    /**
        * \brief 参照者がいないかどうか
        * \return 参照者がいないならtrue
	     */
	    bool IsRefZero() const;

	    /**
	     * \brief 参照者も監視者もいないかどうか
	     * \return いないならtrue
	     */
	    bool Invalid() const;

		int GetRefCount()const
		{
			return m_referenceCount;
		}

		int GetObserverCount()const
		{
			return m_observerCount;
		}
	};

    /**
	 * \brief スマートポインタインタフェース
	 * \tparam T 
	 */
	template <typename T>
	class IPointer : public Object
	{		
        friend class IPointer;

	protected:
		T* m_nativePointer;
        IDeleter* m_deleter;

	protected:

	    /**
		 * \brief 保持しているポインタを削除する
		 */
		virtual void DeletePointer()
		{
		    if ( m_nativePointer != nullptr && m_deleter)
		    {
                m_deleter->Delete();
                TS_DELETE(m_deleter);
		    }
		}

	public:

	    /**
		 * \brief コンストラクタ
		 * \param pointer 
		 */
		IPointer(T* pointer):
			m_nativePointer(pointer)
			, m_deleter(pointer ? TS_NEW(DefaultDeleter<T>)(pointer) : nullptr) {}

        template<typename Deleter>
		IPointer(T* pointer, Deleter& deleter)
			:m_nativePointer(pointer)
			, m_deleter(TS_NEW(CustomDeleter<T, Deleter>)(pointer, deleter))
		{}

	    /**
         * \brief 生ポインタを取得する
         * \return 
         */
        T* GetPointer() { return m_nativePointer; }

	    /**
         * \brief 生ポインタを取得する
         * \return 
         */
        const T* GetPointer()const { return m_nativePointer; }


	    /**
         * \brief ポインタがnullかどうかをチェックする
         * \return null　なら　true
         */
        bool IsNull()const{ return *this == nullptr;}

	    /**
        * \brief ポインタとして振る舞うためのオペレータ
        */        
		virtual T&  operator * () { return *m_nativePointer; }
		virtual T* operator ->() { return m_nativePointer; }
		virtual const T&  operator * ()const { return *m_nativePointer; }
		virtual const T* operator ->()const { return m_nativePointer; }		
        bool operator==(const IPointer& rhs){ return m_nativePointer == rhs.m_nativePointer; }
        bool operator!=(const IPointer& rhs){ return !(*this == rhs);}
        bool operator==(const nullptr_t& null){ return m_nativePointer == null; }
        bool operator!=(const nullptr_t& null){ return !(*this == null);}
        operator bool()const{return (*this != nullptr);}
	};

    /**
	 * \brief 所有権が移動しないポインタ
	 * \tparam T 
	 */
	template<typename T>
	class UniquePtr : public IPointer<T>
	{
	private:
		TS_DISABLE_COPY(UniquePtr);
        TS_DISABLE_MOVE(UniquePtr);
	public:
		UniquePtr(T* pointer): IPointer(pointer){}
        template<typename Deleter>
		UniquePtr(T* pointer, Deleter& deleter)
			: IPointer(pointer, deleter) {}

	    /**
         * \brief デストラクタ
         */
        virtual ~UniquePtr()
        {
            DeletePointer();
        }

	    /**
         * \brief 所有権を移動させる
         * \param from 移動元
         * \return 
         */
        UniquePtr<T>& Move(UniquePtr& from)
		{            
            m_nativePointer = from.m_nativePointer;
            m_deleter = from.m_deleter;

            from.m_nativePointer = nullptr;
            const type_info& info = typeid(int);

            return *this;
		}
	};

    /**
	 * \brief 参照カウント付スマートポインタ
	 * \tparam T 
	 */
	template <typename T>
	class IRefPtr : public  IPointer<T>
	{
        friend class IRefPtr;
	protected:
		ReferenceCounter* m_pRefCounter;
	public:

	    /**
		 * \brief コンストラクタ
		 * \param pointer 
		 */
		IRefPtr(T* pointer) 
			: IRefPtr(pointer,nullptr) {}

		IRefPtr(T* pointer , ReferenceCounter * pRef)
			: IPointer(pointer)
			, m_pRefCounter(pRef){}

        template<typename Deleter>
		IRefPtr(T* pointer, 
				ReferenceCounter * pRef ,Deleter& deleter)
			: IPointer(pointer , deleter)
			, m_pRefCounter(pRef){}

	    /**
		 * \brief 監視者を追加
		 */
		void AddObserver()
		{
			if (m_pRefCounter)
				m_pRefCounter->AddObserver();
		}

        /**
        * \brief 監視者を減らす
        */
		void SubObserver()
		{
			if (m_pRefCounter)
				m_pRefCounter->SubObserver();
		}

	    /**
		 * \brief 参照者を追加
		 */
		void AddRef()
		{
			if (m_pRefCounter)
				m_pRefCounter->AddRef();

		}

        /**
        * \brief 参照者を減らす
        */
		void SubRef()
		{
			if (m_pRefCounter)
				m_pRefCounter->SubRef();
		}

	    /**
		 * \brief 参照者も監視者もいない？
		 * \return 監視者がいなければtrue
		 */
		bool Invalid()const
		{
			return m_pRefCounter->Invalid();
		}

        void CopyRefCounterFromArgument(const IRefPtr& ptr)
        {
            m_deleter = ptr.m_deleter;
            m_pRefCounter = ptr.GetRefConter();
        }

        ReferenceCounter* GetRefConter()const
		{
            return m_pRefCounter;
		}
	};

    /**
    * \brief 強参照スマートポインタ
    *        参照カウンタの操作権限　有
    *        監視カウンタの操作権限　有
    *        メモリの破棄権限　　　　有
    *        参照カウンタの破棄権限　有
    * \tparam T
    */
	template<typename T>
	class SharedPtr : public IRefPtr<T>
	{
        friend class SharedPtr;
	public:
	    /**
		 * \brief コンストラクタ
		 */
		SharedPtr() 
			: IRefPtr<T>(nullptr,nullptr){}

		SharedPtr(T *ptr) 
			: IRefPtr<T>(ptr, ptr != nullptr ? TS_NEW(ReferenceCounter)() : nullptr)
		{
			if(m_pRefCounter)
				m_pRefCounter->AddRef();
		}
        template<typename Deleter>
		SharedPtr(T* pointer, Deleter& deleter)
			: IRefPtr(pointer 
					  ,pointer != nullptr ? TS_NEW(ReferenceCounter)() : nullptr
					  ,deleter)
		{
			if (m_pRefCounter)
				m_pRefCounter->AddRef();
		}
		SharedPtr(const SharedPtr<T> &rhs) : IRefPtr<T>(rhs.m_nativePointer)
		{
			CopyRefCounterFromArgument(rhs);
			if (m_pRefCounter)
				m_pRefCounter->AddRef();
		}

        template<class T_UpCastType> 
	    SharedPtr(SharedPtr<T_UpCastType> &rhs): IRefPtr<T>(rhs.GetPointer())
	    {
            m_deleter = rhs.m_deleter;
            m_pRefCounter = rhs.GetRefConter();
	        if (m_pRefCounter)
	            m_pRefCounter->AddRef();
	    }

	    /**
         * \brief デストラクタ
         */
        virtual ~SharedPtr()
        {
            Release();
        }

        /**
        * \brief 解放処理
        *        参照カウントを減らす。
        *        もし自分が最後の参照者の場合　　メモリの破棄
	    *        さらに自分が最後の監視者の場合　参照カウンタの破棄
        */
        void Release()
        {
            if (m_pRefCounter == nullptr)
                return;

            m_pRefCounter->SubRef();

            if (m_pRefCounter->IsRefZero())
            {
                DeletePointer();
            }
            if (m_pRefCounter->Invalid())
            {
                TS_DELETE(m_pRefCounter);
				m_pRefCounter = nullptr;
            }
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
	};

    /**
	 * \brief 弱参照スマートポインタ
	 *        参照カウンタの操作権限　無
	 *        監視カウンタの操作権限　有
	 *        メモリの破棄権限　　　　無
	 *        参照カウンタの破棄権限　有
	 * \tparam T 
	 */
	template<typename T>
	class WeakPtr : public IRefPtr<T>
	{
        friend class WeakPtr;
	private:

        /**
        * \brief 解放処理
        *        参照カウントを減らす。
        *        もしに自分が最後の監視者の場合　参照カウンタの破棄
        */
        void Release()
        {
            if (m_pRefCounter == nullptr)
                return;

            if (m_pRefCounter != nullptr)
                m_pRefCounter->SubObserver();

            if (m_pRefCounter->Invalid())
            {
                TS_DELETE(m_pRefCounter);
				m_pRefCounter = nullptr;
            }
        }
	public:
	    /**
		 * \brief コンストラクタ
		 */
		WeakPtr() 
			: IRefPtr<T>(nullptr,nullptr){}

		WeakPtr(const SharedPtr<T> &rhs) 
			: IRefPtr<T>(rhs)
		{
            m_pRefCounter = rhs.GetRefConter();
			if(m_pRefCounter != nullptr)
				m_pRefCounter->AddObserver();
		}
        template<typename T_UpCast>
        WeakPtr(SharedPtr<T_UpCast> &rhs): IRefPtr<T>(rhs.GetPointer())
        {
            m_pRefCounter = rhs.GetRefConter();
            if (m_pRefCounter != nullptr)
                m_pRefCounter->AddObserver();
        }

		WeakPtr(const WeakPtr&rhs)
			: IRefPtr<T>(rhs)
		{
            m_pRefCounter = rhs.GetRefConter();
			if (m_pRefCounter != nullptr)
				m_pRefCounter->AddObserver();
		}

	    /**
         * \brief デストラクタ
         */
        virtual ~WeakPtr()
        {
            Release();
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
            std::get_deleter<void(*)(int)>(1);
			return *this;
		}
	};

}

