#include "stdafx.h"
#include "Hf_LPB100_Dialog.h"
#include "resource.h"
#include "SerialPortControl.h"

#pragma warning(disable: 4996 )

#if 0
BEGIN_MESSAGE_MAP(WifiTestProc, CDialog)
ON_WM_NCHITTEST()
END_MESSAGE_MAP()
#endif

const int infoTimerId = 101;
const int info_elapse = 300;


#define RECEIVE_FILE_PATH (".\\testdata.txt")

const UINT uIDEventTime = 3001;
const UINT uElapseTime = 300; //ms

#define RECVBUFMAX  512

#define INI_FILE_NAME  ("BarcodeInput.ini")
//#define LOG_FILE_NAME	".\\BarCodeInputtingLog.csv"

WifiTestProc::WifiTestProc()
{

}

WifiTestProc::~WifiTestProc()
{

}

BOOL CALLBACK WifiTestProc::s_DlgProc(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam) 
{
    WifiTestProc* pWifiTestProc = NULL;

    if( WM_INITDIALOG == wMsg )
    {
        SetWindowLong(hWnd,DWL_USER,(LONG)NULL);
        pWifiTestProc = new WifiTestProc  ;
        SetWindowLong(hWnd,DWL_USER,(LONG)pWifiTestProc);
    }else
    {
        pWifiTestProc = (WifiTestProc*)::GetWindowLong(hWnd,DWL_USER);
    }

    int Result = 0 ;
    if (pWifiTestProc != NULL)
    {
        Result = pWifiTestProc->DlgProc(hWnd,wMsg,wParam,lParam);
    }
    else
    {
        Result = 0;
    }

    if( WM_DESTROY == wMsg )
    {
        delete pWifiTestProc;
        pWifiTestProc = NULL;
        SetWindowLong(hWnd,DWL_USER,(LONG)NULL);
    }

    return Result;
}


LRESULT WifiTestProc::DlgProc(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam)
{
    switch (wMsg)
    {
    case WM_INITDIALOG:
        return OnInitDialog(hWnd,wMsg,wParam,lParam);
    case WM_TIMER:
        return OnTimer(hWnd,wMsg,wParam,lParam);
    case WM_COMMAND:
        return OnCommand(hWnd,wMsg,wParam,lParam);
    case WM_CTLCOLORDLG:
        return OnCtlColorDlg( hWnd, wMsg,wParam,lParam);
    case WM_CLOSE:
        return OnClose(hWnd,wMsg,wParam,lParam);
    case WM_DESTROY:
        return OnDestroy(hWnd,wMsg,wParam,lParam);
    case WM_PAINT:
        return OnPaint(hWnd,wMsg,wParam,lParam);
	case WM_KEYDOWN:
		return OnChar(hWnd,wMsg,wParam,lParam);
	case COM_RECVDATA:
		return OnRecvData(hWnd, wMsg, wParam, lParam);
    }
    return 0;
}

LRESULT WifiTestProc::OnInitDialog( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam )
{
	char szString_c[64] = {0};

	m_setup_btn_state = 0;
	//read ini file
	ReadINIFile(hWnd,wMsg,wParam,lParam);
	// combo com port
	m_combo_com.comboBox_Init(IDC_COMBO_COM);
	m_combo_com.comboBox_add_com_range_string(hWnd, 0, 1, 50);
	m_combo_com.comboBox_selet_string(hWnd, 3, 0);

	//m_combo_com.comboBox_get_string(hWnd, -1, szString_c);
	//SendDlgListboxMessage(hWnd, IDC_LIST_MSG, szString_c);
	
	m_combo_rate.comboBox_Init(IDC_COMBO_RATE);
	m_combo_rate.comboBox_add_string(hWnd, 0, "110");
	m_combo_rate.comboBox_add_string(hWnd, 0, "300");
	m_combo_rate.comboBox_add_string(hWnd, 0, "600");
	m_combo_rate.comboBox_add_string(hWnd, 0, "1200");
	m_combo_rate.comboBox_add_string(hWnd, 0, "2400");
	m_combo_rate.comboBox_add_string(hWnd, 0, "4800");
	m_combo_rate.comboBox_add_string(hWnd, 0, "9600");
	m_combo_rate.comboBox_add_string(hWnd, 0, "14400");
	m_combo_rate.comboBox_add_string(hWnd, 0, "19200");
	m_combo_rate.comboBox_add_string(hWnd, 0, "38400");
	m_combo_rate.comboBox_add_string(hWnd, 0, "57600");
	m_combo_rate.comboBox_add_string(hWnd, 0, "115200");
	m_combo_rate.comboBox_add_string(hWnd, 0, "230400");
	m_combo_rate.comboBox_add_string(hWnd, 0, "380400");
	m_combo_rate.comboBox_add_string(hWnd, 0, "460800");
	m_combo_rate.comboBox_add_string(hWnd, 0, "921600");
	m_combo_rate.comboBox_selet_string(hWnd, 11, 0);


	m_combo_data_bits.comboBox_Init(IDC_COMBO_DATA_BITS);
	m_combo_data_bits.comboBox_add_string(hWnd, 0, "5");
	m_combo_data_bits.comboBox_add_string(hWnd, 0, "6");
	m_combo_data_bits.comboBox_add_string(hWnd, 0, "7");
	m_combo_data_bits.comboBox_add_string(hWnd, 0, "8");
	m_combo_data_bits.comboBox_selet_string(hWnd, 3, 0);


	m_combo_stop_bits.comboBox_Init(IDC_COMBO_STOP_BITS);
	m_combo_stop_bits.comboBox_add_string(hWnd, 0, "1");
	m_combo_stop_bits.comboBox_add_string(hWnd, 0, "1.5");
	m_combo_stop_bits.comboBox_add_string(hWnd, 0, "2");
	m_combo_stop_bits.comboBox_selet_string(hWnd, 0, 0);

#if 0
	// 打开串口.
	int err = 0;
	char szErrcode[64] = { 0 };
	err = OpenSerialPort1("COM16", 115200, 8, 1);
	if (err != 0)
	{
		sprintf(szErrcode, "Open serial Fail: %d", err);
		SendDlgListboxMessage(hWnd, IDC_LIST_MSG, szErrcode);
	}
	//OpenSerialPort1();
	// TODO: Add extra initialization here
	//启动串口监视线程

	DWORD threadID;
	hCommThread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0,
		(LPTHREAD_START_ROUTINE)SerialPort1ThreadProcess,
		hWnd, 0, &threadID);
	if (hCommThread == NULL)
	{
		//::AfxMessageBox("创建串口1处理线程失败");
		//::PostQuitMessage(0);
	}
	return TRUE; // return TRUE unless you set the focus to a control
#endif
	
	return 0;
}

LRESULT WifiTestProc::OnDestroy( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam )
{

    return 0;
}

LRESULT WifiTestProc::OnTimer( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam )
{

    return 0;
}

LRESULT WifiTestProc::OnCommand( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam )
{
    switch (LOWORD(wParam))
    {
		case IDOK:
		{	
		    char szMacText[64] = { 0 };
			char szCommand[512] = { 0 };
			char * buf;
			char *delim = ",";
			int returncode = 0;

			GetDlgItemText(hWnd, IDC_EDIT_MAC, szMacText, 64);
			SetDlgItemText(hWnd, IDC_EDIT_MAC, "");
			returncode = macaddress_parse(szMacText, szCommand);
			if (returncode == -1)
			{
				SendDlgListboxMessage(hWnd, IDC_LIST_MSG, "MAC address format is not correct!" );
				break;
			}
			else if (returncode == -2)
			{
				SendDlgListboxMessage(hWnd, IDC_LIST_MSG, "MAC address format is not correct!");
				break;
			}
#if 0
			buf = strtok(szCommand, delim);
			while (buf != NULL)
			{
				strcpy(szMacText, buf);
				SerialPortWrite(szMacText, strlen(szMacText));
				buf = strtok(NULL, delim);
			}
#else
			if ((szCommand != NULL) && (strlen(szCommand)>0))
			SerialPortWrite(szCommand, strlen(szCommand));
#endif
			
			break;
		}
		case IDCANCEL:
		{
			ReleaseSerialPort();
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
			
			break;
		}
		case IDC_BUTTON_COM_SETUP:
		{
			char szComPort[32] = { 0 };
			char szComPort_tmp[32] = { 0 };

			char szBaudRate[32] = { 0 };
			char szBaudRate_tmp[32] = { 0 };
			int vBaudRate = 0;

			char szDataBits[32] = { 0 };
			char szDataBits_tmp[32] = { 0 };
			int vDataBits = 0;

			char szStopBits[32] = { 0 };
			char szStopBits_tmp[32] = { 0 };
			int vStopBits = 0;
			int retErr = 0;
			if (m_setup_btn_state == 0)
			{
				m_combo_com.comboBox_get_string(hWnd, -1, szComPort);
				sprintf(szComPort_tmp, "current serial Port: %s", szComPort);
				SendDlgListboxMessage(hWnd, IDC_LIST_MSG, szComPort_tmp);

				m_combo_rate.comboBox_get_string(hWnd, -1, szBaudRate);
				sprintf(szBaudRate_tmp, "baud rate: %s", szBaudRate);
				SendDlgListboxMessage(hWnd, IDC_LIST_MSG, szBaudRate_tmp);
				m_combo_rate.comboBox_get_Data_int(hWnd, -1, &vBaudRate);

				m_combo_data_bits.comboBox_get_string(hWnd, -1, szDataBits);
				sprintf(szDataBits_tmp, "Data bits: %s", szDataBits);
				SendDlgListboxMessage(hWnd, IDC_LIST_MSG, szDataBits_tmp);
				m_combo_data_bits.comboBox_get_Data_int(hWnd, -1, &vDataBits);

				m_combo_stop_bits.comboBox_get_string(hWnd, -1, szStopBits);
				sprintf(szStopBits_tmp, "Data bits: %s", szStopBits);
				SendDlgListboxMessage(hWnd, IDC_LIST_MSG, szStopBits_tmp);
				m_combo_stop_bits.comboBox_get_Data_int(hWnd, -1, &vStopBits);

				retErr = OpenSerialPort1(szComPort, vBaudRate, vDataBits, vStopBits);
				if (retErr == 0)
				{
					m_setup_btn_state = 1;
					SetDlgItemText(hWnd, IDC_BUTTON_COM_SETUP, "停止");
					SendDlgListboxMessage(hWnd, IDC_LIST_MSG, "Open serial port success!");
				}
				else
				{
					SendDlgListboxMessage(hWnd, IDC_LIST_MSG, "Open serial port Fail!");
					SendDlgListboxMessage(hWnd, IDC_LIST_MSG, "please check settings!");
				}

				if (retErr == 0)
				{
					//启动串口监视线程

					DWORD threadID;
					hCommThread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0,
						(LPTHREAD_START_ROUTINE)SerialPort1ThreadProcess,
						hWnd, 0, &threadID);
					if (hCommThread == NULL)
					{
						SendDlgListboxMessage(hWnd, IDC_LIST_MSG, "Serial port monitor thread create fail!");
						SendDlgListboxMessage(hWnd, IDC_LIST_MSG, "check why!!!!!!!!!!!!!");
						//SendDlgListboxMessage(hWnd, IDC_LIST_MSG, "please check settings!");
					}
					else
					{
						SendDlgListboxMessage(hWnd, IDC_LIST_MSG, "start to get Message from serial port!");
					}
				}
			}
			else if (m_setup_btn_state == 1)
			{
				m_setup_btn_state = 0;
				SetDlgItemText(hWnd, IDC_BUTTON_COM_SETUP, "设定");
				SendDlgListboxMessage(hWnd, IDC_LIST_MSG, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
				SendDlgListboxMessage(hWnd, IDC_LIST_MSG, "serial port is release!");
				SendDlgListboxMessage(hWnd, IDC_LIST_MSG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
				ReleaseSerialPort();
			}
		}

    }

    return 0;
}

LRESULT WifiTestProc::OnCtlColorDlg( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam )
{		
    return 0;
}

LRESULT WifiTestProc::OnClose( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam )
{
    return 0;
}

LRESULT WifiTestProc::OnPaint( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam )
{
    return 0;
}

LRESULT WifiTestProc::OnChar( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam )
{
	char ascii_code = wParam; //获取按下的ASCII码
	unsigned int key_state = lParam;
    return 0;
}

LRESULT WifiTestProc::OnRecvData(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	char * buf;
	char *delim = "\r\n";
	buf = strtok((char*)wParam, delim);
	while (buf != NULL)
	{
		SendDlgListboxMessage(hWnd, IDC_LIST_MSG, buf);
		buf = strtok(NULL, delim);
	}
	
	return 0;
}

LRESULT WifiTestProc::OnStartStop( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam )
{
    return 0;
}

void WifiTestProc::SendDlgListboxMessage(HWND hWnd, int listboxID, TCHAR* szString)
{
#if 1
	long lNumber;
	SendDlgItemMessage(hWnd, listboxID, LB_ADDSTRING, NULL, reinterpret_cast<LPARAM>(szString));	//打印字串到listbox
	lNumber = SendDlgItemMessage(hWnd, listboxID, LB_GETCOUNT, NULL, NULL);						//获取listbox中字串数目
	SendDlgItemMessage(hWnd, listboxID, LB_SETCURSEL, lNumber - 1, 0);							//使本字串反选, 同时可以使字串在listbox的可见区域.
	SendDlgItemMessage(hWnd, listboxID, LB_SETCURSEL, -1, 0);
	//UpdateData(true);
#endif//取消所有反选.

}

BOOL WifiTestProc::SendData( HWND hWnd )
{
    return TRUE;
}

LRESULT WifiTestProc::OnSend( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam )
{

    return 0;
}

LRESULT WifiTestProc::OnSendSn( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam )
{

    return 0;
}



LRESULT WifiTestProc::OnReset( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam )
{
	return 0;
}


LRESULT WifiTestProc::OnReceive( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam )
{
    return 0;
}

void WifiTestProc::ReadINIFile(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam)
{
}

void WifiTestProc::WriteINIFile()
{
}

LRESULT WifiTestProc::CreateExcelFile( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam)
{

	return 0;
}

BOOL WifiTestProc::PrintLogToExcelFile()
{
	return 0;
}


int WifiTestProc::OnComPortButton_click()
{
	return 0;
}

int WifiTestProc::macaddress_parse(char *pmac, char *pRetCommand)
{
	char szTmp[64] = { 0 };
	if (strlen(pmac) != 12)
	{
		return -1;
	}
	for (int i = 0; i < strlen(pmac); i++)
	{
		if (pmac[i]>='a' && pmac[i] <= 'f')
		{
			continue;
		}
		else if (pmac[i]>='A' && pmac[i] <= 'F')
		{
			continue;
		}
		else if (pmac[i]>='0' && pmac[i] <= '9')
		{
			continue;
		}
		else
		{
			return -2;
		}
	}
	strcpy(pRetCommand, "");
#if 0
	sprintf(pRetCommand, "%s%c%c,", "AT#FLASH -s0x17004 -v0x", pmac[0], pmac[1]);

	sprintf(szTmp, "%s%c%c,", "AT#FLASH -s0x17005 -v0x", pmac[2], pmac[3]);
	strcat(pRetCommand, szTmp);

	sprintf(szTmp, "%s%c%c,", "AT#FLASH -s0x17006 -v0x", pmac[4], pmac[5]);
	strcat(pRetCommand, szTmp);

	sprintf(szTmp, "%s%c%c,", "AT#FLASH -s0x17007 -v0x", pmac[6], pmac[7]);
	strcat(pRetCommand, szTmp);

	sprintf(szTmp, "%s%c%c,", "AT#FLASH -s0x17008 -v0x", pmac[8], pmac[9]);
	strcat(pRetCommand, szTmp);

	sprintf(szTmp, "%s%c%c", "AT#FLASH -s0x17009 -v0x", pmac[10], pmac[11]);
	strcat(pRetCommand, szTmp);
#else
	sprintf(pRetCommand, "%s%c%c\r\n", "AT#FLASH -s0x17004 -v0x", pmac[0], pmac[1]);

	sprintf(szTmp, "%s%c%c\r\n", "AT#FLASH -s0x17005 -v0x", pmac[2], pmac[3]);
	strcat(pRetCommand, szTmp);

	sprintf(szTmp, "%s%c%c\r\n", "AT#FLASH -s0x17006 -v0x", pmac[4], pmac[5]);
	strcat(pRetCommand, szTmp);

	sprintf(szTmp, "%s%c%c\r\n", "AT#FLASH -s0x17007 -v0x", pmac[6], pmac[7]);
	strcat(pRetCommand, szTmp);

	sprintf(szTmp, "%s%c%c\r\n", "AT#FLASH -s0x17008 -v0x", pmac[8], pmac[9]);
	strcat(pRetCommand, szTmp);

	sprintf(szTmp, "%s%c%c", "AT#FLASH -s0x17009 -v0x", pmac[10], pmac[11]);
	strcat(pRetCommand, szTmp);
#endif
	return 0;
}
