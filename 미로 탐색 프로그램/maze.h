#ifndef _MAZE_H_
#define _MAZE_H_

#include <stdio.h>
#include <windows.h>

#define MAZE_SIZE 6 //미로 크기(가로 or 세로)
#define MAX_STACK_SIZE 100 //스택 크기

#define SUCCESS 1 //성공
#define COMPLETE 0 //단순 연산완료
#define FAIL -1 //실패

typedef struct {
	short r; //행
	short c; //열
}element;

typedef struct {
	element data[MAX_STACK_SIZE];
	int top; //FILO 위한 index
}StackType;

static char maze[MAZE_SIZE][MAZE_SIZE] = //6x6 크기의 미로
{
	{ '1','1','1','1','0','0' },
	{ 'e','0','1','0','0','1' },
	{ '1','0','0','0','1','1' },
	{ '1','0','1','0','1','1' },
	{ '1','0','1','0','0','x' },
	{ '0','0','1','1','1','1' },
};
static element here = { 1,0 }; //쥐의 현재 위치

void init_stack(StackType* s);
int is_empty(StackType* s);
int is_full(StackType* s);
void push(StackType* s, element item);
element pop(StackType* s);
element peek(StackType* s);
void printStack(StackType* s);
void maze_print();

int pushloc_escape_root(StackType* s); //현재 위치에서 이동 가능 경로 탐색하여 이동 및 탈출 가능 한 경로 'R'로 기록

/*** Depending on pushloc_escape_root() ***/
int push_loc(StackType* s, int next_r, int next_c); //현재 위치에서 이동 가능 한 위치인 경우 스택에 삽입
int return_to_escape_root(); //현재 위치에서 동서남북 방향으로 이미 방문하였고, 탈출경로로 기록된 위치가 있는 좌표로 이동
int can_move_to_next_pos(element peeked_next_pos); //현재 위치에서 탈출 가능 한 다음 위치로 이동 가능한지 판별
#endif