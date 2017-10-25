#pragma once

namespace TS
{
    class Object
    {
    public:
        /**
         * \brief �N���X�̖��O���擾���܂�
         * \return �N���X�̖��O
         */
        const char*  GetClassName() const;


        /**
         * \brief ���̃N���X�𕶎���ɕϊ����܂�
         * \return �����񉻂��ꂽ�N���X
         */
        virtual const char* ToString()const;

        
        /**
        * \brief const char * �ւ̈Öق̃L���X�g�ł��B
        *        �f�t�H���g�ł�ToString()�Ɠ��������ɂȂ�܂��B
        */
        virtual operator const char*() const;

        
        /**
         * \brief �f�X�g���N�^
         */
        virtual ~Object() = default;
    };
}

