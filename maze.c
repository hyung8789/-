#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAZE_SIZE 6

//============ ���� �ڵ��� ���� ===============

#define MAX_STACK_SIZE 100

typedef struct {
	short r; //��
	short c; //��
}element;


typedef struct {
	element data[MAX_STACK_SIZE];
	int top;
}StackType;

//���� �ʱ�ȭ �Լ�
void init_stack(StackType *s)
{
	s->top = -1;
}

//���� ��� ���� �Լ�
int is_empty(StackType *s)
{
	return (s->top == -1);
}

//��ȭ ���� ���� �Լ�
int is_full(StackType *s)
{
	return (s->top == (MAX_STACK_SIZE - 1));
}

//�����Լ�
void push(StackType *s, element item)
{
	if (is_full(s)) {
		fprintf(stderr, "���� ��ȭ ����\n");
		return;
	}
	else s->data[++(s->top)] = item;
}

//�����Լ�
element pop(StackType *s)
{
	if (is_empty(s)) {
		fprintf(stderr, "���� ���� ����\n");
		exit(1);
	}
	else return s->data[(s->top)--];
}

//��ũ�Լ�
element peek(StackType *s)
{
	if (is_empty(s)) {
		fprintf(stderr, "���� ���� ����\n");
		exit(1);
	}
	else return s->data[s->top];
}

//============ ���� �ڵ��� �� ===============

//�Լ� ���� ����
void printStack(StackType *s);
bool push_loc(StackType *s, int r, int c);
void maze_print(char maze[MAZE_SIZE][MAZE_SIZE]);
void pushloc_escape_root(StackType *s, int r, int c);
void search_change_location(element *loc);

//���� ������ ����Ʈ �ϴ� �Լ�
void printStack(StackType *s)
{
	int i;
	if (is_empty(s))
	{
		printf("������ ����ֽ��ϴ�.\n");
		return;
	}
	printf("\n=========���� ���� ����Ʈ ����=========\n");
	printf("���� �湮���� �ʾҰ�, �� �� �ִ� ��ġ��\n");
	for (i = 0; i <= s->top; i++) //���ÿ� �Էµ� ����ŭ �ݺ�
	{
		printf("data[%d] = ROW: %d, COL : %d\n", i,s->data[i].r, s->data[i].c);
	}
	printf("\n=========���� ���� ����Ʈ ����=========\n");
}

element here = { 1,0 }, entry = { 1,0 };

/*************************
�Ա�=e
�ⱸ=x
�湮�� ���� ��� �迭�� ���� '.'���� ����
Ż�� ��θ� 'R'�� ǥ��
**************************/

char maze[MAZE_SIZE][MAZE_SIZE] = //6x6 ũ���� �̷� ����
{
	{ '1','1','1','1','1','1' },
	{ 'e','0','1','0','0','1' },
	{ '1','0','0','0','1','1' },
	{ '1','0','1','0','1','1' },
	{ '1','0','1','0','0','x' },
	{ '1','1','1','1','1','1' },
};

//��ġ�� ���ÿ� ����
bool push_loc(StackType *s,int r,int c)
{
	if (r < 0 || c < 0) return false; //�迭�� ���� �� �� �̷��� ���� ���ΰ��
	if (maze[r][c] == '1' || maze[r][c] == '.' || maze[r][c] == 'R') return false;

	if(maze[r][c] != '1' && maze[r][c] != '.' && maze[r][c] != 'R')
	{
		element tmp;
		tmp.r = r;
		tmp.c = c;
		push(s, tmp); //���ÿ� ����
	}
	return true;
}

//�̷θ� ȭ�鿡 ����Ѵ�
void maze_print(char maze[MAZE_SIZE][MAZE_SIZE])
{
	printf("\n");
	for (int r = 0; r < MAZE_SIZE; r++) //��
	{
		for (int c = 0; c < MAZE_SIZE; c++) //��
		{
			printf("%c", maze[r][c]);
		}
		printf("\n");
	}
}

//push_loc ������� �� �̷�Ż�� ��θ� 'R'�� ���
void pushloc_escape_root(StackType *s, int r, int c)
{
	int count = 0; //push_loc�� ���� ���� Ƚ�� ī����
	element tmp_final_success; //�������� �̵��ϱ� ���� ��ǥ�� �ӽ������ϱ� ���� ����ü
	element tmp_here; //���� ���� ��ǥ�� �ӽ������ϱ� ���� ����ü

	if(push_loc(s, r, c + 1)) count++; //��
	if(push_loc(s, r, c - 1)) count++; //��
	if(push_loc(s, r + 1, c)) count++; //��
	if(push_loc(s, r - 1, c)) count++; //��

	if (count == 0) //�� �̻� �� ���� ������ (���̳� �̹� �湮�� ��)
	{
		tmp_here.r = here.r;
		tmp_here.c = here.c;
		tmp_final_success=peek(s); //���ÿ��� �������� �̵��� ��ġ�� row�� col�� ���� tmp_final_success�� peek�ؼ� ����
		search_change_location(&tmp_final_success); //���ÿ��� �������� �̵��� ��ġ���� ������������ 'R'�� �ִ� ��ġ�� �˻� �� ����
		/* ���ÿ��� �������� �̵��� ��ġ���� ������������ 'R'���� �����ϸ� �� ��ġ����
		�㰡 �� �̻� �̵��� �Ұ����� ��ġ���� �ǵ��ư��� 'R'�� '.'���� ���� */
		while (1) //���� ������ġ���� tmp_final_success�� ��ǥ���� �̵�
		{
			search_change_location(&tmp_here);
			if (tmp_here.r == tmp_final_success.r && tmp_here.c == tmp_final_success.c) break;
			maze[tmp_here.r][tmp_here.c] = '.';	
		}
	}
	else
	{
		maze[r][c] = 'R';
	}

}

//���� ��ġ���� �������� �������� 'R'�� �ִ� ��ǥ�� ����
void search_change_location(element *loc)
{
	if (maze[loc->r][loc->c + 1] == 'R') loc->c++; //��
	if (maze[loc->r][loc->c - 1] == 'R') loc->c--; //��
	if (maze[loc->r + 1][loc->c] == 'R') loc->r++; //��
	if (maze[loc->r - 1][loc->c] == 'R') loc->r--; //��
}

int main(void)
{
	int r, c;
	StackType s;

	init_stack(&s);
	here = entry;
	while (maze[here.r][here.c] != 'x') //�ⱸ�� �ƴϸ� �ݺ�
	{
		r = here.r;
		c = here.c;
		maze[r][c] = '.';//������ ���� '.'���� ǥ��
		maze_print(maze);
		pushloc_escape_root(&s, r, c); //��ġ�� ���ÿ� ����(��,������ ���� �ƴҰ�쿡��) �� Ż�� ��θ� 'R'�� ���
		printStack(&s); //���� ���� ����Ʈ
		
		if (is_empty(&s))
		{
			printf("����\n");
			return;
		}
		else
			//���ÿ��� �ϳ��� ��ġ�� ������ ���� ��ġ�� ����
			here = pop(&s);
	}
	printf("����\n");
	return 0;
}