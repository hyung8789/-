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
		fprintf(stderr, "스택 포화 에러\n");
		return;
	}
	else s->data[++(s->top)] = item;
}

element pop(StackType *s)
{
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else
		return s->data[(s->top)--];
}

element peek(StackType *s)
{
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[s->top];
}

void printStack(StackType *s)
{
	if (is_empty(s))
	{
		printf("스택이 비어있습니다.\n");
		return;
	}
	printf("\n=========스택 내용 프린트 시작=========\n");
	printf("아직 방문되지 않았고, 갈 수 있는 위치들\n");
	for (int i = 0; i <= s->top; i++) //스택에 입력된 수만큼 반복
	{
		printf("Stack[%d] = ROW: %d, COL : %d\n", i,s->data[i].r, s->data[i].c);
	}
	printf("\n=========스택 내용 프린트 종료=========\n");
}

int push_loc(StackType *s, int next_r, int next_c) //현재 위치에서 이동 가능 한 위치인 경우 스택에 삽입
{
	if (next_r < 0 || next_c < 0 || next_r >= MAZE_SIZE || next_c >= MAZE_SIZE) //범위 초과
		return FAIL;

	if(maze[next_r][next_c] == '0' || maze[next_r][next_c] == 'x') //이동 가능한 위치면
	{
		element tmp;
		tmp.r = next_r;
		tmp.c = next_c;
		push(s, tmp); //스택에 삽입
		
		return SUCCESS;
	}

	return FAIL;
}

void maze_print() //미로 출력
{
	printf("\n");
	for (int r = 0; r < MAZE_SIZE; r++) //행
	{
		for (int c = 0; c < MAZE_SIZE; c++) //열
		{
			printf("%c", maze[r][c]);
		}
		printf("\n");
	}
}

int pushloc_escape_root(StackType *s) //현재 위치에서 이동 가능 경로 탐색하여 이동 및 탈출 가능 한 경로 'R'로 기록
{
	int count = 0; //현재 위치에서 이동 가능한 위치의 수
	element next_pos; //다음으로 이동하기 위한 좌표

	//현재 위치에서 동,서,남,북으로 이동 가능한 방향을 찾고 스택에 삽입
	if(push_loc(s, here.r, here.c + 1) == SUCCESS) count++; //동
	if(push_loc(s, here.r, here.c - 1) == SUCCESS) count++; //서
	if(push_loc(s, here.r + 1, here.c) == SUCCESS) count++; //남
	if(push_loc(s, here.r - 1, here.c) == SUCCESS) count++; //북
	
	printStack(s);
	if (maze[here.r][here.c] == 'x') //출구에 도달하면
	{
		printf("성공\n");
		return SUCCESS;
	}

	if (count == 0) //더 이상 갈 곳이 없으면 (벽이나 이미 방문한 곳)
	{
		next_pos = peek(s); //스택에서 다음으로 이동 가능 한 위치를 peek하여 저장
		
		/***
			스택에서 다음으로 이동할 위치에서 동서남북으로 'R'값이 존재하면 (즉, 이미 방문하였고, 탈출경로로 기록된 위치) 
			그 위치까지 쥐가 더 이상 이동이 불가능한 위치부터 되돌아가며 'R'을 '.'으로 변경
			=> 해당 경로는 올바른 탈출 루트가 아니므로
		***/
		while (1) //쥐의 현재 위치에서 스택에서 peek된 다음으로 이동 가능 한 위치까지 돌아가기
		{
			maze[here.r][here.c] = '.'; //해당 위치는 탈출 경로가 아닌 단순 방문한 위치로 변경

			return_to_escape_root(); //현재 쥐의 위치에서 동서남북 방향으로 이미 방문하였고, 탈출경로로 기록된 위치가 있는 좌표로 돌아감
			
			if (can_move_to_next_pos(next_pos) == SUCCESS) //쥐가 스택에서 다음으로 이동 가능 한 위치까지 돌아왔으면
				break;
		}

		here = pop(s); //스택에서 꺼내어 이동
		if(maze[here.r][here.c] != 'x')
			maze[here.r][here.c] = 'R'; //탈출 경로 표기

		return COMPLETE;
	}
	else //이동 할 곳이 있으면
	{
		here = pop(s); //스택에서 꺼내어 이동
		if (maze[here.r][here.c] != 'x')
			maze[here.r][here.c] = 'R'; //탈출 경로 표기	

		return COMPLETE;
	}
}

int return_to_escape_root() //현재 위치에서 동서남북 방향으로 이미 방문하였고, 탈출경로로 기록된 위치가 있는 좌표로 이동
{
	if (maze[here.r][here.c + 1] == 'R') //동
	{
		here.c++;
		return SUCCESS;
	}
	if (maze[here.r][here.c - 1] == 'R') //서
	{
		here.c--;
		return SUCCESS;
	}
	if (maze[here.r + 1][here.c] == 'R') //남
	{
		here.r++;
		return SUCCESS;
	}
	if (maze[here.r - 1][here.c] == 'R') //북
	{
		here.r--;
		return SUCCESS;
	}

	return FAIL;
}

int can_move_to_next_pos(element peeked_next_pos) //현재 위치에서 탈출 가능 한 다음 위치로 이동 가능한지 판별
{
	if (maze[here.r][here.c + 1] == maze[peeked_next_pos.r][peeked_next_pos.c]) //동
		return SUCCESS;
	if (maze[here.r][here.c - 1] == maze[peeked_next_pos.r][peeked_next_pos.c]) //서
		return SUCCESS;
	if (maze[here.r + 1][here.c] == maze[peeked_next_pos.r][peeked_next_pos.c]) //남
		return SUCCESS;
	if (maze[here.r - 1][here.c] == maze[peeked_next_pos.r][peeked_next_pos.c]) //북
		return SUCCESS;

	return FAIL;
}