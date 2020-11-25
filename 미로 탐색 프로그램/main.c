#include "maze.h"

/***
	입구 = e
	출구 = x
	방문이 끝난 경우 배열의 값을 '.'으로 변경
	탈출 경로를 'R'로 표기
***/

void main()
{
	StackType s;

	init_stack(&s);

	while (1)
	{
		maze_print();
		if (pushloc_escape_root(&s) == SUCCESS) //이동 시작
			break; //출구 도달 시 종료
	}

	system("pause");
}