#include "maze.h"

void init_stack(StackType *s)
{
	s->top = -1;
}

int is_empty(StackType *s)
{
	return (s->top == -1);
}

int is_full(StackType *s)
{
	return (s->top == (MAX_STACK_SIZE - 1));
}

void push(StackType *s, element item)
{
	if (is_full(s)) {
		fprintf(stderr, "���� ��ȭ ����\n");
		return;
	}
	else s->data[++(s->top)] = item;
}

element pop(StackType *s)
{
	if (is_empty(s)) {
		fprintf(stderr, "���� ���� ����\n");
		exit(1);
	}
	else
		return s->data[(s->top)--];
}

element peek(StackType *s)
{
	if (is_empty(s)) {
		fprintf(stderr, "���� ���� ����\n");
		exit(1);
	}
	else return s->data[s->top];
}

void printStack(StackType *s)
{
	if (is_empty(s))
	{
		printf("������ ����ֽ��ϴ�.\n");
		return;
	}
	printf("\n=========���� ���� ����Ʈ ����=========\n");
	printf("���� �湮���� �ʾҰ�, �� �� �ִ� ��ġ��\n");
	for (int i = 0; i <= s->top; i++) //���ÿ� �Էµ� ����ŭ �ݺ�
	{
		printf("Stack[%d] = ROW: %d, COL : %d\n", i,s->data[i].r, s->data[i].c);
	}
	printf("\n=========���� ���� ����Ʈ ����=========\n");
}

int push_loc(StackType *s, int next_r, int next_c) //���� ��ġ���� �̵� ���� �� ��ġ�� ��� ���ÿ� ����
{
	if (next_r < 0 || next_c < 0 || next_r >= MAZE_SIZE || next_c >= MAZE_SIZE) //���� �ʰ�
		return FAIL;

	if(maze[next_r][next_c] == '0' || maze[next_r][next_c] == 'x') //�̵� ������ ��ġ��
	{
		element tmp;
		tmp.r = next_r;
		tmp.c = next_c;
		push(s, tmp); //���ÿ� ����
		
		return SUCCESS;
	}

	return FAIL;
}

void maze_print() //�̷� ���
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

int pushloc_escape_root(StackType *s) //���� ��ġ���� �̵� ���� ��� Ž���Ͽ� �̵� �� Ż�� ���� �� ��� 'R'�� ���
{
	int count = 0; //���� ��ġ���� �̵� ������ ��ġ�� ��
	element next_pos; //�������� �̵��ϱ� ���� ��ǥ

	//���� ��ġ���� ��,��,��,������ �̵� ������ ������ ã�� ���ÿ� ����
	if(push_loc(s, here.r, here.c + 1) == SUCCESS) count++; //��
	if(push_loc(s, here.r, here.c - 1) == SUCCESS) count++; //��
	if(push_loc(s, here.r + 1, here.c) == SUCCESS) count++; //��
	if(push_loc(s, here.r - 1, here.c) == SUCCESS) count++; //��
	
	printStack(s);
	if (maze[here.r][here.c] == 'x') //�ⱸ�� �����ϸ�
	{
		printf("����\n");
		return SUCCESS;
	}

	if (count == 0) //�� �̻� �� ���� ������ (���̳� �̹� �湮�� ��)
	{
		next_pos = peek(s); //���ÿ��� �������� �̵� ���� �� ��ġ�� peek�Ͽ� ����
		
		/***
			���ÿ��� �������� �̵��� ��ġ���� ������������ 'R'���� �����ϸ� (��, �̹� �湮�Ͽ���, Ż���η� ��ϵ� ��ġ) 
			�� ��ġ���� �㰡 �� �̻� �̵��� �Ұ����� ��ġ���� �ǵ��ư��� 'R'�� '.'���� ����
			=> �ش� ��δ� �ùٸ� Ż�� ��Ʈ�� �ƴϹǷ�
		***/
		while (1) //���� ���� ��ġ���� ���ÿ��� peek�� �������� �̵� ���� �� ��ġ���� ���ư���
		{
			maze[here.r][here.c] = '.'; //�ش� ��ġ�� Ż�� ��ΰ� �ƴ� �ܼ� �湮�� ��ġ�� ����

			return_to_escape_root(); //���� ���� ��ġ���� �������� �������� �̹� �湮�Ͽ���, Ż���η� ��ϵ� ��ġ�� �ִ� ��ǥ�� ���ư�
			
			if (can_move_to_next_pos(next_pos) == SUCCESS) //�㰡 ���ÿ��� �������� �̵� ���� �� ��ġ���� ���ƿ�����
				break;
		}

		here = pop(s); //���ÿ��� ������ �̵�
		if(maze[here.r][here.c] != 'x')
			maze[here.r][here.c] = 'R'; //Ż�� ��� ǥ��

		return COMPLETE;
	}
	else //�̵� �� ���� ������
	{
		here = pop(s); //���ÿ��� ������ �̵�
		if (maze[here.r][here.c] != 'x')
			maze[here.r][here.c] = 'R'; //Ż�� ��� ǥ��	

		return COMPLETE;
	}
}

int return_to_escape_root() //���� ��ġ���� �������� �������� �̹� �湮�Ͽ���, Ż���η� ��ϵ� ��ġ�� �ִ� ��ǥ�� �̵�
{
	if (maze[here.r][here.c + 1] == 'R') //��
	{
		here.c++;
		return SUCCESS;
	}
	if (maze[here.r][here.c - 1] == 'R') //��
	{
		here.c--;
		return SUCCESS;
	}
	if (maze[here.r + 1][here.c] == 'R') //��
	{
		here.r++;
		return SUCCESS;
	}
	if (maze[here.r - 1][here.c] == 'R') //��
	{
		here.r--;
		return SUCCESS;
	}

	return FAIL;
}

int can_move_to_next_pos(element peeked_next_pos) //���� ��ġ���� Ż�� ���� �� ���� ��ġ�� �̵� �������� �Ǻ�
{
	if (maze[here.r][here.c + 1] == maze[peeked_next_pos.r][peeked_next_pos.c]) //��
		return SUCCESS;
	if (maze[here.r][here.c - 1] == maze[peeked_next_pos.r][peeked_next_pos.c]) //��
		return SUCCESS;
	if (maze[here.r + 1][here.c] == maze[peeked_next_pos.r][peeked_next_pos.c]) //��
		return SUCCESS;
	if (maze[here.r - 1][here.c] == maze[peeked_next_pos.r][peeked_next_pos.c]) //��
		return SUCCESS;

	return FAIL;
}