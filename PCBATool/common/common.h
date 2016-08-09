#ifndef _COMMON_H_
#define _COMMON_H_
#include <string>
#define UPDATE_LIST         3
#define LIST_EMPTY          4


#define UPDATE_PROMPT       5
#define PROMPT_TESTING      1
#define PROMPT_PASS         2
#define PROMPT_FAIL         3
#define PROMPT_ABORT        4
#define PROMPT_EMPTY        5


#define LIST_INFO 0
#define LIST_TIME 1
#define LIST_WARN 2
#define LIST_ERR  3
typedef enum
{
	PORT_GTI = 0,
	PORT_AT,
	PORT_OCT,
	PORT_CORE_DUMP,
    PORT_PORT_NUM
}USB_PORT_TYPE;

typedef struct _PORTNAME_
{
    const TCHAR *PortName1;
    const TCHAR *PortName2;
}PORTNAME;

class INTELCOMPORT
{
public:
    void clear()
    {
        GtiPort.clear();
        AtPort.clear();
        OctPort.clear();
        ImcPort.clear();
        for(int i = 0; i < PORT_PORT_NUM ;i ++) {
            IntelPort[i].clear();

        }
    }
    std::wstring GtiPort;
    std::wstring AtPort;
    std::wstring OctPort;
    std::wstring ImcPort;
    std::wstring IntelPort[PORT_PORT_NUM];
};

void          RegDevNotification(HWND hWnd);
bool          CheckIntelUSBComPort(INTELCOMPORT *ports);
unsigned char hexstr2ushort(const char * str,int len);
typedef struct  
{
    int flag;
    TCHAR   pszLineText[MAX_PATH];
}STRUCT_LIST_LINE,*PSTRUCT_LIST_LINE;
#endif