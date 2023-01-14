#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <fstream>
#include <string>
//#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define MaxX 30
#define MaxY 15

using namespace std;

DWORD WINAPI SpeedThread(LPVOID lpParam);
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

struct spell{
	string name;
	int dmg;
};

void showConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void setCursorPosition(int a, int b)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	cout.flush();
	COORD coord = { (SHORT)a, (SHORT)b };
	if (!SetConsoleCursorPosition(hConsole, coord))
    {
        cout<<GetLastError();
    }
}

void cls()
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD topLeft = { 0, 0 };
    std::cout.flush();
    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
        abort();
    }
    DWORD length = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD written;
    FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);
    SetConsoleCursorPosition(hOut, topLeft);
}

void MessageBox1(int line, string str, int color)
{
	switch(line){
		case 1:
			SetConsoleTextAttribute(hConsole, color);
			setCursorPosition(0, MaxY-5);
			for(int i=0; i<5; i++){
				for(int j=0; j<MaxX/2; j++){
					if(i==0||i==4||j==0||j==MaxX/2-1)
						cout<<"# ";
					else 
						cout<<"  ";
				}
				cout<<endl;	
			}
			setCursorPosition(2, MaxY-3);
			cout<<str;
			break;
		case 2:
			SetConsoleTextAttribute(hConsole, color);
			Sleep(1000);
			setCursorPosition(3, MaxY-2);
			cout<<str;
	}
	cout<<endl<<endl;
	SetConsoleTextAttribute(hConsole, 7);
}

void Pause()
{
	setCursorPosition(0, MaxY+3);
	Sleep(2000);
	cout<<"Press any key to continue...";
	getch();
	setCursorPosition(0, MaxY+3);
	cout<<"                            ";
	Sleep(1000);
}

class Spell{
	public:
		
<<<<<<< HEAD:file.h
		struct spell{
			string name;
			int dmg;
			int mana;
			int cost;
		};
=======
>>>>>>> 901dadd38c29f73f51f1c9fac043e9c831d1b4a0:enemy.h
		
		spell spells[5];

		void displaySpell(int x, int y, int id, int index) {
			setCursorPosition(x, y);
			cout<<(char)218;

			for(int i=0; i<17; i++)
				cout<<(char)196;
			cout<<(char)191;

			for(int i=0; i<10; i++) {
				setCursorPosition(x, y+i+1);
				cout<<(char)179;
				setCursorPosition(x+18, y+i+1);
				cout<<(char)179;
			}

			setCursorPosition(x, y+11);
			cout<<(char)192;

			for(int i=0; i<17; i++)
				cout<<(char)196;
			cout<<(char)217;

			setCursorPosition(x+3, y+2);
			cout<<index+1<<". "<<spells[id].name;

			setCursorPosition(x+3, y+7);
			cout<<"Damage: "<<spells[id].dmg;

			setCursorPosition(x+3, y+9);
			cout<<"Mana Cost: "<<-spells[id].mana;
		}

		void SpellNameSearch(string code, int num)
		{
			string name;
			char dmg[10];
			ifstream Spell("Data/Spell/Spell.txt");
			while(getline(Spell, name))
			{
				if(name==code)
				{
					getline(Spell, spells[num].name);
					Spell.getline(dmg, 10);
					spells[num].dmg = atoi(dmg);
					Spell.getline(dmg, 10);
					spells[num].mana = atoi(dmg);
					Spell.getline(dmg, 10);
					spells[num].cost = atoi(dmg);
					return;
				}
			}
			return;
		}
		
		int Use(int armor, int nspell)
		{
			return (spells[nspell].dmg/10)*(10-(armor/10));
		}
};

class Enemy{
	public:
		Spell spell;

		struct baseEnemy{
			int life=100;
			int armor=10;
			int dmg=10;
		};

		baseEnemy BaseEnemy;
		baseEnemy NewEnemy;
		string codes[5];

		void DisplayEnemy(int n)
		{
			setCursorPosition(0, 0);
			ifstream Enemy;
			SetConsoleTextAttribute(hConsole, 4);
			
			string line;
			
			switch (n){
				
				case 1:
					Enemy.open("Data/Enemy/boh.txt");
					break;
					
				case 0:
					Enemy.open("Data/Enemy/bohh.txt");
					break;
			}
			while ( getline (Enemy,line) )
			{
	  			cout << line << '\n';
			}
			Enemy.close();
			SetConsoleTextAttribute(hConsole, 7);
		}
				
		void Stats(int lvl)
		{
			NewEnemy.life = BaseEnemy.life*lvl;
			NewEnemy.armor=BaseEnemy.armor*lvl;
			NewEnemy.dmg=BaseEnemy.dmg*lvl;
		}
		
		void DisplayStats()
		{
			setCursorPosition(35, (MaxY-5)/2);
			cout<<"                  ";
			setCursorPosition(34, 1 );
			cout<<"Enemy:";
			
			setCursorPosition(38, 2);
			cout<<"                                                       ";
			setCursorPosition(38, 2);
			cout<<"Health\t"<<NewEnemy.life;
			setCursorPosition(38, 3);
			cout<<"                                                       ";
			setCursorPosition(38, 3);
			cout<<"Armor\t"<<NewEnemy.armor;
			setCursorPosition(38, 4);
			cout<<"                                                       ";
			setCursorPosition(38, 4);
			cout<<"Damage\t"<<NewEnemy.dmg;
			
		}
		
		void End()
		{
			exit(0);
		}

		bool Fight(int &PlayerHealth, int &PlayerMana, float armor)
		{
			char input = getch();

			if(input=='1' || input=='2' || input=='3' || input=='4' || input=='5')
			{
				if(PlayerMana >= spell.spells[(int)input-49].mana)
				{
					PlayerMana -= spell.spells[(int)input-49].mana;
					NewEnemy.life -= spell.Use(NewEnemy.armor, input-49);

					if(NewEnemy.life<=0)
						return false;

					PlayerHealth -= NewEnemy.dmg*100/(100+armor);

					if(PlayerHealth<=0)
						return false;

					DisplayStats();
				}
			}

			return true;
		}
		
		int Rewards(int lvl)
		{
			return 20*lvl;
		}
		
		void Tutorial(int PlayerHealth, int PlayerMana, float armor)
		{
			cls();
			DisplayEnemy(1);
			Sleep(1000);
			setCursorPosition(MaxX+5, (MaxY-5)/2);
			cout<<"This is your enemy";
			Pause();
			setCursorPosition(MaxX+5, (MaxY-5)/2);
			cout<<"                  ";
			DisplayStats();
			setCursorPosition(MaxX+5, MaxY-3);
			cout<<"These are the spell you can use,";
			setCursorPosition(MaxX+5, MaxY-2);
			cout<<"you will earn more of them going on in the game";
			Pause();
			setCursorPosition(MaxX+5, MaxY-3);
			cout<<"                                ";
			setCursorPosition(MaxX+5, MaxY-2);
			cout<<"                                               ";
			setCursorPosition(MaxX+35, 2);
			cout<<"Here you will find ";
			setCursorPosition(MaxX+35, 3);
			cout<<"all the information needed to defeat the enemy";
			Pause();
			cls();
			DisplayEnemy(0);
			Stats(1);
			DisplayStats();
			Fight(PlayerHealth, PlayerMana, armor);
			Rewards(1);
		}
			
		bool EnemyControl(int PlayerHealth, int PlayerMana, float armor)
		{
			char num;
			string line;
			ifstream File ("Data/Enemy/Tut.txt");
			File.get(num);

			if(num=='0') // Tutorial
			{
				ifstream FileOracle ("Data/Enemy/Oracle.txt");

				while ( getline (FileOracle,line) )
    			{
      				cout << line << '\n';
    			}

    			FileOracle.close();
				MessageBox1(1, "So,", 7);
				MessageBox1(2, "this is your first time", 7);
				Pause();
				MessageBox1(1, "Isn't it?", 7);
				Pause();
				File.close();
				ofstream File ("Data/Enemy/Tut.txt");
				File << 1;
				File.close();
				MessageBox1(1, "", 7);
				Tutorial(PlayerHealth, PlayerMana, armor);
				return false;
			}

			return true;
		}
};