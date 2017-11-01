#pragma once


namespace TS
{
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
	};

    /**
	 * \brief スマートポインタインタフェース
	 * \tparam T 
	 */
	template <typename T>
	class IPointer : public Object
	{		
	protected:
		T* m_nativePointer;
		std::function< void(T*) > m_deleter;

	protected:

	    /**
		 * \brief 保持しているポインタを削除する
		 */
		virtual void DeletePointer()
		{
		    if ( m_nativePointer != nullptr )
                m_deleter(m_nativePointer);
		}

	public:

	    /**
		 * \brief コンストラクタ
		 * \param pointer 
		 */
		IPointer(T* pointer):
			m_nativePointer(pointer)
			, m_deleter([](T* p) {TS_DELETE(p); p = nullptr; }) {}

		IPointer(T* pointer,const std::function<void(T*)>& deleter)
			:m_nativePointer(pointer)
			,m_deleter(deleter){}

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
		virtual T*& operator ->() { return m_nativePointer; }
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
		UniquePtr(T* pointer, std::function<void(T*)>& deleter)
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

		IRefPtr(T* pointer, 
				ReferenceCounter * pRef ,
				const std::function<void(T*)>& deleter)
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
			m_pRefCounter = ptr.m_pRefCounter;
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
            }
        }
	public:
	    /**
		 * \brief コンストラクタ
		 */
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

			return *this;
		}
	};

}

