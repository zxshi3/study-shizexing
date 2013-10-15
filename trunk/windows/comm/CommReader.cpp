#include "CommReader.h"
#include <windows.h>
#include <iostream>

#define LOG(ERROR) std::cerr
// http://www.codeproject.com/Articles/2682/Serial-Communication-in-Windows
// http://msdn.microsoft.com/en-us/library/aa363424(v=vs.85).aspx

static std::string readComSync() {
    //const char * devName = "COM12";           // not working
    const char * devName = "\\\\.\\COM12";
    ::HANDLE hCom = ::CreateFileA(devName
        , GENERIC_READ //, GENERIC_READ | GENERIC_WRITE
        , 0
        , 0
        , OPEN_EXISTING
        , 0 // FILE_FLAG_OVERLAPPED
        , 0);
    if (INVALID_HANDLE_VALUE == hCom) {
        LOG(ERROR) << "UBlox6LocationProvider : open " << devName << " fail. errno : " << GetLastError() << ". quit\n";
        return std::string("");
    }

    // set timeout
    COMMTIMEOUTS timeout;
    ::GetCommTimeouts(hCom, &timeout);
    {
        std::cout << "ReadIntervalTimeout : " << timeout.ReadIntervalTimeout << std::endl
            << "ReadTotalTimeoutMultiplier : " << timeout.ReadTotalTimeoutMultiplier << std::endl
            << "ReadTotalTimeoutConstant : " << timeout.ReadTotalTimeoutConstant << std::endl
            << "WriteTotalTimeoutMultiplier : " << timeout.WriteTotalTimeoutMultiplier << std::endl
            << "WriteTotalTimeoutConstant : " << timeout.WriteTotalTimeoutConstant << std::endl;
    }
    timeout.ReadIntervalTimeout = 100;      // 100ms
    ::SetCommTimeouts(hCom, &timeout);      // if we do not set timeout, ReadFile will try to read nNumberOfBytesToRead

    // set parameter
    DCB hDCB = {0};   // Device control block?
    hDCB.DCBlength = sizeof(DCB);
    /*
    typedef struct _DCB {
      DWORD DCBlength;
      DWORD BaudRate;
      DWORD fBinary  :1;
      DWORD fParity  :1;
      DWORD fOutxCtsFlow  :1;
      DWORD fOutxDsrFlow  :1;
      DWORD fDtrControl  :2;
      DWORD fDsrSensitivity  :1;
      DWORD fTXContinueOnXoff  :1;
      DWORD fOutX  :1;
      DWORD fInX  :1;
      DWORD fErrorChar  :1;
      DWORD fNull  :1;
      DWORD fRtsControl  :2;
      DWORD fAbortOnError  :1;
      DWORD fDummy2  :17;
      WORD  wReserved;
      WORD  XonLim;
      WORD  XoffLim;
      BYTE  ByteSize;
      BYTE  Parity;
      BYTE  StopBits;
      char  XonChar;
      char  XoffChar;
      char  ErrorChar;
      char  EofChar;
      char  EvtChar;
      WORD  wReserved1;
    } DCB, *LPDCB;
    */
    ::GetCommState(hCom, &hDCB);
    {
        std::cout << "baud rate : " << hDCB.BaudRate << std::endl
            << "byte size : " << (int)hDCB.ByteSize << std::endl
            << "stop bits : " << (int)hDCB.StopBits << std::endl
            << "parity : " << (int)hDCB.Parity << std::endl;
    }

    ::PurgeComm( hCom, PURGE_RXCLEAR );     // Discards all characters from the output or input buffer of a specified communications resource.

    char buf[4096] = {0};
    DWORD nRead = 0;
    BOOL bRet = ::ReadFile(hCom, &buf[0], sizeof(buf) - 1, &nRead, NULL);
    if (bRet == FALSE) {
        std::cerr << "read from comm fail\n";
    } else {
        std::cout << "read " << nRead << " bytes from " << devName << std::endl;
        buf[nRead] = '\0';
        std::cout << "content = " << buf << std::endl;
    }

    if (hCom != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(hCom);
    }
    return std::string(buf);
}

static std::string readComASync() {
    //const char * devName = "COM12";           // not working
    const char * devName = "\\\\.\\COM12";
    ::HANDLE hCom = ::CreateFileA(devName
        , GENERIC_READ //, GENERIC_READ | GENERIC_WRITE
        , 0
        , 0
        , OPEN_EXISTING
        ,FILE_FLAG_OVERLAPPED   // , 0 // FILE_FLAG_OVERLAPPED
        , 0);
    if (INVALID_HANDLE_VALUE == hCom) {
        LOG(ERROR) << "UBlox6LocationProvider : open " << devName << " fail. errno : " << GetLastError() << ". quit\n";
        return std::string("");
    }

    // set timeout
    COMMTIMEOUTS timeout;
    ::GetCommTimeouts(hCom, &timeout);
    {
        std::cout << "ReadIntervalTimeout : " << timeout.ReadIntervalTimeout << std::endl
            << "ReadTotalTimeoutMultiplier : " << timeout.ReadTotalTimeoutMultiplier << std::endl
            << "ReadTotalTimeoutConstant : " << timeout.ReadTotalTimeoutConstant << std::endl
            << "WriteTotalTimeoutMultiplier : " << timeout.WriteTotalTimeoutMultiplier << std::endl
            << "WriteTotalTimeoutConstant : " << timeout.WriteTotalTimeoutConstant << std::endl;
    }
    timeout.ReadIntervalTimeout = 100;      // 100ms
    ::SetCommTimeouts(hCom, &timeout);      // if we do not set timeout, ReadFile will try to read nNumberOfBytesToRead

    // set parameter
    DCB hDCB = {0};   // Device control block?
    hDCB.DCBlength = sizeof(DCB);
    /*
    typedef struct _DCB {
      DWORD DCBlength;
      DWORD BaudRate;
      DWORD fBinary  :1;
      DWORD fParity  :1;
      DWORD fOutxCtsFlow  :1;
      DWORD fOutxDsrFlow  :1;
      DWORD fDtrControl  :2;
      DWORD fDsrSensitivity  :1;
      DWORD fTXContinueOnXoff  :1;
      DWORD fOutX  :1;
      DWORD fInX  :1;
      DWORD fErrorChar  :1;
      DWORD fNull  :1;
      DWORD fRtsControl  :2;
      DWORD fAbortOnError  :1;
      DWORD fDummy2  :17;
      WORD  wReserved;
      WORD  XonLim;
      WORD  XoffLim;
      BYTE  ByteSize;
      BYTE  Parity;
      BYTE  StopBits;
      char  XonChar;
      char  XoffChar;
      char  ErrorChar;
      char  EofChar;
      char  EvtChar;
      WORD  wReserved1;
    } DCB, *LPDCB;
    */
    ::GetCommState(hCom, &hDCB);
    {
        std::cout << "baud rate : " << hDCB.BaudRate << std::endl
            << "byte size : " << (int)hDCB.ByteSize << std::endl
            << "stop bits : " << (int)hDCB.StopBits << std::endl
            << "parity : " << (int)hDCB.Parity << std::endl;
    }

    ::SetCommMask(hCom, EV_RXCHAR); // A character was received and placed in the input buffer. ***

    // clear previous buffer
    ::PurgeComm( hCom, PURGE_RXCLEAR );     // Discards all characters from the output or input buffer of a specified communications resource.

    OVERLAPPED ov = {0};
    ov.hEvent = CreateEvent(0, TRUE, 0, 0);

    char buf[4096] = {0};
    for (int i = 0; i < 100; i++) {
        std::cout << "reading.... " << std::endl;
        DWORD dwEvtMask;
        BOOL bRet = ::WaitCommEvent(hCom, &dwEvtMask, &ov);
        if (bRet == FALSE) {
            DWORD en = ::GetLastError();
            if (en != ERROR_IO_PENDING) {
                std::cerr << "************ ERROR occur while wait comm event. errno = " << en << std::endl;
                return std::string("");
            }
            std::cout << "try again" << std::endl;
            std::cout << "Waiting...." << std::endl;
            DWORD dwWait = ::WaitForSingleObject(ov.hEvent, 1000);
            if (dwWait == WAIT_OBJECT_0) {
                DWORD nRead = 0;
                bRet = ::ReadFile(hCom, &buf[0], sizeof(buf) - 1, &nRead, &ov);
                if (bRet == FALSE) {
                    en = ::GetLastError();
                    if (en == ERROR_IO_PENDING) {
                        std::cout << "wait........." << std::endl;
                        dwWait = ::WaitForSingleObject(ov.hEvent, 1000);
                        if (dwWait == WAIT_OBJECT_0) {
                            std::cout << "zzzz" << std::endl;
                            BOOL bRet3 = GetOverlappedResult(hCom, &ov, &nRead, FALSE);
                            std::cout << "async read " << nRead << " bytes" << std::endl;
                            buf[nRead] = '\0';
                            std::cout << "content = " << buf << std::endl;
                        }
                    } else {
                        std::cerr << "read from comm fail. errno = " << en << std::endl;
                        if (en == ERROR_INVALID_PARAMETER) {
                            std::cerr << "... invalid parameter ..." << std::endl;
                        }
                    }
                } else {
                    std::cout << "read " << nRead << " bytes from " << devName << std::endl;
                    buf[nRead] = '\0';
                    std::cout << "content = " << buf << std::endl;
                }
            } else {
                std::cerr << "dwWait = " << dwWait << std::endl;
            }
        } else {
            std::cout << "dwEvtMask = " << dwEvtMask << ". expect = " << EV_RXCHAR << std::endl;
            // char buf[4096] = {0};
            DWORD nRead = 0;
            bRet = ::ReadFile(hCom, &buf[0], sizeof(buf) - 1, &nRead, NULL);
            if (bRet == FALSE) {
                std::cerr << "read from comm fail\n";
            } else {
                std::cout << "read " << nRead << " bytes from " << devName << std::endl;
                buf[nRead] = '\0';
                std::cout << "content = " << buf << std::endl;
            }
        }
    }

    if (hCom != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(hCom);
    }
    return std::string(buf);
}

std::string readCom() {
    return readComASync();
}
