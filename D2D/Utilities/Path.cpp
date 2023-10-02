#include"framework.h"
#include"Path.h"

//파일 유무 확인하기
bool Path::ExistFile(const string& path)
{
	return ExistFile(String::ToWstring(path));
}

bool Path::ExistFile(const wstring& path)
{
	DWORD fileValue = GetFileAttributes(path.c_str());
	//파일 속성 알아볼때 활용 : 우클릭 속성 - 파일이 없다면 -1반환
	//DWORD에서 -1은 오버플로우가된다

	return fileValue < 0xFFFFFFFF;
}

//파일 경로가 존재하는지
bool Path::ExistDirectory(const string& path)
{

	return ExistDirectory(String::ToWstring(path));;
}

bool Path::ExistDirectory(const wstring& path)
{
	DWORD attribute = GetFileAttributes(path.c_str());
	BOOL temp = (attribute != INVALID_FILE_ATTRIBUTES && (attribute & FILE_ATTRIBUTE_DIRECTORY));

	return temp == TRUE;
}

string Path::Combine(const string& path1, const string& path2)
{
	return path1 + path2;
}

wstring Path::Combine(const wstring& path1, const wstring& path2)
{
	return path1 + path2;
}

string Path::Combine(vector<string>& paths)
{
	string temp = "";
	for (string path : paths)
		temp += path;

	return temp;
}

wstring Path::Combine(vector<wstring>& paths)
{
	wstring temp = L"";
	for (wstring path : paths)
		temp += path;

	return temp;
}

//파일이름 떼고 경로만 받아오기
string Path::GetDirectoryName(string path)
{
	String::Replace(path, "\\", "/");
	size_t index = path.find_last_of('/');

	return path.substr(0, index + 1);
}

wstring Path::GetDirectoryName(wstring path)
{
	String::Replace(path, L"\\", L"/");
	size_t index = path.find_last_of('/');

	return path.substr(0, index + 1);
}

string Path::GetExtension(string path)
{
	String::Replace(path, "\\", "/");
	size_t index = path.find_last_of('.');

	return path.substr(index + 1, path.length());
}

wstring Path::GetExtension(wstring path)
{
	String::Replace(path, L"\\", L"/");
	size_t index = path.find_last_of('.');

	return path.substr(index + 1, path.length());
}

string Path::GetFileName(string path)
{
	String::Replace(path, "\\", "/");
	size_t index = path.find_last_of('/');

	return path.substr(index + 1, path.length());
}

wstring Path::GetFileName(wstring path)
{
	String::Replace(path, L"\\", L"/");
	size_t index = path.find_last_of('/');

	return path.substr(index + 1, path.length());
}

string Path::GetFileNameWithoutExtension(const string& path)
{
	string fileName = GetFileName(path);
	size_t index = fileName.find_last_of('.');

	return fileName.substr(0, index);
}

wstring Path::GetFileNameWithoutExtension(const wstring& path)
{
	wstring fileName = GetFileName(path);
	size_t index = fileName.find_last_of('.');

	return fileName.substr(0, index);
}

const WCHAR* Path::ImageFilter = L"Image\0*.png; *.bmp; *.jpg";
const WCHAR* Path::ShaderFilter = L"HLSL File\0*.hlsl";
const WCHAR* Path::TextFilter = L"Text File\0*.txt";
const WCHAR* Path::SoundFilter = L"Sound File\0*.mp3;*.wav;*.flac";
const WCHAR* Path::TileMapFilter = L"TileMap File\0*.tile";			//우리가 만든 확장자!

//대화상자 열기
void Path::OpenFileDialog(const wstring& file, const WCHAR* filter, const wstring& folder, function<void(wstring)>& func, const HWND& hwnd)
{
	WCHAR name[255];
	wcscpy_s(name, file.c_str());
	
	wstring tempFolder = folder;
	String::Replace(tempFolder, L"/", L"\\");

	//파일의 정보를 담을 구조체
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = name; // 파일명
	ofn.lpstrTitle = L"로드"; //창 제목표시줄 이름
	ofn.nMaxFile = 255; //파일명 최대 몇글자까지 받아올 수 있는지
	ofn.lpstrInitialDir = tempFolder.c_str(); //초기경로 - 불러오기를 불렀을때 가장먼저 보일 경로
	ofn.Flags = OFN_NOCHANGEDIR; //대화상자 이니셜 디렉토리 변동하지 않음


	if (GetOpenFileName(&ofn) == TRUE) //true를 쓰면 경고가 나와서 대문자 TRUE사용 -옛날기능이라 그렇다
	{
		if (func != nullptr)
		{
#pragma warning(disable : 6054)
			wstring loadName = name;
			String::Replace(loadName, L"\\", L"/");
			func(loadName);
#pragma warning(default : 6054)
		}
	}
}

void Path::SaveFileDialog(const wstring& file, const WCHAR* filter, const wstring& folder, function<void(wstring)>& func, const HWND& hwnd)
{
	WCHAR name[255];
	wcscpy_s(name, file.c_str());

	wstring tempFolder = folder;
	String::Replace(tempFolder, L"/", L"\\");

	//파일의 정보를 담을 구조체 - 세부적인 내용 한번 확인해보기
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = name; // 파일명
	ofn.lpstrTitle = L"세이브"; //창 제목표시줄 이름
	ofn.nMaxFile = 255; //파일명 최대 몇글자까지 받아올 수 있는지
	ofn.lpstrInitialDir = tempFolder.c_str(); //초기경로 - 불러오기를 불렀을때 가장먼저 보일 경로
	ofn.Flags = OFN_NOCHANGEDIR; //대화상자 이니셜 디렉토리 변동하지 않음

	//기본확장자 지정(저장당시 확장자를 입력하지않아도 자동 지정된 확장자로 저장한다)
	if (filter == TextFilter)
		ofn.lpstrDefExt = L".txt";
	else if (filter == ShaderFilter)
		ofn.lpstrDefExt = L".hlsl"; //이펙트있으면 fx도 가능하다
	else if (filter == TileMapFilter)
		ofn.lpstrDefExt = L".tile";

	if (GetSaveFileName(&ofn) == TRUE) //true를 쓰면 경고가 나와서 대문자 TRUE사용 -옛날기능이라 그렇다
	{
		if (func != nullptr)
		{
#pragma warning(disable : 6054)
			wstring saveName = name;
			String::Replace(saveName, L"\\", L"/");
			func(saveName);
#pragma warning(default : 6054)
		}
	}
}

//파일들의 정보를 가져올 함수
void Path::GetFiles(vector<string>& files, const string& path, const string& filter, const bool& bFindSubFolder)
{
	vector<wstring> wFiles;
	wstring wPath = String::ToWstring(path);
	wstring wFilter = String::ToWstring(filter);

	GetFiles(wFiles, wPath, wFilter, bFindSubFolder);
	for (const auto& str : wFiles)
		files.push_back(String::ToString(str));
}

void Path::GetFiles(vector<wstring>& files, const wstring& path, const wstring& filter, const bool& bFindSubFolder)
{
	wstring file = path + filter;

	WIN32_FIND_DATA findData;
	//첫번째로 찾아진 파일의 경로값이 findData로 들어가고 그걸 컨트롤하기 위한 핸들은 더이상 안찾아질때까지 열려있는다
	HANDLE handle = FindFirstFile(file.c_str(), &findData);
	//path랑 filter를 넣은 파일경로로 파일 찾기
	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				if (bFindSubFolder == true && findData.cFileName[0] != '.')
				{
					//파일을 찾았는데 폴더안에 서브폴더 라면, 이름이 .으로 시작하지 않는다면
					wstring folder = path + wstring(findData.cFileName) + L"/";
					//또 그안에 하위폴더가 있다면 재귀구조로 끝까지 타고 들어간다
					GetFiles(files, folder, filter, bFindSubFolder);
				}
			}
			else
			{
				//폴더가 아니라면
				wstring fileName = path + wstring(findData.cFileName);
				files.push_back(fileName);
			}
		} while (FindNextFile(handle, &findData)); //FindNextFile로 더이상 찾을 파일이 없다면 do/while 을 빠져나온다
		
		FindClose(handle);
		//핸들 사용완료 후 반드시 닫아주기
	}
}

void Path::CreateFolder(const string& path)
{
	CreateFolder(String::ToWstring(path));
}

void Path::CreateFolder(const wstring& path)
{
	//해당 경로에 폴더가 존재한다면 만들필요가 없다
	if (ExistDirectory(path) == false)
		CreateDirectory(path.c_str(), nullptr);
}

void Path::CreateFolders(const string& path)
{
	CreateFolders(String::ToWstring(path));
}

void Path::CreateFolders(wstring path)
{
	//여러개의 폴더를 한번에 만든다
	//path안에서 하위폴더들이 존재하게 될경우
	//존재하는 경로는 내비두고 존재하지 않는 경로만 생성해주겠다
	String::Replace(path, L"\\", L"/");

	vector<wstring> folders;
	String::SplitString(folders, path, L"/");

	wstring temp = L"";
	for (const auto& folder : folders)
	{
		temp += folder + L"/";
		CreateFolder(temp);
	}
}
