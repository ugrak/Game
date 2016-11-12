#include <cstdio>
#include <ncurses.h>
#include <pthread.h>
#include <ctime>
#include <cstdlib>
#include <cctype>
#include <queue>
#include <cmath> 

int x = 22;			//	Координата начала стрелки
int counter = 0;
float points = 0;
//const int fine = 5;
bool mistake = false;

struct Arrow {
	int high;
	int direction;
	void print ()
	{  
		if (this->direction == KEY_LEFT)
		{
			mvaddstr(this->high,  x," #");
			mvaddstr(this->high+1,x,"########");
			mvaddstr(this->high+2,x," #");
		} else 
		{
			mvaddstr(this->high,  x,"      #");
			mvaddstr(this->high+1,x,"########");
			mvaddstr(this->high+2,x,"      #");
		}    
	}
};

int key=0;			//	Для выхода из игры


float SlowTime=200;	//	Определяет скорость движения стрелок

int colsMin=50;		//	Минимальное количество столбцов терминала
int linesMin=40;	//	Минимальное количество строк терминала
int LeftWall=17;	//	Координата x для левой стены
int RightWall=33;	//	Координата x для правой стены
int Bottom=38;		//	Координата x для дна

void PrintWalls (void);	//	Рисует стены и дно

static void *guitarhero (void *none)
{
    while (1)
    {
		key = getch();
        napms(100);
        flushinp();
    }
    return NULL;
}

void PrintWalls()
{
    mvprintw(10, RightWall + 2, "Points: %d", (int)points);
    mvprintw(12, RightWall + 2, "Difficulty: %d", (int)(pow(200.0f/SlowTime, 2)));
    for(int i=0; i<Bottom; i++)
    {
        mvaddch(i,LeftWall,'#');
        mvaddch(i,RightWall,'#');
    }
    for(int j=LeftWall; j<=RightWall; j++) mvaddch(Bottom,j,'#');
}


int main()
{
    srand(time(NULL));

    if (NULL == initscr())	// На случай, если не запустится ncurses
    {
        fprintf(stderr,"Ncurses init error\n");
        return -1;
    }

    noecho();
    curs_set(0);
    keypad(stdscr,TRUE);
    raw();

    if(COLS<colsMin || LINES<linesMin)     //	Проверяет размер экрана на пригодность
    {
        endwin();
        fprintf(stderr,"Screen check error\n");
        return -1;
    }
    
    mvprintw(LINES/2, COLS/2-12, "Press any button to Play");
    refresh;
    getch();
    
    std::queue<Arrow> myqueue;

    pthread_t hero;
    pthread_create(&hero, NULL, guitarhero, NULL);//	Запускает поток ввода
    
    while (tolower(key)!='q')
    {
        if(counter++%10==0) myqueue.push({-3, (rand()%2) ? KEY_LEFT : KEY_RIGHT});
        clear();
        PrintWalls();
        for(int i=0 ; i < myqueue.size(); i++)
        {
			Arrow temp = myqueue.front();
			myqueue.pop();
			temp.high++;
			temp.print();
			myqueue.push(temp);
			
			/* myqueue.front().print();
			 * myqueue.front().high++;
			 * myqueue.push(myqueue.front());
			 * myqueue.pop();
			 */
		}
		
		if (mistake) SlowTime -= SlowTime/8;
		mistake = false;
		
		SlowTime -= SlowTime/2000;
		
		if(myqueue.size() > 0 && key==myqueue.front().direction) {
			myqueue.pop();
			points += (200.0f/(float)SlowTime);
			
		}
		else if (key!=0) mistake = true;
		
		key = 0;
		
		if (myqueue.front().high >= Bottom-2) break;
		
        refresh();
        napms(SlowTime);
    }
    pthread_cancel(hero);//завершает поток
    endwin(); //деинициализирует экран ncurses
    printf("%d\n", (int)points);
    return 0;
}
