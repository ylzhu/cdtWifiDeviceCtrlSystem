#ifndef _SERIAL_PORT_CONTROL_H
#define _SERIAL_PORT_CONTROL_H

#define COM_RECVDATA WM_USER+1000//自定义消息

extern HANDLE hCom; //全局变量，串口句柄
extern HANDLE hCommThread; //全局变量，串口线程
//串口监视线程控制函数
extern DWORD WINAPI SerialPort1ThreadProcess(HWND hWnd);
//打开并设置PC串口1(COM1)
extern int OpenSerialPort1(char *szPort, int baudRate, int data_bits, int stop_bits);
extern int ReleaseSerialPort();
extern int SerialPortWrite(char *szBuffer, int count);

#endif