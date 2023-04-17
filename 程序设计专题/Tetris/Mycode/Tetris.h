#pragma once
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "imgui.h"
#include <math.h>
#include <time.h>
#include "file.h"
#include "draw.h"

#define TIMER_BLINK 1//ʱ�ӱ��
#define deltax 0.3;
#define deltay 0.3;

//���ݽṹ�嶨��
struct BLOCK;

//�洢����������
struct link;

//�洢����˹��������
struct BLOCK Block[19] = {
	/*x1,y1,x2,y2,x3,y3,x4,y4, color,  num*/
	{ 0, 0, 0, 1, 0, 2, 1, 2,  "Cyan",  0}, /*        */
	{-1, 1, 0, 1, 1, 0, 1, 1,  "Cyan",  1}, /*  #     */
	{ 0, 0, 1, 0, 1, 1, 1, 2,  "Cyan",  2}, /*  #     */
	{-1, 0,-1, 1, 0, 0, 1, 0,  "Cyan",  3}, /*  ##    */

	{ 0, 0, 0, 1, 0, 2, 1, 0, "Magenta",4},/*         */
	{-1, 0,-1, 1, 0, 1, 1, 1, "Magenta",5},/*  ##     */
	{ 0, 2, 1, 0, 1, 1, 1, 2, "Magenta",6},/*  #      */
	{-1, 0, 0, 0, 1, 0, 1, 1, "Magenta",7},/*  #      */

	{-1, 1, 0, 0, 0, 1, 1, 1, "Blue",   8},/*        */
	{-1, 1, 0, 0, 0, 1, 0, 2, "Blue",   9},/*        */
	{-1, 0, 0, 0, 0, 1, 1, 0, "Blue",  10},/*   #    */
	{ 0, 0, 0, 1, 0, 2, 1, 1, "Blue",  11},/*  ###   */

	{-1, 1, 0, 0, 0, 1, 1, 0, "Yellow",12},/*  ##    */
	{ 0, 0, 0, 1, 1, 1, 1, 2, "Yellow",13},/*   ##   */

	{-1, 0, 0, 0, 0, 1, 1, 1,  "Red"  ,14},/*    ##  */
	{ 0, 1, 0, 2, 1, 0, 1, 1,  "Red"  ,15},/*   ##   */

	{ 0, 0, 0, 1, 0, 2, 0, 3, "Orange" ,16},/*  #     */
	{-1, 0, 0, 0, 1, 0, 2, 0, "Orange" ,17},/*  #     */
										    /*  #     */
										    /*  #     */

	{ 0, 0, 0, 1, 1, 0, 1, 1, "Brown" ,18},/*  ##    */
										   /*  ##    */
};

//��ʼ����
void drawTopic();//��ʼ���桪�����Ʊ��⣬��¼
void ChooseLevel();//ѡ���Ѷȵȼ�
void displaychar();//���û����������
void displaystart();//����¼�����ȷ�ϰ�ť

//��Ϸ����
void DrawMoving(double x, double y);//����ǰ�ķ���
void DrawSilent();//�����Ѿ����µķ���
void DrawLattice();//������
void DrawBackground();//������
void DrawNext(int i);//���ұ߻����ɵ���һ������
void DrawScore();//��ʾ����
void DrawButton();//����ť
void DrawMenu();//���Ϸ��˵��� ��ݼ�
void display();//��ʾ

//���а����
void DrawRank();//��ʾ���а�

//�ص�����
void TimerEventProcess(int timerID);//��ʱ��
void myKeyboardEventProcess(int key, int event);//���̻ص�����
void myMouseEventProcess(int x, int y, int button, int event);//���ص�����
void CharEventProcess(char ch);//��ȡ������ַ�

//��Ϸ����
int Transferx(double tipx);//�ѽ����x���꣨double��ת���������x����
int Transfery(double tipy);//�ѽ����y���꣨double��ת���������y����
int FullLine();//�ж�����
void KillLine(int);//�������
int TouchLeftWall(double x, int num);//�ж�������ƻ᲻�ᴥ����߽�
int TouchRightWall(double x, int num);//�ж�������ƻ᲻�ᴥ���ұ߽�
int GameOver();//��Ϸ����

//��ʼ��
void initRandomNum();//�����ʼ��

//�ļ���д
struct link *SaveData(struct link *p);//������߼�¼��д���ļ�
struct link *initLink();//���ļ���ȡ���ݽ�������
struct link *insert(struct link *p);//�������û�������ͷ�壩
struct link *datasort(struct link *head);//ð�����򣨽����ڵ㷨��

//��������
double winwidth, winheight;//���ڳ��ȡ����
double current_x, current_y;//��ǰ����ԭ������
int board[11][21];//��������Ƿ����
int color[11][21];//����������ɫ
int score;//��¼����
int enter = 0;//�ж��Ƿ������Ϸ
int rank = 0;//�ж��Ƿ�����а����
int mseconds;//��ʱ������ʱ��
int t = 0, r = 0;//��ť
int i;//��ǰ�ķ������
int next;//��һ���������
int enable_mov_Tetris = 1;//�������ƶ�
int if_fall = 0;//����ܷ�����
int anti_keyboard = 1;//���ж�����ʱ��ִ�м��̲���
int c = 0;//�ж��Ƿ�ѡ�����Ѷȵȼ�
int flag = 0;//�ж��Ƿ���exit��������˾ͱ���ɼ����˳�)
char name[80];//�洢��ǰ�û���
struct link *p = NULL;//����ͷ�ڵ�