#include "NamedObject.h"

namespace TS
{

    /**
     * \brief 名前の取得
     * \return 名前
     */
    const char* NamedObject::GetName() const
    {
        return m_name;
    }

    /**
     * \brief 名前の設定
     * \param name 
     * \return 成功 true
     */
    bool NamedObject::SetName(const char* name)
    {
        char * source = const_cast<char*>(name);
        char * dest = m_name;

        do
        {
            (*dest++) = (*source);
        } while (*source++);

        return true;
    }

    NamedObject::NamedObject()
    {
        const int array_size = sizeof(m_name) / sizeof(m_name[0]);

        for (int i = 0; i < array_size; ++i)
            m_name[i] = '\0';
    }
}
