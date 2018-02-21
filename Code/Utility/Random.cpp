#include "Random.h"

namespace TS
{
    template class Random<char>;
    template class Random<unsigned char>;
    template class Random<short>;
    template class Random<unsigned short>;
    template class Random<int>;
    template class Random<unsigned>;
    template class Random<unsigned int>;
    template class Random<float>;
    template class Random<double>;

    template<typename T>
    Random<T> Random<T>::Global(static_cast<unsigned>(timeGetTime()));

    template <typename T>
    unsigned Random<T>::xor128()
    {
        unsigned t;
        t = (x ^ (x << 11));
        x = y;
        y = z;
        z = w;
        return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
    }

    template<>
    float Random<float>::Range(float _val1, float _val2)
    {
        if (abs(_val1 - _val2) < FLT_EPSILON)
            return _val1;

        float _max = max(_val1, _val2);
        float _min = min(_val1, _val2);

        return (xor128() / TS_UINT_MAX_TO_FLT) * (_max - _min) + _min;
    }

    template<>
    double Random<double>::Range(double _val1, double _val2)
    {
        if (abs(_val1 - _val2) < DBL_EPSILON)
            return _val1;

        double _max = max(_val1, _val2);
        double _min = min(_val1, _val2);

        return (xor128() / TS_UINT_MAX_TO_DBL) * (_max - _min) + _min;
    }

    template <typename T>
    T Random<T>::Range(const T _val1, const T _val2)
    {
        if ( !(_val1 ^ _val2) )
            return _val1;

        T _max = _val1 > _val2   ? _val1 : _val2;
        T _min = !(_val1 ^ _max) ? _val2 : _val1;

        return static_cast<T> ((xor128() % (_max - _min)) + _min);
    }

    template <typename T>
    T Random<T>::operator()()
    {
        return static_cast<T>(xor128());
    }

    template <typename T>
    T Random<T>::operator()(const T _val)
    {
        return Range(0, _val);
    }

    template <typename T>
    T Random<T>::operator()(const T _val1, const T _val2)
    {
        return Range(_val1, _val2);
    }

    template <typename T>
    Random<T>::Random(const unsigned seed):
        x(_lrotl(123456789u,13) ^ seed ),
        y(_lrotl(362436069u,7)  ^ seed ),
        z(521288629u),
        w(seed)
    {
    }
}
