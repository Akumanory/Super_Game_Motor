#include <motor/other/Logs.h>
#include <motor/other/StringConverter.h>

#include <iostream>
#include <ctime>
#include <comdef.h>

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    return buf;
}

void Logs::Debug(std::string log_str)
{
	std::cout << "[ " << currentDateTime() << " ] "<< " (Debug) " << log_str << std::endl;
}

void Logs::Error(std::string log_str)
{
    std::cout << "[ " << currentDateTime() << " ] " << " (Error) " << log_str << std::endl;
}

void Logs::Debug(std::wstring log_str)
{
    std::cout << "[ " << currentDateTime() << " ] " << " (Debug) ";
    std::wcout << log_str.c_str();
    std::cout << std::endl;
}

void Logs::Error(std::wstring log_str)
{
    std::cout << "[ " << currentDateTime() << " ] " << " (Error) "; 
    std::wcout << log_str.c_str();
    std::cout << std::endl;
}

void Logs::Error(HRESULT hr, std::string log_str)
{
    _com_error error(hr);

    std::wstring error_message = L"[ " + StringConverter::StringToWide(currentDateTime()) + L" ] " + L"\n" + StringConverter::StringToWide(log_str) + L"\n" + error.ErrorMessage();

    std::cout << "[ " << currentDateTime() << " ] " << " (Error) ";
    std::cout << log_str.c_str();
    std::cout << std::endl;
    MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}

void Logs::Error(HRESULT hr, std::wstring log_str)
{
    _com_error error(hr);

    std::wstring error_message = L"[ " + StringConverter::StringToWide(currentDateTime()) + L" ] "+ L"\n" + log_str + L"\n" + error.ErrorMessage();

    std::cout << "[ " << currentDateTime() << " ] " << " (Error) ";
    std::wcout << log_str.c_str(); 
    std::cout << std::endl;
    MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}

void Logs::Error(COMException& ex)
{
    std::wstring error_msg = ex.what();
    std::cout << "[ " << currentDateTime() << " ] " << " (Error) ";
    std::wcout << error_msg.c_str();
    std::cout << std::endl;
    MessageBoxW(NULL, error_msg.c_str(), L"Error", MB_ICONERROR);
}
