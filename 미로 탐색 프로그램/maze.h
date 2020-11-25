#ifndef _MAZE_H_
#define _MAZE_H_

#include <stdio.h>
#include <windows.h>

#define MAZE_SIZE 6 //�̷� ũ��(���� or ����)
#define MAX_STACK_SIZE 100 //���� ũ��

#define SUCCESS 1 //����
#define COMPLETE 0 //�ܼ� ����Ϸ�
#define FAIL -1 //����

typedef struct {
	short r; //��
	short c; //��
}element;

typedef struct {
	element data[MAX_STACK_SIZE];
	int top; //FILO ���� index
}StackType;

static char maze[MAZE_SIZE][MAZE_SIZE] = //6x6 ũ���� �̷�
{
	{ '1','1','1','1','0','0' },
	{ 'e','0','1','0','0','1' },
	{ '1','0','0','0','1','1' },
	{ '1','0','1','0','1','1' },
	{ '1','0','1','0','0','x' },
	{ '0','0','1','1','1','1' },
};
static element here = { 1,0 }; //���� ���� ��ġ

void init_stack(StackType* s);
int is_empty(StackType* s);
int is_full(StackType* s);
void push(StackType* s, element item);
element pop(StackType* s);
element peek(StackType* s);
void printStack(StackType* s);
void maze_print();

int pushloc_escape_root(StackType* s); //���� ��ġ���� �̵� ���� ��� Ž���Ͽ� �̵� �� Ż�� ���� �� ��� 'R'�� ���

/*** Depending on pushloc_escape_root() ***/
int push_loc(StackType* s, int next_r, int next_c); //���� ��ġ���� �̵� ���� �� ��ġ�� ��� ���ÿ� ����
int return_to_escape_root(); //���� ��ġ���� �������� �������� �̹� �湮�Ͽ���, Ż���η� ��ϵ� ��ġ�� �ִ� ��ǥ�� �̵�
int can_move_to_next_pos(element peeked_next_pos); //���� ��ġ���� Ż�� ���� �� ���� ��ġ�� �̵� �������� �Ǻ�
#endif