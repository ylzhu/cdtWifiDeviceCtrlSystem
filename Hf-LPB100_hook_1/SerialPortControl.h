#ifndef _SERIAL_PORT_CONTROL_H
#define _SERIAL_PORT_CONTROL_H

#define COM_RECVDATA WM_USER+1000//�Զ�����Ϣ

extern HANDLE hCom; //ȫ�ֱ��������ھ��
extern HANDLE hCommThread; //ȫ�ֱ����������߳�
//���ڼ����߳̿��ƺ���
extern DWORD WINAPI SerialPort1ThreadProcess(HWND hWnd);
//�򿪲�����PC����1(COM1)
extern int OpenSerialPort1(char *szPort, int baudRate, int data_bits, int stop_bits);
extern int ReleaseSerialPort();
extern int SerialPortWrite(char *szBuffer, int count);

#endif