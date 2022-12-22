#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
using namespace std;
//■
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
int id, dir, rem, nxt_id, hsc, sc = 0, h = 30, x = 0, y = 12, ms = 175, dif = 3, mp[50][50];
int clr[19] = {14, 12, 12, 10, 10, 9, 9, 13, 13, 13, 13, 11, 11, 11, 11, 8, 8, 8, 8};
void Pos(int xx, int yy) {
    COORD posPoint = {yy, xx}; //设置坐标
    SetConsoleCursorPosition(hout, posPoint);
}
void Cursor(bool show){
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hout,&CursorInfo);
	CursorInfo.bVisible=show;
	SetConsoleCursorInfo(hout,&CursorInfo);
}
void cls() {
	Pos(0, 0);
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 48; j++) {
			cout << " ";
		}
		cout << "\n";
	}
}
void ini() {
	cls();
	for (int i = 0; i < 25; i++) {
		for (int j = 1; j <= 13; j++) {
			mp[i][j] = 0;
		}
	}
	h = 30, x = 0, y = 12, sc = 0;
}
int cg(int n) {
	return n == 0 ? 0 : (n == 1 ? 1 : (n == 2 ? 3 : (n == 3 ? 5 : (n == 4 ? 7 : (n == 5 ? 11 : 15)))));
}
int Rotate(int n) {
	return n == 0 ? 0 : 
		(n == 2 ? 1 : (n == 4 ? 3 : (n == 6 ? 5 : (n == 10 ? 7 : (n == 14 ? 11 : (n == 18 ? 15 : n + 1))))));
}
int csc(int n) {
	return n == 4 ? 100 : (n == 3 ? 60 : (n == 2 ? 30 : (n == 1 ? 10 : 0)));
}
inline void color(int col) {
	SetConsoleTextAttribute(hout, col);
}
/*1 深蓝色   2 深绿色   3 深青色  4 深红色   5 深粉色   6 黄色   7 深白色   8 灰色   9 浅蓝色
10 浅绿色    11 浅青色    12 浅红色    13 浅粉色    14 浅黄色    15 浅白色 */
int xy[19][8] = {
	{0, 0, 0, 1, 1, 0, 1, 1}, //block;
	{0, 0, 1, 0, 2, 0, 3, 0}, {0, 0, 0, 1, 0, 2, 0, 3}, // | ;
	{1, 0, 0, 1, 1, 1, 0, 2}, {0, 0, 1, 0, 1, 1, 2, 1}, //lightning 1;
	{0, 0, 0, 1, 1, 1, 1, 2}, {0, 1, 1, 0, 1, 1, 2, 0}, //lightning 2;
	{1, 0, 0, 1, 1, 1, 1, 2}, {0, 0, 1, 0, 2, 0, 1, 1}, {0, 0, 0, 1, 0, 2, 1, 1}, {0, 1, 1, 0, 1, 1, 2, 1},//T-shaped;
	{0, 0, 1, 0, 1, 1, 1, 2}, {0, 0, 1, 0, 2, 0, 0, 1}, {0, 0, 0, 1, 0, 2, 1, 2}, {0, 1, 1, 1, 2, 1, 2, 0},//L-shaped1;
	{1, 0, 1, 1, 1, 2, 0, 2}, {0, 0, 1, 0, 2, 0, 2, 1}, {0, 0, 1, 0, 0, 1, 0, 2}, {0, 0, 0, 1, 1, 1, 2, 1}//L-shaped 2;
};
void DrawMap() {
	color(15);
	for (int i = 0; i < 25; i++) {
		Pos(i, 0);
		cout << "■";
		mp[i][0] = 1;
		Pos(i, 28);
		cout << "■";
		mp[i][14] = 1;
		Pos(i, 46);
		cout << "■";
		mp[i][23] = 1;
		mp[25][i] = 1;
	}
	Pos(16, 30);
	for (int i = 0; i < 8; i++) {
		cout << "■";
	}
	Pos(2, 32);
	cout << "Next block :";
	Pos(19, 32);
	color(14);
	cout << "Your Score :";
}
bool OK(int ox, int oy, int oid) {
	for (int i = 0; i < 8; i += 2) {
		if (mp[ox + xy[oid][i]][oy + xy[oid][i + 1]] != 0) {
			return false;
		}
	}
	return true;
}
void Draw(int dx, int dy, int iidd, string strrr) {
	color(clr[iidd]);
	for (int i = 0; i < 7; i += 2) {
		Pos(dx + xy[iidd][i], dy + xy[iidd][i + 1] * 2);
		cout << strrr;
	}
}
void Fall(int ax) {
	Sleep(1);
	for (int i = ax; i >= 0; i--) {
		for (int j = 1; j <= 13; j++) {
			mp[i][j] = (i == 0 ? 0 : mp[i - 1][j]);
		}
	}
	cls();
	DrawMap();
	for (int i = 0; i < 25; i++) {
		for (int j = 1; j <= 13; j++) {
			Pos(i, j * 2);
			if (mp[i][j] != 0) {
				color(mp[i][j]);
				cout << "■";
			} else {
				cout << "  ";
			}
		}
	}
}
void End() {
	color(8);
	for (int i = 24; i >= 0; i--) {
		Pos(i, 2);
		for (int j = 1; j <= 13; j++) {
			cout << (mp[i][j] == 0 ? "  " : "■");
		}
		Sleep(10);
	}
	color(15);
	Pos(10, 6);
	cout << "+----------------+";
	Pos(11, 6);
	cout << "|                |";
	Pos(12, 6);
	cout << "|   You Lose.    |";
	Pos(13, 6);
	cout << "|                |";
	Pos(14, 6);
	cout << "+----------------+";
	Sleep(2000);
	getch();
	cls();
	Pos(0, 0);
	cout << "Final Score : " << sc << "\n";
	cout << "Wanna play again ? [Y / N]";
	Cursor(true);
	char c;
	cin >> c;
	Cursor(false);
	if (c == 'Y' || c == 'y') {
		return ;
	}
	exit(0);
}
void Updata() {
	rem = 0;
	for (int i = 0; i < 8; i += 2) {
		mp[x + xy[id][i]][y / 2 + xy[id][i + 1]] = clr[id];
	}
	h = min(h, x);
	int cnt = 0;
	for (int i = 0; i < 25; i++) {
		for (int j = 1; j <= 13; j++) {
			cnt += (mp[i][j] == 0);
		}
		if (cnt == 0) {
			Pos(i, 2);
			color(15);
			
			for(int j = 1; j <= 13; j++) {
				mp[i][j] = 0;
				cout << "■";
			}
			Fall(i);
			rem++;
			h++;
		}
		cnt = 0;
	}
	sc += csc(rem);
	if (h == 0) {
		End();
	}
}
void Run() {
	id = cg(rand() % 7);
	while (1) {
		nxt_id = cg(rand() % 7);
		Draw(4, 35, id, "  ");
		Draw(4, 35, nxt_id, "■");
		Draw(x, y, id, "■");
		while (OK(x + 1, y / 2, id)) {
			if (OK(x + 1, y / 2, id)) {
				Draw(x, y, id, "  ");
				x++;
				Draw(x, y, id, "■");
			}
			for (int asdf = 0; asdf < 10; asdf++) {
				if (kbhit()) {
					dir = getch();
					dir = (dir == 'w' ? 72 : (dir == 'a' ? 75 : (dir == 's' ? 80 : (dir == 'd' ? 77 : dir))));
					switch (dir) {
						case 72 : // w
							if (OK(x, y / 2, Rotate(id))) {
								Draw(x, y, id, "  ");
								id = Rotate(id);
								Draw(x, y, id, "■");
							}
							break;
						case 75 : // a 
							if (OK(x, y / 2 - 1, id)) {
								Draw(x, y, id, "  ");
								y -= 2;
								Draw(x, y, id, "■");
							}
							break;
						case 80 : // s
							if (OK(x + 2, y / 2, id)) {
								Draw(x, y, id, "  ");
								x += 2;
								Draw(x, y, id, "■");
							}
							break;
						case 77 : // d
							if (OK(x, y / 2 + 1, id)) {
								Draw(x, y, id, "  ");
								y += 2;
								Draw(x, y, id, "■");
							}
							break;
						case 32 :
							Pos(10, 33);
							cout << "game paused";
							getch();
							cout << "\b \b\b \b\b \b\b \b\b \b\b \b\b\b \b\b \b\b \b\b \b";
							break;
					}
				}
			}
			if (kbhit()) {
				dir = getch();
				dir = (dir == 'a' ? 75 : (dir == 'd' ? 77 : dir));
				if (OK(x, y / 2 - 76 + dir, id) && abs(dir - 76) == 1) {
					Draw(x, y, id, "  ");
					y = y - 152 + dir * 2;
					Draw(x, y, id, "■");
				}
			}
			Sleep(ms);
		}
		Updata();
		color(14);
		Pos(20, 37);
		cout << sc;
		if (h == 0) {
			break;
		}
		id = nxt_id;
		x = 0, y = 12;
	}
}
int main() {
	srand((unsigned)time(NULL));
	Cursor(false);
	while (1) {
		color(15);
		DrawMap();
		Run();
		ini();
	}
	return 0;
}
