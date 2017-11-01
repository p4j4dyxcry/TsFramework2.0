#pragma once

namespace cereal
{
    TS_SERIALIZE_TEMPLATE_SAVE_BINARY(TS::Array, array)
    {
        ar(cereal::make_size_tag(static_cast<cereal::size_type>(array.size()))); // number of elements
        ar(binary_data(array.data(), array.size() * sizeof(T)));
    }

    TS_SERIALIZE_TEMPLATE_LOAD_BINARY(TS::Array, array)
    {
        cereal::size_type arraySize;
        ar(cereal::make_size_tag(arraySize));

        array = TS::Array<T>(static_cast<unsigned>(arraySize));
        ar(cereal::binary_data(array.data(), static_cast<size_t>(arraySize) * sizeof(T)));
    }

    TS_SERIALIZE_TEMPLATE_SAVE_FILE(TS::Array, array)
    {
        ar(cereal::make_size_tag(static_cast<cereal::size_type>(array.size()))); // number of elements
        for (auto && v : array)
            ar(v);
    }

    TS_SERIALIZE_TEMPLATE_LOAD_FILE(TS::Array, array)
    {
        cereal::size_type size;
        ar(cereal::make_size_tag(size));

        array = TS::Array<T>(static_cast<unsigned>(size));
        for (auto && v : array)
            ar(v);
    }

    template <class Archive>
    void CEREAL_SAVE_FUNCTION_NAME(Archive & ar, TS::Array<bool> const & array)
    {
        ar(cereal::make_size_tag(static_cast<cereal::size_type>(array.size()))); // number of elements
        for (auto && v : array)
            ar(static_cast<bool>(v));
    }

    template <class Archive>
    void CEREAL_LOAD_FUNCTION_NAME(Archive & ar, TS::Array<bool> & array)
    {
        cereal::size_type size;
        ar(cereal::make_size_tag(size));

        array = TS::Array<bool>(static_cast<unsigned>(size));
        for (auto && v : array)
        {
            bool b;
            ar(b);
            v = b;
        }
    }
}

