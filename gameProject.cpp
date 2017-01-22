#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include <glut.h>
#include "GLMetaseq.h"

#define MAX_HEIGHT  SOLID_SIZE*11
#define SOLID_SIZE  100
#define MAX_BLOCK   30
#define ENEMY_SIZE  20
#define ITEM_SIZE   20
#define AREA_SIZE MAX_BLOCK*SOLID_SIZE
#define MAX_ENEMY  150 // 同時に出現する敵の最大数
#define MAX_ITEM  20
// メタセコイアのモデル
MQO_MODEL g_mqoModel;

// グローバル変数
static bool KeyUpON    = false;	// 矢印キーの状態フラグ
static bool KeyDownON  = false;	// 矢印キーの状態フラグ
static bool KeyLeftON  = false;	// 矢印キーの状態フラグ
static bool KeyRightON = false;	// 矢印キーの状態フラグ
static float Scale = 1.0; // 拡大量

void drawInit(void);
void printState(void);


// マップ
int map[MAX_BLOCK][MAX_BLOCK] = {
	{ 0, 9, 9, 9, 9, 9, 8, 8, 9, 9, 9, 9, 0, 0, 0, 9, 9, 9, 9, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9 },
	{ 9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9 },
	{ 9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 5, 8, 8, 9 },
	{ 9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 8, 8, 7, 8, 8, 5, 8, 9 },
	{ 9, 8, 8, 9, 9, 9, 8, 8, 8, 5, 5, 5, 5, 5, 5, 5, 8, 8, 8, 9, 9, 9, 8, 8, 8, 8, 8, 8, 8, 9 },
	{ 9, 8, 8, 9, 9, 9, 8, 8, 8, 5, 1, 1, 1, 1, 1, 1, 1, 1, 8, 8, 8, 8, 8, 8, 8, 8, 5, 8, 8, 9 },
	{ 9, 8, 8, 7, 8, 8, 8, 8, 8, 5, 1, 8, 8, 8, 8, 8, 8, 1, 8, 8, 8, 8, 9, 8, 8, 8, 8, 5, 8, 9 },
	{ 9, 8, 8, 7, 8, 8, 8, 8, 8, 5, 8, 8, 9, 8, 8, 9, 8, 1, 8, 8, 8, 8, 8, 8, 7, 8, 9, 8, 8, 9 },
	{ 9, 8, 8, 8, 7, 0, 0, 8, 8, 5, 8, 8, 8, 9, 9, 8, 8, 1, 8, 8, 8, 8, 0, 0, 0, 0, 9, 8, 8, 9 },
	{ 0, 0, 8, 8, 7, 0, 0, 8, 8, 8, 8, 8, 8, 9, 9, 8, 8, 1, 8, 8, 8, 8, 0, 0, 0, 0, 9, 8, 8, 8 },
	{ 0, 0, 8, 9, 8, 8, 8, 8, 8, 8, 8, 8, 9, 8, 8, 9, 8, 1, 8, 8, 8, 8, 0, 0, 0, 0, 8, 8, 8, 8 },
	{ 0, 0, 8, 8, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 8, 8, 8, 8, 8, 9 },
	{ 9, 8, 8, 8, 0, 9, 0, 8, 8, 8, 1, 1, 1, 1, 1, 1, 1, 1, 8, 8, 8, 8, 7, 7, 8, 8, 8, 8, 8, 8 },
	{ 9, 8, 4, 8, 8, 8, 8, 8, 8, 8, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 8, 8, 8, 8, 8, 8 },
	{ 9, 4, 8, 8, 8, 9, 8, 9, 8, 8, 1, 8, 8, 9, 9, 8, 8, 8, 0, 0, 0, 8, 7, 7, 8, 8, 9, 8, 8, 9 },
	{ 8, 4, 4, 8, 8, 9, 8, 8, 8, 8, 1, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0, 8, 8, 8, 8, 8, 9, 8, 8, 9 },
	{ 8, 8, 8, 8, 8, 9, 8, 8, 9, 8, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 8, 8, 8, 8, 8, 9, 3, 8, 9 },
	{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9 },
	{ 9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 8, 8, 8, 8, 8, 8, 8, 8, 2, 2, 3, 8, 9 },
	{ 9, 8, 1, 1, 1, 1, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 4, 4, 8, 8, 8, 8, 9 },
	{ 9, 8, 1, 0, 0, 9, 1, 1, 8, 1, 8, 8, 8, 8, 9, 8, 9, 8, 9, 8, 8, 4, 8, 8, 8, 8, 8, 8, 8, 9 },
	{ 9, 8, 1, 0, 0, 9, 1, 8, 8, 8, 8, 8, 8, 8, 1, 0, 8, 0, 1, 8, 8, 4, 8, 8, 8, 8, 9, 8, 8, 9 },
	{ 9, 8, 1, 0, 0, 9, 1, 8, 1, 1, 8, 8, 8, 8, 1, 0, 0, 0, 1, 8, 8, 4, 4, 4, 4, 0, 9, 0, 8, 9 },
	{ 8, 8, 1, 1, 1, 1, 1, 1, 8, 8, 8, 8, 9, 9, 9, 9, 9, 1, 9, 8, 8, 8, 8, 0, 4, 1, 9, 0, 8, 9 },
	{ 8, 8, 8, 8, 8, 1, 1, 8, 1, 1, 8, 8, 8, 6, 8, 8, 8, 8, 8, 9, 9, 1, 1, 1, 4, 8, 9, 9, 8, 8 },
	{ 8, 8, 8, 1, 1, 8, 8, 1, 8, 9, 8, 8, 8, 6, 8, 8, 8, 8, 8, 8, 8, 4, 4, 4, 4, 8, 8, 8, 8, 8 },
	{ 9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 1, 1, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9 },
	{ 9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9 },
	{ 9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 8, 0, 0, 8, 8, 8, 8, 8, 8, 8, 9 },
	{ 9, 9, 9, 9, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 8, 8, 9, 9, 9, 9, 9, 8, 8, 9, 9, 9, 9, 9, 9, 9 },
};

// 自機の弾の情報を格納する構造体
#define MAX_SHOT  50 // 同時に撃てる弾の最大数
typedef struct _MyShot{
	int isAlive; // 自機の弾が生きてるか？
	int isUsed;
	float x;  // 弾のx座標
	float y;  // 弾のy座標
	float z;  // 弾のz座標
	float vx; // 弾のx軸方向の速度
	float vy; // 弾のy軸方向の速度
	float vz; // 弾のz軸方向の速度
} MyShot;

int num_of_shot = MAX_SHOT; // 残り弾の数

// 自機の情報を格納する構造体
typedef struct Ship{
	int isAlive;   // 自機が生きてるか？
	int life; // 自機のライフ
	int getpoint; // 獲得ポイント
	float x;  // 自機のx座標
	float y;  // 自機のy座標
	float z;  // 自機のz座標
	float spinx; // 自機のx軸周りの回転量
	float spiny; // 自機のy軸周りの回転量
	float spinz; // 自機のz軸周りの回転量
	float v; 
	MyShot myShot[MAX_SHOT];// 弾を管理する構造体配列
} Ship;
Ship myShip; // 自機の構造体を作成

typedef struct _MyCamera{
	float x; // カメラのx座標
	float y; // カメラのy座標
	float z; // カメラのz座標
} MyCamera;
MyCamera myCamera; // カメラの構造体を作成

// 敵の情報を格納する構造体

typedef struct _Enemy{
	int isAlive;   // 敵が生きてるか？
	float x;  // 敵のx座標
	float y;  // 敵のy座標
	float z;  // 敵のz座標
	float vx; // 敵のx軸方向速度
	float vy; // 敵のy軸方向速度
	float vz; // 敵のz軸方向速度
} Enemy;
Enemy enemy[MAX_ENEMY]; // 敵の構造体配列を作成

typedef struct _Item{
	int isAlive;
	float x;
	float y;
	float z;
	float vx;
	float vy;
	float vz;
} Item;
Item item[MAX_ITEM];
// キーボード入力(スペースキーで弾を発射)
void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
	case ' ':
		for (int i=0;i<MAX_SHOT; i++){
			if(myShip.myShot[i].isAlive == 0 && myShip.myShot[i].isUsed == 0){
				myShip.myShot[i].isAlive = 1;
				myShip.myShot[i].isUsed = 1;
				myShip.myShot[i].x = myShip.x;
				myShip.myShot[i].y = myShip.y;
				myShip.myShot[i].z = myShip.z;

				myShip.myShot[i].vx = cos(myShip.spinx / 180 * 3.14)*sin(myShip.spiny / 180 * 3.14)*(myShip.v+10);
				myShip.myShot[i].vy = -sin(myShip.spinx / 180 * 3.14)*(myShip.v+10);
				myShip.myShot[i].vz = cos(myShip.spinx / 180 * 3.14)*cos(myShip.spiny / 180 * 3.14)*(myShip.v+10);
				num_of_shot--;
				printState();
				break;
			}
		}
		break;
	case 'a':
		myShip.v += 0.1;
		printState();
		break;
	  case 'd':
		if(myShip.v > 5.0) myShip.v -= 0.1;
		printState();
		break;
	case 'r':
		drawInit();
		break;	
  }

}

// キーボード入力(矢印キーを押し続けている間は自機を移動)
void specialKey(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		KeyUpON = true;
		break;
	case GLUT_KEY_DOWN:
		KeyDownON  = true;
		break;
	case GLUT_KEY_LEFT:
		KeyLeftON  = true;
		break;
	case GLUT_KEY_RIGHT:
		KeyRightON = true;
		break;
	}
}

// キーボード入力解除
void specialKeyUp(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		KeyUpON    = false;
		break;
	case GLUT_KEY_DOWN:
		KeyDownON  = false;
		break;
	case GLUT_KEY_LEFT:
		KeyLeftON  = false;
		break;
	case GLUT_KEY_RIGHT:
		KeyRightON = false;
		break;
	}
}



// 壁との衝突を確認
int Conflict(int x, int y, int z)
{
	// 当たり判定
	int i, j, k;

	for (j = 0; j <= MAX_BLOCK; j++){
		for (i = 0; i <= MAX_BLOCK; i++){
			if (map[j][i] != 0){
				if (map[j][i] == 9){
					if (x >= i * SOLID_SIZE - SOLID_SIZE * 0.5 && x <= i * SOLID_SIZE + SOLID_SIZE * 0.5
						&& z >= j * SOLID_SIZE - SOLID_SIZE * 0.5 && z <= j * SOLID_SIZE + SOLID_SIZE * 0.5
						&& y <= MAX_HEIGHT && y >= 0){
						return 1;
					}
				}
				
				else if (map[j][i] == 8){
					for (k = 0; k <= 3; k++){
						if (x >= i * SOLID_SIZE - SOLID_SIZE * 0.5 && x <= i * SOLID_SIZE + SOLID_SIZE * 0.5
							&& z >= j * SOLID_SIZE - SOLID_SIZE * 0.5 && z <= j * SOLID_SIZE + SOLID_SIZE * 0.5
							&& y >= k*3*SOLID_SIZE - SOLID_SIZE * 0.5 && y <= k*3*SOLID_SIZE + SOLID_SIZE * 0.5 ){
							return 1;
						}
					}
				}
				else {
					if (x >= i * SOLID_SIZE - SOLID_SIZE * 0.5 && x <= i * SOLID_SIZE + SOLID_SIZE * 0.5
						&& z >= j * SOLID_SIZE - SOLID_SIZE * 0.5 && z <= j * SOLID_SIZE + SOLID_SIZE * 0.5
						&& y >= map[j][i] * SOLID_SIZE - SOLID_SIZE * 0.5 && y <= map[j][i] * SOLID_SIZE + SOLID_SIZE * 0.5 ){
						return 1;
					}
				}
			}
			
		}
	}

	//画面外(領域外)判定
	if (x < 0 || x > AREA_SIZE || y < -SOLID_SIZE*0.5 || y > MAX_HEIGHT || z < 0 || z > AREA_SIZE) {
		return 2;
	}
}

// 自機の移動
void MyShipMove(void)
{
	// 矢印キーを押し続けている間は自機を移動
	if (KeyUpON == 1 && myShip.spinx > -70) { myShip.spinx -= 1.0; }
	if (KeyDownON == 1 && myShip.spinx < 70) { myShip.spinx += 1.0; }
	
	if (KeyLeftON == 1)	{
		myShip.spiny += 1.5;
		if (myShip.spinz > -50.0){
			myShip.spinz -= 1.5;
		}
	}

	if (KeyRightON == 1) {
		myShip.spiny -= 1.5;
		if (myShip.spinz < 50.0){
			myShip.spinz += 1.5;
		}
	}

	if (myShip.spinz > -0.5) { myShip.spinz -= 1.0; }
	if (myShip.spinz < -0.5) { myShip.spinz += 1.0; }

	myShip.x += cos(myShip.spinx / 180 * 3.14)*sin(myShip.spiny / 180 * 3.14)*myShip.v;
	myShip.y -= sin(myShip.spinx / 180 * 3.14)*myShip.v;
	myShip.z += cos(myShip.spinx / 180 * 3.14)*cos(myShip.spiny / 180 * 3.14)*myShip.v;

	// 当たり判定
	if ( Conflict(myShip.x, myShip.y, myShip.z) == 1 ) {
		myShip.life--;
		drawInit();
		printState();
	}

	if (myShip.life <= 0) { myShip.isAlive = 0; }

	if (myShip.y > MAX_HEIGHT) { myShip.y -= 20; myShip.spinx -= myShip.spinx; }
	if (myShip.y < -60) { myShip.y += 20; myShip.spinx -= myShip.spinx; }
	if (myShip.x > AREA_SIZE) { myShip.x -= 20; myShip.spiny = -90; }
	if (myShip.x < -50) { myShip.x += 20; myShip.spiny = 90; }
	if (myShip.z > AREA_SIZE) { myShip.z -= 20; myShip.spiny = 180; }
	if (myShip.z < -50) { myShip.z += 20; myShip.spiny = 0; }

}

void GetShot(void)
{
	for (int i = 0; i < MAX_SHOT; i++){
		if (myShip.myShot[i].isAlive == 0 && myShip.myShot[i].isUsed == 1){
			myShip.myShot[i].isUsed = 0;
			num_of_shot++;
			break;
		}
	}
}

// 弾の移動
void MyShotMove(void)
{
	// 速度を足して弾を移動させる
	for (int i = 0; i<MAX_SHOT; i++){
		if (myShip.myShot[i].isAlive == 1){
			myShip.myShot[i].x += myShip.myShot[i].vx; //x軸方向の速度を加算
			myShip.myShot[i].y += myShip.myShot[i].vy; //y軸方向の速度を加算
			myShip.myShot[i].z += myShip.myShot[i].vz; //z軸方向の速度を加算
		}


		if (Conflict(myShip.myShot[i].x, myShip.myShot[i].y, myShip.myShot[i].z) == 1){
			myShip.myShot[i].isAlive = 0;
		}
		//画面外(領域外)判定
		else if (Conflict(myShip.myShot[i].x, myShip.myShot[i].y, myShip.myShot[i].z) == 2) {
			myShip.myShot[i].isAlive = 0; //画面外(領域外)に出たら弾を消す
		}

		if (myShip.myShot[i].isAlive == 0){
			myShip.myShot[i].vx = myShip.myShot[i].vy = myShip.myShot[i].vz = 0;
			myShip.myShot[i].x = myShip.myShot[i].y = myShip.myShot[i].z = 0;
					}
	}
}

// 敵の移動
void MoveEnemy(void)
{
	// 速度を足して敵を移動させる
	int NofEnemys = 0; // 出現している敵の数を数える変数
	for (int i = 0; i<MAX_ENEMY; i++){
		if (enemy[i].isAlive == 1){ // i番目の敵は生きているか？
			// 生きていたら敵の位置を乱数（ノイズ）で移動
			if (((enemy[i].x - myShip.x)*(enemy[i].x - myShip.x) +
				(enemy[i].y - myShip.y)*(enemy[i].y - myShip.y) +
				(enemy[i].z - myShip.z)*(enemy[i].z - myShip.z)) < ENEMY_SIZE*ENEMY_SIZE * 200){
				enemy[i].x += (myShip.x - enemy[i].x)*0.1;
				enemy[i].y += (myShip.y - enemy[i].y)*0.1;
				enemy[i].z += (myShip.z - enemy[i].z)*0.1;
			} else {
				int ran = rand() % 4;
				if (ran == 0){ enemy[i].x += enemy[i].vx; }
				if (ran == 1){ enemy[i].x -= enemy[i].vx; }
				if (ran == 2){ enemy[i].z += enemy[i].vz; }
				if (ran == 3){ enemy[i].z -= enemy[i].vz; }
			}			

			// 敵と自機の距離が一定値以内の場合に当たりと判定
			if (((enemy[i].x - myShip.x)*(enemy[i].x - myShip.x) +
				(enemy[i].y - myShip.y)*(enemy[i].y - myShip.y) +
				(enemy[i].z - myShip.z)*(enemy[i].z - myShip.z)) < ENEMY_SIZE*ENEMY_SIZE){
				myShip.life--; // 自機を消す
				drawInit();
				printState();
			}
			for (int j = 0; j<MAX_SHOT; j++){
				// 弾と敵の距離が一定値以内の場合に当たりと判定
				if (((enemy[i].x - myShip.myShot[j].x)*(enemy[i].x - myShip.myShot[j].x) +
					(enemy[i].y - myShip.myShot[j].y)*(enemy[i].y - myShip.myShot[j].y) +
					(enemy[i].z - myShip.myShot[j].z)*(enemy[i].z - myShip.myShot[j].z)) < ENEMY_SIZE*ENEMY_SIZE){
					enemy[i].isAlive = 0;           //敵を消す
					myShip.myShot[j].isAlive = 0;   //弾を消す(ここで弾を消さないと貫通弾になる)
					myShip.myShot[i].x = myShip.myShot[i].y = myShip.myShot[i].z = 0;
					myShip.getpoint++;
					printState();
				}
			}
			if (Conflict(enemy[i].x, enemy[i].y, enemy[i].z) == 1) { enemy[i].isAlive = 0; }
			if (Conflict(enemy[i].x, enemy[i].y, enemy[i].z) == 2) { enemy[i].isAlive = 0; }

			NofEnemys++; // 出現している敵の数を数える
		}		
	}
	// 弾と敵の当たり判定

	// 敵の出現
	// 現存する敵がMAX_ENEMYよりも少なかったら1つ新たに出現させる
	if (NofEnemys<MAX_ENEMY){
		for (int i = 0; i<MAX_ENEMY; i++){
			// isAliveが0の構造体を見つけてisAliveを1にする
			// 出現位置はランダムに変化させる
			if (enemy[i].isAlive == 0){
				enemy[i].x = rand() % (AREA_SIZE - SOLID_SIZE) + SOLID_SIZE;
				enemy[i].y = rand() % (MAX_HEIGHT - 100) + 100;
				enemy[i].z = rand() % (AREA_SIZE - SOLID_SIZE) + SOLID_SIZE;
				if ((enemy[i].x - myShip.x)*(enemy[i].x - myShip.x) +
					(enemy[i].y - myShip.y)*(enemy[i].y - myShip.y) +
					(enemy[i].z - myShip.z)*(enemy[i].z - myShip.z) < ENEMY_SIZE*ENEMY_SIZE * 100){
					break;
				} else {
					enemy[i].isAlive = 1;
					break;
				}
			}
		}
	}
}

void MoveItem(void)
{
	// 速度を足して敵を移動させる
	int Nofitems = 0; // 出現している敵の数を数える変数
	for (int i = 0; i<MAX_ITEM; i++){
		if (item[i].isAlive == 1){ // i番目の敵は生きているか？
			// 生きていたら敵の位置を乱数（ノイズ）で移動

				int ran = rand() % 4;
				if (ran == 0){ item[i].x += item[i].vx; }
				if (ran == 1){ item[i].x -= item[i].vx; }
				if (ran == 2){ item[i].z += item[i].vz; }
				if (ran == 3){ item[i].z -= item[i].vz; }

				if (((item[i].x - myShip.x)*(item[i].x - myShip.x) +
					(item[i].y - myShip.y)*(item[i].y - myShip.y) +
					(item[i].z - myShip.z)*(item[i].z - myShip.z)) < ITEM_SIZE*ITEM_SIZE){
					item[i].isAlive = 0;           //敵を消す
					for (int i = 1; i <= 5; i++){
						GetShot();
					}
					myShip.getpoint++;
					printState();
				}

				for (int j = 0; j<MAX_SHOT; j++){
					// 弾と敵の距離が一定値以内の場合に当たりと判定
					if (((item[i].x - myShip.myShot[j].x)*(item[i].x - myShip.myShot[j].x) +
						(item[i].y - myShip.myShot[j].y)*(item[i].y - myShip.myShot[j].y) +
						(item[i].z - myShip.myShot[j].z)*(item[i].z - myShip.myShot[j].z)) < ITEM_SIZE*ITEM_SIZE){

						item[i].isAlive = 0;           //敵を消す
						myShip.myShot[j].isAlive = 0;   //弾を消す(ここで弾を消さないと貫通弾になる)
						myShip.myShot[i].x = myShip.myShot[i].y = myShip.myShot[i].z = 0;
						for (int i = 1; i <= 10; i++){
							GetShot();
						}
						myShip.getpoint++;
						printState();
					}
				}
				if (Conflict(item[i].x, item[i].y, item[i].z) == 1) { item[i].isAlive = 0; }
				if (Conflict(item[i].x, item[i].y, item[i].z) == 2) { item[i].isAlive = 0; }
			Nofitems++; // 出現している敵の数を数える
		}

	}

	// 敵の出現
	// 現存する敵がMAX_itemよりも少なかったら1つ新たに出現させる
	if (Nofitems<MAX_ITEM){
		for (int i = 0; i<MAX_ITEM; i++){
			// isAliveが0の構造体を見つけてisAliveを1にする
			// 出現位置はランダムに変化させる
			if (item[i].isAlive == 0){
				item[i].x = rand() % (AREA_SIZE - SOLID_SIZE) + SOLID_SIZE;
				item[i].y = rand() % (MAX_HEIGHT - 100) + 100;
				item[i].z = rand() % (AREA_SIZE - SOLID_SIZE) + SOLID_SIZE;
				if ((item[i].x - myShip.x)*(item[i].x - myShip.x) +
					(item[i].y - myShip.y)*(item[i].y - myShip.y) +
					(item[i].z - myShip.z)*(item[i].z - myShip.z) < ITEM_SIZE*ITEM_SIZE * 100){
					break;
				}
				else {
					item[i].isAlive = 1;
					break;
				}
			}
		}
	}
}


// カメラの移動
void CameraMove(void)
{
	myCamera.y = sin(myShip.spinx / 180 * 3.14)*10 + myShip.y;
	myCamera.x =  cos(myShip.spiny / 180 * 3.14) + sin(myShip.spiny / 180 * 3.14) * (-10) + myShip.x;
	myCamera.z = -sin(myShip.spiny / 180 * 3.14) + cos(myShip.spiny / 180 * 3.14) * (-10) + myShip.z;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(myCamera.x, myCamera.y, myCamera.z, myShip.x, myShip.y, myShip.z, 0.0, 100.0, myShip.spinx);
}


// タイマー関数
void timer(int t)             // 指定時間後に呼び出される関数（Timerコールバック関数）
{
	MyShipMove();
	MyShotMove();
	MoveEnemy();
	MoveItem();
	glutPostRedisplay();          // 画面の再描画
	glutTimerFunc(20, timer, 20); // 次回のタイマー関数の呼び出しをセット
}

// 画面のリシェイプ
void reshape(int w, int h)    // ウインドウサイズ変更時に呼び出される関数（Reshapeコールバック関数）
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (double)w/h, 1.0, 10000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 5.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// 面の描画
void DrawArea(void)
{	
	glPushMatrix();
	GLfloat mat[] = { 0.5, 0.5, 0.8, 1.0 }; //環境光と拡散光の反射率をまとめて設定
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat);
	int i, j, k;
	for(j=0; j<= MAX_BLOCK; j++){
		for(i=0; i<= MAX_BLOCK; i++){
			if (map[j][i] != 0){
				if (map[j][i] == 9){
					for (k = 0; k <= MAX_HEIGHT / SOLID_SIZE; k++){
						glPushMatrix();
						glTranslatef(i*SOLID_SIZE, SOLID_SIZE * k, j*SOLID_SIZE);
						glutSolidCube(SOLID_SIZE);
						glPopMatrix();
					}
				}
				else if (map[j][i] == 8){
					for (k = 0; k <= 3; k++){
						glPushMatrix();
						glTranslatef(i*SOLID_SIZE, SOLID_SIZE * k * 3, j*SOLID_SIZE);
						glutSolidCube(SOLID_SIZE);
						glPopMatrix();
					}
				}
				else {
					glPushMatrix();
					glTranslatef(i*SOLID_SIZE, map[j][i] * SOLID_SIZE, j*SOLID_SIZE);
					glutSolidCube(SOLID_SIZE);
					glPopMatrix();
				}
			}
		}
	}
	glPopMatrix();
}

// 自機と弾の描画
void DrawMyShip(void)
{
	glPushMatrix();
		//自機の描画
		glTranslatef(myShip.x, myShip.y, myShip.z); //X軸方向にMoveX，Y軸方向にMoveYだけ移動
		glRotatef(myShip.spiny, 0.0, 1.0, 0.0);
		glRotatef(myShip.spinx, 1.0, 0.0, 0.0);
		glRotatef(myShip.spinz, 0.0, 0.0, 1.0);
		mqoCallModel(g_mqoModel);
	glPopMatrix();

	//自機の弾の描画
	for(int i=0;i<MAX_SHOT;i++){
		if(myShip.myShot[i].isAlive){
			glPushMatrix();
				glTranslatef(myShip.myShot[i].x, myShip.myShot[i].y, myShip.myShot[i].z); //現在の弾の位置		
				glutSolidSphere(0.3,20,20);            //弾は球体
			glPopMatrix();
		}
	}
}

// 敵の描画
void DrawEnemy(void)
{
	GLfloat mat0ambi[] = { 1.0, 0.5, 0.2, 1.0 };//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat0ambi); //環境光の反射率を設定
	GLfloat mat0diff[] = { 1.0, 0.5, 0.3, 1.0 };//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat0diff); //拡散光の反射率を設定
	GLfloat mat0spec[] = { 1.0, 0.5, 0.3, 1.0 };//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat0spec); //鏡面光の反射率を設定
	GLfloat mat0shine[] = { 10.0 };//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat0shine);

	// 物体の描画
	for (int i = 0; i < MAX_ENEMY; i++){
		if (enemy[i].isAlive == 1){
			glPushMatrix();
			glTranslatef(enemy[i].x, enemy[i].y, enemy[i].z); //現在の敵の位置
			glutSolidSphere(ENEMY_SIZE/2, 10, 10);
			glPopMatrix();
		}
	}
}

void DrawItem(void)
{
	GLfloat mat0ambi[] = { 1.0, 1.5, 0.2, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat0ambi); //環境光の反射率を設定
	GLfloat mat0diff[] = { 1.0, 1.5, 0.3, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat0diff); //拡散光の反射率を設定
	GLfloat mat0spec[] = { 1.0, 1.5, 0.3, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat0spec); //鏡面光の反射率を設定
	GLfloat mat0shine[] = { 10.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat0shine);

	// 物体の描画
	for (int i = 0; i < MAX_ITEM; i++){
		if (item[i].isAlive == 1){
			glPushMatrix();
			glTranslatef(item[i].x, item[i].y, item[i].z); //現在の敵の位置
			glutSolidSphere(ITEM_SIZE / 2, 10, 10);
			glPopMatrix();
		}
	}
}

// 描画関数
void display(void)            // 描画時に呼び出される関数（Displayコールバック関数）
{
	glClearColor(0.0, 0.0, 0.0, 1.0); // 画面クリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 画面バッファクリア
	glEnable(GL_DEPTH_TEST); // 隠面消去を有効

	
	// 自機が死んでいたらGame Overを表示する
	if (myShip.life <= 0){
		// 文字列の描画
		glPushMatrix();
		GLfloat matString[] = { 0.8, 0.1, 0.2, 1.0 }; //環境光と拡散光の反射率をまとめて設定
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matString);
		glRasterPos3f(myShip.x, myShip.y, myShip.z);
		char *str = "Game Over";
		while (*str){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
			++str;
		}
		glPopMatrix();		
	}
	else{
		// 自機が生きていたら自機と敵を描画する
		DrawArea();
		DrawMyShip(); // 自機の描画
		DrawEnemy();
		DrawItem();
		CameraMove();
 	}

	glutSwapBuffers(); // 描画実行
}

// 現在の状態を出力
void printState(void)
{
	printf("Life:%d Speed:%.2f Shot:%d Point:%d\n", myShip.life, myShip.v, num_of_shot, myShip.getpoint);

}

// 初期化
void drawInit(void)
{
	// 自機の構造体データを初期化
	myShip.x = SOLID_SIZE + 100;	// 自機の初期位置(X座標)
	myShip.y = MAX_HEIGHT - 100;  // 自機の初期位置(y座標)
	myShip.z = SOLID_SIZE + 100;	// 自機の初期位置(Z座標)
	myShip.spinx = myShip.spiny = myShip.spinz = 0; // 自機の初期回転量


	myCamera.x = myShip.x;
	myCamera.y = myShip.y + 10.0;
	myCamera.z = myShip.z - 10.0;

	// 敵の構造体データを初期化(敵はZ軸方向に毎フレーム0.1移動する)
	for (int i = 0; i<MAX_ENEMY; i++){
		enemy[i].isAlive = 0; // 敵は出現していない
		enemy[i].x = 0;		// 敵の初期位置(X座標)
		enemy[i].y = 100;
		enemy[i].z = 0;	// 敵の初期位置(Z座標)
		enemy[i].vx = 4;	// 敵の基本速度(X座標)
		enemy[i].vy = 4;
		enemy[i].vz = 4;	// 敵の基本速度(Z座標)
	}

	for (int i = 0; i<MAX_ITEM; i++){
		item[i].isAlive = 0; // 敵は出現していない
		item[i].x = 0;		// 敵の初期位置(X座標)
		item[i].y = 100;
		item[i].z = 0;	// 敵の初期位置(Z座標)
		item[i].vx = 4;	// 敵の基本速度(X座標)
		item[i].vy = 4;
		item[i].vz = 4;	// 敵の基本速度(Z座標)
	}
}

// 初期化(光)
void lightInit(void)        // 光源の初期設定(まとめて関数にしているだけ)
{
	glEnable(GL_LIGHTING);  //光源の設定を有効にする
	glEnable(GL_LIGHT0);    //0番目の光源を有効にする(8個まで設定可能)
	glEnable(GL_NORMALIZE); //法線ベクトルの自動正規化を有効

	GLfloat light0pos[] = { 0.0, 10.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos); //光源0の位置を設定

	GLfloat light0ambi[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light0ambi); //光源0の環境光の色を設定
	GLfloat light0diff[] = { 0.8, 0.8, 0.8, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0diff); //光源0の拡散光の色を設定
	GLfloat light0spec[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0spec); //光源0の鏡面光の色を設定

	glShadeModel(GL_SMOOTH); //スムーズシェーディングに設定
}

// メイン関数
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);          // GLUT初期化
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);   // ウィンドウサイズの指定
	glutCreateWindow("window");     // 表示ウィンドウ作成
	glutReshapeFunc(reshape);       // Reshapeコールバック関数の指定
	glutDisplayFunc(display);       // Displayコールバック関数の指定

	glutKeyboardFunc(keyboard);     // 通常キーコールバック関数の指定(押したとき)
	glutSpecialFunc(specialKey);    // 特殊キーコールバック関数の指定(押したとき)
	glutSpecialUpFunc(specialKeyUp);// 特殊キーコールバック関数の指定(離したとき)

	glutTimerFunc(20, timer, 20);   // 定期的に呼び出される関数の指定

	myShip.isAlive = 1;	// 自機は生きている
	myShip.life = 5;
	myShip.v = 6;  // 自機の速度
	myShip.getpoint = 0;
	drawInit();
	num_of_shot = MAX_SHOT;

	// 弾の構造体データを初期化(弾はZ軸方向に毎フレーム-1.0移動する)
	for (int i = 0; i<MAX_SHOT; i++){
		myShip.myShot[i].isAlive = 0; // 弾は発射されていない
		myShip.myShot[i].isUsed = 0;
		myShip.myShot[i].x = 0;		// 弾の初期位置(X座標)
		myShip.myShot[i].y = 0;     // 弾の初期位置(y座標)
		myShip.myShot[i].z = 0;		// 弾の初期位置(z座標)
		myShip.myShot[i].vx = 0;	// 弾の基本速度(X座標)
		myShip.myShot[i].vy = 0;    // 弾の基本速度(y座標)
		myShip.myShot[i].vz = 0;    // 弾の基本速度(z座標)
	}


	lightInit();    // 光源の初期設定(まとめて関数にしているだけ)

	mqoInit(); // GLMetaseqの初期化
	g_mqoModel = mqoCreateModel("airplain.mqo", 0.01); // メタセコイアモデルのロード
	if(g_mqoModel == NULL) {
		printf("Error\n");
		return 1;
	}

	glutMainLoop(); // メインループへ

	return 0;
}
