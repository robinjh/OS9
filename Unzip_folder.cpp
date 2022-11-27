// Unzip_Folder.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "UnzipCTL.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	UnzipCTL uc("test.zip");
	/// 폴더 안에 test.zip 파일을 인식
	uc.extractall("test");
	/// test라는 이름의 폴더로 풀림
	std::cout<<std::endl;
	system("pause");

	return 0;
}

