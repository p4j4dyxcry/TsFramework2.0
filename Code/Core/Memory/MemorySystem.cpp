#include "Code/Core/Develop.h"
#include "MemorySystem.h"
#include "Code/Core/Engine.h"


namespace TS
{
    //!-------------------------------------------------------------------------
    // シンプルな標準アロケータを用意しておく
    // .cppに定義しておくことでエンドユーザからは見えないようにしておく
    //!-------------------------------------------------------------------------
    class SimpleAllocator : public IAllocator
    {
    private:
        TS_DISABLE_COPY(SimpleAllocator);

        StaticMemoryPool* s_MinPool;
        StaticMemoryPool* s_DefaultPool;
        StaticMemoryPool* s_LargePool;
    public:
        SimpleAllocator();
        ~SimpleAllocator();
        
        const char * ToString() const override;

        void* Alloc(size_t size) override;
        bool Free(void* ptr) override;
    };

    SimpleAllocator::SimpleAllocator()
    {
        s_MinPool     = new StaticMemoryPool(32,   4096 * 1024);
        s_DefaultPool = new StaticMemoryPool(128, 4096 * 1024);
        s_LargePool   = new StaticMemoryPool(256, 4096);
    }

    SimpleAllocator::~SimpleAllocator()
    {
        delete s_MinPool;
        delete s_DefaultPool;
        delete s_LargePool;
    }

    const char* SimpleAllocator::ToString() const
    {
        static std::string str;

        std::ostringstream ss;
        ss  << "シンプルアロケータ-----------------------------------" << std::endl
            << "メモリプール1" << std::endl
            << s_MinPool->ToString()
            << "メモリプール2" << std::endl
            << s_DefaultPool->ToString()
            << "メモリプール3" << std::endl
            << s_LargePool->ToString()
            << "-----------------------------------------------------" << std::endl;

        str = ss.str();
        return str.c_str();
    }

    void* SimpleAllocator::Alloc(const size_t size)
    {
        void * ptr;
        if (size <= s_MinPool->GetChunkSize())
        {
            if ((ptr = s_MinPool->Alloc(size)) != nullptr)
                return ptr;
        }

        if (size <= s_DefaultPool->GetChunkSize())
        {
            if ((ptr = s_DefaultPool->Alloc(size)) != nullptr)
                return ptr;
        }

        if ( (ptr = s_LargePool->Alloc(size)) != nullptr)
            return ptr;
        return nullptr;
    }

    bool SimpleAllocator::Free(void* ptr)
    {
        if (s_MinPool->From(ptr))     return s_MinPool->Free(ptr);
        if (s_DefaultPool->From(ptr)) return s_DefaultPool->Free(ptr);
        if (s_LargePool->From(ptr))   return s_LargePool->Free(ptr);
        return false;
    }

    void MemorySystem::AddAllocator(IAllocator* pAllocator)
    {
        m_Allocators.push_back(pAllocator);
    }

    MemorySystem& GetMemorySystem()
    {
        return Engine::Instance()->GetMemorySystem();
    }

    MemorySystem::MemorySystem() :
        m_isLeakChek(false), 
        m_pDefaultAllocator(nullptr), 
        m_pUserAllocator(nullptr),
		m_pHeadMetaData(nullptr),
		m_pCurrentMetaData(nullptr)
    {
    }

    MemorySystem::~MemorySystem()
    {
        if (m_pDefaultAllocator)
            delete m_pDefaultAllocator;       
    }

    IAllocator* MemorySystem::FindAllocator(void* ptr)
    {
        //if( IsEnableMemoryLeak() )
        //    return m_MetaDatas[ptr].pAllocator;

        //todo 
        return m_pDefaultAllocator;

    }

    void MemorySystem::EnableMemoryLeakCheck()
    {
        m_isLeakChek = true;
    }

    bool MemorySystem::IsEnableMemoryLeak() const
    {
        return m_isLeakChek;
    }

    IAllocator* MemorySystem::SetDefaultAllocator(IAllocator* pAllocator)
    {
        m_pUserAllocator = pAllocator;
        return m_pDefaultAllocator;
    }

    IAllocator* MemorySystem::GetDefaultAllocator()
    {
        if (m_pUserAllocator)
            return m_pUserAllocator;

        return GetSystemDefaultAllocator();
    }

    IAllocator* MemorySystem::GetSystemDefaultAllocator()
    {
        if (m_pDefaultAllocator == nullptr)
        {
            m_pDefaultAllocator = new SimpleAllocator();
            AddAllocator(m_pDefaultAllocator);
        }
        return m_pDefaultAllocator;
    }

    void MemorySystem::RegisterMemoryMetaData(MemoryMetaData* pMetaData)
    {
		if (m_pHeadMetaData == nullptr)
		{
			m_pHeadMetaData = pMetaData;
		}

		if (m_pCurrentMetaData != nullptr)
		{
			m_pCurrentMetaData->pNextBlock = pMetaData;
			pMetaData->pPrevBlock = m_pCurrentMetaData;
		}
		
		m_pCurrentMetaData = pMetaData;
		m_pCurrentMetaData->pNextBlock = nullptr;
    }

    void MemorySystem::RemoveMemoryMetaData(MemoryMetaData* pMetaData)
    {
		if (pMetaData->pPrevBlock != nullptr)
			pMetaData->pPrevBlock->pNextBlock = pMetaData->pNextBlock;
		else
			m_pHeadMetaData = pMetaData->pNextBlock;

		if (pMetaData->pNextBlock != nullptr)
			pMetaData->pNextBlock->pPrevBlock = pMetaData->pPrevBlock;
		else
			m_pCurrentMetaData = pMetaData->pPrevBlock;
    }

	int MemorySystem::GetAllocMemoryCount()const
	{
		int count = 0;

		for (auto p = m_pHeadMetaData;
			p != nullptr;
			p = p->pNextBlock, count++)
		{
		}
		return count;
	}

    void MemorySystem::DumpLeak()
    {
        if (m_pCurrentMetaData == nullptr)
        {
			TS_LOG("メモリリークはありません。");
			return;
        }


		TS_LOG_WARNING("%d個のメモリリークを検知しました。\n", GetAllocMemoryCount());
		int count = 0;
		for (auto p = m_pHeadMetaData;
			p != nullptr;
			p = p->pNextBlock)
        {
            ++count;
            std::ostringstream ss;
            ss << "【" << count << "】" << std::endl
                << "\t" << "ファイル " << p->fileName << std::endl
                << "\t" << "行　　　 " << p->line	     << std::endl
                << "\t" << "型　　　 " << p->typeData << std::endl;
            TS_LOG(ss.str().c_str());
        }
    }

    void MemorySystem::DumpInfo()
    {
        if (GetAllocMemoryCount() > 0)
        {
            TS_LOG("%d個のメモリを確保中\n", GetAllocMemoryCount());
        }
        int id = 0;
		int count = 0;
		for (auto p = m_pHeadMetaData;
			p != nullptr;
			p = p->pNextBlock)
		{
			++count;
			std::ostringstream ss;
			ss << "【" << count << "】" << std::endl
				<< "\t" << "ファイル " << p->fileName << std::endl
				<< "\t" << "行　　　 " << p->line << std::endl
				<< "\t" << "型　　　 " << p->typeData << std::endl;
			TS_LOG(ss.str().c_str());
		}
    }
}




