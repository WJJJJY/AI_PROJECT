#include "renju.h"
#include "getval.h"

int array[NUM][NUM];
int value[NUM][NUM][4];
int black[NUM * NUM][3];
int white[NUM * NUM][3];
int numwhite, numblack;

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

int checkHuoSi(int x, int y, int player, int a, int b)
{
	int i = 0;
	if(array[x][y] != player) return 0;
	if(x - a >= NUM || x - a < 0 || y - b < 0 || y - b >= NUM) return 0;
	if(x + 4 * a >= NUM || x + 4 * a < 0 || y + 4 * b >= NUM || y + 4 * b < 0) return 0;
	if(array[x - a][y - b] != EMPTY) return 0;	
	if(array[x + 4 * a][y + 4 * b] != EMPTY) return 0;
	for(i = 1; i < 4; i++) if(array[x + a * i][y + b * i] != player) return 0;	
	return 1;
}

int huoSi(int i, int j, int player)
{
	int huosi = 0;
	if(checkHuoSi(i, j, player, -1, 0)) huosi++; //0
	if(checkHuoSi(i, j, player, -1, 1)) huosi++; //1
	if(checkHuoSi(i, j, player,  0, 1)) huosi++; //2
	if(checkHuoSi(i, j, player,  1, 1)) huosi++; //3
	if(checkHuoSi(i, j, player, 1, 0)) huosi++;
	if(checkHuoSi(i, j, player, 1, -1)) huosi++;
	if(checkHuoSi(i, j, player,  0, -1)) huosi++; //2
	if(checkHuoSi(i, j, player, -1, -1)) huosi++; //3

	return huosi;
}

int checkChongSi(int x, int y, int player, int a, int b)
{
	int t = 0, i = 0, c = 0;
	int flaga = 0, flagb = 0;
	if(array[x][y] != player) return 0;
	if(x + 3 * a < NUM && x + 3 * a >= 0 && y + 3 * b < NUM && y + 3 * b >= 0){
		for(i = 0; i < 4; i++) if(array[x + i * a][y + i * b] == player) c++;
		if(c == 4){
			if(x - a < 0 || x - a >= NUM || y - b < 0 || y - b >= NUM || (x - a >= 0 && x - a < NUM && y - b >= 0 && y - b < NUM && array[x - a][y - b] == (3 - player))) flaga = 1;
			if(x + 4 * a < 0 || x + 4 * a >= NUM || y + 4 * b >= NUM || y + 4 * b < 0 || (x + 4 * a >= 0 && x + 4 * a < NUM && y + 4 * b < NUM && y + 4 * b >= 0 && array[x + 4 * a][y + 4 * b] == (3 - player))) flagb = 1;
			if(flaga ^ flagb) return 1;
		}
	}
	if(x + 4 * a < NUM && x + 4 * a >= 0 && y + 4 * b < NUM && y + 4 * b >= 0){
		for(i = 0; i < 5; i++)
			if(array[x + i * a][y + i * b] != player){
				if(array[x + i * a][y + i * b] == EMPTY) t++;	
				else return 0;
			}
		if(t == 1) return 1;
		return 0;
	}
	return 0;
}

int chongSi(int i, int j, int player)
{
	int chongsi = 0;
	if(checkChongSi(i, j, player, -1, 0)) chongsi++;
	if(checkChongSi(i, j, player, -1, 1)) chongsi++;
	if(checkChongSi(i, j, player, 0, 1)) chongsi++;
	if(checkChongSi(i, j, player, 1, 1)) chongsi++;
	if(checkChongSi(i, j, player, 1, 0)) chongsi++;
	if(checkChongSi(i, j, player, 1, -1)) chongsi++;
	if(checkChongSi(i, j, player, 0, -1)) chongsi++;
	if(checkChongSi(i, j, player, -1, -1)) chongsi++;
	return chongsi;
}

int checkHuoSan(int x, int y, int player, int a, int b)
{
	int i = 0, check = 0;
	if(array[x][y] != player) return 0;
	if(x - a >= 0 && x - a < NUM && y - b >= 0 && y - b < NUM && array[x - a][y - b] == EMPTY){
		array[x - a][y - b] = player;
		check = checkHuoSi(x - a, y - b, player, a, b);
		array[x - a][y - b] = EMPTY;
		if(check) return check;
	}
	for(i = 1; i < 4; i++){
		if(x + i * a >= 0 && x + i * a < NUM && y + i * b >= 0 && y + i * b < NUM){
			if(array[x + i * a][y + i * b] == (3 - player)) break;
			if(array[x + i * a][y + i * b] == EMPTY){
				array[x + i * a][y + i * b] = player;
				check = checkHuoSi(x, y, player, a, b);
				array[x + i * a][y + i * b] = EMPTY;
				if(check) return check;
			}
		}
	}
	return 0;
}

int huoSan(int i, int j, int player)
{
	int huosan = 0;
	if(checkHuoSan(i, j, player, -1, 0)) huosan++;
	if(checkHuoSan(i, j, player, -1, 1)) huosan++;
	if(checkHuoSan(i, j, player, 0, 1)) huosan++;
	if(checkHuoSan(i, j, player, 1, 1)) huosan++;
	if(checkHuoSan(i, j, player, 1, 0)) huosan++;
	if(checkHuoSan(i, j, player, 1, -1)) huosan++;
	if(checkHuoSan(i, j, player, 0, -1)) huosan++;
	if(checkHuoSan(i, j, player, -1, -1)) huosan++;
	return huosan;
}

int checkMianSan(int x, int y, int player, int a, int b)
{
	int check = 0, i = 0;
	if(array[x][y] != player) return 0;
	if(x - a >= 0 && x - a < NUM && y - b >= 0 && y - b < NUM && array[x - a][y - b] == EMPTY){
		array[x - a][y - b] = player;
		check = checkChongSi(x - a, y - b, player, a, b);
		array[x - a][y - b] = EMPTY;
		if(check) return check;
	}
	for(i = 1; i < 4; i++){
		if(x + i * a >= 0 && x + i * b < NUM && y + i * b < NUM && y + i * b >= 0){
			if(array[x + i * a][y + i * b] == (3 - player)) break;
			if(array[x + i * a][y + i * b] == EMPTY){
				array[x + i * a][y + i * b] = player;
				check = checkChongSi(x, y, player, a, b);
				array[x + i * a][y + i * b] = EMPTY;
				if(check) return check;
			}
		}
	}
	return 0;
}

int mianSan(int i, int j, int player)
{
	int miansan = 0;
	if(checkMianSan(i, j, player, -1, 0)) miansan++;
	if(checkMianSan(i, j, player, -1, 1)) miansan++;
	if(checkMianSan(i, j, player, 0, 1)) miansan++;
	if(checkMianSan(i, j, player, 1, 1)) miansan++;
	if(checkMianSan(i, j, player, 1, 0)) miansan++;
	if(checkMianSan(i, j, player, 1, -1)) miansan++;
	if(checkMianSan(i, j, player, 0, -1)) miansan++;
	if(checkMianSan(i, j, player, -1, -1)) miansan++;
	return miansan;
}

int checkHuoEr(int x, int y, int player, int a, int b)
{
	int i = 0, check = 0;
	if(array[x][y] != player) return 0;
	if(x - a >= 0 && x - a < NUM && y - b >= 0 && y - b < NUM && array[x - a][y - b] == EMPTY){
		array[x - a][y - b] = player;
		check = checkHuoSan(x - a, y - b, player, a, b);
		array[x - a][y - b] = EMPTY;
		if(check) return check;
	}
	for(i = 1; i < 3; i++){
		if(x + i * a < NUM && x + i * a >= 0 && y + i * b < NUM && y + i * b >= 0){
			if(array[x + i * a][y + i * b] == (3 - player)) break;
			if(array[x + i * a][y + i * b] == EMPTY){
				array[x + i * a][y + i * b] = player;
				check = checkHuoSan(x, y, player, a, b);
				array[x + i * a][y + i * b] = EMPTY;
				if(check) return 1;
			}
		}
	}
	return 0;
}

int huoEr(int i, int j, int player)
{
	int huoer = 0;
	if(checkHuoEr(i, j, player, -1, 0)) huoer++;
	if(checkHuoEr(i, j, player, -1, 1)) huoer++;
	if(checkHuoEr(i, j, player, 0, 1)) huoer++;
	if(checkHuoEr(i, j, player, 1, 1)) huoer++;
	if(checkHuoEr(i, j, player, 1, 0)) huoer++;
	if(checkHuoEr(i, j, player, 1, -1)) huoer++;
	if(checkHuoEr(i, j, player, 0, -1)) huoer++;
	if(checkHuoEr(i, j, player, -1, -1)) huoer++;
	return huoer;
}

int checkMianEr(int x, int y, int player, int a, int b)
{
	int check = 0, i = 0;
	if(array[x][y] != player) return 0;
	if(x - a >= 0 && x - a < NUM && y - b >= 0 && y - b < NUM && array[x - a][y - b] == EMPTY){
		array[x - a][y - b] = player;
		check = checkMianSan(x - a, y - b, player, a, b);
		array[x - a][y - b] = EMPTY;
		if(check) return 1;
	}
	for(i = 1; i < 4; i++){
		if(x + i * a >= 0 && x + i * a < NUM && y + i * b >= 0 && y + i * b < NUM){
			if(array[x + i * a][y + i * b] == (3 - player)) break;
			if(array[x + i * a][y + i * b] == EMPTY){
				array[x + i * a][y + i * b] = player;
				check = checkMianSan(x, y, player, a, b);
				array[x + i * a][y + i * b] = EMPTY;
				if(check) return check;
			}
		}
	}
	return 0;
}

int mianEr(int i, int j, int player)
{
	int mianer = 0;
	if(checkMianEr(i, j, player, -1, 0)) mianer++;
	if(checkMianEr(i, j, player, -1, 1)) mianer++;
	if(checkMianEr(i, j, player, 0, 1)) mianer++;
	if(checkMianEr(i, j, player, 1, 1)) mianer++;
	if(checkMianEr(i, j, player, 1, 0)) mianer++;
	if(checkMianEr(i, j, player, 1, -1)) mianer++;
	if(checkMianEr(i, j, player, 0, -1)) mianer++;
	if(checkMianEr(i, j, player, -1, -1)) mianer++;
	return mianer;
}

int calculateValue(int nextplayer, int player)
{
	int i = 0, j = 0, sum = 0;
	int mianer = 0, miansan = 0, chongsi = 0, huoer = 0;
	int huosan = 0, huosi = 0;
	int flaga = 0, flagb = 0, t = 0;

	for(t = 0; t <= numblack; t++){
		i = black[t][0], j = black[t][1];
		
		huosi += huoSi(i, j, player);
		chongsi += chongSi(i, j, player);
		huosan += huoSan(i, j, player);
		miansan += mianSan(i, j, player);
		huoer += huoEr(i, j, player);
		mianer += mianEr(i, j, player);
	}

	//printf("%d %d %d %d %d %d\n", huosi, chongsi, huosan, miansan, huoer, mianer);
	/*if(nextplayer == player){
	  sum += (chongsi + huosi) * INF;			
	  sum += (huosan * INF / 100);
	  sum += (miansan * INF / 10000);
	  sum += (huoer * INF / 1000000);
	  sum += (mianer * INF / 10000000);
	  }
	  else{
	  sum += (huosi * INF / 10);
	  sum += (chongsi * INF / 1000);
	  sum += (huosan * INF / 10000);
	  sum += (miansan * INF / 100000);
	  sum += (huoer * INF / 10000000);
	  sum += mianer * INF / 100000000;
	  }*/
	sum += huosi * INF;
	if(huosan <= 1) sum += huosan * INF / 10;
	else sum += INF;
	if(chongsi <= 1) sum += chongsi * INF / 100;
	else sum += INF;
	sum += miansan * INF / 2000;
	sum += huoer * INF / 1000;
	sum += mianer * INF / 10000;
	return sum;
}

int getVal(int nextplayer)
{
	int computervalue = calculateValue(nextplayer, COMPUTER);
	int humanvalue = calculateValue(nextplayer, HUMAN);
	//printf("%d\n", computervalue - humanvalue);
	return computervalue - humanvalue;
}

