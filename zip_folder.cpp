// zip_folder.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "FolderList.h"
#include "ZipCTL.h"
#include <iostream>
int _tmain(int arge, _TCHAR* argv[])
{
	{
	// 폴더 내 파일 목록
	FolderList::ZIP_FLIST flist;

	FolderList fn;
	fn("C:\\Python27", flist);
	/// 파일 경로
	for (auto& item : flist) {
		std::cout<<std::endl<< item.filepath_;
	}
	///파일 읽기

	//압축
	ZipCTL zc("test.zip");
	//test.zip이라는 이름으로 압축 파일 생성.
	zc.make("C:\\Python27", flist);
	// 파일 경로
	}
	std::cout<<std::endl;
	system("pause");
	return 0;
}
