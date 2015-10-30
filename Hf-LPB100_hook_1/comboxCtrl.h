#ifndef _COMBOXCTRL_H_
#define _COMBOXCTRL_H_

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

class comboBoxCtrl
{
public:
	comboBoxCtrl();
	virtual ~comboBoxCtrl();

	int comboBox_Init(int ComboId);
	int comboBox_add_string(HWND hWnd, int port, char *szString);
	int comboBox_add_com_range_string(HWND hWnd, int port, int start, int end);
	int comboBox_insert_string(HWND hWnd, int port, char *szString);
	int comboBox_delete_string(HWND hWnd, int port);
	int comboBox_get_string(HWND hWnd, int port, char *szString);

	int comboBox_get_Data_int(HWND hWnd, int port, int *data);

	int comboBox_selet_string(HWND hWnd, int port, int reserve);

	int comboBox_string_to_value(char *pszString, int* pvValue);

private:
	int m_Combo_id;
};



#endif