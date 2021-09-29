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

MYSQL conn; //mysql 정보를 담을 구조체
MYSQL* connPtr = NULL; //mysql 핸들
MYSQL_RES* Result; //쿼리 성공시 결과를 담는 구조체 포인터
MYSQL_ROW Row; //쿼리 성공시 결과로 나온 행의 정보를 담는 구조체
int Stat; //쿼리 요청 후 결과 (성공/실패)

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
		cout << "이름 : ";
		cin >> name;
		gotoxy(21, 18);
		cout << "아이디 : ";
		cin >> id;
		gotoxy(21, 20);
		cout << "비밀번호 : ";
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
		cout << "아이디 : ";
		cin >> input_id;
		gotoxy(21, 20);
		cout << "비밀번호 : ";
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
	// 사용자 화면 출력 
	int user_print() {
		system("cls");
		menu = 0;
		gotoxy(15, 8);
		cout << "┌────────────────────────────────────┐" << endl;
		gotoxy(15, 9);
		cout << "│             "<<name<<"님 환영합니다!        │";
		gotoxy(15, 10);
		cout << "└────────────────────────────────────┘" << endl;
		gotoxy(20, 16);
		cout << "1.  이용권 구매" << endl;
		gotoxy(20, 20);
		cout << "2.  좌석상태 확인 및 입실" << endl;
		gotoxy(20, 24);
		cout << "3.  퇴실" << endl;

		while (1) {
			gotoxy(15, 30);
			cout << "메뉴 입력 >> ";
			cin >> menu;
			switch (menu) {
			case 1: // 이용권 구매
				system("cls");
				use_check();
				break;
			case 2: // 좌석 상태
				system("cls");
				seat_state();
				break;
			case 3: // 퇴실
				system("cls");
				gotoxy(20, 18);
				cout << "<<<<<< 퇴실 완료 >>>>>>" << endl;
				Sleep(2000);
				DrawMenu();
				break;
			default:
				cout << "메뉴 범위를 벗어났습니다! 다시 입력해주세요." << endl;
			}
		}
	}
	// 이용권 확인 출력
	void use_check() {
		system("cls");
		menu = 0;
		gotoxy(15, 8);
		cout << "┌──────────────────────────────────────────┐" << endl;
		gotoxy(15, 9);
		cout << "│                이용권 구매               │";
		gotoxy(15, 10);
		cout << "└──────────────────────────────────────────┘" << endl;
		gotoxy(20, 18);
		cout << "1.  이용권 구매 및 확인" << endl;
		gotoxy(20, 20);
		cout << "2.  홈으로 돌아기기 " << endl;

		gotoxy(18, 27);
		cout << "메뉴 입력 >> ";
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
	// 이용권 구매
	void use_buy() {
		system("cls");
		gotoxy(15, 8);
		cout << "┌──────────────────────────────────────────┐" << endl;
		gotoxy(15, 9);
		cout << "│                이용권 구매               │";
		gotoxy(15, 10);
		cout << "└──────────────────────────────────────────┘" << endl;

		gotoxy(22, 12);
		cout << "시간 남으셨습니다. 구매하실 이용권을 선택해주세요!";

		gotoxy(22, 14);
		cout << "1. 2시간권 구매" << endl;
		gotoxy(22, 16);
		cout << "2. 6시간권 구매" << endl;
		gotoxy(22, 18);
		cout << "3. 10시간권 구매" << endl;
		gotoxy(22, 20);
		cout << "4. 14시간권 구매" << endl;
		gotoxy(22, 22);
		cout << "5. 18시간권 구매" << endl;
		
		int use_ticket = 0;
		gotoxy(22, 24);
		cout << "이용권 종류 입력 >> ";
		cin >> use_ticket;

		gotoxy(22, 26);
		cout << use_ticket * 2 << "권을 구매하셨습니다!" << endl;

		char YorN[5];
		gotoxy(22, 37);
		cout << "추가 구매하시겠습니까? (y/n)" << endl;
		cin >> YorN;
		if (strcmp(YorN,"y")==0) {
			gotoxy(22, 24);
			cout << "이용권 종류 입력 >> ";
			cin >> use_ticket;
		}
		else {
			use_check();
		}
	}
	// 좌석 상태 초기화
	void seat_reset(int a, int b) {

	}
	// 입실 
	void comeIn() {
		int i = 0, j = 0;
		gotoxy(15, 25);
		cout << "자리 입력 : ";
		cin >> j;

		cout << endl << endl << endl << endl << endl << endl;
		cout << "\t\t\t\t___________________________________________" << endl;
		cout << "\t\t\t\t    ** 이 좌석은 이미 사용 중입니다. **" << endl << endl;
		cout << "\t\t\t\t      ** 다른 좌석을 이용해주세요. **" << endl;
		cout << "\t\t\t\t___________________________________________" << endl;
		cout << endl;

		cout << "\t\t\t\t        <<<<<< 입실이 완료되었습니다 >>>>>>       " << endl;

	}
	// 좌석 상태
	void seat_state() {
		system("cls");
		int i, j;
		int cnt = 0;

		gotoxy(21, 8);
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
		comeIn();
	}

};
class Manager : public User {
	string id = "admin";
	int pw = 1329;
public:
	Manager() {}
	~Manager() { }

	// 관리자 화면 출력
	int manager_print() {
		system("cls");
		int menu;

		gotoxy(15, 8);
		cout << "┌────────────────────────────────────┐" << endl;
		gotoxy(15, 9);
		cout << "│             관리자 계정            │";
		gotoxy(15, 10);
		cout << "└────────────────────────────────────┘" << endl;
		gotoxy(20, 18);
		cout << "1. 좌석 상태" << endl;
		gotoxy(20, 20);
		cout << "2. 회원 보기" << endl;
		gotoxy(20, 22);
		cout << "3. 매출액 확인" << endl;

		while (1) {
			gotoxy(15, 30);
			cout << "메뉴 입력 >> ";
			cin >> menu;
			switch (menu) {
			case 1:
				system("cls");
				cout << "좌석 상태" << endl;
				break;
			case 2:

				break;
			case 3:
				system("cls");
				cout << "매출액 확인" << endl;
				break;
			default:
				cout << "메뉴 범위를 벗어났습니다! 다시 입력하세요" << endl;
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
			cout << "\n\n\n\n\n           * * * * * * * * * * * * * * 작심 3일을 종료합니다 * * * * * * * * * * * * * * " << endl;
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