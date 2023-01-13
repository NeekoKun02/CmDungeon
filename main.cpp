#include <windows.h>	// Hey! I don't like doin' it flashy....
#include <Windows.h>
#include <Wincon.h>
#include <winbase.h>
#include <iomanip>		// Boh
#include <ctime>		// Random Level/Floors/Zones generation
#include <conio.h>		// Get keypress char data without waiting for newline command
#include <fstream>		// File reader for saves and sample zones/floors
#include <iostream>		// It's basic (I/O system)
#include <cstdlib>		// IDK wtf does this do
#include "enemy.h"

#define WIN32_LEAN_AND_MEAN
#define SPAWN_TYPE 0
#define BOSS_TYPE 1
#define SHOP_TYPE 2
#define BOSS_COLOR 13
#define PLAYER_COLOR 11
#define ENEMY_COLOR 4
#define OPT_COLOR 6
#define SHOP_COLOR 13
#define DOOR_COLOR 7
#define f_r 10
#define f_c 25
#define r 15
#define c 15
#define x_d 5
#define y_d 3
#define zones 14
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ESC_KEY 27
#define MAX_LVL 2

using namespace std;

Enemy enemy;
int NumSpell = 1;
string codes[5] = {"10000001", "0", "0", "0", "0"};

void refresh(int x, int y);
void display(char M[r][c]);

int n;
char UD = 179;
char UDL = 180;
char TUDL = 185;
char TUD = 186;
char TDL = 187;
char TUL = 188;
char DL = 191;
char UR = 192;
char ULR = 193;
char DLR = 194;
char UDR = 195;
char LR = 196;
char UDLR = 197;
char TUR = 200;
char TDR = 201;
char TULR = 202;
char TDLR = 203;
char TUDR = 204;
char TLR = 205;
char TUDLR = 206;
char UL = 217;
char DR = 218;
char PLAYER = '@';
char SHOP = char(207);
char ENEMY = 'E';
char OPT = 'P';
char DOOR = 'A';
char WALL = '#';
int lvl;

void setCursorPosition(int, int);
void refresh(int x, int y);

struct myZone {
	bool done;
	bool discovered;
	int enemies_count;
	int enemies[r][c];
	int zoneType;
	int doors;
	int chests[r][c];
	int x;
	int y;
};

struct myFloor {
	int lvl;
	myZone floor[f_r][f_c];
};

myFloor map;

bool interactable;

void shutdown() {
	exit(0);
}

class Player {
	public:
		bool hasMoved;
		bool fought;
		int x;
		int y;
		int f_y;
		int f_x;
		
		char move(char M[r][c], int dir) {
			char dest;
			
			switch(dir) {
				case 1:{
					dest = M[y-1][x];
					break;
				}
				case 2:{
					dest = M[y][x-1];
					break;
				}
				case 3: {
					dest = M[y+1][x];
					break;
				}
				case 4:{
					dest = M[y][x+1];
					break;
				}
			}
			
			if(dest == WALL)
				return WALL;
			
			if(dest == DOOR && !map.floor[f_y][f_x].done){
				cout<<map.floor[f_y][f_x].enemies_count;
				return 'A';	
			}
			
			if(dest == ENEMY) {
				cls();
				enemy.EnemyControl(codes);
				enemy.DisplayEnemy(0);
				enemy.Stats(0.5);
				enemy.DisplayStats(codes);
				for(int i=0; i++; i<5)
				{
					cout<<codes[i];
				}
				enemy.Fight();
				display(M);
				refresh(x, y);
				map.floor[f_y][f_x].enemies[y][x] = 0;
				map.floor[f_y][f_x].enemies_count--;
				if(map.floor[f_y][f_x].enemies_count == 0) {
					map.floor[f_y][f_x].done = true;
				}
				fought = true;
			}
			
			switch(dir) {
				case 1:{
					y--;
					if(fought)
						M[y][x] = ' ';
					break;
					fought = false;
				}
				case 2:{
					x--;
					if(fought)
						M[y][x] = ' ';
					break;
					fought = false;
				}
				case 3: {
					y++;
					if(fought)
						M[y][x] = ' ';
					break;
					fought = false;
				}
				case 4:{
					x++;
					if(fought)
						M[y][x] = ' ';
					break;
					fought = false;
				}
			}
			
			hasMoved = true;
			return ' ';
		}
};

Player player1;

void refresh(int x, int y) {
	cout << ' ';
	setCursorPosition(x*2, y);
	SetConsoleTextAttribute(hConsole, PLAYER_COLOR);
	cout << PLAYER;
	SetConsoleTextAttribute(hConsole, 15);	
	setCursorPosition(x*2, y);
}

void display(char M[r][c]) {
	system("CLS");
	for(int i=0; i<r; i++) {
		for(int j=0; j<c; j++){
			if(M[i][j] == ENEMY){
				SetConsoleTextAttribute(hConsole, ENEMY_COLOR);
				cout<<M[i][j]<<' ';
				SetConsoleTextAttribute(hConsole, 15);
			}else if(M[i][j] == OPT){
				SetConsoleTextAttribute(hConsole, OPT_COLOR);
				cout<<M[i][j]<<' ';
				SetConsoleTextAttribute(hConsole, 15);
			}else if(M[i][j] == '*'){
				cout<<"  ";
			}
			else
				cout<<M[i][j]<<' ';
		}
		cout<<endl;
	}	
	player1.hasMoved = true;
	refresh(player1.x, player1.y);
	setCursorPosition(player1.x, player1.y);
}

void get_sample(int n, char M[r][c]) {
	ifstream myFile;
	char a;
	myFile >> noskipws;
	switch (n) {
		case -1: {
			return;	
		}
		case 0: {
			myFile.open("data/spawn.txt");
			break;
		}
		case 1: {
			myFile.open("data/boss.txt");
			break;
		}
		case 2:  {
			myFile.open("data/shop.txt");
			break;
		}
		case 3: {
			myFile.open("data/zone_03.txt");
			break;
		}
		case 4: {
			myFile.open("data/zone_04.txt");
			break;
		}
		case 5: {
			myFile.open("data/zone_05.txt");
			break;
		}
		case 6: {
			myFile.open("data/zone_06.txt");
			break;
		}
		case 7: {
			myFile.open("data/zone_07.txt");
			break;
		}
		case 8: {
			myFile.open("data/zone_08.txt");
			break;
		}
		case 9: {
			myFile.open("data/zone_09.txt");
			break;
		}
		case 10: {
			myFile.open("data/zone_10.txt");
			break;
		}
		case 11: {
			myFile.open("data/zone_11.txt");
			break;
		}
		case 12: {
			myFile.open("data/zone_12.txt");
			break;
		}
		case 13: {
			myFile.open("data/zone_13.txt");
			break;
		}
		case 14: {
			myFile.open("data/zone_14.txt");
			break;
		}
		case 15: {
			myFile.open("data/zone_15.txt");
			break;
		}
		case 16: {
			myFile.open("data/zone_16.txt");
			break;
		}
	}
	
	for(int i=0; i<r; i++) {
    	for(int j=0; j<c; j++) {
    		myFile >> a;
    		if(a != '\n'){
    			M[i][j] = a;
    		} else {
    			j--;
    		}
      		
    	}
  	}
}

void generate_enemies(char M[r][c], bool opt = false, bool boss = false) {
	int count = 0;
	int i, j;
	
	srand(time(NULL));
	map.floor[player1.f_y][player1.f_x].enemies_count = rand()%2+lvl+2;
	
	while((count < map.floor[player1.f_y][player1.f_x].enemies_count) && !boss){
		i = rand()%16;
		j = rand()%16;
		
		if(M[i][j] == ' ') {
			M[i][j] = ENEMY;
			map.floor[player1.f_y][player1.f_x].enemies[i][j] = 1;
			count++;
		}
	}
	
	if(!opt){
		for(i=0; i<r; i++){
			for(j=0; j<c; j++){
				if(M[i][j] == '*')
					M[i][j] = ' ';
			}
		}	
	
		return;	
	}

	if(boss){
		for(i=0; i<r; i++){
			for(j=0; j<c; j++){
				if(M[i][j] == 'X')
					M[i][j] = 'X';
			}
		}	
	
		return;	
	}
	
	for(i=0; i<r; i++){
		for(j=0; j<c; j++){
			if(M[i][j] == '*'){
				if(rand()%(10-lvl*2) == 0){
					map.floor[player1.f_y][player1.f_x].enemies[i][j] = 2;
					M[i][j] = OPT;
				}
			}
		}
	}
}

void generate_zone(char M[r][c], bool spawn = false, bool shop = false, bool boss = false) {
	int a;
	int type = map.floor[player1.f_y][player1.f_x].zoneType;
	int enemies = 0;
	interactable = false;
	srand(time(NULL));
	get_sample(type, M);
	
	if(map.floor[player1.f_y+1][player1.f_x].zoneType != -1)
		M[14][7] = DOOR;
	if(map.floor[player1.f_y-1][player1.f_x].zoneType != -1)
		M[0][7] = DOOR;
	if(map.floor[player1.f_y][player1.f_x+1].zoneType != -1)
		M[7][14] = DOOR;
	if(map.floor[player1.f_y][player1.f_x-1].zoneType != -1)
		M[7][0] = DOOR;
		
	if(spawn) {
		player1.y = 7;
		player1.x = 7;
 	}
 	
 	if(spawn || shop)
 		map.floor[player1.f_y][player1.f_x].done = true;
		 	
 	if(!spawn && !shop && !boss && !map.floor[player1.f_y][player1.f_x].done)
		generate_enemies(M, 1);
		
	display(M);
	
	map.floor[player1.f_y][player1.f_x].discovered = true;
}

void generate_floor(int level) {
	int x, y;
	int rooms = 100;
	int fixed_rooms = 0;
	int a, b;
	bool acceptable = true;
	myZone queue[100];
	
	a = rand()%5+f_r/2-2;
	b = rand()%8+f_c/2-2;
	
	player1.f_x = b;
	player1.f_y = a;
	
	srand(time(NULL));
	
	for(int i=0; i<f_r; i++) {
		for(int j=0; j<f_c; j++)
			map.floor[i][j].zoneType = -1;			// Sets all possible zone positions as empty
	}
	
	map.floor[a][b].zoneType = SPAWN_TYPE;					// Sets spawn_TYPE room
	queue[0] = map.floor[a][b];
	queue[0].x = a;
	queue[0].y = b;
	fixed_rooms++;

	while(fixed_rooms < rooms) {
		cout<<"Fixed Rooms: "<<fixed_rooms<<endl;
		for(int i=0; i<fixed_rooms; i++) {
		
			x = queue[i].x;
			y = queue[i].y;

			if(rand()%2) {
				if(rand()%2) {
					x++;
				} else {
					x--;
				}
			} else {
				if(rand()%2) {
					y++;
				} else {
					y--;
				}
			}
			// This mf of an if statement works to check if the cell is occupied, has more than 1 neightbour or a random 50% chance to know if it can become a room
			if(x != 0 && x != f_r-1 && y != 0 && y != f_c-1 && map.floor[x][y].zoneType == -1 && ((map.floor[x+1][y].zoneType != -1) + (map.floor[x-1][y].zoneType != -1) + (map.floor[x][y+1].zoneType != -1) + (map.floor[x][y-1].zoneType != -1) + (map.floor[x-1][y-1].zoneType != -1) + (map.floor[x+1][y-1].zoneType != -1) + (map.floor[x-1][y+1].zoneType != -1) + (map.floor[x+1][y+1].zoneType != -1)) < 4 && rand()%2) {
				do{
					map.floor[x][y].zoneType = rand()%zones+3;
				} while(map.floor[x][y].zoneType == queue[i].zoneType);
			
				queue[fixed_rooms] = map.floor[x][y];
				queue[fixed_rooms].x = x;
				queue[fixed_rooms].y = y;
				fixed_rooms++;
				i = fixed_rooms;
			}
		}
	}

	for(int i=0; i<rooms; i++){
		myZone here = queue[i];

		if(((map.floor[here.x+1][here.y].zoneType == -1) + (map.floor[here.x][here.y-1].zoneType == -1) + (map.floor[here.x-1][here.y].zoneType == -1) + (map.floor[here.x][here.y+1].zoneType == -1)) == 3){
			map.floor[here.x][here.y].zoneType = BOSS_TYPE;
			i = rooms;
		}
	}

}

void generateMap(char miniMap[(f_r*2)+y_d*2][(f_c*4)+x_d*2]) {
	
	for(int i=0; i<(f_r*2)+y_d*2; i++){												// generate base frame for map
		for(int j=0; j<(f_c*4)+x_d*2; j++){
			miniMap[i][j] = ' ';
			if(i == 0 || i == (f_r*2)+y_d*2-1)
				miniMap[i][j] = LR;
			if(j == 0 || j == (f_c*4)+x_d*2-1)
				miniMap[i][j] = UD;
			if(i == 0 && j == 0)
				miniMap[i][j] =  DR;
			if(i == 0 && j == (f_c*4)+x_d*2-1)
				miniMap[i][j] = DL;
			if(i == (f_r*2)+y_d*2-1 && j == 0)
				miniMap[i][j] = UR;
			if(i == (f_r*2)+y_d*2-1 && j == (f_c*4)+x_d*2-1)
				miniMap[i][j] = UL;
		}
	}
	
	for(int i=0; i<f_r; i++) {
		for(int j=0; j<f_c; j++) {
			if(map.floor[i][j].zoneType != -1) {
				
				if(miniMap[(2*i)+y_d][(4*j)+x_d] == TDL)
					miniMap[(2*i)+y_d][(4*j)+x_d] = TDLR;
				else if(miniMap[(2*i)+y_d][(4*j)+x_d] == TUL || miniMap[(2*i)+y_d][(4*j)+x_d] == TULR || miniMap[(2*i)+y_d][(4*j)+x_d] == TUDL)
					miniMap[(2*i)+y_d][(4*j)+x_d] = TUDLR;
				else if(miniMap[(2*i)+y_d][(4*j)+x_d] == TUR)
					miniMap[(2*i)+y_d][(4*j)+x_d] = TUDR;
				else if(miniMap[(2*i)+y_d][(4*j)+x_d] == ' ')
					miniMap[(2*i)+y_d][(4*j)+x_d] = TDR;
				
				////////////////////////////////////////////////////////////////
				
				miniMap[(2*i)+y_d][(4*j)+x_d+1] = TLR;
				miniMap[(2*i)+y_d][(4*j)+x_d+3] = TLR;
				if(miniMap[(2*i)+y_d][(4*j)+x_d+2] == TLR)
					miniMap[(2*i)+y_d][(4*j)+x_d+2] = LR;
				else
					miniMap[(2*i)+y_d][(4*j)+x_d+2] = TLR;
					
				////////////////////////////////////////////////////////////////
				
				if(miniMap[(2*i)+y_d][(4*j)+x_d+4] == TUR || miniMap[(2*i)+y_d][(4*j)+x_d+4] == TUDR || miniMap[(2*i)+y_d][(4*j)+x_d+4] == TULR)
					miniMap[(2*i)+y_d][(4*j)+x_d+4] = TUDLR;
				else if(miniMap[(2*i)+y_d][(4*j)+x_d+4] == TUL)
					miniMap[(2*i)+y_d][(4*j)+x_d+4] = TUDL;
				else if(miniMap[(2*i)+y_d][(4*j)+x_d+4] == ' ')
					miniMap[(2*i)+y_d][(4*j)+x_d+4] = TDL;
					
				////////////////////////////////////////////////////////////////
				
				if(miniMap[(2*i)+y_d+1][(4*j)+x_d+4] == TUD)
					miniMap[(2*i)+y_d+1][(4*j)+x_d+4] = UD;
				else
					miniMap[(2*i)+y_d+1][(4*j)+x_d+4] = TUD;
					
				if(miniMap[(2*i)+y_d+1][(4*j)+x_d] == TUD)
					miniMap[(2*i)+y_d+1][(4*j)+x_d] = UD;
				else
					miniMap[(2*i)+y_d+1][(4*j)+x_d] = TUD;
					
				////////////////////////////////////////////////////////////////
				
				if(miniMap[(2*i)+y_d+2][(4*j)+x_d] == TUL)
					miniMap[(2*i)+y_d+2][(4*j)+x_d] = TULR;
				else if(miniMap[(2*i)+y_d+2][(4*j)+x_d] == ' ')
					miniMap[(2*i)+y_d+2][(4*j)+x_d] = TUR;
					
				////////////////////////////////////////////////////////////////
				
				miniMap[(2*i)+y_d+2][(4*j)+x_d+1] = TLR;
				miniMap[(2*i)+y_d+2][(4*j)+x_d+2] = TLR;
				miniMap[(2*i)+y_d+2][(4*j)+x_d+3] = TLR;
					
				////////////////////////////////////////////////////////////////
				
				miniMap[(2*i)+y_d+2][(4*j)+x_d+4] = TUL;
				
				////////////////////////////////////////////////////////////////
				
				if(i == player1.f_y && j == player1.f_x)
					miniMap[(2*i)+y_d+1][(4*j)+x_d+2] = PLAYER;
				else if(map.floor[i][j].zoneType == 0)
					miniMap[(2*i)+y_d+1][(4*j)+x_d+2] = 245;
				else if(map.floor[i][j].zoneType == BOSS_TYPE)
					miniMap[(2*i)+y_d+1][(4*j)+x_d+2] = 'X';
				else
					miniMap[(2*i)+y_d+1][(4*j)+x_d+2] = 126;
			}
		}
	}
}

void showMap() {
	char input;
	char miniMap[(f_r*2)+y_d*2][(f_c*4)+x_d*2];
	int x, y;
	system("CLS");
	setCursorPosition(0, 0);
	
	generateMap(miniMap);
	
	for(int i=0; i<(f_r*2)+y_d*2; i++){
		for(int j=0; j<(f_c*4)+x_d*2; j++){
			if(j<3 || j>(f_c*4+x_d*2)-4 || i<3 || i>(f_r*2+y_d*2)-4)
				cout<<miniMap[i][j];
				// If it is a corner:
			else if((miniMap[i][j] == TUL) || (miniMap[i][j] == TUR) || (miniMap[i][j] == TDL) || (miniMap[i][j] == TDR) || (miniMap[i][j] == TUDLR) || (miniMap[i][j] == TUDL) || (miniMap[i][j] == TUDR) || (miniMap[i][j] == TULR) || miniMap[i][j] == TDLR){
				if(map.floor[(i-y_d)/2][(j-x_d)/4].discovered || map.floor[((i-y_d)/2)-1][(j-x_d)/4].discovered || map.floor[(i-y_d)/2][((j-x_d)/4)-1].discovered || map.floor[((i-y_d)/2)-1][((j-x_d)/4)-1].discovered)
					cout<<miniMap[i][j];
				else
					cout<<' ';
			}
				// If it is a vertical pipe:
			else if(miniMap[i][j] == TUD || miniMap[i][j] == UD){
				if(map.floor[(i-y_d-1)/2][(j-x_d)/4].discovered || map.floor[(i-y_d-1)/2][(j-x_d)/4-1].discovered)
					cout<<miniMap[i][j];
				else
					cout<<' ';	
			}
				// If it is an orizontal pipe
			else if(miniMap[i][j] == TLR || miniMap[i][j] == LR) {
				if(map.floor[(i-y_d-(i%2))/2][(j-x_d)/4].discovered || map.floor[(i-y_d-(i%2))/2+1][(j-x_d)/4].discovered)
					cout<<miniMap[i][j];
				else
					cout<<' ';
			}
			else if(miniMap[i][j] == '@'){
				SetConsoleTextAttribute(hConsole, PLAYER_COLOR);
				cout<<'@';
				SetConsoleTextAttribute(hConsole, 15);
			}
			else if(miniMap[i][j] == char(245)){
				SetConsoleTextAttribute(hConsole, 8);
				cout<<char(245);
				SetConsoleTextAttribute(hConsole, 15);
			}
			else if(miniMap[i][j] == '~' && !map.floor[(i-y_d-1)/2][(j-x_d-2)/4].done && map.floor[(i-y_d-1)/2][(j-x_d-2)/4].discovered) {
				SetConsoleTextAttribute(hConsole, ENEMY_COLOR);
				cout<<'~';
				SetConsoleTextAttribute(hConsole, 15);
			}
			else if(miniMap[i][j] == 'X'){
				SetConsoleTextAttribute(hConsole, BOSS_COLOR);
				cout<<'X';
				SetConsoleTextAttribute(hConsole, 15);
			}

				// If a character:
			else{
				if(map.floor[(i-y_d-1)/2][(j-x_d-2)/4].discovered)
					cout<<miniMap[i][j];
				else
					cout<<' ';
			}
				
		}
		cout<<endl;
	}
	
	getch();
	return;
}

void konamiCode() {
	for(int i=0; i<f_r; i++){
		for(int j=0; j<f_c; j++)
			map.floor[i][j].discovered = true;
	}
}

void action(char zone[r][c], char input) {
	char a;
	switch(input) {
		case 'w': {
			n=0;
			player1.move(zone, 1);
			break;
		}
		case KEY_UP: {
			if(n < 2)
				n++;
			else
				n=0;
			player1.move(zone, 1);
			break;
		}
		case 'a': {
			if(n == 8)
				n++;
			else
				n=0;		
			player1.move(zone, 2);
			break;
		}
		case 'b': {
			if(n == 9)
				n++;
			else
				n=0;
			break;
		}
		case KEY_LEFT: {
			if(n == 4 || n == 6)
				n++;
			else
				n=0;
			player1.move(zone, 2);
			break;
		}
		case 's': {
			n=0;
			player1.move(zone, 3);
			break;
		}
		case KEY_DOWN: {
			if(n<4 && n>1)
				n++;
			else
				n=0;
			player1.move(zone, 3);
			break;
		}
		case 'd': {
			n=0;
			player1.move(zone, 4);
			break;
		}
		case KEY_RIGHT: {
			if(n == 5 || n == 7)
				n++;
			else
				n=0;
			player1.move(zone, 4);
			break;
		}
		case 'x': {
			n=0;
			system("CLS");
			cout << "Are you sure you want to exit? Every unsaved progresses will be lost (Y/N)";
			showConsoleCursor(true);
			a = getch();
			if(a == 'y' || a == 'Y')
				shutdown();
			else
				showConsoleCursor(false);
				display(zone);
			break;
		}
		case 'm': {
				showMap();
				display(zone);
			break;
		}
	}
	
	if(n == 10)
		konamiCode();
}

int main() {
	char zone[r][c];
	char input;
	bool level_done = false;
	bool openMap = false;
	
	showConsoleCursor(false);
	ShowWindow(GetConsoleWindow(),SW_MAXIMIZE);
	SendMessage(GetConsoleWindow(),WM_SYSKEYDOWN,VK_RETURN,0x20000000);
	
	for(int i=1; i<MAX_LVL; i++) {
		lvl = i;
		
		generate_floor(i);
		setCursorPosition(7, 7);
	
		generate_zone(zone, true);
		
		while(!level_done) {
			showConsoleCursor(false);
			
			if(player1.hasMoved)
				refresh(player1.x, player1.y);
			
			input = getch();
			
			action(zone, input);
			
			if(map.floor[player1.f_y][player1.f_x].enemies_count == 0 && map.floor[player1.f_y][player1.f_x].zoneType != SPAWN_TYPE && !map.floor[player1.f_y][player1.f_x].done)
				map.floor[player1.f_y][player1.f_x].done == true;
		
			if(player1.x <= 0 && player1.y == 7) {
				player1.f_x--;
				player1.x = 13;
				generate_zone(zone);
			}else if(player1.x >= 14 && player1.y == 7) {
				player1.f_x++;
				player1.x = 1;
				generate_zone(zone);
			}else if(player1.x == 7 && player1.y <= 0) {
				player1.f_y--;
				player1.y = 13;
				generate_zone(zone);
			}else if(player1.x == 7 && player1.y >= 14) {
				player1.f_y++;
				player1.y = 1;
				generate_zone(zone);
			}
		}
	}

}
