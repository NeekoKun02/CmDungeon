#include <windows.h>	// Hey! I don't like doin' it flashy....
#include <iomanip>		// Boh
#include <ctime>		// Random Level/Floors/Zones generation
#include <conio.h>		// Get keypress char data without waiting for newline command
#include <fstream>		// File reader for saves and sample zones/floors
#include <iostream>		// It's basic (I/O system)
#include <cstdlib>		// IDK wtf does this do
#include <cmath>		// Required for computing SQRTs
#include <fcntl.h>
#include <io.h>
#include <string>
#include "enemy.h"

#define WIN32_LEAN_AND_MEAN

#define SPAWN_TYPE 0
#define BOSS_TYPE 1
#define SHOP_TYPE 2
#define ENEMY_TYPE 3
#define OPT_TYPE 4
#define WALL_TYPE 5
#define DOOR_TYPE 6

#define MAX_BOSSES 2
#define MAX_SHOPS 2

#define BOSS_COLOR 13
#define PLAYER_COLOR 11
#define ENEMY_COLOR 4
#define OPT_COLOR 6
#define SHOP_COLOR 9
#define DOOR_COLOR 7
#define WALL_COLOR 15

#define f_r 12
#define f_c 25
#define r 15
#define c 15
#define x_d 5			//frame x and y buffer
#define y_d 3
#define x_ds 12			//shop x and y offset
#define y_ds 3
#define zones 18 
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

void display(char M[r][c]);
void setCursorPosition(int, int);

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
char SPAWN = char(245);
char ENEMY = 'E';
char OPT = 'P';
char BOSS = 'X';
char SHOP = '$';
char DOOR = 'A';
char WALL = '#';
int lvl;


struct myZone {
	bool done;
	bool discovered;
	int enemies_count;
	int npc_map[r][c];
	int zoneType;
	int doors;
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

void drawShop(bool erase = false) {
	setCursorPosition(c*2+x_ds, y_ds);
	if(erase) {
		wcout<<' ';
		for(int i=0; i<30; i++)
			wcout<<' ';
		wcout<<' ';
		for(int i=0; i<7; i++){
			setCursorPosition(c*2+x_ds, y_ds+i+1);
			wcout<<' ';
			for(int j=0; j<30; j++){
				wcout<<' ';
			}
			wcout<<' ';
		}
		setCursorPosition(c*2+x_ds, y_ds+8);
		wcout<<' ';
		for(int i=0; i<30; i++)
			wcout<<' ';
		wcout<<' ';

		return;
	}

	wcout<<TDR;
	for(int i=0; i<30; i++)
		wcout<<TLR;
	wcout<<TDL;
	for(int i=0; i<7; i++){
		setCursorPosition(c*2+x_ds, y_ds+i+1);
		wcout<<TUD;
		for(int j=0; j<30; j++){
			wcout<<' ';
		}
		wcout<<TUD;
	}
	setCursorPosition(c*2+x_ds, y_ds+8);
	wcout<<TUR;
	for(int i=0; i<30; i++)
		wcout<<TLR;
	wcout<<TUL;

	return;
}

class Player {
	public:
		bool hasMoved;
		bool shopping = false;
		int x;
		int y;
		int f_x;
		int f_y;
		int l_x = 7;
		int l_y = 7;

		void refresh(char M[r][c]) {

			if(map.floor[f_y][f_x].npc_map[l_y][l_x] == SHOP_TYPE) {
				SetConsoleTextAttribute(hConsole, SHOP_COLOR);
				setCursorPosition(l_x*2, l_y);
				wcout << SHOP;
			}else if(map.floor[f_y][f_x].npc_map[l_y][l_x] == BOSS_TYPE) {
				SetConsoleTextAttribute(hConsole, BOSS_COLOR);
				setCursorPosition(l_x*2, l_y);
				wcout << BOSS;;
			}else if(map.floor[f_y][f_x].npc_map[l_y][l_x] == ENEMY_TYPE) {
				SetConsoleTextAttribute(hConsole, ENEMY_COLOR);
				setCursorPosition(l_x*2, l_y);
				wcout << ENEMY;
			}else if(map.floor[f_y][f_x].npc_map[l_y][l_x] == OPT_TYPE) {
				SetConsoleTextAttribute(hConsole, OPT_COLOR);
				setCursorPosition(l_x*2, l_y);
				wcout << OPT;
			}else{
				setCursorPosition(l_x*2, l_y);
 				wcout << ' ';
			}

			setCursorPosition(x*2, y);
			SetConsoleTextAttribute(hConsole, PLAYER_COLOR);
			wcout << PLAYER;
			SetConsoleTextAttribute(hConsole, 15);
		}
		
		char move(char M[r][c], int dir) {
			char dest;

			l_x = x;
			l_y = y;
			
			switch(dir) {
				case 1:{
					dest = map.floor[f_y][f_x].npc_map[y-1][x];
					break;
				}
				case 2:{
					dest = map.floor[f_y][f_x].npc_map[y][x-1];
					break;
				}
				case 3: {
					dest = map.floor[f_y][f_x].npc_map[y+1][x];
					break;
				}
				case 4:{
					dest = map.floor[f_y][f_x].npc_map[y][x+1];
					break;
				}
			}
			
			if(dest == WALL_TYPE)
				return WALL;
			
			if(dest == DOOR_TYPE && !map.floor[f_y][f_x].done){
				setCursorPosition(c, r+3);
				if(map.floor[f_y][f_x].enemies_count == 1)
					wcout<<"You cannot exit until you have defeated all the enemies. "<<map.floor[f_y][f_x].enemies_count<<" enemy remaning";
				else
					wcout<<"You cannot exit until you have defeated all the enemies. "<<map.floor[f_y][f_x].enemies_count<<" enemies remaning";
				setCursorPosition(x*2, y);
				return 'A';	
			}

			switch(dir) {
				case 1:{
					y--;
					break;
				}
				case 2:{
					x--;
					break;
				}
				case 3: {
					y++;
					break;
				}
				case 4:{
					x++;
					break;
				}
			}

			if(dest == SHOP_TYPE) {
				drawShop();
				shopping = true;
			}else if(shopping){
				drawShop(true);
			}
			
			if(dest == ENEMY_TYPE) {
				cls();
				enemy.EnemyControl(codes);
				enemy.DisplayEnemy(0);
				enemy.Stats(0.5);
				enemy.DisplayStats(codes);
				enemy.Fight();
				display(M);
				refresh(M);

				M[y][x] = ' ';
				map.floor[f_y][f_x].npc_map[y][x] = 0;
				map.floor[f_y][f_x].enemies_count--;


				if(map.floor[f_y][f_x].enemies_count == 0) {
					map.floor[f_y][f_x].done = true;
				}
			}
			
			hasMoved = true;
			return dest;
		}
};

Player player1;

void display(char M[r][c]) {
	system("CLS");

	for(int i=0; i<r; i++) {
		for(int j=0; j<c; j++){
			if(M[i][j] == ENEMY){
				SetConsoleTextAttribute(hConsole, ENEMY_COLOR);
				wcout<<ENEMY<<' ';
				SetConsoleTextAttribute(hConsole, 15);
			}else if(M[i][j] == OPT){
				SetConsoleTextAttribute(hConsole, OPT_COLOR);
				wcout<<OPT<<' ';
				SetConsoleTextAttribute(hConsole, 15);
			}else if(M[i][j] == DOOR){
				SetConsoleTextAttribute(hConsole, DOOR_COLOR);
				wcout<<DOOR<<' ';
				SetConsoleTextAttribute(hConsole, 15);
			}else if(M[i][j] == BOSS){
				SetConsoleTextAttribute(hConsole, BOSS_COLOR);
				wcout<<BOSS<<' ';
				SetConsoleTextAttribute(hConsole, 15);
			}else if(M[i][j] == SHOP){
				SetConsoleTextAttribute(hConsole, SHOP_COLOR);
				wcout<<SHOP<<' ';
				SetConsoleTextAttribute(hConsole, 15);
			}else if(M[i][j] == '#'){
				SetConsoleTextAttribute(hConsole, WALL_COLOR);
				wcout<<WALL<<' ';
				SetConsoleTextAttribute(hConsole, 15);
			}else if(M[i][j] == '*'){
				wcout<<"  ";
			}else{
				wcout<<M[i][j]<<' ';
			}
		}
		wcout<<endl;
	}	

	player1.hasMoved = true;
	player1.refresh(M);
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
			myFile.open("data/zone_01.txt");
			break;
		}
		case 4: {
			myFile.open("data/zone_02.txt");
			break;
		}
		case 5: {
			myFile.open("data/zone_03.txt");
			break;
		}
		case 6: {
			myFile.open("data/zone_04.txt");
			break;
		}
		case 7: {
			myFile.open("data/zone_05.txt");
			break;
		}
		case 8: {
			myFile.open("data/zone_06.txt");
			break;
		}
		case 9: {
			myFile.open("data/zone_07.txt");
			break;
		}
		case 10: {
			myFile.open("data/zone_08.txt");
			break;
		}
		case 11: {
			myFile.open("data/zone_09.txt");
			break;
		}
		case 12: {
			myFile.open("data/zone_10.txt");
			break;
		}
		case 13: {
			myFile.open("data/zone_11.txt");
			break;
		}
		case 14: {
			myFile.open("data/zone_12.txt");
			break;
		}
		case 15: {
			myFile.open("data/zone_13.txt");
			break;
		}
		case 16: {
			myFile.open("data/zone_14.txt");
			break;
		}
		case 17: {
			myFile.open("data/zone_15.txt");
			break;
		}
		case 18: {
			myFile.open("data/zone_16.txt");
			break;
		}
		case 19: {
			myFile.open("data/zone_17.txt");
			break;
		}
		case 20: {
			myFile.open("data/zone_18.txt");
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

void generate_enemies(char M[r][c], bool opt = false, bool boss = false, bool shop = false) {
	int count = 0;
	int i, j;
	
	srand(time(NULL));
	map.floor[player1.f_y][player1.f_x].enemies_count = rand()%2+lvl+2;
	
	while((count < map.floor[player1.f_y][player1.f_x].enemies_count) && !boss){
		i = rand()%16;
		j = rand()%16;
		
		if(M[i][j] == ' ') {
			M[i][j] = ENEMY;
			map.floor[player1.f_y][player1.f_x].npc_map[i][j] = ENEMY_TYPE;
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
	
	for(i=0; i<r; i++){
		for(j=0; j<c; j++){
			if(M[i][j] == '*'){
				if(rand()%(10-lvl*2) == 0){
					map.floor[player1.f_y][player1.f_x].npc_map[i][j] = OPT_TYPE;
					M[i][j] = OPT;
				}
			}
		}
	}
}

void generate_zone(char M[r][c], bool spawn = false) {
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
 	
 	if(spawn || map.floor[player1.f_y][player1.f_x].zoneType == SHOP_TYPE)
 		map.floor[player1.f_y][player1.f_x].done = true;
		 	
 	if(map.floor[player1.f_y][player1.f_x].zoneType != SPAWN_TYPE && map.floor[player1.f_y][player1.f_x].zoneType != SHOP_TYPE && map.floor[player1.f_y][player1.f_x].zoneType != BOSS_TYPE && !map.floor[player1.f_y][player1.f_x].done)
		generate_enemies(M, 1);
	
	for(int i=0; i<r; i++){
		for(int j=0; j<c; j++){
			if(M[i][j] == BOSS)
				map.floor[player1.f_y][player1.f_x].npc_map[i][j] = BOSS_TYPE;
			
			if(M[i][j] == SHOP)
				map.floor[player1.f_y][player1.f_x].npc_map[i][j] = SHOP_TYPE;

			if(M[i][j] == WALL)
				map.floor[player1.f_y][player1.f_x].npc_map[i][j] = WALL_TYPE;
				
			if(M[i][j] == DOOR)
				map.floor[player1.f_y][player1.f_x].npc_map[i][j] = DOOR_TYPE;
		}
	}

	if(map.floor[player1.f_y][player1.f_x].zoneType == BOSS_TYPE) {
		for(int i=0; i<r; i++){
			for(int j=0; j<c; j++){
				if(M[i][j] == BOSS)
					M[i][j] = BOSS;
			}
		}
	}
		
	if(map.floor[player1.f_y][player1.f_x].zoneType == SHOP_TYPE) {
		for(int i=0; i<r; i++){
			for(int j=0; j<c; j++){
				if(M[i][j] == SHOP)
					M[i][j] = SHOP;
			}
		}
	}

	display(M);
	
	map.floor[player1.f_y][player1.f_x].discovered = true;
}

void generate_floor(int level) {
	int x, y;
	int rooms = 20;
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
	
	map.floor[a][b].zoneType = SPAWN_TYPE;					// Sets spawn room
	queue[0] = map.floor[a][b];
	queue[0].x = a;
	queue[0].y = b;
	fixed_rooms++;

	while(fixed_rooms < rooms) {
		wcout<<"Fixed Rooms: "<<fixed_rooms<<endl;
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
			if(x != 0 && x != f_r-1 && y != 0 && y != f_c-1 && map.floor[x][y].zoneType == -1 && ((map.floor[x+1][y].zoneType != -1) + (map.floor[x-1][y].zoneType != -1) + (map.floor[x][y+1].zoneType != -1) + (map.floor[x][y-1].zoneType != -1) + (map.floor[x-1][y-1].zoneType != -1) + (map.floor[x+1][y-1].zoneType != -1) + (map.floor[x-1][y+1].zoneType != -1) + (map.floor[x+1][y+1].zoneType != -1)) < 3 && rand()%2) {
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

	int count = 0;

	for(int i=0; i<rooms; i++){
		myZone here = queue[i];

		if(((map.floor[here.x+1][here.y].zoneType == -1) + (map.floor[here.x][here.y-1].zoneType == -1) + (map.floor[here.x-1][here.y].zoneType == -1) + (map.floor[here.x][here.y+1].zoneType == -1)) == 3){
			map.floor[here.x][here.y].zoneType = BOSS_TYPE;
			count++;
			if(count >= MAX_BOSSES)
				i = rooms;
		}
	}
	
	count = 0;

	for(int i=0; i<rooms; i++){
		myZone here = queue[i];

		if((((map.floor[here.x+1][here.y].zoneType == -1) + (map.floor[here.x][here.y-1].zoneType == -1) + (map.floor[here.x-1][here.y].zoneType == -1) + (map.floor[here.x][here.y+1].zoneType == -1)) == 3) && map.floor[here.x][here.y].zoneType != BOSS_TYPE && map.floor[here.x][here.y].zoneType != SPAWN_TYPE){
			map.floor[here.x][here.y].zoneType = SHOP_TYPE;
			count++;
			if(count >= MAX_SHOPS)
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
				else if(map.floor[i][j].zoneType == BOSS_TYPE && map.floor[i][j].discovered)
					miniMap[(2*i)+y_d+1][(4*j)+x_d+2] = BOSS;
				else if(map.floor[i][j].zoneType == SHOP_TYPE && map.floor[i][j].discovered)
					miniMap[(2*i)+y_d+1][(4*j)+x_d+2] = SHOP;
				else
					miniMap[(2*i)+y_d+1][(4*j)+x_d+2] = '~';
			}
		}
	}
}

void showMap() {
	char input;
	char miniMap[(f_r*2)+y_d*2][(f_c*4)+x_d*2];
	int x, y;

    _setmode(_fileno(stdout), 0x10000);
	system("CLS");
	setCursorPosition(0, 0);
	
	generateMap(miniMap);
	
	for(int i=0; i<(f_r*2)+y_d*2; i++){
		for(int j=0; j<(f_c*4)+x_d*2; j++){
			if(j<3 || j>(f_c*4+x_d*2)-4 || i<3 || i>(f_r*2+y_d*2)-4)
				wcout<<miniMap[i][j];
				// If it is a corner:
			else if((miniMap[i][j] == TUL) || (miniMap[i][j] == TUR) || (miniMap[i][j] == TDL) || (miniMap[i][j] == TDR) || (miniMap[i][j] == TUDLR) || (miniMap[i][j] == TUDL) || (miniMap[i][j] == TUDR) || (miniMap[i][j] == TULR) || miniMap[i][j] == TDLR){
				if(map.floor[(i-y_d)/2][(j-x_d)/4].discovered || map.floor[((i-y_d)/2)-1][(j-x_d)/4].discovered || map.floor[(i-y_d)/2][((j-x_d)/4)-1].discovered || map.floor[((i-y_d)/2)-1][((j-x_d)/4)-1].discovered)
					wcout<<miniMap[i][j];
				else
					wcout<<' ';
			}   // If it is a vertical pipe:
			else if(miniMap[i][j] == TUD || miniMap[i][j] == UD){
				if(map.floor[(i-y_d-1)/2][(j-x_d)/4].discovered || map.floor[(i-y_d-1)/2][(j-x_d)/4-1].discovered)
					wcout<<miniMap[i][j];
				else
					wcout<<' ';	
			}   // If it is an orizontal pipe
			else if(miniMap[i][j] == TLR || miniMap[i][j] == LR) {
				if(map.floor[(i-y_d-(i%2))/2][(j-x_d)/4].discovered || map.floor[(i-y_d-(i%2))/2+1][(j-x_d)/4].discovered)
					wcout<<miniMap[i][j];
				else
					wcout<<' ';
			}
			else if(miniMap[i][j] == PLAYER){
				SetConsoleTextAttribute(hConsole, PLAYER_COLOR);
				wcout<<PLAYER;
				SetConsoleTextAttribute(hConsole, 15);
			}
			else if(miniMap[i][j] == SPAWN){
				SetConsoleTextAttribute(hConsole, 8);
				wcout<<SPAWN;
				SetConsoleTextAttribute(hConsole, 15);
			}
			else if(miniMap[i][j] == '~' && !map.floor[(i-y_d-1)/2][(j-x_d-2)/4].done && map.floor[(i-y_d-1)/2][(j-x_d-2)/4].discovered) {
				SetConsoleTextAttribute(hConsole, ENEMY_COLOR);
				wcout<<'~';
				SetConsoleTextAttribute(hConsole, 15);
			}
			else if(miniMap[i][j] == BOSS){
				SetConsoleTextAttribute(hConsole, BOSS_COLOR);
				wcout<<BOSS;
				SetConsoleTextAttribute(hConsole, 15);
			}
			else if(miniMap[i][j] == SHOP){
				SetConsoleTextAttribute(hConsole, SHOP_COLOR);
				wcout<<SHOP;
				SetConsoleTextAttribute(hConsole, 15);
			}
			else{
				if(map.floor[(i-y_d-1)/2][(j-x_d-2)/4].discovered)
					wcout<<miniMap[i][j];
				else
					wcout<<' ';
			}
				
		}
		wcout<<endl;
	}
	
	getch();
	return;
}

void konamiCode() {
	for(int i=0; i<f_r; i++){
		for(int j=0; j<f_c; j++){
			map.floor[i][j].done = true;
			map.floor[i][j].discovered = true;
		}
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
			wcout << "Are you sure you want to exit? Every unsaved progresses will be lost (Y/N)";
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

	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
	
	for(int i=1; i<MAX_LVL; i++) {
		lvl = i;
		
		generate_floor(i);
		generate_zone(zone, true);
		
		while(!level_done) {
			showConsoleCursor(false);
			
			if(player1.hasMoved)
				player1.refresh(zone);
			
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
