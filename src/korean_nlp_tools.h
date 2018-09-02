/**
 * This source code doesn't belong to Facebook.
 * And it doesn't claim any kind of license or copyright either.
 * So it's all your responsibility no matter what could happen
 * if you attempt to run this code on your environment.
 */

#pragma once

#include <iostream>
#include <vector>
#include <wchar.h>
#include <locale>

#if defined(__clang__) || defined(__GNUC__)
# define FASTTEXT_DEPRECATED(msg) __attribute__((__deprecated__(msg)))
#elif defined(_MSC_VER)
# define FASTTEXT_DEPRECATED(msg) __declspec(deprecated(msg))
#else
# define FASTTEXT_DEPRECATED(msg)
#endif

namespace fasttext {

size_t break_hangul_syllables(const wchar_t* str, wchar_t* buffer, size_t nSize);

std::wstring mbs_to_wcs(const std::string& str, const std::locale& loc = std::locale("ko_KR.UTF-8"));

std::string wcs_to_mbs(const std::wstring& str, const std::locale& loc = std::locale("ko_KR.UTF-8"));

bool isHangulSyllable(const std::string& mbsLetter);

const wchar_t& getLastWStrChar(const std::string& mbsStr);

bool isLastCharKoreanSyllable(const std::string& mbsStr);

}
