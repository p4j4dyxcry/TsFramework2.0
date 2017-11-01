#pragma once
/**
 * \brief ２つのストリームに同時にデータを出力するためのクラス
 *        
 *        例)以下の様に記述することでコンソールとファイルに同時出力することができる。
 *           std::ostream os("Debug.log");
 *           TS::cout2( os , std::cout ) << "x= " << x << endl;
 */
namespace TS
{
    /**
     * \brief ２つのストリームに同時にデータを出力するためのクラス
     */
    typedef class DoubleOutStream
    {
    private:
        std::ostream& _stream0;
        std::ostream& _stream1;
    public:
        /**
         * \brief コンストラクタ
         * \param _os1 
         * \param _os2 
         */
        DoubleOutStream( std::ostream& _os1 ,
               std::ostream& _os2 = std::cout )
            :_stream0(_os1)
            ,_stream1(_os2)            
        {
        };

        template <typename T>
        DoubleOutStream& operator<< (const T &rhs)
        {
            _stream0 << rhs;
            _stream1 << rhs; 
            return *this;
        };

        DoubleOutStream& operator<< (std::ostream& (*__pf)( std::ostream&) )
        {
            __pf(_stream0);
            __pf(_stream1);
            return *this;
        };
    } cout2;
}
