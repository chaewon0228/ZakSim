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

string input_id = "", name = "";
int seat_num = 0;

clock_t start, end;
double duration;


class Database {
	// 대여 가능 여부 ( 0:사용중 / 1 : 미사용 )
	int possible[100];
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

	// 잔여 이용 시간
	char* residual_time() {
		string Query = "select time_format(sec_to_time(resudual_time), '%k시간 %i분 %s초') from student where student_id = '" + input_id + "';";
		Stat = mysql_query(connPtr, Query.c_str());
		
		if (Stat != 0) {
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(5000);
		}
		Result = mysql_store_result(&conn);

		while ((Row = mysql_fetch_row(Result)) != NULL) {
			char* t = Row[0];
			return t;
		}
	}
	// 잔여 이용 시간 숫자
	char* residual_time_int() {
		string Query = "select resudual_time from student where student_id = '" + input_id + "';";
		Stat = mysql_query(connPtr, Query.c_str());

		if (Stat != 0) {
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(5000);
		}
		Result = mysql_store_result(&conn);

		while ((Row = mysql_fetch_row(Result)) != NULL) {
			char* t = Row[0];
			return t;
		}
		
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

	void update_re(string id, int mi) {
		string Query = "update student set resudual_time = "+ to_string(mi) +" where student_id = '"+id+"'; ";
		Stat = mysql_query(connPtr, Query.c_str());
		if (Stat != 0) {
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(5000);
		}
	}
	// 좌석 상태
	void seat_state() {
		int i = 0, j = 0;
		for(int k=0; k<20; k++) possible[k] = 0;

		string Query = "select * from seat;";
		Stat = mysql_query(connPtr, Query.c_str());
		if (Stat != 0) {
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(9000);
		}
		Result = mysql_store_result(&conn);
		while ((Row = mysql_fetch_row(Result)) != NULL) {
			possible[i] = stoi(Row[1]);
			i++;
		}
		 
		int index = 0;
		for (i = 0; i < 20; i++) {
			if (i < 9) {
				if (i == 0 || i==5) {
					cout << "\t┌──── 0" << i + 1 << " ────┐   ";
				}
				else cout << " ┌──── 0" << i + 1 << " ────┐   ";
			}
			else {
				if (i == 10 || i == 15 || i == 20) {
					cout << "\t┌──── " << i + 1 << " ────┐   ";
				}
				else cout << " ┌──── " << i + 1 << " ────┐   ";
			}
			if (i == 4 || i == 9 || i == 14 || i == 19) {
				j = 0;
				cout << endl;
				while (j < 5) {
					if (possible[index++] == 1) {
						if (j != 0) cout << " │            │   ";
						else cout << "\t│            │   ";
					}
					else {
						if (j != 0) {
							cout << " │   ";
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
							cout << "사용중";
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
							cout << "   │   ";
						}
						else {
							cout << "\t│   ";
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
							cout << "사용중";
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
							cout << "   │   ";
						}
					}
					j++;
				}
				cout << endl;
				for (int k = 0; k < 5; k++) {
					if(k==0) cout << "\t└────────────┘   ";
					else cout << " └────────────┘   ";
				}
				cout << endl << endl;
			}
		}
	}

	void update_rentQ(int seat_num) {
		string Query = "update seat set rent_q = 0 where seat_no = " + to_string(seat_num) + ";";
		Stat = mysql_query(connPtr, Query.c_str());
		if (Stat != 0) {
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(5000);
		}
	}
	void update_NorentQ(int seat_num) {
		string Query = "update seat set rent_q = 0 where seat_no = " + to_string(seat_num) + ";";
		Stat = mysql_query(connPtr, Query.c_str());
		if (Stat != 0) {
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(5000);
		}
	}
	// 입실 
	int comeIn(string id) {
		cout << "\n\n\n\t\t\t좌석 번호 입력 : ";
		cin >> seat_num;

		if (1 <= seat_num && seat_num <= 20) {
			// 자리 대여
			if (possible[seat_num-1] == 0) {
				cout << endl << endl;
				cout << "\t\t\t\t___________________________________________" << endl;
				cout << "\t\t\t\t    ** 이 좌석은 이미 사용 중입니다! **" << endl << endl;
				cout << "\t\t\t\t      ** 다른 좌석을 이용해주세요. **" << endl;
				cout << "\t\t\t\t-------------------------------------------" << endl;
				cout << endl;
				Sleep(3000);
				return -1;
			}
			else {
				string Query = "insert into Rent(student_id, seat_no) values('" + id + "',  " + to_string(seat_num) + "); ";
				Stat = mysql_query(connPtr, Query.c_str());
				
				if (Stat != 0) {
					fprintf(stderr, "error:%s", mysql_error(&conn)); Sleep(5000);
					return -1;
				}
				else {
					cout << "\t\t\t\t    <<<<<< 입실이 완료되었습니다 >>>>>>       " << endl;
					Sleep(3000);

					// 시간 측정 시작
					start = clock();
					update_NorentQ(seat_num);
					return 0;
				}
				Result = mysql_store_result(connPtr);
			}
		}
		else {
			cout << "\n\n\n\t\t\t자리가 존재하지 않습니다! 다시 입력해주세요.\n\n";
			return -1;
		}
	}

	// 총 매출
	void all_getMoney() {
		string Query = "select sum(Ticket.price) from Purchase, Ticket where Purchase.ticket_no = Ticket.ticket_no;";
		Stat = mysql_query(connPtr, Query.c_str());
		Result = mysql_store_result(&conn);
		while ((Row = mysql_fetch_row(Result)) != NULL) {
			cout << "\t\t총계 :  "<< Row[0] << "원";
		}
	}
	// 월별 총 매출 
	int month_getMoney(int curr_date) {
		char temp[10];
		string Query = "drop procedure if exists monthly_total_sales; delimiter $$ create procedure monthly_total_sales(in _year int) begin select concat(month(";
		_itoa(curr_date, temp, 10);
		Query += temp;
		Query += "), '월') as '기간', concat(format(sum(Ticket.price), 0), '원') as '매출' from Purchase, Ticket where Purchase.ticket_no = Ticket.ticket_no and year(";
		Query += temp;
		Query += ") = _yeargroup by month(";
		Query += temp;
		Query += ") order by month(";
		Query += temp;
		Query += "); end $$ delimiter; ";
		Query += "call monthly_total_sales(2021);";

		Stat = mysql_query(connPtr, Query.c_str());
		if (Stat != 0) {
			cout << stderr << "error : " << mysql_error(&conn);
			Sleep(5000);
		}
		Result = mysql_store_result(&conn);
		while ((Row = mysql_fetch_row(Result)) != NULL) {
			if (Row[0] == "") cout << "매출이 없습니다!";
			else cout << Row[0] << " |     " << Row[1] << endl;
		}
		mysql_free_result(Result);
		return 0;
	}

	// 구매한 티켓 번호 
	void buyTicketNum(string id, int t_num) {
		string Query = "update student set resudual_time = resudual_time + (select time from ticket where ticket_no = "+to_string(t_num)+") where student_id = '"+id+"';";
		Stat = mysql_query(connPtr, Query.c_str());
		Result = mysql_store_result(&conn);
		cout << endl;
	}

	// 회원 조회 (아이디, 이름, 가입날짜, 잔여시간 )
	void showMember() {
		string Query = "select * from student;";
		Stat = mysql_query(connPtr, Query.c_str());
		Result = mysql_store_result(&conn);
		int i = 1;
		while ((Row = mysql_fetch_row(Result)) != NULL) {
			if(strcmp(Row[0],"admin") == 0) { }
			else {
				cout << "\t\t" << i << ". 아이디 : " << Row[0] << ", 가입 날짜 : " << Row[3] << ", 잔여 이용시간 : " << Row[4] << endl << endl;
				i++;
			}
		}
	}

	// 퇴실
	void outPut(string id) {
		string Query = "update seat set rent_q = 1 where seat_no = any(select seat_no from rent where student_id = '" + id + "');";
		Stat = mysql_query(connPtr, Query.c_str());
		Result = mysql_store_result(&conn);
	}
	
	void insert(string id) {
		string Query = " insert into qa(student_id, q, a) values('"+id+"', '. . .', '. . .');";
		Stat = mysql_query(connPtr, Query.c_str());

		if (Stat != 0) {
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(5000);
		}
		Result = mysql_store_result(&conn);
	}
	// 문의 사항 Q
	void update_Q(string q, string id) {
		string Query = "update qa set q ='"+ q + "' where student_id = '" + id + "'; ";
		Stat = mysql_query(connPtr, Query.c_str());

		if (Stat != 0) {
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(5000);
		}
		Result = mysql_store_result(&conn);
	}
	// 문의 사항 A
	void update_A(string a, string id) {
		string Query = "update qa set a ='" + a + "' where student_id = '" + id + "'; ";
		Stat = mysql_query(connPtr, Query.c_str());

		if (Stat != 0) {
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(5000);
		}
		Result = mysql_store_result(&conn);
	}
	void Q() {
		string Query = "select * from qa";
		Stat = mysql_query(connPtr, Query.c_str());

		if (Stat != 0) {
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(5000);
		}
		Result = mysql_store_result(&conn);
		while ((Row = mysql_fetch_row(Result)) != NULL) {
			cout << "\t\t=>";
			cout << Row[2];
			cout << " : ";
			cout << Row[0] << endl << endl;
		}
	}
	// 문의 사항 QA
	void show_Q() {
		string Query = "select q from qa where student_id = '" + input_id + "'; ";
		Stat = mysql_query(connPtr, Query.c_str());

		if (Stat != 0) {
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(5000);
		}
		Result = mysql_store_result(&conn);
			
		while ((Row = mysql_fetch_row(Result)) != NULL) {
			cout << "\t\t=> ";
			show_id(input_id);
			//cout << " : " << Row[0] << endl;
		}
		
	}
	// 문의 사항 QA
	void show_A() {
		string Query = " select * from qa where student_id = '" + input_id + "'; ";
		Stat = mysql_query(connPtr, Query.c_str());

		if (Stat != 0) {
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(5000);
		}
		Result = mysql_store_result(&conn);
		while ((Row = mysql_fetch_row(Result)) != NULL) {
			cout << Row[1];
		}
	}
	// 문의 사항 QA
	void show_id(string id) {
		string Query = " select * from qa where student_id = '" + id + "'; ";
		Stat = mysql_query(connPtr, Query.c_str());

		if (Stat != 0) {
			fprintf(stderr, "error:%s", mysql_error(&conn));
			Sleep(5000);
		}
		Result = mysql_store_result(&conn);
		while ((Row = mysql_fetch_row(Result)) != NULL) {
			cout << Row[2];
		}
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
	system("title ChoSim StudyCafe");
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

// 현재 날짜 찍기
void print_currTime() {
	time_t timer = time(NULL);
	struct tm* t = localtime(&timer);
	timer = time(NULL);
	t = localtime(&timer);
	
	gotoxy(0, 0);
	cout << t->tm_year + 1900 << "." << t->tm_mon + 1 << "." << t->tm_mday;
	cout << "(";
	
	switch (t->tm_wday) {
		case 0: cout << "일"; break;
		case 1: cout << "월"; break;
		case 2: cout << "화"; break;
		case 3: cout << "수"; break;
		case 4: cout << "목"; break;
		case 5: cout << "금"; break;
		case 6: cout << "토"; break;
	}
	cout << ")";
	cout << " " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec;
}

// 로고 찍기
void DrawMenu() {
	system("cls");
	print_currTime();

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
		input_id = "";
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
				gotoxy(14, 25);
				cout << "비밀번호가 일치하지 않습니다! 다시 입력해주세요.";
				Sleep(2000);
				signin();
			}
			else if (input_id != Row[0] && input_pw == Row[2]) {
				gotoxy(14, 25);
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
};

class studyCafe : public User {
	Database db;
	int h = 0, m = 0, s = 0;
	int menu;
	int mi = 0;
	string q="";
	int input;
public:
	studyCafe() {}
	~studyCafe() {}

	// 사용자 화면 출력 
	int user_print() {
		system("cls");

		gotoxy(16, 5);
		cout << "┌────────────────────────────────────┐" << endl;
		gotoxy(16, 6);
		cout << "│     "<< name << " 회원님, 반갑습니다!    │";
		gotoxy(16, 7);
		cout << "└────────────────────────────────────┘" << endl;
		gotoxy(19, 15);
		cout << "1.  이용권 구매" << endl;
		gotoxy(19, 18);
		cout << "2.  좌석상태 확인 및 입실" << endl;
		gotoxy(19, 21);
		cout << "3.  문의 사항 접수하기" << endl;
		gotoxy(19, 24);
		cout << "4.  퇴실" << endl;
		
		gotoxy(15, 30);
		cout << "메뉴 입력 >> ";
		cin >> menu;
		switch (menu) {
			case 1: // 이용권 구매
				if(use_check() == 0) user_print();
				break;
			case 2: // 좌석 상태
				system("cls");
				if (db.residual_time() == "0시간 0분 0초") {
					gotoxy(20, 18);
					cout << "이용가능한 시간이 없습니다! 이용권을 먼저 구매해주세요.";
					Sleep(3000);
					user_print();
				}
				else {
					gotoxy(21, 3);
					cout << " ◁  좌석 상태  ▷ \n\n\n\n\n";

					db.seat_state();
					if (db.comeIn(input_id) == -1) {
						system("cls");
						gotoxy(21, 3);
						cout << " ◁  좌석 상태  ▷ \n\n\n\n\n";
						db.seat_state();
						db.comeIn(input_id);
					}
					else user_print();
				}
				break;
			case 3:
				system("cls");
				cin.ignore(256, '\n');
				gotoxy(16, 5);
				cout << "┌────────────────────────────────────┐" << endl;
				gotoxy(16, 6);
				cout << "│              문의 사항             │";
				gotoxy(16, 7);
				cout << "└────────────────────────────────────┘" << endl;

				cout << endl << endl << endl;
				cout << "\t\t문의 사항을 남겨 주세요." << endl;
				db.show_Q();
				cout << endl << "\t\t =>";
				getline(cin, q);
				db.update_Q(q, input_id);

				cout << endl << endl << endl <<"\t\t\t\t\t\t\t  =>  ";
				db.show_A();

				gotoxy(16, 30);
				cout << "문의 사항 접수 완료.........";
				

				cout << "\n\n\t\t\t문의사항을 종료하시려면 [SPACE] 를 누르세요.";

				
				input = _getch();
				if (input == SPACE) {
					gotoxy(15, 33);
					cout << "\n\t\t\t\t문의사항 종료.......";
					Sleep(2500);
					user_print();
				}
				
				break;
			case 4: // 퇴실
				system("cls");

				// 시간 측정 종료
				::end = clock();
				duration = (double)(::end - start) / CLOCKS_PER_SEC;

				// 잔여시간 - duration -> (mi) => 트리거 갱신
				mi = atoi(db.residual_time_int()) - (int)duration;
				db.update_re(input_id, mi);

				h = duration / 3600;
				duration = (int)duration % 3600;
				m = duration / 60;
				duration = (int)duration % 60;
				s = duration;
				

				gotoxy(18, 15);
				cout << h << "시간 " << m << "분 " << s << "초 이용하셨습니다." << endl;

				gotoxy(18, 18);
				cout << "<<<<<<<  퇴실 완료  >>>>>>>" << endl;
				db.outPut(input_id);

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

		gotoxy(9, 9);
		cout << db.residual_time();
		cout << " 남으셨습니다. 구매하실 이용권을 선택해주세요!";
		
		gotoxy(19, 12);
		cout << "1. 2시간권 구매  [3천원]" << endl;
		gotoxy(19, 14);
		cout << "2. 4시간권 구매  [5천원]" << endl;
		gotoxy(19, 16);
		cout << "3. 6시간권 구매  [7천원]" << endl;
		gotoxy(19, 18);
		cout << "4. 8시간권 구매  [9천원]" << endl;
		gotoxy(19, 20);
		cout << "5. 12시간권 구매  [1만원]" << endl;
		gotoxy(19, 22);
		cout << "6. 50시간권 구매  [6만원]" << endl;
		gotoxy(19, 24);
		cout << "7. 100시간권 구매 [10만원]" << endl;
		gotoxy(19, 26);
		cout << "8. 150시간권 구매 [16만원]" << endl;
		gotoxy(19, 28);
		cout << "9. 200시간권 구매 [20만원]" << endl;

		gotoxy(15, 32);
		cout << "이용권 종류 입력 >> ";
		cin >> use_ticket;

		gotoxy(15, 32);
		if (use_ticket > 4) {
			if(use_ticket == 4) cout << "8시간권을 선택하셨습니다!" << endl;
			else cout << 50 * (use_ticket - 4) << "시간권을 선택하셨습니다!" << endl;
		}
		else if (use_ticket > 0 && use_ticket < 5) 
			cout << 2 + (2 * (use_ticket - 1)) << "시간권을 선택하셨습니다!" << endl;
		else {
			cout << "존재하지 않는 이용권입니다! 다시 선택해주세요." << endl;
			Sleep(1500);
			use_buy();
		}
		
		// 이용권 구매 
		db.buyTicket(input_id, use_ticket);
		db.buyTicketNum(input_id, use_ticket);
		
		string YorN;
		gotoxy(15, 34);
		cout << "추가 구매하시겠습니까? (Y / N) => " ;
		cin >> YorN;
		if (YorN == "Y") {
			gotoxy(15, 37);
			cout << "이용권 종류 입력 >> ";
			cin >> use_ticket;
			
			db.trigger_buyTicket();
			db.buyTicket(input_id, use_ticket);
		}
		else if (YorN == "N") {
			gotoxy(14, 36);
			cout << "=============== 이용권 선택 종료 ================";
			Sleep(1500);

			system("cls");
			gotoxy(15, 18);
			cout << "<<<<<<<  "<< "투입구에 카드를 넣어주세요." << "  >>>>>>>" << endl;
			Sleep(1300);
			
			gotoxy(6, 25);
			cout << " ============================= $$  결제 완료  $$ =============================" << endl;
			Sleep(2100);

			return 0;
		}
	}
};

class Manager : public User {
	Database db;
	string a;
	int me;
public:
	Manager() {}
	~Manager() { }

	// 관리자 화면 출력
	int manager_print() {
		system("cls");
		int menu = 0;
		gotoxy(16, 5);
		cout << "┌────────────────────────────────────┐" << endl;
		gotoxy(16, 6);
		cout << "│             관리자 계정            │";
		gotoxy(16, 7);
		cout << "└────────────────────────────────────┘" << endl;
		gotoxy(21, 15);
		cout << "1. 좌석 상태" << endl;
		gotoxy(21, 18);
		cout << "2. 회원 / 매출 확인" << endl;
		gotoxy(21, 21);
		cout << "3. 문의 사항" << endl;
		gotoxy(21, 24);
		cout << "4. 종료" << endl;

		gotoxy(15, 30);
		cout << "메뉴 입력 >> ";
		cin >> me;

		int input = 0;
		string id = "";
		switch (me) {
			case 1: // 좌석 상태 조회
				system("cls");
				gotoxy(21, 3);
				cout << " ◁  좌석 상태  ▷ \n\n\n\n\n";
				db.seat_state();
				gotoxy(15, 30);
				cout << "좌석 보기를 종료하시려면 [SPACE] 를 누르세요.";
	
				input = _getch();
				if (input == SPACE) {
					gotoxy(15, 33);
					cout << "좌석 보기 종료.......";
					Sleep(2500);
					manager_print();
				}
				break;
			case 2: // 회원 정보 조회
				system("cls");
				gotoxy(16, 5);
				cout << "┌────────────────────────────────────┐" << endl;
				gotoxy(16, 6);
				cout << "│           회원 정보 조회           │";
				gotoxy(16, 7);
				cout << "└────────────────────────────────────┘" << endl;

				gotoxy(2, 12);
				db.showMember();

				gotoxy(2, 27);
				checkSales();

				gotoxy(15, 30);
				cout << "회원 조회를 종료하시려면 [SPACE] 를 누르세요.";

				input = _getch();
				if (input == SPACE) {
					gotoxy(15, 33);
					cout << "회원 정보조회 종료.......";
					Sleep(2500);
					manager_print();
				}
				break;
			case 3: //문의사항
				system("cls");
				cin.ignore(256, '\n');
				gotoxy(16, 5);
				cout << "┌────────────────────────────────────┐" << endl;
				gotoxy(16, 6);
				cout << "│           문의 사항 조회           │";
				gotoxy(16, 7);
				cout << "└────────────────────────────────────┘" << endl;

				cout << endl << endl << endl;
				db.Q();
				cout << endl << endl;

				cout << endl << endl << endl;
				cout << "\t\t답변을 남길 아이디를 입력하세요. : ";
				cin >> id;

				cout << "\t\t답변을 입력하세요. : ";
				cout << endl << "\n\t\t\t\t\t\t\t =>";
				cin >> a;
				db.update_A(a, id);

				gotoxy(16, 30);
				cout << "문의 사항 답변 완료.........";

				cout << "\n\n\t\t\t문의사항을 종료하시려면 [SPACE] 를 누르세요.";

				input = _getch();
				if (input == SPACE) {
					gotoxy(15, 33);
					cout << "\n\t\t\t\t문의사항 종료.......";
					Sleep(2500);
					manager_print();
				}
				
				break;
			case 4: // 프로그램 종료
				system("cls");
				gotoxy(15, 35);
				cout << "프로그램 종료............................";
				Sleep(2500);
				return 0;
			default:
				nothingMenu();
				manager_print();
		}
	}
	
	// 매출액 확인
	int checkSales() {
		time_t timer = time(NULL);
		struct tm* t = localtime(&timer);
		timer = time(NULL);
		t = localtime(&timer);

		string date = to_string(t->tm_year + 1900) + to_string(t->tm_mon + 1) + to_string(t->tm_mday);

		cout << "\t\t---------------------------------" << endl;
		db.all_getMoney();
		return 0;
	}
	
};
int main() {
	CursorView();
	SetConsoleView();
	connectDB();

	Database db;
	User use;
	Manager m; studyCafe s;
	int re = 0;
	
	// 초기화
	//db.set_ticket();
	///db.set_seat();
	//db.insert(input_id);

	switch (Control()) {
		case SIGNIN:
			if (use.signin() == 0) {
				if(m.manager_print() == 0) main();
			}
			else {
				if (s.user_print() == 0) main();
			}
			break;
		case SIGNUP:
			use.signup();
			if (use.signin() == 0)
				m.manager_print();
			else s.user_print();
			break;
		case END:
			cout << "\n\n\n\n\n  * * * * * * * * * * * * * * 초심 스터디카페 프로그램을 종료합니다 * * * * * * * * * * * * * * " << endl;
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