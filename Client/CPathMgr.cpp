#include "pch.h"
#include "CPathMgr.h"

#include "CCore.h"

CPathMgr::CPathMgr()
	: m_szContentPath{}
	, m_szRelativePath{}
{

}

CPathMgr::~CPathMgr()
{

}

void CPathMgr::init()
{
	// F5 로 실행하거나 Ctrl + F5 로 실행하면 실행파일이 Output 폴더에 있는 bin 폴더에 있는데 프로그램이 아님
	// Visual Studio 에서 실행할 때는 실행파일이 있는 경로를 기준으로 해야 함
	// => 프로젝트 속성 -> 구성 속성 -> 디버깅 -> 작업 디렉토리를 $(SolutionDir)\Output\bin\ 로 설정 (실행파일이 있는 경로로 설정)
	GetCurrentDirectory(255, m_szContentPath);
	int iLen = (int)wcslen(m_szContentPath);

	// 마지막 폴더(실행파일이 있는) 경로만 남기고 나머지 부분에 null을 넣는 \0 문자
	for (int i = iLen - 1; i >= 0; --i)
	{
		if (m_szContentPath[i] == '\\')
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}

	wcscat_s(m_szContentPath, 255, L"\\bin\\content\\");

	//SetWindowText(CCore::GetInst()->GetMainHWnd(), m_szContentPath);
}

wstring CPathMgr::GetRelativePath(const wchar_t* _filePath)
{
	wstring strFilePath = _filePath;

	size_t iAbsLen = wcslen(m_szContentPath);
	size_t iFullLen = strFilePath.length();

	wstring strRelativePath = strFilePath.substr(iAbsLen, iFullLen - iAbsLen);

	return strRelativePath;
}

