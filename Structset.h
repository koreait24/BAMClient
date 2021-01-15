#pragma once
#include <Windows.h>

#define		S		"first section"

#define		S_1_1	"log_in_id"
#define		S_1_2	"log_in_pw"
#define		S_1_3	"log_in_complete"

#define		S_2_1	"reg_id"
#define		S_2_2	"reg_pw"

#define		E_1		"cannot using null / ? : < > \" \\ * character"

struct USER
{
	char user_id[20];
	char user_pw[20];
	char file_name[100];
};

struct console
{
	void GOTOXY(int x, int y)
	{
		COORD Cur;
		Cur.X = x;
		Cur.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
	}
};
