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
	// F5 �� ���� �����ϰų� Ctrl + F5 �� ���� �����ϰ� �Ǹ� �����س��� Output ���� ���� bin ������ ��ü���� ���α׷��� �ƴ�
	// Visual Studio ��� ���� ���� �̿��� �����ϰ� �Ǳ� ������ ������Ʈ�� ������ ���ִ� ��θ� ��Ŀ����.
	// => ������Ʈ �Ӽ� -> ���� �Ӽ� -> ����� -> �۾� ���丮�� $(SolutionDir)\Output\bin\ ���� ���� (��������� �����ص� ������ ����)
	GetCurrentDirectory(255, m_szContentPath);
	int iLen = (int)wcslen(m_szContentPath);

	// ���� ������ (���̺귯�� ����) ��θ� ���� ���� �κп� null �� �����Ǵ� \0 ����
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

