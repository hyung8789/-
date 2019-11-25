#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAZE_SIZE 6

//============ 스택 코드의 시작 ===============

#define MAX_STACK_SIZE 100

typedef struct {
	short r; //행
	short c; //열
}element;


typedef struct {
	element data[MAX_STACK_SIZE];
	int top;
}StackType;

//스택 초기화 함수
void init_stack(StackType *s)
{
	s->top = -1;
}

//공백 상대 검출 함수
int is_empty(StackType *s)
{
	return (s->top == -1);
}

//포화 상태 검출 함수
int is_full(StackType *s)
{
	return (s->top == (MAX_STACK_SIZE - 1));
}

//삽입함수
void push(StackType *s, element item)
{
	if (is_full(s)) {
		fprintf(stderr, "스택 포화 에러\n");
		return;
	}
	else s->data[++(s->top)] = item;
}

//삭제함수
element pop(StackType *s)
{
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[(s->top)--];
}

//피크함수
element peek(StackType *s)
{
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[s->top];
}

//============ 스택 코드의 끝 ===============

//함수 원형 정의
void printStack(StackType *s);
bool push_loc(StackType *s, int r, int c);
void maze_print(char maze[MAZE_SIZE][MAZE_SIZE]);
void pushloc_escape_root(StackType *s, int r, int c);
void search_change_location(element *loc);

//스택 내용을 프린트 하는 함수
void printStack(StackType *s)
{
	int i;
	if (is_empty(s))
	{
		printf("스택이 비어있습니다.\n");
		return;
	}
	printf("\n=========스택 내용 프린트 시작=========\n");
	printf("아직 방문되지 않았고, 갈 수 있는 위치들\n");
	for (i = 0; i <= s->top; i++) //스택에 입력된 수만큼 반복
	{
		printf("data[%d] = ROW: %d, COL : %d\n", i,s->data[i].r, s->data[i].c);
	}
	printf("\n=========스택 내용 프린트 종료=========\n");
}

element here = { 1,0 }, entry = { 1,0 };

/*************************
입구=e
출구=x
방문이 끝난 경우 배열의 값을 '.'으로 변경
탈출 경로를 'R'로 표기
**************************/

char maze[MAZE_SIZE][MAZE_SIZE] = //6x6 크기의 미로 생성
{
	{ '1','1','1','1','1','1' },
	{ 'e','0','1','0','0','1' },
	{ '1','0','0','0','1','1' },
	{ '1','0','1','0','1','1' },
	{ '1','0','1','0','0','x' },
	{ '1','1','1','1','1','1' },
};

//위치를 스택에 삽입
bool push_loc(StackType *s,int r,int c)
{
	if (r < 0 || c < 0) return false; //배열의 범위 밖 즉 미로의 범위 밖인경우
	if (maze[r][c] == '1' || maze[r][c] == '.' || maze[r][c] == 'R') return false;

	if(maze[r][c] != '1' && maze[r][c] != '.' && maze[r][c] != 'R')
	{
		element tmp;
		tmp.r = r;
		tmp.c = c;
		push(s, tmp); //스택에 삽입
	}
	return true;
}

//미로를 화면에 출력한다
void maze_print(char maze[MAZE_SIZE][MAZE_SIZE])
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

//push_loc 연산수행 및 미로탈출 경로를 'R'로 기록
void pushloc_escape_root(StackType *s, int r, int c)
{
	int count = 0; //push_loc의 연산 성공 횟수 카운터
	element tmp_final_success; //다음으로 이동하기 위한 좌표를 임시저장하기 위한 구조체
	element tmp_here; //현재 쥐의 좌표를 임시저장하기 위한 구조체

	if(push_loc(s, r, c + 1)) count++; //동
	if(push_loc(s, r, c - 1)) count++; //서
	if(push_loc(s, r + 1, c)) count++; //남
	if(push_loc(s, r - 1, c)) count++; //북

	if (count == 0) //더 이상 갈 곳이 없으면 (벽이나 이미 방문한 곳)
	{
		tmp_here.r = here.r;
		tmp_here.c = here.c;
		tmp_final_success=peek(s); //스택에서 다음으로 이동할 위치의 row와 col의 값을 tmp_final_success에 peek해서 저장
		search_change_location(&tmp_final_success); //스택에서 다음으로 이동할 위치에서 동서남북으로 'R'이 있는 위치를 검색 후 변경
		/* 스택에서 다음으로 이동할 위치에서 동서남북으로 'R'값이 존재하면 그 위치까지
		쥐가 더 이상 이동이 불가능한 위치부터 되돌아가며 'R'을 '.'으로 변경 */
		while (1) //쥐의 현재위치에서 tmp_final_success의 좌표까지 이동
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

//현재 위치에서 동서남북 방향으로 'R'이 있는 좌표로 변경
void search_change_location(element *loc)
{
	if (maze[loc->r][loc->c + 1] == 'R') loc->c++; //동
	if (maze[loc->r][loc->c - 1] == 'R') loc->c--; //서
	if (maze[loc->r + 1][loc->c] == 'R') loc->r++; //남
	if (maze[loc->r - 1][loc->c] == 'R') loc->r--; //북
}

int main(void)
{
	int r, c;
	StackType s;

	init_stack(&s);
	here = entry;
	while (maze[here.r][here.c] != 'x') //출구가 아니면 반복
	{
		r = here.r;
		c = here.c;
		maze[r][c] = '.';//지나온 길은 '.'으로 표시
		maze_print(maze);
		pushloc_escape_root(&s, r, c); //위치를 스택에 삽입(벽,지나온 길이 아닐경우에만) 및 탈출 경로를 'R'로 기록
		printStack(&s); //스택 내용 프린트
		
		if (is_empty(&s))
		{
			printf("실패\n");
			return;
		}
		else
			//스택에서 하나의 위치를 꺼내어 현재 위치로 만듬
			here = pop(&s);
	}
	printf("성공\n");
	return 0;
}