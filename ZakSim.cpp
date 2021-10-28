#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <iostream>
#include <ctime> 
#include <string>
#include <Windows.h>
#include <conio.h>
#define YELLOW 14
#define BROWN 6 
#define WHITE 15 
#define RED 4 
#define SPACE 32   
#define WAY_KEY 224
using namespace std;

#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

MYSQL conn; //mysql ����ü
MYSQL* connPtr = NULL; //mysql �ڵ�
MYSQL_RES* Result; //����ü ������
MYSQL_ROW Row; //���� ������ ����� ���� ���� ������ ��� ����ü
int Stat; 
void connectDB();

string input_id, name;
int seat_num = 0;


class Database {
public:
	// ticket �� �ʱ�ȭ
	void set_ticket() {
		string Query = "insert into ticket(ticket_no, price, time)";
		Query += " values(1, 3000, 7200), (2, 5000, 14400),";
		Query += "(3, 7000, 21600), (4, 9000, 28800),";
		Query += "(5, 43200, 10000), (6, 180000, 60000),";
		Query += "(7, 360000, 100000), (8, 540000, 160000), (9, 720000, 200000);";

		Stat = mysql_query(connPtr, Query.c_str());
		if (Stat != 0) {
			cout << stderr << "error : " << mysql_error(&conn);
		}
	}

	// seat �� �ʱ�ȭ
	void set_seat() {
		char temp[10];
		string Query = "insert into seat values";
		int i;
		for (i = 1; i <= 19; i++) {
			Query += "(";
			_itoa(i, temp, 10);
			Query += temp;
			Query += ", 1),";
		}
		Query += "(20, 1);";
		Stat = mysql_query(connPtr, Query.c_str());
		if (Stat != 0) {
			cout << stderr << "error : " << mysql_error(&conn);
			Sleep(5000);
		}
	}

	// �̿�� ����
	void buyTicket(string id, int ticket_no) {
		string Query = "insert into purchase(student_id, ticket_no) values('"+ id +"', "+ to_string(ticket_no) +");";
		Stat = mysql_query(connPtr, Query.c_str());
		Result = mysql_store_result(&conn);
	}

	// �ܿ� �̿�ð� ���� Ʈ����
	void trigger_buyTicket() {  
		string Query = "drop trigger if exists update_residual_time_after_purchase;";
		Query += "create trigger update_residual_time_after_purchase";
		Query += "after insert on purchase";
		Query += "for each row";
		Query += "update student";
		Query += "set residual_time = residual_time + (select time from ticket where ticket_no = new.ticket_no)";
		Query += "where student_id = new.student_id;";
		Stat = mysql_query(connPtr, Query.c_str());
		Result = mysql_store_result(&conn); 
	}

	// �̿� �ݾ�
	void get_money() {
		string Query = "select sum(Ticket.price) from Purchase, Ticket where Purchase.ticket_no = Ticket.ticket_no;";
		Stat = mysql_query(connPtr, Query.c_str());
		Result = mysql_store_result(&conn);
	}

	// ������ Ƽ�� ��ȣ 
	void buyTicketNum(string id) {
		string Query = "select ticket_no from purchase where student_id = '"+ id +"';";
		Stat = mysql_query(connPtr, Query.c_str());
		Result = mysql_store_result(&conn);
	}

	// Ƽ�� ����
	void ticketPrice(int ticket_no) {
		string Query = "select price from ticket where ticket_no = "+ to_string(ticket_no) +"; ";
		Stat = mysql_query(connPtr, Query.c_str());
		Result = mysql_store_result(&conn);
	}

	// ���
	void outPut(int seat_no) {
		string Query = "update seat set rent_q = 1 where seat_no = " + to_string(seat_no) + ";";
		Stat = mysql_query(connPtr, Query.c_str());
		Result = mysql_store_result(&conn);
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

	gotoxy(34, 3);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BROWN);
	cout << "/\\_ /\\" << endl;
	gotoxy(34, 4);
	cout << "((@V@))" << endl;

	gotoxy(13, 5);
	cout << "   ______  __                      _      ():::()" << endl;
	gotoxy(13, 6);
	cout << " .' ___  |[  |                    (_)     VV - VV" << endl;
	gotoxy(13, 7);
	cout << "/ .'   \\_| | |--.   .--.   .--.   __   _ .--..--.  " << endl;
	gotoxy(13, 8);
	cout << "| |        | .-. |/ .'`\\ \\( (`\\] [  | [ `.-. .-. |" << endl;
	gotoxy(13, 9);
	cout << "\\ `.___.'\\ | | | || \\__. | `'.'.  | |  | | | | | | " << endl;
	gotoxy(13, 10);
	cout << " `.____ .'[___]|__]'.__.' [\\__) )[___][___||__||__] " << endl;
	gotoxy(9, 11);
	cout << "        _                  __              ______           ___        " << endl;
	gotoxy(9, 12);
	cout << "       / |_               |  ]           .' ___  |        .' ..]       " << endl;
	gotoxy(9, 13);
	cout << " .--. `| |-'__   _    .--.| |   _   __  / .'   \\_| ,--.  _| |_  .---.  " << endl;
	gotoxy(9, 14);
	cout << "( (`\\] | | [  | | | / /'`\\' |  [ \\ [  ] | |       `'_\\ :'-| |-'/ /__\\\\ " << endl;
	gotoxy(9, 15);
	cout << " `'.'. | |, | \\_/ |,| \\__/  |   \\ '/ /  \\ `.___.'\\// | |, | |  | \\__., " << endl;
	gotoxy(9, 16);
	cout << "[\\__) )\\__/ '.__.'_/ '.__.;__][\\_:  /    `.____ .'\\'-;__/[___]  '.__.' " << endl;
	gotoxy(9, 17);
	cout << "                               \\__.'                                   " << endl;

	gotoxy(22, 22);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	cout << "  �� �� ��";
	gotoxy(22, 24);
	cout << " ȸ �� �� ��";
	gotoxy(22, 26);
	cout << "    �� ��" << endl;

}

// �޴� ȭ��ǥ
void DrawArrow(int& x) {
	if (x <= 0) x = 0;
	else if (x >= 2) x = 2;
	gotoxy(21, 22 + x * 2);
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

// �޴� ���� ����� �� 
void nothingMenu() {
	cout << "�޴� ������ ������ϴ�! �ٽ� �Է����ּ���." << endl;
	Sleep(2000);
}

// ����� Ŭ����
class User {
	Database db;
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

		time_t timer;
		struct tm* t;
		timer = time(NULL);
		t = localtime(&timer);
		string str = to_string(t->tm_year + 1900) + "-" + to_string(t->tm_mon + 1) + "-" + to_string(t->tm_mday) + " " + to_string(t->tm_hour) + ":" + to_string(t->tm_min);

		string Query = "insert into student(student_id, name, pw, join_date) values('" + in_id + "', '" + in_name + "', '" + in_pw + "', '"+str+"'); ";
		Stat = mysql_query(connPtr, Query.c_str());
		Result = mysql_store_result(&conn);
		if (Stat != 0) {
			cout << stderr << "error : " << mysql_error(&conn);
			Sleep(9000);
		}
	}
	int signin() {
		system("cls");
		string input_pw;
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

		string Query = "SELECT * FROM student where student_id = '"+input_id+"';";
		Stat = mysql_query(connPtr, Query.c_str());
		if (Stat != 0) { 
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(5000);
		}
		Result = mysql_store_result(connPtr);
		while ((Row = mysql_fetch_row(Result)) != NULL) {
			name = Row[1];
			if (input_id == "admin" && input_pw == "1329") return 0;
			else if (input_id == Row[0] && input_pw == Row[2]) return 1; 
			else if (input_pw != Row[2] && input_id == Row[0]) {
				gotoxy(9, 25);
				cout << "��й�ȣ�� ��ġ���� �ʽ��ϴ�! �ٽ� �Է����ּ���.";
				Sleep(2000);
				signin();
			}
			else if (input_id != Row[0] && input_pw == Row[2]) {
				gotoxy(9, 25);
				cout << "���̵� ��ġ���� �ʽ��ϴ�! �ٽ� �Է����ּ���.";
				Sleep(2000);
				signin();
			}
			else {
				gotoxy(9, 25);
				cout << "�α��ο� �����߽��ϴ�! ���̵�� ��й�ȣ�� �ٽ� �ѹ� Ȯ�����ּ���.";
				Sleep(2000);
				signin();
			}
		}
	}

	// �¼� ����
	void seat_state() {
		system("cls");
		int i, j;
		int seat[20];
		gotoxy(21, 5);
		cout << " ��  �¼� ����  �� " << endl;

		string Query = "select * from seat where rent_q = " + to_string(seat_num) + ";";
		Stat = mysql_query(connPtr, Query.c_str());
		if (Stat != 0) {
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(5000);
		}
		Result = mysql_store_result(&conn);
		while ((Row = mysql_fetch_row(Result)) != NULL) {
			for (i = 0; i < 5; i++) {
				if (Row[i] == 0) {
					seat[i] = 0;
					cout << Row[i] << endl;
					Sleep(7000);
				}
				else {
					seat[i] = 1;
					cout << Row[i] << endl;
					Sleep(7000);
				}
			}
		}
		
	}
};

class studyCafe : public User {
	Database db;
	int menu;
public:
	studyCafe() {}
	~studyCafe() {}

	// ����� ȭ�� ��� 
	int user_print() {
		system("cls");
		menu = 0;
		gotoxy(16, 5);
		cout << "����������������������������������������������������������������������������" << endl;
		gotoxy(16, 6);
		cout << "��    "<< name << " ȸ����, �ݰ����ϴ�!    ��";
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
				if(use_check() == 0) user_print();
				break;
			case 2: // �¼� ����
				seat_state();
				break;
			case 3: // ���
				system("cls");
				gotoxy(20, 18);
				cout << "<<<<<<<  ��� �Ϸ�  >>>>>>>" << endl;
				Sleep(2100);
				return 0;
			default:
				nothingMenu();
				user_print();
		}
	}

	// �̿�� Ȯ�� ���
	int use_check() {
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
				if(use_buy() == 0) use_check();
				break;
			case 2:
				return 0;
			default: 
				nothingMenu();
				use_check();
		}
	}
	// �̿�� ����
	int use_buy() {
		system("cls");
		int use_ticket = 0, time = 2;

		gotoxy(14, 5);
		cout << "����������������������������������������������������������������������������������������" << endl;
		gotoxy(14, 6);
		cout << "��                �̿�� ����               ��";
		gotoxy(14, 7);
		cout << "����������������������������������������������������������������������������������������" << endl;

		
		gotoxy(10, 9);

		// ���� �ð� ��� 
		string Query = "select time_format(sec_to_time(resudual_time), '%k�ð� %i�� %s��') from student where student_id = '" + input_id + "';";
		Stat = mysql_query(connPtr, Query.c_str());

		if (Stat != 0) {
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(5000);
		}
		Result = mysql_store_result(&conn);

		while ((Row = mysql_fetch_row(Result)) != NULL) {
			cout << Row[0];
		}

		cout << " �����̽��ϴ�. �����Ͻ� �̿���� �������ּ���!";
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
		// ���� ���̺�
		db.buyTicket(input_id, use_ticket);
		db.trigger_buyTicket();

		char YorN[5];
		gotoxy(19, 33);
		cout << "�߰� �����Ͻðڽ��ϱ�? (y/n) => " ;
		cin >> YorN;
		if (YorN == "y") {
			gotoxy(15, 37);
			cout << "�̿�� ���� �Է� >> ";
			cin >> use_ticket;
			db.buyTicket(input_id, use_ticket);
		}
		else if (YorN == "n") {
			gotoxy(19, 35);
			cout << "============= �̿�� ���� ���� ==============";
			Sleep(1500);
			return 0;
		}
	}

	// �Խ� 
	void comeIn() {
		gotoxy(15, 23);
		cout << "�ڸ� �Է� : ";
		cin >> seat_num;
	
		// �ڸ� �뿩
		string Query = "insert into Rent(student_id, seat_no) values('" + input_id + "',  " + to_string(seat_num) + "); ";
		Query += "update seat set rent_q = 0 where seat_no = "+ to_string(seat_num) +";";
		Stat = mysql_query(connPtr, Query.c_str());

		if (Stat != 0) {
			cout << endl << endl << endl << endl ;
			cout << "\t\t\t\t___________________________________________" << endl;
			cout << "\t\t\t\t    ** �� �¼��� �̹� ��� ���Դϴ�! **" << endl << endl;
			cout << "\t\t\t\t      ** �ٸ� �¼��� �̿����ּ���. **" << endl;
			cout << "\t\t\t\t-------------------------------------------" << endl;
			cout << endl;
			Sleep(5000);
		}
		else {
			cout << "\t\t\t\t        <<<<<< �Խ��� �Ϸ�Ǿ����ϴ� >>>>>>       " << endl;
			Sleep(4000);
		}
		Result = mysql_store_result(connPtr);
	}
};

class Manager : public User {
public:
	Manager() {}
	~Manager() { }

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
		gotoxy(19, 29);
		cout << "4. ���� ����" << endl;

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
			case 4:
				system("cls");
				cout << "���� ���� Ȯ��" << endl;
				break;
			default:
				nothingMenu();
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
			if (use.signin() == 0)  
				m.manager_print();
			else {
				if(s.user_print() == 0) DrawMenu();
			}
			Control();
			break;
		case SIGNUP:
			use.signup();
			if (use.signin() == 0)  
				m.manager_print();
			else s.user_print();
			break;
		case END:
			cout << "\n\n\n\n\n           * * * * * * * * * * * * * * �۽� 3���� �����մϴ� * * * * * * * * * * * * * * " << endl;
			system("pause>null");
			exit(0);
			return 0;
	}
}

void connectDB() {
	mysql_init(&conn);
	connPtr = mysql_real_connect(&conn, "localhost", "root", "hope@153", "studyCafe", 3306, NULL, 0);
	if (connPtr == NULL) cout << "error : " << mysql_error(&conn);
	else {}
}