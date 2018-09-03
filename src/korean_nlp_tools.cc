/**
 * This source code doesn't belong to Facebook.
 * And it doesn't claim any kind of license or copyright either.
 * So it's all your responsibility no matter what could happen
 * if you attempt to run this code on your environment.
 */

#include "korean_nlp_tools.h"

namespace fasttext {

// reference: https://social.msdn.microsoft.com/Forums/vstudio/ko-KR/991e9328-0fe7-43a1-9eb4-eee5ff04aac7/5462044544-52488-51473-5133349457-48516475325461644592?forum=visualcplusko
size_t break_hangul_syllables(const wchar_t* str, wchar_t* buffer, size_t nSize)
{
    //초성
    static const wchar_t wcHead[] = {
        L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ',
        L'ㄸ', L'ㄹ', L'ㅁ', L'ㅂ',
        L'ㅃ', L'ㅅ', L'ㅆ', L'ㅇ',
        L'ㅈ', L'ㅉ', L'ㅊ', L'ㅋ',
        L'ㅌ', L'ㅍ', L'ㅎ'};

    //중성
    static const wchar_t wcMid[] = {
        L'ㅏ', L'ㅐ', L'ㅑ', L'ㅒ',
        L'ㅓ', L'ㅔ', L'ㅕ', L'ㅖ',
        L'ㅗ', L'ㅘ', L'ㅙ', L'ㅚ',
        L'ㅛ', L'ㅜ', L'ㅝ', L'ㅞ',
        L'ㅟ', L'ㅠ', L'ㅡ', L'ㅢ', L'ㅣ'};

    //종성
    static const wchar_t wcTail[] = {
        L'▒', L'ㄱ', L'ㄲ', L'ㄳ',
        L'ㄴ', L'ㄵ', L'ㄶ', L'ㄷ',
        L'ㄹ', L'ㄺ', L'ㄻ', L'ㄼ',
        L'ㄽ', L'ㄾ', L'ㄿ', L'ㅀ',
        L'ㅁ', L'ㅂ', L'ㅄ', L'ㅅ',
        L'ㅆ', L'ㅇ', L'ㅈ', L'ㅊ',
        L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ'};

    size_t pos = 0;

    std::fill(buffer, buffer + nSize, '\0');

    while(*str != '\0') {
        if (0xAC00 <= *str && *str <= 0xD7A3) {  // between '가' and '힣'
            if (pos + 3 > nSize - 1)
                break;

            buffer[pos] = wcHead[(*str - 0xAC00) / (21*28)];
            buffer[pos+1] = wcMid[(*str - 0xAC00) % (21 * 28) / 28];
            buffer[pos+2] = wcTail[(*str - 0xAC00) % 28];
            pos+=3;
        } else {
            if (pos + 1 > nSize - 1)
                break;

            buffer[pos] = *str;
            ++pos;
        }
        ++str;
    }

    buffer[pos] = '\0';

    return pos;
}

// reference: http://cozyu-textcube.blogspot.com/2008/01/string-wstring-%EB%B3%80%ED%99%98.html
std::wstring mbs_to_wcs(const std::string& str, const std::locale& loc)
{
    typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
    const codecvt_t& codecvt = std::use_facet<codecvt_t>(loc);
    std::mbstate_t state = std::mbstate_t();
    std::vector<wchar_t> buf(str.size() + 1);
    const char* in_next = str.c_str();
    wchar_t* out_next = &buf[0];
    codecvt_t::result r = codecvt.in(state,
        str.c_str(), str.c_str() + str.size(), in_next,
        &buf[0], &buf[0] + buf.size(), out_next);

    return std::wstring(&buf[0]);
}

// reference: http://cozyu-textcube.blogspot.com/2008/01/string-wstring-%EB%B3%80%ED%99%98.html
std::string wcs_to_mbs(const std::wstring& str, const std::locale& loc)
{
    typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
    const codecvt_t& codecvt = std::use_facet<codecvt_t>(loc);
    std::mbstate_t state = std::mbstate_t();;
    std::vector<char> buf((str.size() + 1) * codecvt.max_length());
    const wchar_t* in_next = str.c_str();
    char* out_next = &buf[0];
    codecvt_t::result r = codecvt.out(state,
        str.c_str(), str.c_str() + str.size(), in_next,
        &buf[0], &buf[0] + buf.size(), out_next);
    return std::string(&buf[0]);
}

bool isHangulSyllable(const std::string& mbsLetter) {
    const std::wstring& wcsLetter = mbs_to_wcs(mbsLetter);
    const wchar_t* wcpChar = wcsLetter.c_str();
    return 0xAC00 <= *wcpChar && *wcpChar <= 0xD7A3;  // between '가' and '힣'
}

const wchar_t& getLastWStrChar(const std::string& mbsStr) {
    const std::wstring& wStr = mbs_to_wcs(mbsStr);
    return wStr[wStr.size() - 1];
}

bool isLastCharKoreanSyllable(const std::string& mbsStr) {
    const std::wstring& wStr = mbs_to_wcs(mbsStr);
    const wchar_t& wChar = wStr[wStr.size() - 1];
    return 0xAC00 <= wChar && wChar <= 0xD7A3;  // between '가' and '힣'
}

}
