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

MYSQL conn; //mysql 구조체
MYSQL* connPtr = NULL; //mysql 핸들
MYSQL_RES* Result; //구조체 포인터
MYSQL_ROW Row; //쿼리 성공시 결과로 나온 행의 정보를 담는 구조체
int Stat; 
void connectDB();

string input_id, name;
int seat_num = 0;


class Database {
public:
	// ticket 값 초기화
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

	// seat 값 초기화
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

	// 이용권 구매
	void buyTicket(string id, int ticket_no) {
		string Query = "insert into purchase(student_id, ticket_no) values('"+ id +"', "+ to_string(ticket_no) +");";
		Stat = mysql_query(connPtr, Query.c_str());
		Result = mysql_store_result(&conn);
	}

	// 잔여 이용시간 갱신 트리거
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

	// 이용 금액
	void get_money() {
		string Query = "select sum(Ticket.price) from Purchase, Ticket where Purchase.ticket_no = Ticket.ticket_no;";
		Stat = mysql_query(connPtr, Query.c_str());
		Result = mysql_store_result(&conn);
	}

	// 구매한 티켓 번호 
	void buyTicketNum(string id) {
		string Query = "select ticket_no from purchase where student_id = '"+ id +"';";
		Stat = mysql_query(connPtr, Query.c_str());
		Result = mysql_store_result(&conn);
	}

	// 티켓 가격
	void ticketPrice(int ticket_no) {
		string Query = "select price from ticket where ticket_no = "+ to_string(ticket_no) +"; ";
		Stat = mysql_query(connPtr, Query.c_str());
		Result = mysql_store_result(&conn);
	}

	// 퇴실
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
	cout << "  로 그 인";
	gotoxy(22, 24);
	cout << " 회 원 가 입";
	gotoxy(22, 26);
	cout << "    종 료" << endl;

}

// 메뉴 화살표
void DrawArrow(int& x) {
	if (x <= 0) x = 0;
	else if (x >= 2) x = 2;
	gotoxy(21, 22 + x * 2);
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

// 메뉴 범위 벗어났을 때 
void nothingMenu() {
	cout << "메뉴 범위를 벗어났습니다! 다시 입력해주세요." << endl;
	Sleep(2000);
}

// 사용자 클래스
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
		cout << "이름 : ";
		cin >> in_name;
		gotoxy(21, 22);
		cout << "아이디 : ";
		cin >> in_id;
		gotoxy(21, 24);
		cout << "비밀번호 : ";
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
		cout << "아이디 : ";
		cin >> input_id;
		gotoxy(21, 20);
		cout << "비밀번호 : ";
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
				cout << "비밀번호가 일치하지 않습니다! 다시 입력해주세요.";
				Sleep(2000);
				signin();
			}
			else if (input_id != Row[0] && input_pw == Row[2]) {
				gotoxy(9, 25);
				cout << "아이디가 일치하지 않습니다! 다시 입력해주세요.";
				Sleep(2000);
				signin();
			}
			else {
				gotoxy(9, 25);
				cout << "로그인에 실패했습니다! 아이디와 비밀번호를 다시 한번 확인해주세요.";
				Sleep(2000);
				signin();
			}
		}
	}

	// 좌석 상태
	void seat_state() {
		system("cls");
		int i, j;
		int seat[20];
		gotoxy(21, 5);
		cout << " ◁  좌석 상태  ▷ " << endl;

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

	// 사용자 화면 출력 
	int user_print() {
		system("cls");
		menu = 0;
		gotoxy(16, 5);
		cout << "┌────────────────────────────────────┐" << endl;
		gotoxy(16, 6);
		cout << "│    "<< name << " 회원님, 반갑습니다!    │";
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
				if(use_check() == 0) user_print();
				break;
			case 2: // 좌석 상태
				seat_state();
				break;
			case 3: // 퇴실
				system("cls");
				gotoxy(20, 18);
				cout << "<<<<<<<  퇴실 완료  >>>>>>>" << endl;
				Sleep(2100);
				return 0;
			default:
				nothingMenu();
				user_print();
		}
	}

	// 이용권 확인 출력
	int use_check() {
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
				if(use_buy() == 0) use_check();
				break;
			case 2:
				return 0;
			default: 
				nothingMenu();
				use_check();
		}
	}
	// 이용권 구매
	int use_buy() {
		system("cls");
		int use_ticket = 0, time = 2;

		gotoxy(14, 5);
		cout << "┌──────────────────────────────────────────┐" << endl;
		gotoxy(14, 6);
		cout << "│                이용권 구매               │";
		gotoxy(14, 7);
		cout << "└──────────────────────────────────────────┘" << endl;

		
		gotoxy(10, 9);

		// 남은 시간 출력 
		string Query = "select time_format(sec_to_time(resudual_time), '%k시간 %i분 %s초') from student where student_id = '" + input_id + "';";
		Stat = mysql_query(connPtr, Query.c_str());

		if (Stat != 0) {
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(5000);
		}
		Result = mysql_store_result(&conn);

		while ((Row = mysql_fetch_row(Result)) != NULL) {
			cout << Row[0];
		}

		cout << " 남으셨습니다. 구매하실 이용권을 선택해주세요!";
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
		// 구매 테이블
		db.buyTicket(input_id, use_ticket);
		db.trigger_buyTicket();

		char YorN[5];
		gotoxy(19, 33);
		cout << "추가 구매하시겠습니까? (y/n) => " ;
		cin >> YorN;
		if (YorN == "y") {
			gotoxy(15, 37);
			cout << "이용권 종류 입력 >> ";
			cin >> use_ticket;
			db.buyTicket(input_id, use_ticket);
		}
		else if (YorN == "n") {
			gotoxy(19, 35);
			cout << "============= 이용권 구매 종료 ==============";
			Sleep(1500);
			return 0;
		}
	}

	// 입실 
	void comeIn() {
		gotoxy(15, 23);
		cout << "자리 입력 : ";
		cin >> seat_num;
	
		// 자리 대여
		string Query = "insert into Rent(student_id, seat_no) values('" + input_id + "',  " + to_string(seat_num) + "); ";
		Query += "update seat set rent_q = 0 where seat_no = "+ to_string(seat_num) +";";
		Stat = mysql_query(connPtr, Query.c_str());

		if (Stat != 0) {
			cout << endl << endl << endl << endl ;
			cout << "\t\t\t\t___________________________________________" << endl;
			cout << "\t\t\t\t    ** 이 좌석은 이미 사용 중입니다! **" << endl << endl;
			cout << "\t\t\t\t      ** 다른 좌석을 이용해주세요. **" << endl;
			cout << "\t\t\t\t-------------------------------------------" << endl;
			cout << endl;
			Sleep(5000);
		}
		else {
			cout << "\t\t\t\t        <<<<<< 입실이 완료되었습니다 >>>>>>       " << endl;
			Sleep(4000);
		}
		Result = mysql_store_result(connPtr);
	}
};

class Manager : public User {
public:
	Manager() {}
	~Manager() { }

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
		gotoxy(19, 29);
		cout << "4. 문의 사항" << endl;

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
			case 4:
				system("cls");
				cout << "문의 사항 확인" << endl;
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
			cout << "\n\n\n\n\n           * * * * * * * * * * * * * * 작심 3일을 종료합니다 * * * * * * * * * * * * * * " << endl;
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