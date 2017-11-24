#pragma once




namespace TS
{
    template<class T = int>
    class Random : public Object
    {
    public:
        
        /**
         * \brief グローバル乱数
         */
        static Random<T> Global;

        /**
         * \brief レンジを指定せず乱数を取得
         * \return 0 ～ UINT_MAX
         */
        T operator()();

        /**
        * \brief 最大を指定して乱数を取得
        * \return 0 ～ _valの間
        */
        T operator()(const T _val);

        /**
        * \brief 範囲を指定して乱数を取得
        * \return _val1 ～ _val2の間
        */
        T operator()(const T _val1, const T _val2);

        /**
         * \brief コンストラクタ
         * \param seed 初期シード
         */
        Random(const unsigned seed);

    private:
        Random<T>(const Random<T>&) = delete;
        Random<T>& operator=(const Random<T>&) = delete;

        Random<T>(Random<T>&&) = delete;
        Random<T>& operator=(Random<T>&&) = delete;

    private:
        T Range(const T _val1, const T _val2);
        unsigned xor128();
        unsigned x, y, z, w;
    };

    inline int Rand() { return Random<int>::Global(0,INT_MAX); }
    inline int Rand(int x) { return Random<int>::Global(0, x); }
    inline int Rand(int x, int y) { return Random<int>::Global(x, y); }

    inline float Randf() { return Random<float>::Global(0, 1.0f); }
    inline float Randf(float x) { return Random<float>::Global(0, x); }
    inline float Randf(float x, float y) { return Random<float>::Global(x, y); }

    template<typename T>
    void Shuffle(T& _array ,const int size )
    {
        int count = size;

        while(count > 1)
        {
            int random = Rand(size);

            T temp = _array[count];
            _array[count] = _array[random];
            _array[random] = temp;
            --count;
        }
    }
}
