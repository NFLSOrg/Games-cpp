#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#define setcolor SetConsoleTextAttribute
#define cls() system("cls")
#define clss() Pos(0, 0)
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
using namespace std;
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
HWND hw = GetForegroundWindow();
string colors[7] = {"white", "red", "blue", "green", "yellow", "purple", "cyan"};
struct grid {
	int owner, armys, type;
} mp[30][30];
struct Position {
	int xx, yy;
} AIPos[7], AIcap[7], AItarg[7];
struct playerank {
	int name, armys, lands;
} ranking[7], recording[7];
struct node {
	int nx, ny, c; string str;
} ;
bool cmp(playerank pa, playerank pb) {
	return (pa.armys == pb.armys ? (pa.lands == pb.lands ? pa.name < pb.name : pa.lands > pb.lands) : 
		pa.armys > pb.armys);
}
string dirrr = "LURD";
ifstream ifile;
vector<Position> initarrp;
int Alive[7], AItargtype[7];// AItargtype : -1 for no targ; 0 for random targ; 1 for defend; 9 for attack capital
int dirx[4] = {0, -1, 0, 1};// L U R D
int diry[4] = {-1, 0, 1, 0};
int players, initarr[1000], nx = 5, ny = 5, dir, H = 20, W = 20, Cs = 40, Ms = 40; //cities, mountains
int tad, alive, turns;
int vis[605][605];
bool debugs = false, finished = false, halfarmy[7], backed[7];
void Pos(int x, int y) {
	COORD cpos = {(short)x, (short)y};
	SetConsoleCursorPosition(hout, cpos);
}
void HideCursor(){
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hout, & CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(hout, & CursorInfo);
}
void ShowCursor(){
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hout, & CursorInfo);
	CursorInfo.bVisible = true;
	SetConsoleCursorInfo(hout, & CursorInfo);
}
void exxit() {
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hStdin, mode);
	exxit();
}
void getFiles(string path, string path2, vector<string>& files) {
    long hFile = 0;
    struct _finddata_t fileinfo;
    string p, p2;
    if ((hFile = _findfirst(p.assign(path).append(path2).append("*").c_str(), &fileinfo)) != -1) {
        do {
            if ((fileinfo.attrib &  _A_SUBDIR)) {
                if(strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) 
                    getFiles(p.assign(path).append("\\"), p2.assign(fileinfo.name).append("\\"), files);
            } else {
                files.push_back(p.assign(path2).append(fileinfo.name));
            }
        } while(_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }  
}
void color(string str) {
	if (str == "dred") {
		setcolor(hout, FOREGROUND_RED);
	} else if (str == "dyellow") {
		setcolor(hout, FOREGROUND_GREEN | FOREGROUND_RED);
	} else if (str == "grey")  {
		setcolor(hout, FOREGROUND_INTENSITY);
	} else if (str == "white") {
		setcolor(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	} else if (str == "red") {
		setcolor(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
	} else if (str == "purple") {
		setcolor(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
	} else if (str == "blue") {
		setcolor(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	} else if (str == "cyan") {
		setcolor(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	} else if (str == "yellow") {
		setcolor(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
	} else if (str == "green") {
		setcolor(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	} 
}
inline int tkdig(int tk) {
	int ret = 0;
	while (tk) {
		tk /= 10;
		ret++;
	}
	return ret;
}
void Custom() {
    
}
string choosemap() {/*
    vector<string> files, mpfiles;
	char buffer[MAX_PATH];
	getcwd(buffer, MAX_PATH);
	string filePath;
	filePath.assign(buffer).append("\\");
	getFiles(filePath, "", files);
    mpfiles.push_back("random");
    for (int i = 0; i < files.size(); i++) {
        if (files[i].size() > 4) {
            if (files[i].substr(0, 4) == "gmap") {
                mpfiles.push_back(files[i].substr(5));
            }
        }
    }
    cls();
    int csd = 0, asdf, sise = mpfiles.size();
    while (asdf != 13) {
        cout << "choose a map:\n";
        for (int i = 0; i < sise; i++) {
            cout << (i == csd ? "> " : "  ");
            cout << mpfiles[i] << "\n";
        }
    //    cout << (csd == sise ? "> " : "  ") << "custom map\n";
        asdf = getch();
        if (asdf == 72) csd = max(csd - 1, 0);
        if (asdf == 80) csd = min(sise - 1, csd + 1);
        clss();
    }
    if (csd != sise)
        return mpfiles[csd];
    else {
        Custom();
        return choosemap();
    }*/
    return "random";
}
void getmap(string str, int a, int b) {
    if (str == "random") {
    	for (int i = 0; i < H; i++) {
    		for (int j = 0; j < W; j++) {
    			initarr[i * H + j] = i * H + j;
    			mp[i + 1][j + 1].armys = 0;
    			mp[i + 1][j + 1].owner = 0;
    			mp[i + 1][j + 1].type = 0;
    		}
    	}
    	random_shuffle(initarr, initarr + 400);
    	for (int i = 0; i < Cs; i++) {
    		mp[initarr[i] / H + 1][initarr[i] % H + 1].type = 1; // cities
    		mp[initarr[i] / H + 1][initarr[i] % H + 1].armys = rand() % 11 + 40;
    	}
    	for (int i = Cs; i < Cs + Ms; i++) {
    		mp[initarr[i] / H + 1][initarr[i] % H + 1].type = -1; // mountains
    	}
    	for (int i = Cs + Ms; i <= Cs + Ms + players; i++) {
    		nx = initarr[i] / H + 1, ny = initarr[i] % H + 1;
        	mp[nx][ny].type = 9, mp[nx][ny].owner = i - Cs - Ms + 1;
    		AIPos[i - Cs - Ms + 1].xx = nx, AIPos[i - Cs - Ms + 1].yy = ny;
    		AIcap[i - Cs - Ms + 1].xx = nx, AIcap[i - Cs - Ms + 1].yy = ny;
    	}
    } else {
        initarrp.clear();
        ifile.open(str, ios::in);
        ifile >> H >> W;
        int a, mxplayers = 0;
        for (int i = 1; i <= H; i++) {
            for (int j = 1; j <= W; j++) {
    			mp[i][j].armys = 0;
    			mp[i][j].owner = 0;
    			mp[i][j].type = 0;
                ifile >> a;
                if (a == -1) 
                    mp[i][j].type = -1;
                else if (a == 0) 
                    mp[i][j].type = 0;
                else if (a == 9) {
                    initarrp.push_back((Position){i, j});
                    mxplayers++;
                } else {
                    mp[i][j].type = 1;
                    mp[i][j].armys = a;
                }
            }// cout << "\n";
        }
        mxplayers = min(5, mxplayers);
        players = min(players, mxplayers);
        random_shuffle(initarrp.begin(), initarrp.end());
        alive = players + 1;
        for(int i = 0; i <= players; i++) {
            nx = initarrp[i].xx, ny = initarrp[i].yy;
            mp[nx][ny].type = 9, mp[nx][ny].owner = i + 1;
            AIPos[i + 1].xx = nx, AIPos[i + 1].yy = ny;
            AIcap[i + 1].xx = nx, AIcap[i + 1].yy = ny;
        }
        ifile.close();
    }
    nx = AIcap[1].xx, ny = AIcap[1].yy;
}
void ini() {
    getmap(choosemap(), 0, 0);
	finished = false, debugs = false, turns = 0;
	for (int i = 0; i < H; i++) {
		Alive[i % 7] = 1;
		AItarg[i % 7].xx = -1;
		AItargtype[i % 7] = -1;
		backed[i % 7] = false;
		halfarmy[i % 7] = false;
	}
	for (int i = 0; i < 7; i++) {
		ranking[i].name = i + 1;
	}
    //getmap("random", 0, 0);
	//mp[nx][ny].armys = 99999;
}
bool see(int xx, int yy, int seeker) {
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (mp[xx + i][yy + j].owner == seeker) {
				return true;
			}
		}
	}
	return false;
}
Position seecapital(int xx, int yy) {
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (mp[xx + i][yy + j].type == 9) {
				return (Position){xx + i, yy + j};
			}
		}
	}
	return (Position){-1, -1};
}
void putgrid(int armyss, int i, int j) {
	string outputstr = "";
	if (armyss / 10000 != 0) {
		outputstr += (char)(armyss / 10000 + 48);
		outputstr += 'W';
	} else if (armyss / 1000 != 0) {
		outputstr += (char)(armyss / 1000 + 48);
		outputstr += 'K';
	} else if (armyss / 100 != 0) {
		outputstr += (char)(armyss / 100 + 48);
		outputstr += 'H';
	} else if (armyss / 10 != 0) {
		outputstr += (char)(armyss / 10 + 48);
		outputstr += (char)(armyss % 10 + 48);
	} else {
		outputstr += '0';
		outputstr += (char)(armyss + 48);
	}
	if (!see(i / 2 + 1, j, 1) && (!debugs)) {
		color("grey");
		fputs((mp[i / 2 + 1][j].type == -1 || mp[i / 2 + 1][j].type == 1) ? "/\\/\\" : "    ", stdout);
		return ;
	}
	if (halfarmy[1] && (i / 2 + 1 == nx) && j == ny) {
		color("dred");
	}
	if (mp[i / 2 + 1][j].type == 9) {
        cout << "{" << outputstr << "}";
	} else if (mp[i / 2 + 1][j].type == 1 || mp[i / 2 + 1][j].type == 2) {
		cout << "[" << outputstr << "]";
	} else if (mp[i / 2 + 1][j].type == -1) {
		fputs("/\\/\\", stdout);
	} else if (mp[i / 2 + 1][j].type == 0) {
		cout << " " << (mp[i / 2 + 1][j].owner != 0 ? outputstr : "  ") << " ";
	}
	color("white");
}
void UpData() {
	for (int i = 1; i <= H; i++) {
		for (int j = 1; j <= W; j++) {
			if (mp[i][j].owner != 0) {
				recording[mp[i][j].owner - 1].armys += mp[i][j].armys;
				recording[mp[i][j].owner - 1].lands++;
			}
		}
	}
	for (int i = 0; i <= players; i++) {
		ranking[i] = recording[i];
	}
}
void ranklist() {
	Pos(106, 0);
	cout << "--------------Ranklist--------------\n";
	Pos(106, 1);
	cout << "    player     armies      lands\n";
	UpData();
	sort(ranking, ranking + players + 1, cmp);
	for (int i = 0; i <= players; i++) {
		Pos(112, i + 2);
		color(colors[ranking[i].name + 1]);
		cout << ranking[i].name + 1 << "          " << ranking[i].armys;
		int dddd = tkdig(ranking[i].armys);
		for (int j = 0; j < 12 - dddd; j++) {
			cout << " ";
		}
		cout << ranking[i].lands << "     ";
		recording[i].name = i, recording[i].armys = 0, recording[i].lands = 0;
	}/*
	Pos(112, 10);
	for (int i = 1; i <= players + 1; i++) {
		Pos(112, 10 + i);
		cout << AItarg[i].xx << " " << AItarg[i].yy << " " << AIPos[i].xx << " " << AIPos[i].yy << "   ";
	}*/
}
void Print() {
	Pos(0, 0);
	bool flag = false, flag2 = false;
	for (int i = 0; i < H * 2 + 1; i++) {
		for (int j = 1; j <= W; j++) {
			if ((i & 1) == 0) {
				flag2 = flag;
				if (nx == i / 2 + 1 && j == ny || nx == (i - 1) / 2 + 1 && j == ny) {
					color("dyellow");
					flag = true;
				} else {
					color("white");
				}
				if (!see(i / 2 + 1, j, 1)) {
					color("grey");
				} 
				cout << "+";
				if (flag2) {
					flag = false;
					flag2 = false;
					color("white");
				}
				if (nx == i / 2 + 1 && j == ny || nx == (i - 1) / 2 + 1 && j == ny) {
					color("dyellow");
					flag = true;
				} else {
					color("white");
				}
				if (!see(i / 2 + 1, j, 1) && (!see(i / 2, j, 1))) {
					color("grey");
				}
				fputs("----", stdout);
			} else {
				if (nx == i / 2 + 1 && (j == ny || j == ny + 1)) {
					color("dyellow");
				} else {
					color("white");
				}
				if (!see(i / 2 + 1, j, 1) && (!see(i / 2 + 1, j - 1, 1))) {
					color("grey");
				}
				fputs("|", stdout);
				color("white");
				color(colors[mp[i / 2 + 1][j].owner]);
				putgrid(mp[i / 2 + 1][j].armys, i, j);
			}
		}
		cout << ((i & 1) ? "|\n" : "+\n");
		color("white");
	}
	ranklist();
}
void Welcome() {
	system("cls");
	color("cyan");
	cout << "Welcome to generals.exe!\n";
	cout << "w a s d for operations, but arrow keys also allowed (but slower);\n";
	cout << "r for restart, p for seeing the whole map, space for pause, Esc for exit.\n";
	cout << "How many artificial idiots do you want? [1, 5]\n";
	ShowCursor();
	cin >> players;
	players = (players >= 6 ? 5 : players);
	alive = players + 1;
	HideCursor();
}
void Updatecity() {
	for (int i = 1; i <= H; i++) {
		for (int j = 1; j <= W; j++) {
			if (mp[i][j].owner != 0 && mp[i][j].type >= 1) {
				mp[i][j].armys++;
			}
		}
	}
}
void Updateland() {
	for (int i = 1; i <= H; i++) {
		for (int j = 1; j <= W; j++) {
			if (mp[i][j].owner != 0 && mp[i][j].type == 0) {
				mp[i][j].armys++;
			}
		}
	}
}
void Capture(int er, int ed) {
	for (int i = 1; i <= H; i++) {
		for (int j = 1; j <= W; j++) {
			if (mp[i][j].owner == ed) {
				mp[i][j].armys /= 2;
				mp[i][j].owner = er;
				mp[i][j].armys = max(mp[i][j].armys, 1);
			}
		}
	}
	mp[AIcap[ed].xx][AIcap[ed].yy].type = 2;
	Alive[ed] = 0;
	alive--;
	if (alive == 1) {
		int survived;
		for (survived = 1; survived < 7; survived++) {
			if (Alive[survived] == 1) {
				break;
			}
		}
		debugs = true;
		Print();
		Pos(40, 0);
		color("yellow");
		cout << "Player" << survived << " won!";
		Sleep(3000);
		getch();
		finished = true;
		return ;
	}
}
int MarchTo(int sx, int sy, int ex, int ey, int who) {
	int usable = (mp[sx][sy].armys / ((sx == nx && sy == ny && halfarmy[who]) + 1)) - 1 + halfarmy[who];
	int left = mp[sx][sy].armys - usable;
	if (mp[sx][sy].owner != who || mp[ex][ey].type == -1 || ex < 1 || ey < 1 || ex > H || ey > W || 
		(who >= 2 && mp[ex][ey].type == 1 && mp[ex][ey].armys > mp[sx][sy].armys)) {
		return -1;
	}
	if (mp[sx][sy].owner == mp[ex][ey].owner) {
		mp[ex][ey].armys += usable;
	} else {
		if (usable > mp[ex][ey].armys) {
			mp[ex][ey].armys = usable - mp[ex][ey].armys;
			if (mp[ex][ey].type == 9) {
				Capture(mp[sx][sy].owner, mp[ex][ey].owner);
			}
			mp[ex][ey].owner = mp[sx][sy].owner;
		} else if (usable < mp[ex][ey].armys) {
			mp[ex][ey].armys = mp[ex][ey].armys - usable;
		} else if (mp[ex][ey].owner == 0 && mp[ex][ey].type == 0) {
			return 0;
		} else {
			mp[ex][ey].armys = 0;
		}
	}
	mp[sx][sy].armys = left;
	if (sx == nx && sy == ny && who == 1) {
		halfarmy[who] = false;
	} else if (sx == AIPos[who].xx && sy == AIPos[who].yy) {
		halfarmy[who] = false;
	}
	return 1;
}
Position mostarmy(int numm) {
	Position ret;
	ret.xx = AIcap[numm].xx, ret.yy = AIcap[numm].yy;
	int mostarm = -1;
	for (int i = 1; i <= H; i++) {
		for (int j = 1; j <= W; j++) {
			if (mp[i][j].owner == numm && mp[i][j].armys > mostarm && mp[i][j].type != 9) {
				ret.xx = i, ret.yy = j;
				mostarm = mp[i][j].armys;
			}
		}
	}
	return ret;
}
string bfs(int sx, int sy, int ex, int ey) {
	queue<node> q;
	int mpp[50][50];
	string anss = "asdf"; int ans;
	q.push((node){sx, sy, 0, ""});
	for (int i = 0; i <= H; i++) {
		for (int j = 0; j <= W; j++) {
			vis[i][j] = 191981000;
			mpp[i][j] = mp[i][j].armys + 1;
		}
	}
	while (!q.empty()) {
		for (int i  = 0; i < 4; i++) {
			int xx = q.front().nx, yy = q.front().ny, cst = q.front().c;
			string nstep = q.front().str;
			xx += dirx[i], yy += diry[i];
			if ((cst + mpp[xx][yy] >= vis[xx][yy]) || xx <= 0 || yy <= 0 || xx > H || yy > W 
				|| mp[xx][yy].type == -1) {
				continue;
			} else if (xx == ex && yy == ey) {
				nstep += dirrr[i];
				vis[xx][yy] = cst + mpp[xx][yy];
				ans = cst, anss = nstep;
				continue;
			} else {
				nstep += dirrr[i];
				vis[xx][yy] = cst + mpp[xx][yy];
				q.push((node){xx, yy, cst + mpp[xx][yy], nstep});
			}
		}
		q.pop();
	}
	return anss;
}
Position PickForeign(int who) {
	vector<Position> vec;
	vec.clear();
	for (int i = 1; i <= H; i++) {
		for (int j = 1; j <= W; j++) {
			if (mp[i][j].owner != who && mp[i][j].type != -1) {
				vec.push_back((Position){i, j});
			}
		}
	}
	return vec[rand() % vec.size()];
}
bool capitalseen(int whose) {
	int capx = AIcap[whose].xx, capy = AIcap[whose].yy;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (mp[capx + i][capy + j].owner != whose && mp[capx + i][capy + j].owner != 0) {
				return true;
			}
		}
	}
	return false;
}
void MoveAI(int num) {
	int nposx = AIPos[num].xx, nposy = AIPos[num].yy, opt = rand() % (turns + 5), AIdir = rand() % 4;
	if (mp[nposx][nposy].armys == 0 || mp[nposx][nposy].armys == 1 || mp[nposx][nposy].owner != num || opt == 1) {
		Position mostarm = mostarmy(num);
		nposx = mostarm.xx, nposy = mostarm.yy;
		if (mp[AIcap[num].xx][AIcap[num].yy].armys > mp[nposx][nposy].armys) {
			nposx = AIcap[num].xx, nposy = AIcap[num].yy;
			halfarmy[num] = true;
		}
	}
	if (capitalseen(num) && (!backed[num])) {
		Position mostarm = mostarmy(num);
		nposx = mostarm.xx, nposy = mostarm.yy;
		AItarg[num] = AIcap[num];
		AItargtype[num] = 1;
	}
	if (!capitalseen(num)) {
		backed[num] = false;
	}
	if (AItarg[num].xx == -1) {
		AIdir = rand() % 4;
		int pick = rand() % 10;
		if (pick == 1) {
			Position tar = PickForeign(num);
			AItarg[num] = tar;
			AItargtype[num] = 0;
		}
	} else {
		int targx = AItarg[num].xx, targy = AItarg[num].yy;
		string adv = bfs(nposx, nposy, targx, targy);
		Pos(105, 10 + num);
	//	cout << adv.substr(0, 5);
		if (adv[0] == 'U') { AIdir = 0; }
		else if (adv[0] == 'D') { AIdir = 1; }
		else if (adv[0] == 'R') { AIdir = 2; }
		else if (adv[0] == 'L') { AIdir = 3; }
	//	cout << AIdir;
	//	getch();
	}
	if (AIdir == 0) {
		nposx -= (MarchTo(nposx, nposy, nposx - 1, nposy, num) == 1); // up
	} else if (AIdir == 1) {
		nposx += (MarchTo(nposx, nposy, nposx + 1, nposy, num) == 1); // down
	} else if (AIdir == 2) {
		nposy += (MarchTo(nposx, nposy, nposx, nposy + 1, num) == 1); // right
	} else if (AIdir == 3) {
		nposy -= (MarchTo(nposx, nposy, nposx, nposy - 1, num) == 1); // left
	}
	AIPos[num].xx = nposx, AIPos[num].yy = nposy;
	if (AIPos[num].xx == AIcap[num].xx && AIPos[num].yy == AIcap[num].yy) {
		halfarmy[num] = true;
	}
	Position pers = seecapital(nposx, nposy);
	if (pers.xx != -1 && mp[pers.xx][pers.yy].owner != num) {
		AItarg[num].xx = pers.xx, AItarg[num].yy = pers.yy;
		AItargtype[num] = 9;
	}
	if (mp[AItarg[num].xx][AItarg[num].yy].type != 9 && AItargtype[num] == 9) {
		AItarg[num].xx = -1;
	} else if (nposx == AItarg[num].xx && nposy == AItarg[num].yy && AItargtype[num] == 0 && AItarg[num].xx != -1) {
		AItarg[num].xx = -1;
	} else if (mp[AItarg[num].xx][AItarg[num].yy].type == -1) {
		AItarg[num].xx = -1;
	} else if (nposx == AItarg[num].xx && nposy == AItarg[num].yy && AItargtype[num] == 1) {
		AItarg[num].xx = -1;
		AItargtype[num] = -1;
		backed[num] = true;
		Position mostarm = mostarmy(num);
		nposx = mostarm.xx, nposy = mostarm.yy;
		AIPos[num] = mostarm;
	}
}
void Start() {
	cls();
	while (!finished) {
		for (int i = 0; i < 25 && !finished; i++) {
			for (int j = 0; j <= 1; j++) {
				if (_kbhit()) {
					dir = getch();
					if (dir == 'w' || dir == 72) {
						MarchTo(nx, ny, nx - 1, ny, 1);
						if (mp[nx - 1][ny].type == -1 || nx == 1) {
							continue;
						}
						nx--;
					} else if (dir == 's' || dir == 80) {
						MarchTo(nx, ny, nx + 1, ny, 1);
						if (mp[nx + 1][ny].type == -1 || nx == H) {
							continue;
						}
						nx++;
					} else if (dir == 'a' || dir == 75) {
						MarchTo(nx, ny, nx, ny - 1, 1);
						if (mp[nx][ny - 1].type == -1 || ny == 1) {
							continue;
						}
						ny--;
					} else if (dir == 'd' || dir == 77) {
						MarchTo(nx, ny, nx, ny + 1, 1);
						if (mp[nx][ny + 1].type == -1 || ny == W) {
							continue;
						}
						ny++;
					} else if (dir == 'p') {
						debugs ^= 1;
						continue;
					} else if (dir == 'z') {
						halfarmy[1] = true;
						continue;
					} else if (dir == 'g') {
						nx = AIcap[1].xx, ny = AIcap[1].yy;
						continue;
					} else if (dir == 'r') {
						system("cls");
						cout << "Are you sure you want to restart ? [y/n]\n";
						ShowCursor();
						char choice;
						cin >> choice;
						if (choice == 'Y' || choice == 'y') {
							finished = true;
						}
						HideCursor();
						continue;
					} else if (dir == 32) {
						getch();
						continue;
					} else if (dir == 27) {
						exxit();
					}
				}
				if(KEY_DOWN(VK_LBUTTON)) {
					POINT p;
					int clickx, clicky;
					GetCursorPos(&p);
					ScreenToClient(hw, &p);
					clickx = p.x / 8, clicky = p.y / 16;
					if (clickx % 5 == 0 || clicky % 2 == 0) {
					//	continue;
					} else {
						ny = clickx / 5 + 1, nx = clicky / 2 + 1;
					}
				//	Pos(0, 50);
				//	cout << clickx / 4 << " " << clicky / 2;
				}
				for (int i = 2; i < 2 + players; i++) {
					MoveAI(i);
				}
                int t1 = clock();
				Print();
                int t2 = clock();
                tad == 0 ? tad = (t2 - t1) * 1.0 / CLOCKS_PER_SEC * 1000.0 : 0;
            //    cout << (t2 - t1) * 1.0 / CLOCKS_PER_SEC * 1000.0;
            //    system("pause");
				clss();
				Sleep(12 + 122 - tad);
			}
			Updatecity();
			turns++;
		}
		Updateland();
	//	clss();
	}
}
int main() {
	srand((unsigned)time(NULL));
	system("title generals.exe");
	system("mode con cols=150 lines=45");
	HideCursor();
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hStdin, mode);
	while (1) {
		Welcome();
		ini();
		Start();
	}
	return 0;
}
