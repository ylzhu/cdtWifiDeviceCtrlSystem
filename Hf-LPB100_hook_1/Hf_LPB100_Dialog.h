#ifndef _HF_LPB100_DIALOG_H_
#define _HF_LPB100_DIALOG_H_

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "comboxCtrl.h"

class WifiTestProc
{
public:
    WifiTestProc();
    virtual ~WifiTestProc();
    static BOOL CALLBACK s_DlgProc(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam);

protected:
private:
    LRESULT DlgProc(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam);

    LRESULT OnInitDialog(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam);
    LRESULT OnDestroy(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam);

    LRESULT OnTimer(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam);
    LRESULT	OnCommand(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam);
    LRESULT OnCtlColorDlg( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam );
    LRESULT OnClose( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam );
    LRESULT OnPaint(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam);
    LRESULT OnChar(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam);
	LRESULT OnRecvData(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);

    LRESULT OnSend(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam);
    LRESULT OnReset(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam);
    LRESULT OnReceive(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam);
    LRESULT OnSendSn(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam);
    LRESULT OnBtnLock(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam);

	void SendDlgListboxMessage(HWND hWnd, int listboxID, TCHAR* szString);

    BOOL    SendData(HWND hWnd);

    
    LRESULT OnStartStop(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam);

	void ReadINIFile(HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam);
	void WriteINIFile();
	LRESULT CreateExcelFile( HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam);
	BOOL PrintLogToExcelFile();

	
	//CAutoCompleteComboBox m_combo;
	//CEdit m_edit;
	comboBoxCtrl m_combo_com;
	comboBoxCtrl m_combo_rate;
	comboBoxCtrl m_combo_data_bits;
	comboBoxCtrl m_combo_stop_bits;

public:
	int OnComPortButton_click();
protected:
	int m_setup_btn_state;
};
#endif

