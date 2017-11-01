#pragma once

namespace TS
{
    typedef unsigned char u8;
    typedef signed char s8;

    typedef unsigned short u16;
    typedef signed short   s16;

    typedef unsigned long u32;
    typedef signed long   s32;

    typedef unsigned long long  u64;
    typedef signed long long    s64;

    typedef float f32;
    typedef double f64;

    typedef unsigned int uint;
    typedef u8 byte;

    //! 固定配列クラス
    template<typename T>
    struct Array : Object
    {
        Array():array_size(0),value(nullptr)
        {
            
        }

        Array(unsigned sz) : array_size(sz)
        {
            value = TS_NEWARRAY(T, sz);
        }
        virtual ~Array()
        {
            array_size = 0;
            TS_DELETE(value);
        }

        Array(Array&& rhs) TS_NOEXCEPT
        {
            *this = rhs;
        }

        Array& operator=(Array&& rhs) TS_NOEXCEPT
        {
            array_size = rhs.array_size;
            value = rhs.value;
            return *this;
        }

        T& operator[](const int index)
        {
            return value[index];
        }


        T* begin() const
        {
            return value;
        }
        T* end() const
        {
            return value + array_size;
        }

        bool Empty() const
        {
            return array_size == 0;
        }
        unsigned size()const
        {
            return array_size;
        }
        T* data()const
        {
            return value;
        }
    private:
        TS_DISABLE_COPY(Array);

        unsigned array_size;
        T* value;
    };


}


