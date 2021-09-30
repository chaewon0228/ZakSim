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

MYSQL conn; //mysql 구조체
MYSQL* connPtr = NULL; //mysql 핸들
MYSQL_RES* Result; //구조체 포인터
MYSQL_ROW Row; //쿼리 성공시 결과로 나온 행의 정보를 담는 구조체
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
void SetConsoleView() { // 콘솔창 크기
	system("mode con: lines=40  cols=100");
	system("title ZakSim 3day");
}
void CursorView() {
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; // 커서 굵기
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
void gotoxy(int x, int y) { // 좌표
	COORD Pos;
	Pos.X = 2 * x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
// 로고 찍기
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
	cout << "  로 그 인";
	gotoxy(21, 23);
	cout << " 회 원 가 입";
	gotoxy(21, 25);
	cout << "    종 료" << endl;

}
// 메뉴 화살표
void DrawArrow(int& x) {
	if (x <= 0) x = 0;
	else if (x >= 2) x = 2;
	gotoxy(20, 21 + x * 2);
	cout << "▷";
}
// 메뉴 화살표 옮기기
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

// 사용자 클래스
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
		cout << "이름 : ";
		cin >> in_name;
		gotoxy(21, 22);
		cout << "아이디 : ";
		cin >> in_id;
		gotoxy(21, 24);
		cout << "비밀번호 : ";
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
		cout << "아이디 : ";
		cin >> input_id;
		gotoxy(21, 20);
		cout << "비밀번호 : ";
		cin >> input_pw;

		if (input_id == "admin" && input_pw == "1329") return 0;
		else if (input_id == name && input_pw == pw) return 1;
		else {
			gotoxy(9, 25);
			cout << "로그인에 실패했습니다! 아이디와 비밀번호를 다시 한번 확인해주세요.";
			Sleep(2000);
			signin();
		}
	}
	// 좌석 상태
	void seat_state() {
		system("cls");
		int i, j;
		int cnt = 0;
		gotoxy(21, 5);
		cout << " ◁  좌석 상태  ▷ " << endl;

		gotoxy(13, 15);
		cout << " ┌──01──┐ ┌──02──┐ ┌──03──┐ ┌──04──┐ ┌──05──┐ " << endl;
		for (i = 0; i < 5; i++) {
			for (j = 0; j < 5; j++) {
				/*if (seat[i][j].state == 1) {
					gotoxy(13, 16);
					cout << "│";
					gotoxy(13, 17);
					cout << " ";
					gotoxy(13, 18);
					cout << "│";
				}
				else {
					gotoxy(13, 16);
					cout << "│";
					gotoxy(13, 17);
					cout << "사용중";
					gotoxy(13, 18);
					cout << "│";
				}*/
			}
		}
		gotoxy(13, 19);
		cout << "└──────┘ └──────┘ └──────┘ └──────┘ └──────┘" << endl;
	}
};

class studyCafe : public User {
	int menu;
public:
	studyCafe() {}
	~studyCafe() {}
	// 사용자 화면 출력 
	void user_print() {
		system("cls");
		menu = 0;
		gotoxy(16, 5);
		cout << "┌────────────────────────────────────┐" << endl;
		gotoxy(16, 6);
		cout << "│         회원님, 반갑습니다!        │";
		gotoxy(16, 7);
		cout << "└────────────────────────────────────┘" << endl;
		gotoxy(19, 15);
		cout << "1.  이용권 구매" << endl;
		gotoxy(19, 18);
		cout << "2.  좌석상태 확인 및 입실" << endl;
		gotoxy(19, 21);
		cout << "3.  퇴실" << endl;

		gotoxy(15, 30);
		cout << "메뉴 입력 >> ";
		cin >> menu;
		switch (menu) {
			case 1: // 이용권 구매
				use_check();
				break;
			case 2: // 좌석 상태
				seat_state();
				comeIn();
				break;
			case 3: // 퇴실
				system("cls");
				gotoxy(20, 18);
				cout << "<<<<<< 퇴실 완료 >>>>>>" << endl;
				Sleep(2100);
				DrawMenu();
				break;
			default:
				cout << "메뉴 범위를 벗어났습니다! 다시 입력해주세요." << endl;
				Sleep(2000);
				user_print();
		}
	}
	// 이용권 확인 출력
	void use_check() {
		system("cls");
		menu = 0;
		gotoxy(14, 5);
		cout << "┌──────────────────────────────────────────┐" << endl;
		gotoxy(14, 6);
		cout << "│                이용권 구매               │";
		gotoxy(14, 7);
		cout << "└──────────────────────────────────────────┘" << endl;
		gotoxy(19, 15);
		cout << "1.  이용권 구매 및 확인" << endl;
		gotoxy(19, 18);
		cout << "2.  홈으로 돌아기기 " << endl;

		gotoxy(15, 30);
		cout << "메뉴 입력 >> ";
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
	// 이용권 구매
	int use_buy() {
		int use_ticket, time = 2;
		system("cls");
		use_ticket = 0;
		gotoxy(14, 5);
		cout << "┌──────────────────────────────────────────┐" << endl;
		gotoxy(14, 6);
		cout << "│                이용권 구매               │";
		gotoxy(14, 7);
		cout << "└──────────────────────────────────────────┘" << endl;
		gotoxy(10, 9);
		cout << "시간 남으셨습니다. 구매하실 이용권을 선택해주세요!";
		gotoxy(19, 12);
		cout << "1. 2시간권 구매" << endl;
		gotoxy(19, 14);
		cout << "2. 4시간권 구매" << endl;
		gotoxy(19, 16);
		cout << "3. 6시간권 구매" << endl;
		gotoxy(19, 18);
		cout << "4. 12시간권 구매" << endl;
		gotoxy(19, 20);
		cout << "5. 50시간권 구매" << endl;
		gotoxy(19, 22);
		cout << "6. 100시간권 구매" << endl;
		gotoxy(19, 24);
		cout << "7. 150시간권 구매" << endl;
		gotoxy(19, 26);
		cout << "8. 200시간권 구매" << endl;
		gotoxy(15, 29);
		cout << "이용권 종류 입력 >> ";
		cin >> use_ticket;
		gotoxy(19, 31);
		if(use_ticket > 4) cout << 50 * use_ticket - 4 << "시간권을 구매하셨습니다!" << endl;
		else cout << 2 + (2 * (use_ticket - 1)) << "시간권을 구매하셨습니다!" << endl;

		char YorN[5];
		gotoxy(19, 33);
		cout << "추가 구매하시겠습니까? (y/n) => " ;
		cin >> YorN;
		if (YorN == "y") {
			gotoxy(15, 37);
			cout << "이용권 종류 입력 >> ";
			cin >> use_ticket;
		}
		else if (YorN == "n") {
			gotoxy(19, 35);
			cout << "===== 이용권 구매 종료 =====";
			Sleep(1500);
			return 0;
		}
	}
	// 좌석 상태 초기화
	void seat_reset(int a, int b) {

	}
	// 입실 
	void comeIn() {
		int seat_num = 0;
		gotoxy(15, 23);
		cout << "자리 입력 : ";
		cin >> seat_num;

		cout << endl << endl << endl << endl << endl << endl;
		cout << "\t\t\t\t___________________________________________" << endl;
		cout << "\t\t\t\t    ** 이 좌석은 이미 사용 중입니다. **" << endl << endl;
		cout << "\t\t\t\t      ** 다른 좌석을 이용해주세요. **" << endl;
		cout << "\t\t\t\t___________________________________________" << endl;
		cout << endl;
		cout << "\t\t\t\t        <<<<<< 입실이 완료되었습니다 >>>>>>       " << endl;

	}
};
class Manager : public User {
public:
	Manager() {}
	~Manager() { }
	string get_id() { return id; }
	string get_pw() { return pw; }
	// 관리자 화면 출력
	void manager_print() {
		system("cls");
		int menu = 0;
		gotoxy(16, 5);
		cout << "┌────────────────────────────────────┐" << endl;
		gotoxy(16, 6);
		cout << "│             관리자 계정            │";
		gotoxy(16, 7);
		cout << "└────────────────────────────────────┘" << endl;
		gotoxy(19, 17);
		cout << "1. 좌석 상태" << endl;
		gotoxy(19, 21);
		cout << "2. 회원 보기" << endl;
		gotoxy(19, 25);
		cout << "3. 매출액 확인" << endl;

		gotoxy(15, 30);
		cout << "메뉴 입력 >> ";
		cin >> menu;
		switch (menu) {
			case 1:
				seat_state();
				break;
			case 2:
				cout << "회원 보기" << endl;
				break;
			case 3:
				system("cls");
				cout << "매출액 확인" << endl;
				break;
			default:
				cout << "메뉴 범위를 벗어났습니다! 다시 입력해주세요." << endl;
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
			cout << "\n\n\n\n\n           * * * * * * * * * * * * * * 작심 3일을 종료합니다 * * * * * * * * * * * * * * " << endl;
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