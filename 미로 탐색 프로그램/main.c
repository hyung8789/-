#include "maze.h"

/***
	�Ա� = e
	�ⱸ = x
	�湮�� ���� ��� �迭�� ���� '.'���� ����
	Ż�� ��θ� 'R'�� ǥ��
***/

void main()
{
	StackType s;

	init_stack(&s);

	while (1)
	{
		maze_print();
		if (pushloc_escape_root(&s) == SUCCESS) //�̵� ����
			break; //�ⱸ ���� �� ����
	}

	system("pause");
}