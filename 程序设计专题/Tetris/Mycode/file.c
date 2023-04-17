#pragma once
#include "file.h"

//���ļ���ȡ���ݽ�������
struct link *initLink()
{
	FILE *fp;
	char ch;
	struct link *p = (struct link*)malloc(sizeof(struct link));//����һ��ͷ���
	memset(p, 0, sizeof(struct link));
	struct link *temp = p;//����һ��ָ��ָ��ͷ��㣬���ڱ�������
	//��������
	if ((fp = fopen("����˹����.txt", "r")) == NULL) exit(0);
	ch = fgetc(fp);
	if (ch == EOF)//�ж��Ƿ�Ϊ���ļ�
	{
		fclose(fp);
	}
	else
	{
		rewind(fp);//�ض�λ���׵�ַ
		while (1)
		{
			struct link *a = (struct link*)malloc(sizeof(struct link));
			memset(a, 0, sizeof(struct link));
			fscanf(fp, "%s %d", a->name, &a->score);
			a->next = NULL;
			temp->next = a;
			temp = temp->next;
			if (feof(fp)) break;
		}
		if (fclose(fp)) exit(0);
	}
	return p;
}

//������߼�¼��д���ļ�
struct link *SaveData(struct link * p)
{
	FILE *fp;
	struct link *temp = p;//������ʱ���temp
	struct link *t = p;//������ʱ���t
	int find = 0;//�����Ƿ�Ϊ��ע���û�
	while (t->next && !find) {
		t = t->next;
		if (!strcmp(t->name, name))
		{
			find = 1;
			//�洢��߷�
			if (t->score < score)
			{
				t->score = score;
			}
		}
	}
	if (!find)//���û��������½��
	{
		p = insert(p);
	}
	//����
	p = datasort(p);
	//д���ļ�
	if ((fp = fopen("����˹����.txt", "w")) == NULL) exit(0);
	if (temp->next)
	{
		temp = temp->next;
		fprintf(fp, "%s %d", temp->name, temp->score);
	}
	while (temp->next)
	{
		temp = temp->next;
		fprintf(fp, "\n%s %d", temp->name, temp->score);
	}
	if (fclose(fp)) exit(0);
	return  p;
}

//�������û�������ͷ�壩
struct link *insert(struct link *p)
{
	struct link *temp = p;
	struct link *c;
	c = (struct link*)malloc(sizeof(struct link));
	memset(c, 0, sizeof(struct link));
	strcpy(c->name, name);
	c->score = score;
	//�������в�����
	c->next = temp->next;
	temp->next = c;
	return p;
}

//ð�����򣨽����ڵ㷨��
struct link *datasort(struct link *head) {
	struct link *pre, *cur, *next, *end;//preǰһ�� cur��ǰ�� next��һ�� end����ѭ������
	end = NULL;
	int flag = 1;//�Ż�ð��
	while (head->next != end && flag)
	{
		flag = 0;
		//��ʼ������ָ�� ; �ж��Ƿ񵽴����λ�� ; ����ָ�뼯�����
		for (pre = head, cur = pre->next, next = cur->next; next != end; pre = pre->next, cur = cur->next, next = next->next)
		{
			if (cur->score < next->score) //��С����
			{
				flag = 1;
				pre->next = next;
				cur->next = next->next;
				next->next = cur;

				//��ʱnext��ǰһ�cur���һ��  ����ָ��
				struct link *temp = cur; cur = next; next = temp;
			}
		}

		//һ��ѭ������ end��ǰһ��
		end = cur;
	}
	return head;
}
