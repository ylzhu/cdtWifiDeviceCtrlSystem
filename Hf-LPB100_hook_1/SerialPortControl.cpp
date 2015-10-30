#include "StdAfx.h"
#include <windows.h>
#include <stdio.h>
#include "SerialPortControl.h"

HANDLE hCom; //全局变量，串口句柄
HANDLE hCommThread; //全局变量，串口线程

int OpenSerialPort1(char *szPort , int baudRate, int data_bits, int stop_bits)
{
	//打开并设置COM1
	int err =0;
	char szString_port[32] = { 0 };
	err = GetLastError();
	if (strlen(szPort) > 4)
	{
		sprintf(szString_port, "\\\\.\\%s", szPort);
	}
	else
	{
		strcpy(szString_port, szPort);
	}
	hCom = ::CreateFile(szString_port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	err = GetLastError();
	if (hCom == (HANDLE)-1)
	{
		//MessageBox("打开COM1失败");
		return err;
	}
	else
	{
		COMMTIMEOUTS TimeOuts;
		DCB wdcb;
		SetupComm(hCom, 128, 128); //输入缓冲区和输出缓冲区的大小都是1024
		
		//设定读超时
		TimeOuts.ReadIntervalTimeout = 80;
		TimeOuts.ReadTotalTimeoutMultiplier = 0;
		TimeOuts.ReadTotalTimeoutConstant = 0;

		//设定写超时

		TimeOuts.WriteTotalTimeoutMultiplier = 500;
		TimeOuts.WriteTotalTimeoutConstant = 2000;
		SetCommTimeouts(hCom, &TimeOuts); //设置超时
		
		GetCommState(hCom, &wdcb);
		wdcb.BaudRate = baudRate;//波特率：9600，其他：不变
		wdcb.ByteSize = data_bits;
		wdcb.StopBits = stop_bits;
		SetCommState(hCom, &wdcb);
		PurgeComm(hCom, PURGE_TXCLEAR);
	}
	return err;
}

int ReleaseSerialPort()
{
	if ((hCom != (HANDLE)-1) && (hCom != NULL))
	{
		CloseHandle(hCom);
		hCom = NULL;
	}
	return 0;
}

//以一个线程不同监控串口行接收的数据
DWORD WINAPI SerialPort1ThreadProcess(HWND hWnd) //主窗口句柄
{
	char str[101];
	DWORD wCount; //读取的字节数
#if 1
	char lpInBuffer[256];
	DWORD dwBytesRead = 256;
	DWORD dwRealRead = 0;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	OVERLAPPED m_osRead;
	DWORD dwError;

	BOOL bReadStatus;

	memset(&m_osRead, 0, sizeof(OVERLAPPED));
	m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
#endif
	
	while (1)
	{
#if 1
		
		ClearCommError(hCom, &dwErrorFlags, &ComStat);
		dwBytesRead = 256;
		
		PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		bReadStatus = ReadFile(hCom, lpInBuffer, dwBytesRead, &dwRealRead, &m_osRead);
		if (!bReadStatus) //如果ReadFile函数返回FALSE
		{
			//GetLastError()函数返回ERROR_IO_PENDING,表明串口正在进行读操作
			if (dwError = GetLastError() == ERROR_IO_PENDING)
			{
				// WaitForSingleObject(m_osRead.hEvent, 2000);
				//使用WaitForSingleObject函数等待，直到读操作完成或延时已达到2秒钟
				//当串口读操作进行完毕后，m_osRead的hEvent事件会变为有信号
				//GetOverlappedResult(hCom,&m_osRead, &dwBytesRead, TRUE);
				GetOverlappedResult(hCom, &m_osRead, &dwRealRead, TRUE);
				if (dwRealRead > 0) //收到数据
				{
					lpInBuffer[dwRealRead] = '\0';
					::PostMessage(hWnd, COM_RECVDATA, (unsigned int)lpInBuffer, dwRealRead);
					//发送消息给对话框主窗口，以进行接收内容的显示
					
				}
					
				//PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
				//return dwBytesRead;

			}
		}
		else
		{
			//PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
			if (dwRealRead > 0) //收到数据
			{
				lpInBuffer[dwRealRead] = '\0';
				::PostMessage(hWnd, COM_RECVDATA, (unsigned int)lpInBuffer, dwRealRead);
				//发送消息给对话框主窗口，以进行接收内容的显示
			}
		}
		
	
#else
		ReadFile(hCom, str, 100, &wCount, NULL);
		if (wCount > 0) //收到数据
		{
			str[wCount] = '\0';
			::PostMessage(hWnd, COM_RECVDATA, (unsigned int)str, wCount);
			//发送消息给对话框主窗口，以进行接收内容的显示
		}
#endif
	}
	return TRUE;
}

int SerialPortWrite(char *szBuffer, int count)
{
	// TODO: Add your control notification handler code here
#if 0
	DWORD wCount = 0;
	WriteFile(hCom, szBuffer, count, &wCount, NULL);//发送数据
	return (int)wCount;
#else
	//char buffer[1024];
	//DWORD dwBytesWritten = 1024;
	DWORD dwErrorFlags;
	COMSTAT ComStat;
	OVERLAPPED m_osWrite;
	BOOL bWriteStat;
	DWORD dwRealSend = 0;

	strcat(szBuffer, "\r\n");
	count = count + 2;
	
	ZeroMemory(&m_osWrite, sizeof(m_osWrite));
	if (m_osWrite.hEvent != NULL)
	{
		ResetEvent(m_osWrite.hEvent);
		m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	if (ClearCommError(hCom, &dwErrorFlags, NULL))
	{
		PurgeComm(hCom, PURGE_TXABORT | PURGE_TXCLEAR);
		bWriteStat = WriteFile(hCom, szBuffer, count,(DWORD*)(&count), &m_osWrite);
		if (!bWriteStat)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				while (!GetOverlappedResult(hCom, &m_osWrite, &dwRealSend, FALSE))
				{

					if (GetLastError() == ERROR_IO_INCOMPLETE)
					{
						//cout<<"写未完成，继续!"<     
						continue;
					}
				}
				//WaitForSingleObject(m_osWrite.hEvent, 1000);
				//GetOverlappedResult(hCom, &m_osWrite, (DWORD*)(&count), TRUE);
				return count;
			}
			return 0;
		}
	}
	return count;
#endif
}
