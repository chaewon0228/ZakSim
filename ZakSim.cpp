#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <iostream>
#include <ctime> 
#include <cstring>
#include <Windows.h>
#include <conio.h>
#define YELLOW 14 
#define WHITE 15 
#define RED 4 
#define SPACE 32   
#define WAY_KEY 224
using namespace std;

#include <mysql.h>
#pragma comment(lib, "libmysql.lib")
void connectDB();

MYSQL conn; //mysql ����ü
MYSQL* connPtr = NULL; //mysql �ڵ�
MYSQL_RES* Result; //����ü ������
MYSQL_ROW Row; //���� ������ ����� ���� ���� ������ ��� ����ü
int Stat; 

class Database {
public:
	void set_ticket() {

	}
	void set_login() {

	}
	void set_seat() {

	}
	void get_residual_time() {

	}
	void get_all_ticket() {}
	void trigger_ticketBuy() {

	}
	void set_ticketBuy() {

	}
	void get_purchase_list() {

	}
	void get_rentable_seat_list() {

	}
};
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
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
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
	gotoxy(12, 11);
	cout << " ______   ______        _     ____  ____   ______   " << endl;
	gotoxy(12, 12);
	cout << "/ ____ `.|_   _ `.     / \\   |_  _||_  _|.' ____ \\  " << endl;
	gotoxy(12, 13);
	cout << "`'  __) |  | | `. \\   / _ \\    \\ \\  / /  | (___ \\_| " << endl;
	gotoxy(12, 14);
	cout << "_  |__ '.  | |  | |  / ___ \\    \\ \\/ /    _.____`.  " << endl;
	gotoxy(12, 15);
	cout << "| \\____)| _| |_.' /_/ /   \\ \\_  _|  |_   | \\____) | " << endl;
	gotoxy(12, 16);
	cout << "\\______.'|______.'|____| |____||______|   \\______.' " << endl;
	gotoxy(21, 21);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
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
public:
	User() { }
	~User() { }
	void signup() {
		system("cls");
		string in_name, in_id, in_pw;
		gotoxy(12, 8);
		cout << "  ______    _                    _____  _____        " << endl;
		gotoxy(12, 9);
		cout << ".' ____ \\  (_)                  |_   _||_   _|        " << endl;
		gotoxy(12, 10);
		cout << "| (___ \\_| __   .--./) _ .--.     | |    | | _ .--.   " << endl;
		gotoxy(12, 11);
		cout << " _.____`. [  | / /'`\\;[ `.-. |    | '    ' |[ '/'`\\ \\ " << endl;
		gotoxy(12, 12);
		cout << "| \\____) | | | \\ \\._// | | | |     \\ \\__/ /  | \\__/ | " << endl;
		gotoxy(12, 13);
		cout << " \\______.'[___].',__` [___||__]     `.__.'   | ;.__/  " << endl;
		gotoxy(12, 14);
		cout << "              ( ( __))                      [__|      " << endl;

		gotoxy(21, 20);
		cout << "�̸� : ";
		cin >> in_name;
		gotoxy(21, 22);
		cout << "���̵� : ";
		cin >> in_id;
		gotoxy(21, 24);
		cout << "��й�ȣ : ";
		cin >> in_pw;

		name = in_name; id = in_id; pw = in_pw;
		/*string Query = "insert into student(student_id, name, password) values('" + id + "', '" + name + "', '" + pw + "'); ";
		Stat = mysql_query(connPtr, Query.c_str());
		if (Stat != 0) fprintf(stderr, "Mysql quert error : %s\n", mysql_error(&conn));*/
		
		/*Result = mysql_use_result(connPtr);
		Stat = mysql_num_fields(Result);
		while ((Row = mysql_fetch_row(Result))) {
			for (int i = 0; i < Stat; i++) {
				printf("%s ", Row[i] ? Row[i] : "NULL");
			}
		}*/
		/*
		insert into ticket(time, storable, price) values(7200, false, 3000);
		insert into ticket(time, storable, price) values(14400, false, 5000);
		insert into ticket(time, storable, price) values(21600, false, 7000);
		insert into ticket(time, storable, price) values(28800, false, 9000);
		insert into ticket(time, storable, price) values(43200, false, 10000);
		insert into ticket(time, storable, price) values(180000, true, 60000);
		insert into ticket(time, storable, price) values(360000, true, 100000);
		insert into ticket(time, storable, price) values(540000, true, 160000);
		insert into ticket(time, storable, price) values(720000, true, 200000);
		*/
		mysql_close(connPtr);
	}
	int signin() {
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

		if (input_id == "admin" && input_pw == "1329") return 0;
		else if (input_id == name && input_pw == pw) return 1;
		else {
			gotoxy(9, 25);
			cout << "�α��ο� �����߽��ϴ�! ���̵�� ��й�ȣ�� �ٽ� �ѹ� Ȯ�����ּ���.";
			Sleep(2000);
			signin();
		}
	}
	// �¼� ����
	void seat_state() {
		system("cls");
		int i, j;
		int cnt = 0;
		gotoxy(21, 5);
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
	}
};

class studyCafe : public User {
	int menu;
public:
	studyCafe() {}
	~studyCafe() {}
	// ����� ȭ�� ��� 
	void user_print() {
		system("cls");
		menu = 0;
		gotoxy(16, 5);
		cout << "����������������������������������������������������������������������������" << endl;
		gotoxy(16, 6);
		cout << "��         ȸ����, �ݰ����ϴ�!        ��";
		gotoxy(16, 7);
		cout << "����������������������������������������������������������������������������" << endl;
		gotoxy(19, 15);
		cout << "1.  �̿�� ����" << endl;
		gotoxy(19, 18);
		cout << "2.  �¼����� Ȯ�� �� �Խ�" << endl;
		gotoxy(19, 21);
		cout << "3.  ���" << endl;

		gotoxy(15, 30);
		cout << "�޴� �Է� >> ";
		cin >> menu;
		switch (menu) {
			case 1: // �̿�� ����
				use_check();
				break;
			case 2: // �¼� ����
				seat_state();
				comeIn();
				break;
			case 3: // ���
				system("cls");
				gotoxy(20, 18);
				cout << "<<<<<< ��� �Ϸ� >>>>>>" << endl;
				Sleep(2100);
				DrawMenu();
				break;
			default:
				cout << "�޴� ������ ������ϴ�! �ٽ� �Է����ּ���." << endl;
				Sleep(2000);
				user_print();
		}
	}
	// �̿�� Ȯ�� ���
	void use_check() {
		system("cls");
		menu = 0;
		gotoxy(14, 5);
		cout << "����������������������������������������������������������������������������������������" << endl;
		gotoxy(14, 6);
		cout << "��                �̿�� ����               ��";
		gotoxy(14, 7);
		cout << "����������������������������������������������������������������������������������������" << endl;
		gotoxy(19, 15);
		cout << "1.  �̿�� ���� �� Ȯ��" << endl;
		gotoxy(19, 18);
		cout << "2.  Ȩ���� ���Ʊ�� " << endl;

		gotoxy(15, 30);
		cout << "�޴� �Է� >> ";
		cin >> menu;
		switch (menu) {
			case 1:
				use_buy();
				use_check();
				break;
			case 2:
				user_print();
				break;
		}
	}
	// �̿�� ����
	int use_buy() {
		int use_ticket, time = 2;
		system("cls");
		use_ticket = 0;
		gotoxy(14, 5);
		cout << "����������������������������������������������������������������������������������������" << endl;
		gotoxy(14, 6);
		cout << "��                �̿�� ����               ��";
		gotoxy(14, 7);
		cout << "����������������������������������������������������������������������������������������" << endl;
		gotoxy(10, 9);
		cout << "�ð� �����̽��ϴ�. �����Ͻ� �̿���� �������ּ���!";
		gotoxy(19, 12);
		cout << "1. 2�ð��� ����" << endl;
		gotoxy(19, 14);
		cout << "2. 4�ð��� ����" << endl;
		gotoxy(19, 16);
		cout << "3. 6�ð��� ����" << endl;
		gotoxy(19, 18);
		cout << "4. 12�ð��� ����" << endl;
		gotoxy(19, 20);
		cout << "5. 50�ð��� ����" << endl;
		gotoxy(19, 22);
		cout << "6. 100�ð��� ����" << endl;
		gotoxy(19, 24);
		cout << "7. 150�ð��� ����" << endl;
		gotoxy(19, 26);
		cout << "8. 200�ð��� ����" << endl;
		gotoxy(15, 29);
		cout << "�̿�� ���� �Է� >> ";
		cin >> use_ticket;
		gotoxy(19, 31);
		if(use_ticket > 4) cout << 50 * use_ticket - 4 << "�ð����� �����ϼ̽��ϴ�!" << endl;
		else cout << 2 + (2 * (use_ticket - 1)) << "�ð����� �����ϼ̽��ϴ�!" << endl;

		char YorN[5];
		gotoxy(19, 33);
		cout << "�߰� �����Ͻðڽ��ϱ�? (y/n) => " ;
		cin >> YorN;
		if (YorN == "y") {
			gotoxy(15, 37);
			cout << "�̿�� ���� �Է� >> ";
			cin >> use_ticket;
		}
		else if (YorN == "n") {
			gotoxy(19, 35);
			cout << "===== �̿�� ���� ���� =====";
			Sleep(1500);
			return 0;
		}
	}
	// �¼� ���� �ʱ�ȭ
	void seat_reset(int a, int b) {

	}
	// �Խ� 
	void comeIn() {
		int seat_num = 0;
		gotoxy(15, 23);
		cout << "�ڸ� �Է� : ";
		cin >> seat_num;

		cout << endl << endl << endl << endl << endl << endl;
		cout << "\t\t\t\t___________________________________________" << endl;
		cout << "\t\t\t\t    ** �� �¼��� �̹� ��� ���Դϴ�. **" << endl << endl;
		cout << "\t\t\t\t      ** �ٸ� �¼��� �̿����ּ���. **" << endl;
		cout << "\t\t\t\t___________________________________________" << endl;
		cout << endl;
		cout << "\t\t\t\t        <<<<<< �Խ��� �Ϸ�Ǿ����ϴ� >>>>>>       " << endl;

	}
};
class Manager : public User {
public:
	Manager() {}
	~Manager() { }
	string get_id() { return id; }
	string get_pw() { return pw; }
	// ������ ȭ�� ���
	void manager_print() {
		system("cls");
		int menu = 0;
		gotoxy(16, 5);
		cout << "����������������������������������������������������������������������������" << endl;
		gotoxy(16, 6);
		cout << "��             ������ ����            ��";
		gotoxy(16, 7);
		cout << "����������������������������������������������������������������������������" << endl;
		gotoxy(19, 17);
		cout << "1. �¼� ����" << endl;
		gotoxy(19, 21);
		cout << "2. ȸ�� ����" << endl;
		gotoxy(19, 25);
		cout << "3. ����� Ȯ��" << endl;

		gotoxy(15, 30);
		cout << "�޴� �Է� >> ";
		cin >> menu;
		switch (menu) {
			case 1:
				seat_state();
				break;
			case 2:
				cout << "ȸ�� ����" << endl;
				break;
			case 3:
				system("cls");
				cout << "����� Ȯ��" << endl;
				break;
			default:
				cout << "�޴� ������ ������ϴ�! �ٽ� �Է����ּ���." << endl;
				Sleep(2000);
				manager_print();
		}
	}
};
int main() {
	CursorView();
	SetConsoleView();
	connectDB();
	User use;
	Manager m; studyCafe s;
	switch (Control()) {
		case SIGNIN:
			use.signin();
			break;
		case SIGNUP:
			use.signup();
			if (use.signin() == 0)  m.manager_print();
			else s.user_print();
			break;
		case END:
			cout << "\n\n\n\n\n           * * * * * * * * * * * * * * �۽� 3���� �����մϴ� * * * * * * * * * * * * * * " << endl;
			system("pause>null");
			return 0;
	}
}
void connectDB() {
	mysql_init(&conn);
	connPtr = mysql_real_connect(&conn, "localhost", "root", "hope@153", "studyCafe", 3306, NULL, 0);
	if (connPtr == NULL) cout << "error : " << mysql_error(&conn);
	else {}
}