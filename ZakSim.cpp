#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <iostream>
#include <ctime> 
#include <cstring>
#include <Windows.h>
#include <conio.h>
#define SPACE 32   
#define WAY_KEY 224
using namespace std;

#include <mysql.h>
#pragma comment(lib, "libmysql.lib")
void connectDB();

MYSQL conn; //mysql ������ ���� ����ü
MYSQL* connPtr = NULL; //mysql �ڵ�
MYSQL_RES* Result; //���� ������ ����� ��� ����ü ������
MYSQL_ROW Row; //���� ������ ����� ���� ���� ������ ��� ����ü
int Stat; //���� ��û �� ��� (����/����)

// system("pause>null");
enum MENU {
	SIGNIN,
	SIGNUP,
	END
};
enum KEYBOARD {
	UP = 72,
	LEFT = 75,
	RIGHT = 77,
	DOWN = 80
};
void SetConsoleView() { // �ܼ�â ũ��
	system("mode con: lines=40  cols=100");
	system("title ZakSim 3day");
}
void CursorView() {
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; // Ŀ�� ����
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
void gotoxy(int x, int y) { // ��ǥ
	COORD Pos;
	Pos.X = 2 * x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
// �ΰ� ���
void DrawMenu() {
	system("cls");

	gotoxy(38, 1);
	cout << "/\\_ /\\" << endl;
	gotoxy(38, 2);
	cout << "((@V@))" << endl;
	gotoxy(38, 3);
	cout << "():::()" << endl;
	gotoxy(38, 4);
	cout << "VV - VV" << endl;

	gotoxy(8, 5);
	cout << " ________       _       ___  ____     ______   _____  ____    ____    " << endl;
	gotoxy(8, 6);
	cout << "|  __   _|     / \\     |_  ||_  _|  .' ____ \\ |_   _||_   \\  /   _|  " << endl;
	gotoxy(8, 7);
	cout << "|_/  / /      / _ \\      | |_/ /    | (___ \\_|  | |    |   \\/   |    " << endl;
	gotoxy(8, 8);
	cout << "   .'.' _    / ___ \\     |  __'.     _.____`.   | |    | |\\  /| |    " << endl;
	gotoxy(8, 9);
	cout << " _/ /__/ | _/ /   \\ \\_  _| |  \\ \\_  | \\____) | _| |_  _| |_\\/_| |_" << endl;
	gotoxy(8, 10);
	cout << "|________||____| |____||____||____|  \\______.'|_____||_____||_____|  " << endl;
	gotoxy(11, 11);
	cout << " ______   ______        _     ____  ____   ______   " << endl;
	gotoxy(11, 12);
	cout << "/ ____ `.|_   _ `.     / \\   |_  _||_  _|.' ____ \\  " << endl;
	gotoxy(11, 13);
	cout << "`'  __) |  | | `. \\   / _ \\    \\ \\  / /  | (___ \\_| " << endl;
	gotoxy(11, 14);
	cout << "_  |__ '.  | |  | |  / ___ \\    \\ \\/ /    _.____`.  " << endl;
	gotoxy(11, 15);
	cout << "| \\____)| _| |_.' /_/ /   \\ \\_  _|  |_   | \\____) | " << endl;
	gotoxy(11, 16);
	cout << "\\______.'|______.'|____| |____||______|   \\______.' " << endl;
	gotoxy(21, 21);
	cout << "  �� �� ��";
	gotoxy(21, 23);
	cout << " ȸ �� �� ��";
	gotoxy(21, 25);
	cout << "    �� ��" << endl;

}
// �޴� ȭ��ǥ
void DrawArrow(int& x) {
	if (x <= 0) x = 0;
	else if (x >= 2) x = 2;
	gotoxy(20, 21 + x * 2);
	cout << "��";
}
// �޴� ȭ��ǥ �ű��
MENU Control() {
	int x = 0;
	int input = 0;
	while (true) {
		DrawMenu();
		DrawArrow(x);
		input = _getch();
		if (input == WAY_KEY) {
			switch (_getch()) {
			case UP:
				--x;
				break;
			case DOWN:
				++x;
				break;
			}
		}
		else if (input == SPACE) {
			switch (x) {
			case 0: return SIGNIN;
			case 1: return SIGNUP;
			case 2: return END;
			}
		}
	}
}

// ����� Ŭ����
class User {
protected:
	string name, id, pw;
	Manager m; studyCafe user;
public:
	User() { }
	~User() { }
	void signup() {
		system("cls");
		gotoxy(15, 8);
		cout << "  ______    _                    _____  _____        " << endl;
		gotoxy(15, 9);
		cout << ".' ____ \\  (_)                  |_   _||_   _|        " << endl;
		gotoxy(15, 10);
		cout << "| (___ \\_| __   .--./) _ .--.     | |    | | _ .--.   " << endl;
		gotoxy(15, 11);
		cout << " _.____`. [  | / /'`\\;[ `.-. |    | '    ' |[ '/'`\\ \\ " << endl;
		gotoxy(15, 12);
		cout << "| \\____) | | | \\ \\._// | | | |     \\ \\__/ /  | \\__/ | " << endl;
		gotoxy(15, 13);
		cout << " \\______.'[___].',__` [___||__]     `.__.'   | ;.__/  " << endl;
		gotoxy(15, 14);
		cout << "              ( ( __))                      [__|      " << endl;

		gotoxy(21, 16);
		cout << "�̸� : ";
		cin >> name;
		gotoxy(21, 18);
		cout << "���̵� : ";
		cin >> id;
		gotoxy(21, 20);
		cout << "��й�ȣ : ";
		cin >> pw;

		string Query = "insert into student(student_id, name, password) values('" + id + "', '" + name + "', '" + pw + "'); ";
		Stat = mysql_query(connPtr, Query.c_str());
		if (Stat != 0) fprintf(stderr, "Mysql quert error : %s\n", mysql_error(&conn));
		
		/*Result = mysql_use_result(connPtr);
		Stat = mysql_num_fields(Result);
		while ((Row = mysql_fetch_row(Result))) {
			for (int i = 0; i < Stat; i++) {
				printf("%s ", Row[i] ? Row[i] : "NULL");
			}
		}*/
		gotoxy(21, 30);
		cout << "Press [ ENTER ] ";
		mysql_close(connPtr);
	}
	void signin() {
		system("cls");
		string input_id, input_pw;
		gotoxy(15, 8);
		cout << "  _____                      _            " << endl;
		gotoxy(15, 9);
		cout << " |_   _|                    (_)           " << endl;
		gotoxy(15, 10);
		cout << "   | |       .--.   .--./)  __   _ .--.   " << endl;
		gotoxy(15, 11);
		cout << "   | |   _ / .'`\\ \\/ /'`\\; [  | [ `.-. |  " << endl;
		gotoxy(15, 12);
		cout << "  _| |__/ || \\__. |\\ \\._//  | |  | | | |  " << endl;
		gotoxy(15, 13);
		cout << " |________| '.__.' .',__`  [___][___||__] " << endl;
		gotoxy(15, 14);
		cout << "                  ( ( __))                " << endl;
		gotoxy(21, 18);
		cout << "���̵� : ";
		cin >> input_id;
		gotoxy(21, 20);
		cout << "��й�ȣ : ";
		cin >> input_pw;

		if (input_id == "admin" && input_pw == "1234") m.manager_print();
		else if (input_id == name && input_pw == pw) user.user_print();

		system("pause>null");
	}
};

class studyCafe : public User {
	int menu;
public:
	studyCafe() {}
	~studyCafe() {}
	// ����� ȭ�� ��� 
	int user_print() {
		system("cls");
		menu = 0;
		gotoxy(15, 8);
		cout << "����������������������������������������������������������������������������" << endl;
		gotoxy(15, 9);
		cout << "��             "<<name<<"�� ȯ���մϴ�!        ��";
		gotoxy(15, 10);
		cout << "����������������������������������������������������������������������������" << endl;
		gotoxy(20, 16);
		cout << "1.  �̿�� ����" << endl;
		gotoxy(20, 20);
		cout << "2.  �¼����� Ȯ�� �� �Խ�" << endl;
		gotoxy(20, 24);
		cout << "3.  ���" << endl;

		while (1) {
			gotoxy(15, 30);
			cout << "�޴� �Է� >> ";
			cin >> menu;
			switch (menu) {
			case 1: // �̿�� ����
				system("cls");
				use_check();
				break;
			case 2: // �¼� ����
				system("cls");
				seat_state();
				break;
			case 3: // ���
				system("cls");
				gotoxy(20, 18);
				cout << "<<<<<< ��� �Ϸ� >>>>>>" << endl;
				Sleep(2000);
				DrawMenu();
				break;
			default:
				cout << "�޴� ������ ������ϴ�! �ٽ� �Է����ּ���." << endl;
			}
		}
	}
	// �̿�� Ȯ�� ���
	void use_check() {
		system("cls");
		menu = 0;
		gotoxy(15, 8);
		cout << "����������������������������������������������������������������������������������������" << endl;
		gotoxy(15, 9);
		cout << "��                �̿�� ����               ��";
		gotoxy(15, 10);
		cout << "����������������������������������������������������������������������������������������" << endl;
		gotoxy(20, 18);
		cout << "1.  �̿�� ���� �� Ȯ��" << endl;
		gotoxy(20, 20);
		cout << "2.  Ȩ���� ���Ʊ�� " << endl;

		gotoxy(18, 27);
		cout << "�޴� �Է� >> ";
		while (1) {
			cin >> menu;
			switch (menu) {
			case 1:
				use_buy();
				break;
			case 2:
				user_print();
			}
		}
	}
	// �̿�� ����
	void use_buy() {
		system("cls");
		gotoxy(15, 8);
		cout << "����������������������������������������������������������������������������������������" << endl;
		gotoxy(15, 9);
		cout << "��                �̿�� ����               ��";
		gotoxy(15, 10);
		cout << "����������������������������������������������������������������������������������������" << endl;

		gotoxy(22, 12);
		cout << "�ð� �����̽��ϴ�. �����Ͻ� �̿���� �������ּ���!";

		gotoxy(22, 14);
		cout << "1. 2�ð��� ����" << endl;
		gotoxy(22, 16);
		cout << "2. 6�ð��� ����" << endl;
		gotoxy(22, 18);
		cout << "3. 10�ð��� ����" << endl;
		gotoxy(22, 20);
		cout << "4. 14�ð��� ����" << endl;
		gotoxy(22, 22);
		cout << "5. 18�ð��� ����" << endl;
		
		int use_ticket = 0;
		gotoxy(22, 24);
		cout << "�̿�� ���� �Է� >> ";
		cin >> use_ticket;

		gotoxy(22, 26);
		cout << use_ticket * 2 << "���� �����ϼ̽��ϴ�!" << endl;

		char YorN[5];
		gotoxy(22, 37);
		cout << "�߰� �����Ͻðڽ��ϱ�? (y/n)" << endl;
		cin >> YorN;
		if (strcmp(YorN,"y")==0) {
			gotoxy(22, 24);
			cout << "�̿�� ���� �Է� >> ";
			cin >> use_ticket;
		}
		else {
			use_check();
		}
	}
	// �¼� ���� �ʱ�ȭ
	void seat_reset(int a, int b) {

	}
	// �Խ� 
	void comeIn() {
		int i = 0, j = 0;
		gotoxy(15, 25);
		cout << "�ڸ� �Է� : ";
		cin >> j;

		cout << endl << endl << endl << endl << endl << endl;
		cout << "\t\t\t\t___________________________________________" << endl;
		cout << "\t\t\t\t    ** �� �¼��� �̹� ��� ���Դϴ�. **" << endl << endl;
		cout << "\t\t\t\t      ** �ٸ� �¼��� �̿����ּ���. **" << endl;
		cout << "\t\t\t\t___________________________________________" << endl;
		cout << endl;

		cout << "\t\t\t\t        <<<<<< �Խ��� �Ϸ�Ǿ����ϴ� >>>>>>       " << endl;

	}
	// �¼� ����
	void seat_state() {
		system("cls");
		int i, j;
		int cnt = 0;

		gotoxy(21, 8);
		cout << " ��  �¼� ����  �� " << endl;

		gotoxy(13, 15);
		cout << " ������01������ ������02������ ������03������ ������04������ ������05������ " << endl;
		for (i = 0; i < 5; i++) {
			for (j = 0; j < 5; j++) {
				/*if (seat[i][j].state == 1) {
					gotoxy(13, 16);
					cout << "��";
					gotoxy(13, 17);
					cout << " ";
					gotoxy(13, 18);
					cout << "��";
				}
				else {
					gotoxy(13, 16);
					cout << "��";
					gotoxy(13, 17);
					cout << "�����";
					gotoxy(13, 18);
					cout << "��";
				}*/
			}
		}
		gotoxy(13, 19);
		cout << "���������������� ���������������� ���������������� ���������������� ����������������" << endl;
		comeIn();
	}

};
class Manager : public User {
	string id = "admin";
	int pw = 1329;
public:
	Manager() {}
	~Manager() { }

	// ������ ȭ�� ���
	int manager_print() {
		system("cls");
		int menu;

		gotoxy(15, 8);
		cout << "����������������������������������������������������������������������������" << endl;
		gotoxy(15, 9);
		cout << "��             ������ ����            ��";
		gotoxy(15, 10);
		cout << "����������������������������������������������������������������������������" << endl;
		gotoxy(20, 18);
		cout << "1. �¼� ����" << endl;
		gotoxy(20, 20);
		cout << "2. ȸ�� ����" << endl;
		gotoxy(20, 22);
		cout << "3. ����� Ȯ��" << endl;

		while (1) {
			gotoxy(15, 30);
			cout << "�޴� �Է� >> ";
			cin >> menu;
			switch (menu) {
			case 1:
				system("cls");
				cout << "�¼� ����" << endl;
				break;
			case 2:

				break;
			case 3:
				system("cls");
				cout << "����� Ȯ��" << endl;
				break;
			default:
				cout << "�޴� ������ ������ϴ�! �ٽ� �Է��ϼ���" << endl;
			}
		}
		system("pause>null");
	}
};

int main() {
	CursorView();
	SetConsoleView();
	connectDB();
	while (1) {
		switch (Control()) {
		case SIGNIN:
			signin();
			break;
		case SIGNUP:
			signup();
			signin();
			break;
		case END:
			cout << "\n\n\n\n\n           * * * * * * * * * * * * * * �۽� 3���� �����մϴ� * * * * * * * * * * * * * * " << endl;
			system("pause>null");
			return 0;
		}
	}
}
void connectDB() {
	mysql_init(&conn);
	connPtr = mysql_real_connect(&conn, "localhost", "root", "hope@153", "studyCafe", 3306, NULL, 0);
	if (connPtr == NULL) cout << "error : " << mysql_error(&conn);
	else {}
}