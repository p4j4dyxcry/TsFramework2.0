#include "INamed.h"

namespace TS
{
    /**
     * \brief 名前の取得
     * \return 名前
     */
    const char* INamed::GetIName() const
    {
        return m_iname;
    }

    /**
     * \brief 名前の設定
     * \param name 
     * \return 成功 true
     */
    bool INamed::SetIName(const char* name)
    {
        char * source = const_cast<char*>(name);
        char * dest = m_iname;

        do
        {
            (*dest++) = (*source);
        } while (*source++);

        return true;
    }

    INamed::INamed()
    {
        const int array_size = sizeof(m_iname) / sizeof(m_iname[0]);

        for (int i = 0; i < array_size; ++i)
            m_iname[i] = '\0';
    }
}
