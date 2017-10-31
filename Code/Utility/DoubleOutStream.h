#pragma once
/**
 * \brief �g������
 * 
 * �ȉ��̗l�ɋL�q���邱�ƂŃR���\�[���ƃt�@�C���ɓ����o�͂��邱�Ƃ��ł���B
 * 
 * std::ostream os("Debug.log");
 * TS::cout2( os , std::cout ) << "x= " << x << endl;
 */
namespace TS
{
    typedef class DoubleOutStream
    {
    private:
        std::ostream& _stream0;
        std::ostream& _stream1;
    public:
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
