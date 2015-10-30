#include "stdafx.h"
#include "comboxCtrl.h"

comboBoxCtrl::comboBoxCtrl()
{

}

comboBoxCtrl::~comboBoxCtrl()
{

}

int comboBoxCtrl::comboBox_Init(int ComboId)
{
	m_Combo_id = ComboId;
	return 0;
}

int comboBoxCtrl::comboBox_add_string(HWND hWnd, int port, char *szString)
{
	//CB_ADDSTRING是在最后添加数据  
	HWND hWndComboBox = GetDlgItem(hWnd, m_Combo_id);
	SendMessage(hWndComboBox, CB_ADDSTRING, port, (LPARAM)szString);
	return 0;
}

int comboBoxCtrl::comboBox_add_com_range_string(HWND hWnd, int port, int start, int end)
{
	// CB_ADDSTRING是在最后添加数据  
	// 增加一个范围的数据
	char szString[16] = "COM";
	char szTmp[16] = {0};
	HWND hWndComboBox = GetDlgItem(hWnd, m_Combo_id);
	//sprintf(szTmp, "%s%s\0", szString, );
	for (int reg = start; reg <= end; reg++)
	{
		sprintf(szTmp, "%s%d\0", szString, reg);
		SendMessage(hWndComboBox, CB_ADDSTRING, port, (LPARAM)szTmp);
	}
	
	return 0;
}

int comboBoxCtrl::comboBox_insert_string(HWND hWnd, int port, char *szString)
{
	//CB_ADDSTRING是在指定位置添加数据  
	HWND hWndComboBox = GetDlgItem(hWnd, m_Combo_id);
	SendMessage(hWndComboBox, CB_INSERTSTRING, port, (LPARAM)szString);
	return 0;
}

int comboBoxCtrl::comboBox_delete_string(HWND hWnd, int port)
{
	HWND hWndComboBox = GetDlgItem(hWnd, m_Combo_id);
	SendMessage(hWndComboBox, CB_DELETESTRING, port, 0); 
	return 0;
}

int comboBoxCtrl::comboBox_get_string(HWND hWnd, int port, char *szString)
{
	HWND hWndComboBox = GetDlgItem(hWnd, m_Combo_id);
	if (port < 0)
	{
		SendMessage(hWndComboBox, CB_GETLBTEXT, SendMessage(hWndComboBox, CB_GETCURSEL, 0, 0), (LPARAM)szString);
	}
	else
	{
		SendMessage(hWndComboBox, CB_GETLBTEXT, port, (LPARAM)szString);
	}
	
	return 0;
}


int comboBoxCtrl::comboBox_get_Data_int(HWND hWnd, int port, int *data)
{
	char szString[32] = { 0 };
	HWND hWndComboBox = GetDlgItem(hWnd, m_Combo_id);
	if (port < 0)
	{
		SendMessage(hWndComboBox, CB_GETLBTEXT, SendMessage(hWndComboBox, CB_GETCURSEL, 0, 0), (LPARAM)szString);
	}
	else
	{
		SendMessage(hWndComboBox, CB_GETLBTEXT, port, (LPARAM)szString);
	}

	if (m_Combo_id == 1009) // IDC_COMBO_STOP_BITS
	{
		if (strcmp(szString, "1") == 0)
		{
			*data = 0;
		}
		else if (strcmp(szString, "1.5") == 0)
		{
			*data = 1;
		}
		else if (strcmp(szString, "2") == 0)
		{
			*data = 2;
		}
		return 0;
	}
	else
	{
		*data = atoi(szString);
	}



	return 0;
}

int comboBoxCtrl::comboBox_selet_string(HWND hWnd, int port, int reserve)
{
	HWND hWndComboBox = GetDlgItem(hWnd, m_Combo_id);
	SendMessage(hWndComboBox, CB_SETCURSEL, port, 0);
	return 0;
}
int comboBoxCtrl::comboBox_string_to_value(char *pszString, int* pvValue)
{
	*pvValue = atoi(pszString);
	return 0;
}