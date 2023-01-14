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

void setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
		SetConsoleCursorPosition(hOut, coord);
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
	setCursorPosition(0, MaxY+1);
	Sleep(2000);
	cout<<"Press any key to continue...";
	getch();
	setCursorPosition(0, MaxY+1);
	cout<<"                            ";
	Sleep(1000);
}

class Spell{
	public:
		
		
		spell spells[5];
		
		void SpellDisplay(string* code, int PlayerHealth)
			{
				setCursorPosition(0, MaxY-5);
				for(int i=0; i<8; i++){
					for(int j=0; j<MaxX/2; j++){
						if(i==0||i==7||j==0||j==MaxX/2-1)
							cout<<"# ";
						else 
							cout<<"  ";
					}
					cout<<endl;	
				}
				setCursorPosition(2, MaxY-4);
				cout<<"        ";
				setCursorPosition(2, MaxY-4);
				cout<<"Life: "<<PlayerHealth<<endl;
				for(int i=0; i<5; i++)
				{
					try{
						setCursorPosition(2, MaxY-3+i);
						SpellNameSearch(code[i], i);
						cout<<i+1<<". "<<spells[i].name<<"      "<<spells[i].dmg;
					}catch(...){}
					
				}
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
		
		void DisplayStats(int PlayerHealth)
		{
			setCursorPosition(MaxX+5, (MaxY-5)/2);
			cout<<"                  ";
			spell.SpellDisplay(codes, PlayerHealth);
			setCursorPosition(MaxX+4, 1 );
			cout<<"Enemy:";
			
			setCursorPosition(MaxX+4, 2);
			cout<<"                                                           ";
			setCursorPosition(MaxX+4, 2);
			cout<<"\tLife\t"<<NewEnemy.life;
			setCursorPosition(MaxX+4, 3);
			cout<<"                                                           ";
			setCursorPosition(MaxX+4, 3);
			cout<<"\tArmor\t"<<NewEnemy.armor;
			setCursorPosition(MaxX+4, 4);
			cout<<"                                                           ";
			setCursorPosition(MaxX+4, 4);
			cout<<"\tAttack Damage\t"<<NewEnemy.dmg;
			
		}		
		
		void End()
		{
			exit(0);
		}

		void Fight(int &PlayerHealth)
		{
			while(true)
			{
				char input = getch();
				if(input=='1' || input=='2' || input=='3' || input=='4' || input=='5')
				{
					NewEnemy.life-=spell.Use(NewEnemy.armor, input-49);
					if(NewEnemy.life<=0)
						return;
					PlayerHealth-=NewEnemy.dmg;
					if(PlayerHealth<=0)
						End();
					DisplayStats(PlayerHealth);
				}
			}
		}
		
		void Rewards(int lvl)
		{
			return;
		}
		
		void Tutorial(int PlayerHealth)
		{
			cls();
			DisplayEnemy(1);
			Sleep(1000);
			setCursorPosition(MaxX+5, (MaxY-5)/2);
			cout<<"This is your enemy";
			Pause();
			setCursorPosition(MaxX+5, (MaxY-5)/2);
			cout<<"                  ";
			DisplayStats(PlayerHealth);
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
			DisplayStats(PlayerHealth);
			Fight(PlayerHealth);
			Rewards(1);
		}
		
		bool EnemyControl(int PlayerHealth)
		{
			char num;
			string line;
			ifstream File ("Data/Enemy/Tut.txt");
			File.get(num);
			if(num=='0')
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
				Tutorial(PlayerHealth);
				return false;
			}
			return true;

		}
};




