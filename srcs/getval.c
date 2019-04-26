#include "renju.h"

int array[NUM][NUM];
int value[NUM][NUM][4];

int huoSi(player)
{
	int i = 0, j = 0, huosi = 0;
	for(i = 0; i < NUM; i++)
		for(j = 0; j < NUM; j++){
			if(array[i][j] == player){
				if(value[i][j][0] >= 4 && i + 1 < NUM && i - 4 >= 0 && array[i + 1][j] != (3 - player) && array[i - 4][j] != (3 - player)) huosi++;
				if(value[i][j][1] >= 4 && i + 1 < NUM && j - 1 >= 0 && i - 4 >= 0 && j + 4 < NUM && array[i + 1][j - 1] != (3 - player) && array[i - 4][j + 4] != (3 - player)) huosi++;
				if(value[i][j][2] >= 4 && j - 1 >= 0 && j + 4 < NUM && array[i][j - 1] != (3 - player) && array[i][j + 4] != (3 - player)) huosi++;
				if(value[i][j][3] >= 4 && i - 1 >= 0 && j - 1 >= 0 && i + 4 < NUM && j + 4 < NUM && array[i - 1][j - 1] != (3 - player) && array[i + 4][j + 4] != (3 - player)) huosi++;
			}
		}

	return huosi;
}

int calculateValue(int nextplayer, int player)
{
	int i = 0, j = 0, sum = 0;
	int mianer = 0, miansan = 0, chongsi = 0, huoer = 0;
	int huosan = 0, huosi = 0;
	int flaga = 0, flagb = 0, wulian = 0;

	huosi = huoSi(player);

	//活三
	for(i = 0; i < NUM; i++)
		for(j = 0; j < NUM; j++){
			if(array[i][j] == player){
				if(value[i][j][0] == 3 && i + 1 < NUM && i - 3 >= 0 && array[i + 1][j] != (3 - player) && array[i - 3][j] != (3 - player)) huosan++;
				if(value[i][j][1] == 3 && i + 1 < NUM && j - 1 >= 0 && i - 3 >= 0 && j + 3 < NUM && array[i+ 1][j - 1] != (3 - player) && array[i - 3][j + 3] != (3 - player)) huosan++;
				if(value[i][j][2] == 3 && j - 1 >= 0 && j + 3 < NUM && array[i][j - 1] != (3 - player) && array[i][j + 3] != (3 - player)) huosan++;
				if(value[i][j][3] == 3 && i - 1 >= 0 && j - 1 >= 0 && i + 3 < NUM && j + 3 < NUM && array[i - 1][j - 1] != (3 - player) && array[i + 3][j + 3] != (3 - player)) huosan++;
			}
		}

	//活二
	for(i = 0; i < NUM; i++)
		for(j = 0; j < NUM; j++){
			if(array[i][j] == player){
				if(value[i][j][0] == 2 && i + 1 < NUM && i - 2 >= 0 && array[i + 1][j] != (3 - player) && array[i - 2][j] != (3 - player)) huoer++;
				if(value[i][j][1] == 2 && i + 1 < NUM && j - 1 >= 0 && i - 2 >= 0 && j + 2 < NUM && array[i+ 1][j - 1] != (3 - player) && array[i - 2][j + 2] != (3 - player)) huoer++;
				if(value[i][j][2] == 2 && j - 1 >= 0 && j + 2 < NUM && array[i][j - 1] != (3 - player) && array[i][j + 2] != (3 - player)) huoer++;
				if(value[i][j][3] == 2 && i - 1 >= 0 && j - 1 >= 0 && i + 2 < NUM && j + 2 < NUM && array[i - 1][j - 1] != (3 - player) && array[i + 2][j + 2] != (3 - player)) huoer++;
			}
		}

	//冲四
	for(i = 0; i < NUM; i++)
		for(j = 0; j < NUM; j++){
			if(array[i][j] == player){
				if(value[i][j][0] == 4){
					flaga = 0, flagb = 0;
					if(i + 1 >= NUM || (i + 1 < NUM && array[i + 1][j] == (3 -  player))) flaga = 1;
					if(i - 4 < 0 || (i - 4 >= 0 && array[i - 4][j] == (3 - player))) flagb = 1;
					if(flaga ^ flagb) chongsi++;
				}
				if(value[i][j][1] == 4){
					flaga = 0, flagb = 0;
					if(i + 1 >= NUM || j - 1 < 0 || (i + 1 < NUM && j - 1 >= 0 && array[i + 1][j - 1] == (3 - player))) flaga = 1;
					if(i - 4 < 0 || j + 4 >= NUM || (i - 4 >= 0 && j + 4 < NUM && array[i - 4][j + 4] == (3 - player))) flagb = 1;
					if(flaga ^ flagb) chongsi++;
				}
				if(value[i][j][2] == 4){
					flaga = 0, flagb = 0;
					if(j - 1 < 0 || (j - 1 >= 0 && array[i][j - 1] == (3 - player))) flaga = 1;
					if(j + 4 >= NUM || (j + 4 < NUM && array[i][j + 4] == (3 - player))) flagb = 1;
					if(flaga ^ flagb) chongsi++;
				}
				if(value[i][j][3] == 4){
					flaga = 0, flagb = 0;
					if(i - 1 < 0 || j - 1 < 0 || (i - 1 >= 0 && j - 1 >= 0 && array[i - 1][j - 1] == (3 - player))) flaga = 1;
					if(i + 4 >= NUM || j + 4 >= NUM || (i + 4 < NUM && j + 4 < NUM && array[i + 4][j + 4] == (3 - player))) flagb = 1;
					if(flaga ^ flagb) chongsi++;
				}
			}
		}

	//眠三
	for(i = 0; i < NUM; i++)
		for(j = 0; j < NUM; j++){
			if(array[i][j] == player){
				if(value[i][j][0] == 3){
					flaga = 0, flagb = 0;
					if(i + 1 >= NUM || (i + 1 < NUM && array[i + 1][j] == (3 -  player))) flaga = 1;
					if(i - 3 < 0 || (i - 3 >= 0 && array[i - 3][j] == (3 - player))) flagb = 1;
					if(flaga ^ flagb) miansan++;
				}
				if(value[i][j][1] == 3){
					flaga = 0, flagb = 0;
					if(i + 1 >= NUM || j - 1 < 0 || (i + 1 < NUM && j - 1 >= 0 && array[i + 1][j - 1] == (3 - player))) flaga = 1;
					if(i - 3 < 0 || j + 3 >= NUM || (i - 3 >= 0 && j + 3 < NUM && array[i - 3][j + 3] == (3 - player))) flagb = 1;
					if(flaga ^ flagb) miansan++;
				}
				if(value[i][j][2] == 3){
					flaga = 0, flagb = 0;
					if(j - 1 < 0 || (j - 1 >= 0 && array[i][j - 1] == (3 - player))) flaga = 1;
					if(j + 3 >= NUM || (j + 3 < NUM && array[i][j + 3] == (3 - player))) flagb = 1;
					if(flaga ^ flagb) miansan++;
				}
				if(value[i][j][3] == 3){
					flaga = 0, flagb = 0;
					if(i - 1 < 0 || j - 1 < 0 || (i - 1 >= 0 && j - 1 >= 0 && array[i - 1][j - 1] == (3 - player))) flaga = 1;
					if(i + 3 >= NUM || j + 3 >= NUM || (i + 3 < NUM && j + 3 < NUM && array[i + 3][j + 3] == (3 - player))) flagb = 1;
					if(flaga ^ flagb) miansan++;
				}
			}
		}

	//眠二
	for(i = 0; i < NUM; i++)
		for(j = 0; j < NUM; j++){
			if(array[i][j] == player){
				if(value[i][j][0] == 2){
					flaga = 0, flagb = 0;
					if(i + 1 >= NUM || (i + 1 < NUM && array[i + 1][j] == (3 -  player))) flaga = 1;
					if(i - 2 < 0 || (i - 2 >= 0 && array[i - 2][j] == (3 - player))) flagb = 1;
					if(flaga ^ flagb) mianer++;
				}
				if(value[i][j][1] == 2){
					flaga = 0, flagb = 0;
					if(i + 1 >= NUM || j - 1 < 0 || (i + 1 < NUM && j - 1 >= 0 && array[i + 1][j - 1] == (3 - player))) flaga = 1;
					if(i - 2 < 0 || j + 2 >= NUM || (i - 2 >= 0 && j + 2 < NUM && array[i - 2][j + 2] == (3 - player))) flagb = 1;
					if(flaga ^ flagb) mianer++;
				}
				if(value[i][j][2] == 2){
					flaga = 0, flagb = 0;
					if(j - 1 < 0 || (j - 1 >= 0 && array[i][j - 1] == (3 - player))) flaga = 1;
					if(j + 2 >= NUM || (j + 2 < NUM && array[i][j + 2] == (3 - player))) flagb = 1;
					if(flaga ^ flagb) mianer++;
				}
				if(value[i][j][3] == 2){
					flaga = 0, flagb = 0;
					if(i - 1 < 0 || j - 1 < 0 || (i - 1 >= 0 && j - 1 >= 0 && array[i - 1][j - 1] == (3 - player))) flaga = 1;
					if(i + 2 >= NUM || j + 2 >= NUM || (i + 2 < NUM && j + 2 < NUM && array[i + 2][j + 2] == (3 - player))) flagb = 1;
					if(flaga ^ flagb) mianer++;
				}
			}
		}

	if(nextplayer == player){
		sum += (chongsi + huosi) * INF;			
		sum += (huosan * INF >> 6);
		sum += (miansan * INF >> 8);
		sum += (huoer * INF >> 10);
		sum += (mianer * INF >> 12);
	}
	else{
		sum += (huosi * INF >> 2);
		sum += (chongsi * INF >> 4);
		sum += (huosan * INF >> 8);
		sum += (miansan * INF >> 8);
		sum += (huoer * INF >> 12);
		sum += mianer * INF >> 14;
	}
	return sum;
}

int getVal(int nextplayer)
{
	int computervalue = calculateValue(nextplayer, COMPUTER);
	int humanvalue = calculateValue(nextplayer, HUMAN);
	return computervalue - humanvalue;
}

