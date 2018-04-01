#include <stdio.h>
#include <stdlib.h>

int throwsWithoutCL(int board[])
{
	int throws = 0;
	int throws_list[73] = {0};
	int pos = 0, i=0;
	while(pos<=73)
	{
		pos+=6;
		if(board[pos-1]==pos)
			throws_list[throws] = 6;
		else
		{
			if(pos-73>0)
			throws_list[throws] = 73 - pos + 6;
			else
			{
				throws_list[throws] = 5;
				pos = pos - 6 + 5;
			}
		}
		throws++;
	}

	while(i<throws)
	{
		printf("----> %d\n",throws_list[i]);
		i++;
	}
	return throws;
}

int main()
{
	int board[73]={0};
	for(int i=0;i<73;i++)
	board[i] = i+1;

	board[2] = 21;
	board[4] = 46;
	board[22] = 43;
	board[26] = 47;
	board[51] = 71;

	board[18] = 1;
	board[55] = 8;
	board[48] = 20;
	board[35] = 28;
	board[64] = 45;
	board[69] = 51;

	for(int i=0;i<73;i++)
	printf("%d\n",board[i]);


	int temp = throwsWithoutCL(board);
	printf("%d\n",temp);

}
