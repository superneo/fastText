#include <iostream>
#include <vector>
#include <wchar.h>
#include <locale>
#include "korean_nlp_tools.h"

using namespace std;
using namespace fasttext;

int main(int argc, char* argv[])
{
	//setlocale(LC_ALL, "Korean");
	setlocale(LC_ALL, "");
	//std::wcout.imbue(std::locale("kor"));
	wcout.imbue(locale(""));
	const wchar_t* str = L"나는 한AB국CD을 사랑합니다...ㅎㅎㅎ 이건 되나?";
	printf("%S\n", L"이거 나오나?");
	cout << "한글" << endl;
	wcout << str << endl;
	wchar_t buffer[4096];
	int num_buf_elems = sizeof(buffer)/sizeof(buffer[0]);
	cout << "# elements in buffer: " << num_buf_elems << endl;
	break_hangul_syllables(str, buffer, num_buf_elems);
	printf("%S\n", buffer);

	string lines[] = {
		"❷ 옷을 벗다(果) : 퇴직하다(因)",
		"또 다른 친구를 데리고 나갔거든요?",
		"◆ 좋은 학원을 선택하는 요령",
		"재미있게 봤습니다. 매력적인 행복이네요, '-' ㅎㅎㅎ.",
		"OST가 좋은 영화!!",
		"아재밌당아재밌당아재밌당아재밌당아재밌당",
		"좋은니다..^.,^~~",
		"2.3 (10자 제한)",
		"개꿀잼ㅋ개꿀잼ㅋ10자",
		"the play brought more funny to my life"};
	int num_lines = sizeof(lines)/sizeof(lines[0]);
	for (int i = 0; i < num_lines; ++i) {
		fill_n(buffer, num_buf_elems, 0);
		cout << "<line " << i << ">" << endl;
		cout << "	[original] " << lines[i] << endl;
		wstring wstr = mbs_to_wcs(lines[i], std::locale(""));
		break_hangul_syllables(wstr.c_str(), buffer, num_buf_elems);
		wcout << "	[broken] " << buffer << endl;
	}

	return 0;
}

