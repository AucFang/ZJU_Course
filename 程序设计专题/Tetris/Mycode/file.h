#pragma once
#include<stdio.h>
#include <string.h>

//�洢����������
extern struct link {
	char	name[80];
	int     score;
	struct link	*next;
};
extern char name[80];
extern int score;

struct link *SaveData(struct link *p);//������߼�¼��д���ļ�
struct link *initLink();//���ļ���ȡ���ݽ�������
struct link *insert(struct link *p);//�������û�������ͷ�壩
struct link *datasort(struct link *head);//ð�����򣨽����ڵ㷨��
