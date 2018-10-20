#include <stdio.h>
#include <stdlib.h>

//Function resultant codes
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define MARK 'x'

//Status is a type of function whose value is of function resultant codes
typedef int Status;

#define STACK_INIT_SIZE 100 //Original distributed space of storage
#define STACK_INCREMENT 10 //Additional distributed space of storage

//The coordinate of positions
typedef struct
{
    int x;
    int y;
}PosType;

//Elements in the stack
typedef struct
{
    int ord; //The sequence number of accessible points
    PosType seat; //The coordinate position of accessible points
    int di; //Direction to the next points
}SElemType; //Type of the stack

//Defination of a stack
typedef struct
{
    SElemType *base;
    SElemType *top;
    int stacksize;
}SqStack;

//Initiate a new stack
Status InitStack(SqStack &S)
{
    S.base = (SElemType*) malloc(STACK_INIT_SIZE * sizeof(SElemType));
    if(!S.base)
        exit(OVERFLOW);
    S.top = S.base;
    S.stacksize = STACK_INIT_SIZE;
    return OK;
}

//Push element e into the stack
Status Push(SqStack &S, SElemType e)
{
    if(S.top-S.base >= S.stacksize)
    {
        S.base = (SElemType*)realloc(S.base, (S.stacksize + STACK_INCREMENT)*sizeof(SElemType));
        if(!S.base)
            exit(OVERFLOW);
        S.top = S.base + S.stacksize;
        S.stacksize += STACK_INCREMENT;
    }
    *S.top++ = e;
    return OK;
}

//Pop the upper element out of the stack, and its value is conveyed to e
Status Pop(SqStack &S, SElemType &e)
{
    if(S.top == S.base)
        return ERROR;
    e = *--S.top;
    return OK;
}

//Judge if a stack is empty. If so, return TURE, otherwise return FALSE
Status StackEmpty(SqStack S)
{
    if(S.top == S.base)
        return TRUE;
    else
        return FALSE;
}

//Test whether the present position accessible
Status Pass(PosType &curpos,int maze[][100])
{
    if(maze[curpos.x][curpos.y]==0)
        return OK;
    else
        return ERROR;
}

//Print the maze
void PrintMaze(int maze[][100])
{
    int i,j;
    for(i=0;i<12;i++)
    {
        for(j=0;j<17;j++)
        {
			if(maze[i][j]=='x')
				printf("%3c",maze[i][j]);
			else
				printf("%3d",maze[i][j]);
        }
        printf("\n");
    }
}

//Mark the footprint
void FootPrint(PosType &curpos, int &curstep, int maze[][100])
{
    maze[curpos.x][curpos.y]=curstep;
}

//The direction of next step
PosType NextPos(PosType curpos, int di)
{
	switch (di) //find the next point's coordinate in terms of the direction
	{
	case 1:curpos.x+=1; curpos.y+=0; break;
	case 2:curpos.x+=1; curpos.y+=1; break;
	case 3:curpos.x+=0; curpos.y+=1; break;
	case 4:curpos.x+=-1; curpos.y+=1; break;
	case 5:curpos.x+=-1; curpos.y+=0; break;
	case 6:curpos.x+=-1; curpos.y+=-1; break;
	case 7:curpos.x+=0; curpos.y+=-1; break;
	case 8:curpos.x+=1; curpos.y+=-1;
	}
    return curpos;
}

//Mark the unaccessible points
void MarkPrint(PosType &curpos, int maze[][100])
{
    maze[curpos.x][curpos.y]=MARK; // MARK==-3
}

//If there is any path from the start point to the outlet end, store one of them in the stack(from the base to the top),
//then return the aggregate number of steps; otherwise, return FALSE
Status MazePath(PosType Start, PosType End, int maze[][100])
{
	SqStack S;
	SElemType e;
	PosType curpos;
	int curstep;
	InitStack(S); curpos = Start; //Set "present position" as "entrance position"
	curstep=2; //Take the first step
    do
    {
        if(Pass(curpos,maze))
        {
            FootPrint(curpos,curstep,maze);
            e.ord = curstep;
            e.seat.x = curpos.x;
            e.seat.y = curpos.y;
            e.di = 1;
            Push(S,e);
            if(curpos.x==End.x && curpos.y==End.y)  return (maze[End.x][End.y]-1);
            curpos = NextPos(curpos, e.di);
			curstep++;
        }
        else
		{
			if(!StackEmpty(S))
            {
                Pop(S, e);
                while(e.di == 7 && !StackEmpty(S))
                {
                    MarkPrint(e.seat,maze);
                    Pop(S, e);
		curstep--; // To count the steps correctly, when coming back from the blocked way, the number should subtrate into the precedent condition
                }
                if(e.di < 7)
                {
                    e.di++;
                    Push(S,e);
                    curpos = NextPos(e.seat,e.di);
                }
            }
        }
    }while(!StackEmpty(S));
    return (FALSE);
}






int main()
{
	//Set up a maze
	int maze[100][100] =
    {
		    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1},
        {1,1,0,0,0,1,1,0,1,1,1,0,0,1,1,1,1},
        {1,0,1,1,0,0,0,0,1,1,1,1,0,0,1,1,1},
        {1,1,1,0,1,1,1,1,0,1,1,0,1,1,0,0,1},
        {1,1,1,0,1,0,0,1,0,1,1,1,1,1,1,1,1},
        {1,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1},
        {1,0,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1},
        {1,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,1},
        {1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,0,1},
        {1,0,1,0,0,1,1,1,1,1,0,1,1,1,1,0,1},
		    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

	//Initiation
	int stepstatistic;
    PosType START, END;

	//Demonstrate the maze
    printf("Now here comes a maze:\n");
    PrintMaze(maze);
	printf("\n");

	//Set up the start position
    printf("Please set up a beginning position:\n");
	printf("format: row_column\n");
    scanf("%d_%d",&START.x,&START.y);
	if(START.x>12 || START.y>17)
	{
		printf("ERROR!");
		return 0;
	}

	//Set up the end position
    printf("Please set up an end position:\n");
	printf("format: row_column\n");
    scanf("%d_%d",&END.x,&END.y);
    if(END.x>12 || END.y>17)
    {
		printf("ERROR!");
		return 0;
	}

	//Find a possible way then print it out in a demonstration
	if(stepstatistic=MazePath(START,END,maze)) //Find a way as well as count the steps
	{
		printf("SUCCESS!:\n");
		printf("The route is shown as follows from 2 to the end:\n");
		PrintMaze(maze);
		printf("\n");
		printf("The aggregate number of steps is %d\n",stepstatistic); //Display the count of steps
	}
	else
	{
		printf("FAIL!\n");
		printf("No accessible route!\n");
    }
    return 0;
}
//The End
