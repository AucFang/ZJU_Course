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

#define TIMER_BLINK 1//ʱ�ӱ��

//���ݽṹ�嶨��
extern struct BLOCK
{
	double x1;
	double y1;
	double x2;
	double y2;
	double x3;
	double y3;
	double x4;
	double y4;
	char color[8];
	int num;
};

//�洢����˹��������
extern struct BLOCK Block[19];

extern double current_x, current_y;//��ǰ����ԭ������
extern double winwidth, winheight;//���ڳ��ȡ����
extern int c;//�ж��Ƿ�ѡ�����Ѷȵȼ�
extern int mseconds;//��ʱ������ʱ��
extern int t, r;//��ť
extern int enter;//�ж��Ƿ������Ϸ
extern int rank;//�ж��Ƿ�����а����
extern int i;//��ǰ�ķ������
extern int next;//��һ���������
extern int enable_mov_Tetris;//�������ƶ�
extern int anti_keyboard;//���ж�����ʱ��ִ�м��̲���
extern int flag;//�ж��Ƿ���exit��������˾ͱ���ɼ����˳�)
extern int board[11][21];//��������Ƿ����
extern int color[11][21];//����������ɫ
extern char name[80];
extern struct link *p;//����ͷ�ڵ�

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