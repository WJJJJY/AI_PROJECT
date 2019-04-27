#include "getval.h"
#include "renju.h"

int array[NUM][NUM];                    /* 棋盘数组 */
int value[NUM][NUM][4];
int node[NUM * NUM][3];
int checknode[NUM][NUM];
int isfirststep;
int numnode;
int count = 0;

int checkWuLian(int x, int y, int player, int a, int b)
{
	int i = 0, j = 0;
	int flag = 0, p = 0, q = 0;
	for(i = 4; i >= 0; i--){
		if(x - a * i >= 0 && x - a * i < NUM && y - b * i >= 0 && y - b * i < NUM){
			flag = 0, p = x - a * i, q = y - b * i;
			for(j = 0; j <= 4; j++){
				if(p + j * a < NUM && p + j * a >= 0 && q + j * b < NUM && q + j * b >= 0 && array[p + j * a][q + j * b] == player) flag++;
			}
			if(flag == 5) return 1;
		}
	}
	return 0;
}

int wuLian(int x, int y, int player)
{
	if(checkWuLian(x, y, player, -1, 0)) return 1;
	if(checkWuLian(x, y, player, -1, 1)) return 1;
	if(checkWuLian(x, y, player, 0, 1)) return 1;
	if(checkWuLian(x, y, player, 1, 1)) return 1;
	return 0;
}

int Check(int x, int y, int player)
{
	array[x][y] = player;
	if(huoSi(x, y, player) > 0 || chongSi(x, y, player) > 0 || huoSan(x, y, player) > 0 || mianSan(x, y, player) > 0 || huoEr(x, y, player) > 0 || mianEr(x, y, player) > 0){
		array[x][y] = EMPTY;
		return 1;
	}
	array[x][y] = 3 - player;
	if(huoSi(x, y, 3 - player) > 0 || chongSi(x, y, 3 - player) > 0 || huoSan(x, y, 3 - player) > 0 || wuLian(x, y, 3 - player) > 0){
		array[x][y] = EMPTY;
		return 1;
	}
	return 0;
}

void addnode(int x, int y)
{
	int i = 0, j = 0;
	for(i = -2; i <= 2; i++){
		for(j = -2; j <= 2; j++){
			if(x + i >= 0 && x + i < NUM && y + j >= 0 && y + j < NUM && array[x + i][y + j] == EMPTY){
				if(checknode[x + i][y + j] == 0){
					checknode[x + i][y + j] = 1;
					numnode++;
					node[numnode][0] = x + i;
					node[numnode][1] = y + j;
				}
			}
		}
	}
}

void subtractValue(int x, int y, int player)
{		
	int i = 0;
	for(i = 1; i < 5; i++){
		if(y - i < 0 || array[x][y - i] != player) break;
		value[x][y - i][2] -= value[x][y][2];
	}
	for(i = 1; i < 5; i++){
		if(x + i >= NUM || array[x + i][y] != player) break;
		value[x + i][y][0] -= value[x][y][0];
	}
	for(i = 1; i < 5; i++){
		if(x + i >= NUM || y - i < 0 || array[x + i][y - i] != player) break;
		value[x + i][y - i][1] -= value[x][y][1];
	}
	for(i = 1; i < 5; i++){
		if(x - i < 0 || y - i < 0 || array[x - i][y - i] != player) break;
		value[x - i][y - i][3] -= value[x][y][3];
	}

	value[x][y][0] = 0, value[x][y][1] = 0, value[x][y][2] = 0;
}

void addValue(int x, int y, int player)
{
	int i = 0;

	if(x - 1 < 0 || array[x - 1][y] != player) value[x][y][0] = 1;
	else value[x][y][0] = value[x - 1][y][0] + 1;

	if(y + 1 >= NUM || array[x][y + 1] != player) value[x][y][2] = 1;
	else value[x][y][2] = value[x][y + 1][2] + 1;

	if(x - 1 < 0 || y + 1 >= NUM || array[x - 1][y + 1] != player) value[x][y][1] = 1;
	else value[x][y][1] = value[x - 1][y + 1][1] + 1;

	if(x + 1 >= NUM || y + 1 >= NUM || array[x + 1][y + 1] != player) value[x][y][3] = 1;
	else value[x][y][3] = value[x + 1][y + 1][3] + 1;

	for(i = 1; i < 5; i++){
		if(x + i >= NUM || array[x + i][y] != player) break;
		value[x + i][y][0] += value[x][y][0];
	}
	for(i = 1; i < 5; i++){
		if(x + i >= NUM || y - i < 0 || array[x + i][y - i] != player) break;
		value[x + i][y - i][1] += value[x][y][1];
	}
	for(i = 1; i < 5; i++){
		if(y - i < 0 || array[x][y - i] != player) break;
		value[x][y - i][2] += value[x][y][2];
	}
	for(i = 1; i < 5; i++){
		if(x - i < 0 || y - i < 0 || array[x - i][y - i] != player) break;
		value[x - i][y - i][3] += value[x][y][3];
	}
}

int dfs(int d, int player, int alpha, int beta)
{
	int val = 0;
	if(d == 0){
		count++;
		//printf("%d\n", count);
		return getVal(player);
	}

	if(d & 1){
		int val = INF * 100, i = 0, j = 0;
		int k = 0, now = 0;
		beta = INF * 100;
		for(k = 0; k <= numnode; k++){
			i = node[k][0], j = node[k][1];
			if(array[i][j] == EMPTY && Check(i, j, player)){
				array[i][j] = player;
				//addValue(i, j, player);
				now = numnode;
				addnode(i, j);
				if(wuLian(i, j, player)) beta = -INF * 20;
				val = min(val, dfs(d - 1, 3 - player, alpha, beta));
				array[i][j] = EMPTY;
				//subtractValue(i, j, player);

				beta = min(beta, val);
				while(numnode > now){
					checknode[node[numnode][0]][node[numnode][1]] = 0;
					numnode--;
				}
				if(alpha > beta){
					//printf("++++++++++++++++++++\n");
					return alpha;
				}
			}
		}

		return beta;
	}
	else{
		int val = -INF * 100, i = 0, j = 0;
		alpha = -INF * 100;
		int k = 0, now = 0;
		for(k = 0; k <= numnode; k++){
			i = node[numnode][0], j = node[numnode][1];
			if(array[i][j] == EMPTY && Check(i, j, player)){
				array[i][j] = player;
				//addValue(i, j, player);
				now = numnode;
				addnode(i, j);
				if(wuLian(i, j, player)) alpha = INF * 20;
				val = max(val, dfs(d - 1, 3 - player, alpha, beta));
				array[i][j] = EMPTY;
				//subtractValue(i, j, player);

				alpha = max(val, alpha);
				while(numnode > now){
					checknode[node[numnode][0]][node[numnode][1]] = 0;
					numnode--;
				}
				if(alpha > beta){
					//printf("++++++++++++++++++++\n");
					return beta;
				}
			}
		}

		return alpha;
	}

}

int play(int x1, int y1, int *x2, int *y2)// x1,y1为HUMAN.
{
	if(array[y1][x1]==EMPTY)	{
		//if(isIllegal(HUMAN, x1, y1)==1)
		//return 3;

		struct timeval start, end;
		double timeuse = 0;
		gettimeofday(&start, NULL);

		array[y1][x1] = HUMAN;
		//addValue(y1, x1, HUMAN);
		addnode(y1, x1);

		if(wuLian(y1, x1, HUMAN))
			return HUMAN;
		if(isfirststep){
			firstStep(x1, y1, x2, y2);
			isfirststep = 0;

			gettimeofday(&end, NULL);
			timeuse = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
			printf("time=%lfs\n", timeuse);

			return 0;
		}
		else{	
			count = 0;
			int i = 0, j = 0, k = 0;
			int val = -INF * 100, now = 0;
			for(k = 0; k <= numnode; k++){
				i = node[k][0], j = node[k][1];
				if(array[i][j] == EMPTY && Check(i, j, COMPUTER)){
					array[i][j] = COMPUTER;
					//addValue(i, j, COMPUTER);
					now = numnode;
					addnode(i, j);
					if(wuLian(i, j, COMPUTER)){
						*x2 = j, *y2 = i;
						for(i = 0; i < NUM; i++){
							for(j = 0; j < NUM; j++)
								printf("%d ", array[i][j]);
							printf("\n");
						}
						if(checkWuLian(i, j, COMPUTER, -1, 0)) printf("11111\n");
						if(checkWuLian(i, j, COMPUTER, -1, 1)) printf("22222\n");
						if(checkWuLian(i, j, COMPUTER, 0, 1)) printf("33333\n");
						if(checkWuLian(i, j, COMPUTER, 1, 1)) printf("44444\n");

						return COMPUTER;
					}
					int t = dfs(3, HUMAN, -INF * 10, 10 * INF);
					//int t = getVal(HUMAN);
					array[i][j] = EMPTY;
					//subtractValue(i, j, COMPUTER);
					while(numnode > now){
						checknode[node[numnode][0]][node[numnode][1]] = 0;
						numnode--;
					}
					if(t > val) val = t, *x2 = j, *y2 = i;
				}
			}

			array[*y2][*x2] = COMPUTER;
			i = *y2, j = *x2;
			//addValue(i, j, COMPUTER);

			for(i = 0; i < NUM; i++){
				for(j = 0; j < NUM; j++){
					printf("%d ", array[i][j]);
				}
				printf("\n");
			}

			//for(i = 0; i < numnode; i++) printf("%d %d\n", node[i][0], node[i][1]);
			gettimeofday(&end, NULL);
			timeuse = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
			printf("time=%lfs\n", timeuse);

			return 0;
		}
	}
}

int isWin(int player)
{
	int i, j, drY, drX;

	for(i=0; i<NUM; i++)
		for(j=0; j<NUM; j++){
			if(array[i][j]!=player) continue;
			if(value[i][j][0] >= 5) return 1;
			if(value[i][j][2] >= 5) return 1;
			if(value[i][j][1] >= 5) return 1;
			if(value[i][j][3] >= 5) return 1;
		}
	return 0;
}

int firstStep(int x1, int y1, int *x2, int *y2)
{
	int drY, drX, a, b;

	for(drY=-1; drY<2; drY++)
		for(drX=-1; drX<2; drX++){
			if(drY==0 && drX==0)    continue;
			if((y1+drY)>=0 && (y1+drY)<NUM \
					&& (x1+drX)>=0 && (x1+drX)<NUM \
					&& array[y1+drY][x1+drX]==EMPTY ){
				*y2 = y1+drY;
				*x2 = x1+drX;
				array[*y2][*x2] = COMPUTER;
				a = *y2, b = *x2;
				//addValue(a, b, COMPUTER);
				addnode(a, b);
				return 1;
			}
		}

	return 0;
}

int judge(int player, int num, int *x2, int *y2)
{
	int i, j, drY, drX, tmp, op;
	op = ((player==HUMAN) ? COMPUTER:HUMAN);

	for(i=0; i<NUM; i++)
		for(j=0; j<NUM; j++)
		{
			if(array[i][j]!=player)
				continue;
			if(judgePoint(player, num, 1, i, j, x2, y2)==1)
				return 1;
		}

	for(i=0; i<NUM; i++)
		for(j=0; j<NUM; j++)
		{
			if(array[i][j]!=player)
				continue;
			if(judgePoint(player, num, 3, i, j, x2, y2)==1)
				return 1;
		}

	return 0;
}

int judgePoint(int player, int num, int mode, int i, int j, int *x2, int *y2)
{
	int drY, drX, tmp, op;
	op = ((player==HUMAN) ? COMPUTER:HUMAN);

	for(drY=-1; drY<2; drY++)
		for(drX=-1; drX<2; drX++)
		{
			int rtn=0;
			if(drY==0 && drX==0)    continue;

			for(tmp=1; tmp<5; tmp++)
				if((i+drY*tmp)<0 || (i+drY*tmp)>=NUM \
						|| (j+drX*tmp)<0 || (j+drX*tmp)>=NUM \
						|| array[i+drY*tmp][j+drX*tmp]==op)
					rtn = 1;

			if(rtn==0)
			{
				int flag=0, count=1, k=1, cflag=0;

				for(tmp=1; tmp<=num; tmp++)
				{
					if((array[i+drY*tmp][j+drX*tmp]==player \
								|| (array[i+drY*tmp][j+drX*tmp]==EMPTY \
									&& flag==0 && ++flag)) \
							&& (flag==0 && (++count) || 1))
					{
						if(k==num)
						{
							if(((player==COMPUTER && num==2) \
										|| (player==HUMAN && num==3)) \
									&& (i-drY)>=0 && (i-drY)<NUM \
									&& (j-drX)>=0 && (j-drX)<NUM \
									&& array[i-drY][j-drX]!=op )
								cflag=1;
							if((player==COMPUTER && (num==3||num==4)) \
									||(player==HUMAN && num==4) )
								cflag=1;

							if(cflag==1){
								if(mode==1)
								{
									if(player==HUMAN)
									{
										if(judgePoint(op, 3, 2, i+drY*count, j+drX*count, x2, y2)==1 \
												|| judgePoint(op, 4, 2, i+drY*count, j+drX*count, x2, y2)==1)
										{
											*y2 = i+drY*count;
											*x2 = j+drX*count;
											array[*y2][*x2] = COMPUTER;
											return 1;
										}

										if(judgePoint(player, 3, 2, i+drY*count, j+drX*count, x2, y2)==1 \
												|| judgePoint(player, 4, 2, i+drY*count, j+drX*count, x2, y2)==1)
										{
											*y2 = i+drY*count;
											*x2 = j+drX*count;
											array[*y2][*x2] = COMPUTER;
											return 1;
										}
									}
								}
								if(mode==2)
									return 1;
								if(mode==3)
								{
									*y2 = i+drY*count;
									*x2 = j+drX*count;
									array[*y2][*x2] = COMPUTER;
									return 1;
								}
							}
						}
						k++;
					}
				}
			}
		}
	return 0;
}

int forcast(int player, int num, int *x2, int *y2)
{
	int i, j, drY, drX, tmp, op, fnum, dbflag;
	op = ((player==HUMAN) ? COMPUTER:HUMAN);

	for(i=0; i<NUM; i++)
		for(j=0; j<NUM; j++)
		{
			if(array[i][j]!=EMPTY)    continue;
			fnum = 0;
			dbflag = 0;

			for(drY=-1; drY<2; drY++)
				for(drX=-1; drX<2; drX++)
				{
					int rtn=0;
					if(drY==0 && drX==0)    continue;

					for(tmp=1; tmp<4; tmp++)
						if((i+drY*tmp)<0 || (i+drY*tmp)>=NUM \
								|| (j+drX*tmp)<0 || (j+drX*tmp)>=NUM \
								|| array[i+drY*tmp][j+drX*tmp]==op)
							rtn = 1;

					if(rtn==0)
					{
						int flag=0, count=1, k=1, cflag=0;

						for(tmp=1; tmp<=num; tmp++)
						{
							if((array[i+drY*tmp][j+drX*tmp]==player \
										|| (array[i+drY*tmp][j+drX*tmp]==EMPTY \
											&& flag==0 && ++flag)) \
									&& (flag==0 && (++count) || 1))
							{
								if(k==num \
										&& (i-drY)>=0 && (i-drY)<NUM \
										&& (j-drX)>=0 && (j-drX)<NUM \
										&& array[i-drY][j-drX]!=op )
								{
									if(count==3 && (i+drY*4)>=0 && (i+drY*4)>=NUM \
											&& (j+drX*4)>=0 && (j+drX*4)<NUM \
											&& array[i+drY*4][j+drX*4]==EMPTY)
										dbflag++;

									fnum++;
									if( findTwo(player ,i+drY*count, j+drX*count, drY, drX, x2, y2)==1)
										return 1;
								}
								k++;
							}
						}
					}
				}

			if(dbflag <2 &&fnum>1){
				*y2 = i;
				*x2 = j;
				array[*y2][*x2] = COMPUTER;
				return 1;
			}
		}
	return 0;
}

int findTwo(int player, int i, int j, int d1, int d2,int *x2, int *y2)
{
	int num=0, count, drY, drX, op;
	int dr[2][4]={{-1, -1, -1, 0}, \
		{-1, 0, 1, 1}};
	op = ((player==HUMAN) ? COMPUTER:HUMAN);

	for(count=0; count<4; count++){
		drY = dr[0][count];
		drX = dr[2][count];

		if((drY==d1 && drX==d2) || (-drY==d1 && -drX==d2))    continue;

		if( (i+drY)>=0 && (i+drY)<NUM \
				&& (j+drX)>=0 && (j+drX)<NUM \
				&& (i+drY)>=0 && (i+drY)<NUM \
				&& (j+drX)>=0 && (j+drX)<NUM \
				&& array[i+drY][j+drX]==player \
				&& array[i+drY][j+drX]!=op )
			if( (i-drY)>=0 && (i-drY)<NUM \
					&& (j-drX)>=0 && (j-drX)<NUM \
					&& (i-drY)>=0 && (i-drY)<NUM \
					&& (j-drX)>=0 && (j-drX)<NUM \
					&& array[i-drY][j-drX]==player \
					&& array[i-drY][j-drX]!=op )
				num++;
	}

	if(num>0){
		*y2 = i;
		*x2 = j;
		array[*y2][*x2] = COMPUTER;
		return 1;
	}

	return 0;
}

int isIllegal(int player, int j, int i)
{
	int drY, drX, tmp, op, dbflag;
	op = ((player==HUMAN) ? COMPUTER:HUMAN);

	dbflag = 0;

	for(drY=-1; drY<2; drY++)
		for(drX=-1; drX<2; drX++)
		{
			int rtn=0;
			if(drY==0 && drX==0)    continue;

			for(tmp=1; tmp<4; tmp++)
				if((i+drY*tmp)<0 || (i+drY*tmp)>=NUM \
						|| (j+drX*tmp)<0 || (j+drX*tmp)>=NUM \
						|| array[i+drY*tmp][j+drX*tmp]==op)
					rtn = 1;

			if((i+drY*4)<0 || (i+drY*4)>=NUM \
					|| (j+drX*4)<0 || (j+drX*4)>=NUM \
					|| array[i+drY*4][j+drX*4]!=EMPTY)
				rtn = 1;

			if(rtn==0)
			{
				int flag=0, count=1, k=1, cflag=0;

				if(array[i+drY][j+drX]==player \
						&& array[i+drY*2][j+drX*2]==player \
						&& (i-drY)>=0 && (i-drY)<NUM \
						&& (j-drX)>=0 && (j-drX)<NUM \
						&& array[i-drY][j-drX]==EMPTY)
					dbflag++;   
			}
		}

	if(dbflag>1)
		return 1;

	return 0;
}
