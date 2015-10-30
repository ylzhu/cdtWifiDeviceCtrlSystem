#include "StdAfx.h"
#include <windows.h>
#include <stdio.h>
#include "SerialPortControl.h"

HANDLE hCom; //ȫ�ֱ��������ھ��
HANDLE hCommThread; //ȫ�ֱ����������߳�

int OpenSerialPort1(char *szPort , int baudRate, int data_bits, int stop_bits)
{
	//�򿪲�����COM1
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
		//MessageBox("��COM1ʧ��");
		return err;
	}
	else
	{
		COMMTIMEOUTS TimeOuts;
		DCB wdcb;
		SetupComm(hCom, 128, 128); //���뻺����������������Ĵ�С����1024
		
		//�趨����ʱ
		TimeOuts.ReadIntervalTimeout = 80;
		TimeOuts.ReadTotalTimeoutMultiplier = 0;
		TimeOuts.ReadTotalTimeoutConstant = 0;

		//�趨д��ʱ

		TimeOuts.WriteTotalTimeoutMultiplier = 500;
		TimeOuts.WriteTotalTimeoutConstant = 2000;
		SetCommTimeouts(hCom, &TimeOuts); //���ó�ʱ
		
		GetCommState(hCom, &wdcb);
		wdcb.BaudRate = baudRate;//�����ʣ�9600������������
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

//��һ���̲߳�ͬ��ش����н��յ�����
DWORD WINAPI SerialPort1ThreadProcess(HWND hWnd) //�����ھ��
{
	char str[101];
	DWORD wCount; //��ȡ���ֽ���
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
		if (!bReadStatus) //���ReadFile��������FALSE
		{
			//GetLastError()��������ERROR_IO_PENDING,�����������ڽ��ж�����
			if (dwError = GetLastError() == ERROR_IO_PENDING)
			{
				// WaitForSingleObject(m_osRead.hEvent, 2000);
				//ʹ��WaitForSingleObject�����ȴ���ֱ����������ɻ���ʱ�Ѵﵽ2����
				//�����ڶ�����������Ϻ�m_osRead��hEvent�¼����Ϊ���ź�
				//GetOverlappedResult(hCom,&m_osRead, &dwBytesRead, TRUE);
				GetOverlappedResult(hCom, &m_osRead, &dwRealRead, TRUE);
				if (dwRealRead > 0) //�յ�����
				{
					lpInBuffer[dwRealRead] = '\0';
					::PostMessage(hWnd, COM_RECVDATA, (unsigned int)lpInBuffer, dwRealRead);
					//������Ϣ���Ի��������ڣ��Խ��н������ݵ���ʾ
					
				}
					
				//PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
				//return dwBytesRead;

			}
		}
		else
		{
			//PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
			if (dwRealRead > 0) //�յ�����
			{
				lpInBuffer[dwRealRead] = '\0';
				::PostMessage(hWnd, COM_RECVDATA, (unsigned int)lpInBuffer, dwRealRead);
				//������Ϣ���Ի��������ڣ��Խ��н������ݵ���ʾ
			}
		}
		
	
#else
		ReadFile(hCom, str, 100, &wCount, NULL);
		if (wCount > 0) //�յ�����
		{
			str[wCount] = '\0';
			::PostMessage(hWnd, COM_RECVDATA, (unsigned int)str, wCount);
			//������Ϣ���Ի��������ڣ��Խ��н������ݵ���ʾ
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
	WriteFile(hCom, szBuffer, count, &wCount, NULL);//��������
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
						//cout<<"дδ��ɣ�����!"<     
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
