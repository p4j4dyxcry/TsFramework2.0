#pragma once


inline bool IsUtf8(const char *str, int length)
{
    const unsigned char lead = *str;
    unsigned char trail = *(str + 1);
    unsigned char ab = 2;

    if (length < 2)
        return false;


    if (0xE0 > lead || lead > 0xED) {
        return false;
    }

    if (lead == 0xE0 && 0xA0 > trail) {
        return false;
    }

    if (lead == 0xED && trail > 0x9F) {
        return false;
    }

    auto utf8_trail = [](char c) { return (c & 0xC0) == 0x80; };

    do {
        trail = *(++str);
        if (!utf8_trail(trail)) {
            return false;
        }

    } while (--ab);

    return true;
}


inline std::string UTF8toSjis(std::string srcUTF8) 
{
    if (IsUtf8(&srcUTF8[0], srcUTF8.size()) == false)
        return srcUTF8;

    //Unicodeへ変換後の文字列長を得る
    int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);

    //必要な分だけUnicode文字列のバッファを確保
    wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

    //UTF8からUnicodeへ変換
    MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

    //ShiftJISへ変換後の文字列長を得る
    int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

    //必要な分だけShiftJIS文字列のバッファを確保
    char* bufShiftJis = new char[lengthSJis];

    //UnicodeからShiftJISへ変換
    WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

    std::string strSJis(bufShiftJis);

    delete[] bufUnicode;
    delete[] bufShiftJis;

    return strSJis;
}

std::string SjistoUTF8(std::string srcSjis) {
    //Unicodeへ変換後の文字列長を得る
    int lenghtUnicode = MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, NULL, 0);

    //必要な分だけUnicode文字列のバッファを確保
    wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

    //ShiftJISからUnicodeへ変換
    MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, bufUnicode, lenghtUnicode);


    //UTF8へ変換後の文字列長を得る
    int lengthUTF8 = WideCharToMultiByte(CP_UTF8, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

    //必要な分だけUTF8文字列のバッファを確保
    char* bufUTF8 = new char[lengthUTF8];

    //UnicodeからUTF8へ変換
    WideCharToMultiByte(CP_UTF8, 0, bufUnicode, lenghtUnicode + 1, bufUTF8, lengthUTF8, NULL, NULL);

    std::string strUTF8(bufUTF8);

    delete[] bufUnicode;
    delete[] bufUTF8;

    return strUTF8;
}

