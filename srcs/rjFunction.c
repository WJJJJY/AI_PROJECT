#include "getval.h"
#include "renju.h"

int array[NUM][NUM];                    /* 棋盘数组 */
int value[NUM][NUM][4];
int isfirststep;


int Check(int x, int y)
{
	if(array[x][y] != EMPTY) return 0;
	int flag = 0, i = 0, j = 0;
	for(i = -2; i <= 2; i++)
		for(j = -2; j <= 2; j++){
			if(x + i < 0 || y + j < 0 || x + i >= NUM || y + j >= NUM)
				continue;
			if(array[x + i][y + j] != EMPTY) flag = 1;
		}

	return flag;
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

int dfs(int d, int player)
{
	int val = 0;
	if(d == 0) return getVal(player);

	if(d & 1){
		int val = INF * 100, i = 0, j = 0;
		for(i = 0; i < NUM; i++)
			for(j = 0; j < NUM; j++){
				if(Check(i, j)){
					array[i][j] = player;
					addValue(i, j, player);
					if(isWin(player)){
						array[i][j] = EMPTY;
						subtractValue(i, j, player);
						return -INF * 100;
					}
					val = min(val, dfs(d - 1, 3 - player));
					array[i][j] = EMPTY;
					subtractValue(i, j, player);
				}
			}

		return val;
	}
	else{
		int val = -INF * 100, i = 0, j = 0;
		for(i = 0; i < NUM; i++)
			for(j = 0; j < NUM; j++){
				if(Check(i, j)){
					array[i][j] = player;
					addValue(i, j, player);
					if(isWin(player)){
						array[i][j] = EMPTY;
						subtractValue(i, j, player);
						return INF * 100;
					}
					val = max(val, dfs(d - 1, 3 - player));
					array[i][j] = EMPTY;
					subtractValue(i, j, player);
				}
			}

		return val;
	}

}

int play(int x1, int y1, int *x2, int *y2)// x1,y1为HUMAN.
{
	if(array[y1][x1]==EMPTY)	{
		if(isIllegal(HUMAN, x1, y1)==1)
			return 3;

		array[y1][x1] = HUMAN;
		//printf("%d %d\n", y1, x1);
		addValue(y1, x1, HUMAN);
		if(isWin(HUMAN))
			return HUMAN;
		if(isfirststep){
			firstStep(x1, y1, x2, y2);
			isfirststep = 0;
			return 0;
		}
		else{	
			int i = 0, j = 0;
			int val = -INF * 100;
			for(i = 0; i < NUM; i++)
				for(j = 0; j < NUM; j++){
					if(Check(i, j)){
						array[i][j] = COMPUTER;
						addValue(i, j, COMPUTER);
						if(isWin(COMPUTER)) return COMPUTER;
						//int t = dfs(3, HUMAN);
						int t = getVal(HUMAN);
						array[i][j] = EMPTY;
						subtractValue(i, j, COMPUTER);
						if(t > val) val = t, *x2 = j, *y2 = i;
					}
				}

			array[*y2][*x2] = COMPUTER;
			i = *y2, j = *x2;
			addValue(i, j, COMPUTER);

			/*for(i = 0; i < NUM; i++){
			  for(j = 0; j < NUM; j++){
			  printf("%d ", array[i][j]);
			  }
			  printf("\n");
			  }*/

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
				addValue(a, b, COMPUTER);
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
