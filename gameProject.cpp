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
#define MAX_ENEMY  150 // �����ɏo������G�̍ő吔
#define MAX_ITEM  20
// ���^�Z�R�C�A�̃��f��
MQO_MODEL g_mqoModel;

// �O���[�o���ϐ�
static bool KeyUpON    = false;	// ���L�[�̏�ԃt���O
static bool KeyDownON  = false;	// ���L�[�̏�ԃt���O
static bool KeyLeftON  = false;	// ���L�[�̏�ԃt���O
static bool KeyRightON = false;	// ���L�[�̏�ԃt���O
static float Scale = 1.0; // �g���

void drawInit(void);
void printState(void);


// �}�b�v
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

// ���@�̒e�̏����i�[����\����
#define MAX_SHOT  50 // �����Ɍ��Ă�e�̍ő吔
typedef struct _MyShot{
	int isAlive; // ���@�̒e�������Ă邩�H
	int isUsed;
	float x;  // �e��x���W
	float y;  // �e��y���W
	float z;  // �e��z���W
	float vx; // �e��x�������̑��x
	float vy; // �e��y�������̑��x
	float vz; // �e��z�������̑��x
} MyShot;

int num_of_shot = MAX_SHOT; // �c��e�̐�

// ���@�̏����i�[����\����
typedef struct Ship{
	int isAlive;   // ���@�������Ă邩�H
	int life; // ���@�̃��C�t
	int getpoint; // �l���|�C���g
	float x;  // ���@��x���W
	float y;  // ���@��y���W
	float z;  // ���@��z���W
	float spinx; // ���@��x������̉�]��
	float spiny; // ���@��y������̉�]��
	float spinz; // ���@��z������̉�]��
	float v; 
	MyShot myShot[MAX_SHOT];// �e���Ǘ�����\���̔z��
} Ship;
Ship myShip; // ���@�̍\���̂��쐬

typedef struct _MyCamera{
	float x; // �J������x���W
	float y; // �J������y���W
	float z; // �J������z���W
} MyCamera;
MyCamera myCamera; // �J�����̍\���̂��쐬

// �G�̏����i�[����\����

typedef struct _Enemy{
	int isAlive;   // �G�������Ă邩�H
	float x;  // �G��x���W
	float y;  // �G��y���W
	float z;  // �G��z���W
	float vx; // �G��x���������x
	float vy; // �G��y���������x
	float vz; // �G��z���������x
} Enemy;
Enemy enemy[MAX_ENEMY]; // �G�̍\���̔z����쐬

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
// �L�[�{�[�h����(�X�y�[�X�L�[�Œe�𔭎�)
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

// �L�[�{�[�h����(���L�[�����������Ă���Ԃ͎��@���ړ�)
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

// �L�[�{�[�h���͉���
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



// �ǂƂ̏Փ˂��m�F
int Conflict(int x, int y, int z)
{
	// �����蔻��
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

	//��ʊO(�̈�O)����
	if (x < 0 || x > AREA_SIZE || y < -SOLID_SIZE*0.5 || y > MAX_HEIGHT || z < 0 || z > AREA_SIZE) {
		return 2;
	}
}

// ���@�̈ړ�
void MyShipMove(void)
{
	// ���L�[�����������Ă���Ԃ͎��@���ړ�
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

	// �����蔻��
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

// �e�̈ړ�
void MyShotMove(void)
{
	// ���x�𑫂��Ēe���ړ�������
	for (int i = 0; i<MAX_SHOT; i++){
		if (myShip.myShot[i].isAlive == 1){
			myShip.myShot[i].x += myShip.myShot[i].vx; //x�������̑��x�����Z
			myShip.myShot[i].y += myShip.myShot[i].vy; //y�������̑��x�����Z
			myShip.myShot[i].z += myShip.myShot[i].vz; //z�������̑��x�����Z
		}


		if (Conflict(myShip.myShot[i].x, myShip.myShot[i].y, myShip.myShot[i].z) == 1){
			myShip.myShot[i].isAlive = 0;
		}
		//��ʊO(�̈�O)����
		else if (Conflict(myShip.myShot[i].x, myShip.myShot[i].y, myShip.myShot[i].z) == 2) {
			myShip.myShot[i].isAlive = 0; //��ʊO(�̈�O)�ɏo����e������
		}

		if (myShip.myShot[i].isAlive == 0){
			myShip.myShot[i].vx = myShip.myShot[i].vy = myShip.myShot[i].vz = 0;
			myShip.myShot[i].x = myShip.myShot[i].y = myShip.myShot[i].z = 0;
					}
	}
}

// �G�̈ړ�
void MoveEnemy(void)
{
	// ���x�𑫂��ēG���ړ�������
	int NofEnemys = 0; // �o�����Ă���G�̐��𐔂���ϐ�
	for (int i = 0; i<MAX_ENEMY; i++){
		if (enemy[i].isAlive == 1){ // i�Ԗڂ̓G�͐����Ă��邩�H
			// �����Ă�����G�̈ʒu�𗐐��i�m�C�Y�j�ňړ�
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

			// �G�Ǝ��@�̋��������l�ȓ��̏ꍇ�ɓ�����Ɣ���
			if (((enemy[i].x - myShip.x)*(enemy[i].x - myShip.x) +
				(enemy[i].y - myShip.y)*(enemy[i].y - myShip.y) +
				(enemy[i].z - myShip.z)*(enemy[i].z - myShip.z)) < ENEMY_SIZE*ENEMY_SIZE){
				myShip.life--; // ���@������
				drawInit();
				printState();
			}
			for (int j = 0; j<MAX_SHOT; j++){
				// �e�ƓG�̋��������l�ȓ��̏ꍇ�ɓ�����Ɣ���
				if (((enemy[i].x - myShip.myShot[j].x)*(enemy[i].x - myShip.myShot[j].x) +
					(enemy[i].y - myShip.myShot[j].y)*(enemy[i].y - myShip.myShot[j].y) +
					(enemy[i].z - myShip.myShot[j].z)*(enemy[i].z - myShip.myShot[j].z)) < ENEMY_SIZE*ENEMY_SIZE){
					enemy[i].isAlive = 0;           //�G������
					myShip.myShot[j].isAlive = 0;   //�e������(�����Œe�������Ȃ��Ɗђʒe�ɂȂ�)
					myShip.myShot[i].x = myShip.myShot[i].y = myShip.myShot[i].z = 0;
					myShip.getpoint++;
					printState();
				}
			}
			if (Conflict(enemy[i].x, enemy[i].y, enemy[i].z) == 1) { enemy[i].isAlive = 0; }
			if (Conflict(enemy[i].x, enemy[i].y, enemy[i].z) == 2) { enemy[i].isAlive = 0; }

			NofEnemys++; // �o�����Ă���G�̐��𐔂���
		}		
	}
	// �e�ƓG�̓����蔻��

	// �G�̏o��
	// ��������G��MAX_ENEMY�������Ȃ�������1�V���ɏo��������
	if (NofEnemys<MAX_ENEMY){
		for (int i = 0; i<MAX_ENEMY; i++){
			// isAlive��0�̍\���̂�������isAlive��1�ɂ���
			// �o���ʒu�̓����_���ɕω�������
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
	// ���x�𑫂��ēG���ړ�������
	int Nofitems = 0; // �o�����Ă���G�̐��𐔂���ϐ�
	for (int i = 0; i<MAX_ITEM; i++){
		if (item[i].isAlive == 1){ // i�Ԗڂ̓G�͐����Ă��邩�H
			// �����Ă�����G�̈ʒu�𗐐��i�m�C�Y�j�ňړ�

				int ran = rand() % 4;
				if (ran == 0){ item[i].x += item[i].vx; }
				if (ran == 1){ item[i].x -= item[i].vx; }
				if (ran == 2){ item[i].z += item[i].vz; }
				if (ran == 3){ item[i].z -= item[i].vz; }

				if (((item[i].x - myShip.x)*(item[i].x - myShip.x) +
					(item[i].y - myShip.y)*(item[i].y - myShip.y) +
					(item[i].z - myShip.z)*(item[i].z - myShip.z)) < ITEM_SIZE*ITEM_SIZE){
					item[i].isAlive = 0;           //�G������
					for (int i = 1; i <= 5; i++){
						GetShot();
					}
					myShip.getpoint++;
					printState();
				}

				for (int j = 0; j<MAX_SHOT; j++){
					// �e�ƓG�̋��������l�ȓ��̏ꍇ�ɓ�����Ɣ���
					if (((item[i].x - myShip.myShot[j].x)*(item[i].x - myShip.myShot[j].x) +
						(item[i].y - myShip.myShot[j].y)*(item[i].y - myShip.myShot[j].y) +
						(item[i].z - myShip.myShot[j].z)*(item[i].z - myShip.myShot[j].z)) < ITEM_SIZE*ITEM_SIZE){

						item[i].isAlive = 0;           //�G������
						myShip.myShot[j].isAlive = 0;   //�e������(�����Œe�������Ȃ��Ɗђʒe�ɂȂ�)
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
			Nofitems++; // �o�����Ă���G�̐��𐔂���
		}

	}

	// �G�̏o��
	// ��������G��MAX_item�������Ȃ�������1�V���ɏo��������
	if (Nofitems<MAX_ITEM){
		for (int i = 0; i<MAX_ITEM; i++){
			// isAlive��0�̍\���̂�������isAlive��1�ɂ���
			// �o���ʒu�̓����_���ɕω�������
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


// �J�����̈ړ�
void CameraMove(void)
{
	myCamera.y = sin(myShip.spinx / 180 * 3.14)*10 + myShip.y;
	myCamera.x =  cos(myShip.spiny / 180 * 3.14) + sin(myShip.spiny / 180 * 3.14) * (-10) + myShip.x;
	myCamera.z = -sin(myShip.spiny / 180 * 3.14) + cos(myShip.spiny / 180 * 3.14) * (-10) + myShip.z;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(myCamera.x, myCamera.y, myCamera.z, myShip.x, myShip.y, myShip.z, 0.0, 100.0, myShip.spinx);
}


// �^�C�}�[�֐�
void timer(int t)             // �w�莞�Ԍ�ɌĂяo�����֐��iTimer�R�[���o�b�N�֐��j
{
	MyShipMove();
	MyShotMove();
	MoveEnemy();
	MoveItem();
	glutPostRedisplay();          // ��ʂ̍ĕ`��
	glutTimerFunc(20, timer, 20); // ����̃^�C�}�[�֐��̌Ăяo�����Z�b�g
}

// ��ʂ̃��V�F�C�v
void reshape(int w, int h)    // �E�C���h�E�T�C�Y�ύX���ɌĂяo�����֐��iReshape�R�[���o�b�N�֐��j
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (double)w/h, 1.0, 10000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 5.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// �ʂ̕`��
void DrawArea(void)
{	
	glPushMatrix();
	GLfloat mat[] = { 0.5, 0.5, 0.8, 1.0 }; //�����Ɗg�U���̔��˗����܂Ƃ߂Đݒ�
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

// ���@�ƒe�̕`��
void DrawMyShip(void)
{
	glPushMatrix();
		//���@�̕`��
		glTranslatef(myShip.x, myShip.y, myShip.z); //X��������MoveX�CY��������MoveY�����ړ�
		glRotatef(myShip.spiny, 0.0, 1.0, 0.0);
		glRotatef(myShip.spinx, 1.0, 0.0, 0.0);
		glRotatef(myShip.spinz, 0.0, 0.0, 1.0);
		mqoCallModel(g_mqoModel);
	glPopMatrix();

	//���@�̒e�̕`��
	for(int i=0;i<MAX_SHOT;i++){
		if(myShip.myShot[i].isAlive){
			glPushMatrix();
				glTranslatef(myShip.myShot[i].x, myShip.myShot[i].y, myShip.myShot[i].z); //���݂̒e�̈ʒu		
				glutSolidSphere(0.3,20,20);            //�e�͋���
			glPopMatrix();
		}
	}
}

// �G�̕`��
void DrawEnemy(void)
{
	GLfloat mat0ambi[] = { 1.0, 0.5, 0.2, 1.0 };//�^�J
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat0ambi); //�����̔��˗���ݒ�
	GLfloat mat0diff[] = { 1.0, 0.5, 0.3, 1.0 };//�^�J
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat0diff); //�g�U���̔��˗���ݒ�
	GLfloat mat0spec[] = { 1.0, 0.5, 0.3, 1.0 };//�^�J
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat0spec); //���ʌ��̔��˗���ݒ�
	GLfloat mat0shine[] = { 10.0 };//�^�J
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat0shine);

	// ���̂̕`��
	for (int i = 0; i < MAX_ENEMY; i++){
		if (enemy[i].isAlive == 1){
			glPushMatrix();
			glTranslatef(enemy[i].x, enemy[i].y, enemy[i].z); //���݂̓G�̈ʒu
			glutSolidSphere(ENEMY_SIZE/2, 10, 10);
			glPopMatrix();
		}
	}
}

void DrawItem(void)
{
	GLfloat mat0ambi[] = { 1.0, 1.5, 0.2, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat0ambi); //�����̔��˗���ݒ�
	GLfloat mat0diff[] = { 1.0, 1.5, 0.3, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat0diff); //�g�U���̔��˗���ݒ�
	GLfloat mat0spec[] = { 1.0, 1.5, 0.3, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat0spec); //���ʌ��̔��˗���ݒ�
	GLfloat mat0shine[] = { 10.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat0shine);

	// ���̂̕`��
	for (int i = 0; i < MAX_ITEM; i++){
		if (item[i].isAlive == 1){
			glPushMatrix();
			glTranslatef(item[i].x, item[i].y, item[i].z); //���݂̓G�̈ʒu
			glutSolidSphere(ITEM_SIZE / 2, 10, 10);
			glPopMatrix();
		}
	}
}

// �`��֐�
void display(void)            // �`�掞�ɌĂяo�����֐��iDisplay�R�[���o�b�N�֐��j
{
	glClearColor(0.0, 0.0, 0.0, 1.0); // ��ʃN���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ��ʃo�b�t�@�N���A
	glEnable(GL_DEPTH_TEST); // �B�ʏ�����L��

	
	// ���@������ł�����Game Over��\������
	if (myShip.life <= 0){
		// ������̕`��
		glPushMatrix();
		GLfloat matString[] = { 0.8, 0.1, 0.2, 1.0 }; //�����Ɗg�U���̔��˗����܂Ƃ߂Đݒ�
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
		// ���@�������Ă����玩�@�ƓG��`�悷��
		DrawArea();
		DrawMyShip(); // ���@�̕`��
		DrawEnemy();
		DrawItem();
		CameraMove();
 	}

	glutSwapBuffers(); // �`����s
}

// ���݂̏�Ԃ��o��
void printState(void)
{
	printf("Life:%d Speed:%.2f Shot:%d Point:%d\n", myShip.life, myShip.v, num_of_shot, myShip.getpoint);

}

// ������
void drawInit(void)
{
	// ���@�̍\���̃f�[�^��������
	myShip.x = SOLID_SIZE + 100;	// ���@�̏����ʒu(X���W)
	myShip.y = MAX_HEIGHT - 100;  // ���@�̏����ʒu(y���W)
	myShip.z = SOLID_SIZE + 100;	// ���@�̏����ʒu(Z���W)
	myShip.spinx = myShip.spiny = myShip.spinz = 0; // ���@�̏�����]��


	myCamera.x = myShip.x;
	myCamera.y = myShip.y + 10.0;
	myCamera.z = myShip.z - 10.0;

	// �G�̍\���̃f�[�^��������(�G��Z�������ɖ��t���[��0.1�ړ�����)
	for (int i = 0; i<MAX_ENEMY; i++){
		enemy[i].isAlive = 0; // �G�͏o�����Ă��Ȃ�
		enemy[i].x = 0;		// �G�̏����ʒu(X���W)
		enemy[i].y = 100;
		enemy[i].z = 0;	// �G�̏����ʒu(Z���W)
		enemy[i].vx = 4;	// �G�̊�{���x(X���W)
		enemy[i].vy = 4;
		enemy[i].vz = 4;	// �G�̊�{���x(Z���W)
	}

	for (int i = 0; i<MAX_ITEM; i++){
		item[i].isAlive = 0; // �G�͏o�����Ă��Ȃ�
		item[i].x = 0;		// �G�̏����ʒu(X���W)
		item[i].y = 100;
		item[i].z = 0;	// �G�̏����ʒu(Z���W)
		item[i].vx = 4;	// �G�̊�{���x(X���W)
		item[i].vy = 4;
		item[i].vz = 4;	// �G�̊�{���x(Z���W)
	}
}

// ������(��)
void lightInit(void)        // �����̏����ݒ�(�܂Ƃ߂Ċ֐��ɂ��Ă��邾��)
{
	glEnable(GL_LIGHTING);  //�����̐ݒ��L���ɂ���
	glEnable(GL_LIGHT0);    //0�Ԗڂ̌�����L���ɂ���(8�܂Őݒ�\)
	glEnable(GL_NORMALIZE); //�@���x�N�g���̎������K����L��

	GLfloat light0pos[] = { 0.0, 10.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos); //����0�̈ʒu��ݒ�

	GLfloat light0ambi[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light0ambi); //����0�̊����̐F��ݒ�
	GLfloat light0diff[] = { 0.8, 0.8, 0.8, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0diff); //����0�̊g�U���̐F��ݒ�
	GLfloat light0spec[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0spec); //����0�̋��ʌ��̐F��ݒ�

	glShadeModel(GL_SMOOTH); //�X���[�Y�V�F�[�f�B���O�ɐݒ�
}

// ���C���֐�
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);          // GLUT������
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);   // �E�B���h�E�T�C�Y�̎w��
	glutCreateWindow("window");     // �\���E�B���h�E�쐬
	glutReshapeFunc(reshape);       // Reshape�R�[���o�b�N�֐��̎w��
	glutDisplayFunc(display);       // Display�R�[���o�b�N�֐��̎w��

	glutKeyboardFunc(keyboard);     // �ʏ�L�[�R�[���o�b�N�֐��̎w��(�������Ƃ�)
	glutSpecialFunc(specialKey);    // ����L�[�R�[���o�b�N�֐��̎w��(�������Ƃ�)
	glutSpecialUpFunc(specialKeyUp);// ����L�[�R�[���o�b�N�֐��̎w��(�������Ƃ�)

	glutTimerFunc(20, timer, 20);   // ����I�ɌĂяo�����֐��̎w��

	myShip.isAlive = 1;	// ���@�͐����Ă���
	myShip.life = 5;
	myShip.v = 6;  // ���@�̑��x
	myShip.getpoint = 0;
	drawInit();
	num_of_shot = MAX_SHOT;

	// �e�̍\���̃f�[�^��������(�e��Z�������ɖ��t���[��-1.0�ړ�����)
	for (int i = 0; i<MAX_SHOT; i++){
		myShip.myShot[i].isAlive = 0; // �e�͔��˂���Ă��Ȃ�
		myShip.myShot[i].isUsed = 0;
		myShip.myShot[i].x = 0;		// �e�̏����ʒu(X���W)
		myShip.myShot[i].y = 0;     // �e�̏����ʒu(y���W)
		myShip.myShot[i].z = 0;		// �e�̏����ʒu(z���W)
		myShip.myShot[i].vx = 0;	// �e�̊�{���x(X���W)
		myShip.myShot[i].vy = 0;    // �e�̊�{���x(y���W)
		myShip.myShot[i].vz = 0;    // �e�̊�{���x(z���W)
	}


	lightInit();    // �����̏����ݒ�(�܂Ƃ߂Ċ֐��ɂ��Ă��邾��)

	mqoInit(); // GLMetaseq�̏�����
	g_mqoModel = mqoCreateModel("airplain.mqo", 0.01); // ���^�Z�R�C�A���f���̃��[�h
	if(g_mqoModel == NULL) {
		printf("Error\n");
		return 1;
	}

	glutMainLoop(); // ���C�����[�v��

	return 0;
}
