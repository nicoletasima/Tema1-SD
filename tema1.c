




#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h>


typedef struct List
{
	int timestamp;
	double data;
	struct List *next;
	struct List *prev;
}*List;

List initList(int t, double d)
{
	List l=(List)malloc(sizeof(struct List));
	l->timestamp=t;
	l->data=d;
	l->next=NULL;
	l->prev=NULL;
	return l;
}	

void printList(List l)
{
	if (l!=NULL)
	{
		printf("%d %.2lf\n",l->timestamp,l->data);
		printList(l->next);
	}
}

List addLast(List l, int t, double d)
{
	List p=l;
	if (l==NULL)
		l=initList(t,d);
	else
	{
		List new=initList(t,d);
		while (p->next)
			p=p->next;
		p->next=new;
		new->prev=p;
	}
	return l;
}

List deleteList(List l)
{
	List k;
	while (l!=NULL)
	{
		k=l;
		l=l->next;
		free(k);
	}
	return l;
}

List sort(List l)
{
	List p=l,q;
	double aux;
	while (p->next!=NULL)
	{
		q=p->next;
		while (q!=NULL)
		{
			if (p->data > q->data)
			{
				aux=p->data;
				p->data=q->data;
				q->data=aux;
			}
		q=q->next;
		}
	p=p->next;
	}
	return l;
}
int main(int argc,char *argv[])
{
	int j,k=5;
	
	for (j=0;j<argc;j++)
	{
		if (strcmp(argv[j],"--e1")==0)
		{
			int n,i,x,nl;
			double y,s,t,m,sigma;
			List l=NULL,new=NULL;
			scanf("%d",&n);
			for (i=0;i<n;i++)
			{
				scanf("%d %lf",&x,&y);
				l=addLast(l,x,y);
				if (i==0 || i==1)
					new=addLast(new,x,y);
			}
			i=2;
			l=l->next->next;
			nl=2;
			while (i<n-2)
			{
				s=0;
				sigma=0;
				t=0;
				m=0;
				s=l->data+l->prev->data+l->prev->prev->data+l->next->data+l->next->next->data;
				m=s/k;
				t=(l->data-m)*(l->data-m)+(l->prev->data-m)*(l->prev->data-m)+(l->prev->prev->data-m)*(l->prev->prev->data-m)+(l->next->data-m)*(l->next->data-m)+(l->next->next->data-m)*(l->next->next->data-m);
				sigma=sqrt(t/k);
				if ((l->data > m-sigma)&&(l->data < m+sigma))
					{new=addLast(new,l->timestamp,l->data);
					nl++;
					}
				l=l->next;
				i++;	
			}
			new=addLast(new,l->timestamp,l->data);
			new=addLast(new,l->next->timestamp,l->next->data);
			nl=nl+2;
			printf("%d\n",nl);
			printList(new);
			deleteList(new);
		}
		if (strcmp(argv[j],"--e2")==0)
		{
			int i,n21,x21;
			double y21;
			List l21=NULL,mediana=NULL,sublist;
			scanf("%d",&n21);
			for (i=0;i<n21;i++)
			{
				scanf("%d %lf",&x21,&y21);
				l21=addLast(l21,x21,y21);
			}
			l21=l21->next->next;
			i=2;
			while (i<n21-2)
			{
				sublist=NULL;
				sublist=addLast(sublist,l21->prev->prev->timestamp,l21->prev->prev->data);
				sublist=addLast(sublist,l21->prev->timestamp,l21->prev->data);
				sublist=addLast(sublist,l21->timestamp,l21->data);
				sublist=addLast(sublist,l21->next->timestamp,l21->next->data);
				sublist=addLast(sublist,l21->next->next->timestamp,l21->next->next->data);
				sublist=sort(sublist);
				mediana=addLast(mediana,sublist->next->next->timestamp,sublist->next->next->data);
				deleteList(sublist);
				l21=l21->next;
				i++;
			}
			printf("%d\n",n21-4);
			printList(mediana);
			deleteList(mediana);
		}
		if (strcmp(argv[j],"--e3")==0)
		{
			int n22,i,x22;
			double y22,s2,m2;
			List l22=NULL,average=NULL;
			scanf("%d",&n22);
			for (i=0;i<n22;i++)
			{
				scanf("%d %lf",&x22,&y22);
				l22=addLast(l22,x22,y22);
			}
			l22=l22->next->next;
			i=2;
			while (i<n22-2)
			{
				s2=0;
				m2=0;
				s2=s2+l22->data+l22->prev->data+l22->prev->prev->data+l22->next->data+l22->next->next->data;
				m2=s2/k;
				average=addLast(average,l22->timestamp,m2);
				l22=l22->next;
				i++;
			}
			printf("%d\n",n22-4);
			printList(average);
			deleteList(average);
		}
		if (strcmp(argv[j],"--u")==0)
		{
			int n3,i,x;
			double y;
			List l3=NULL,q;
			scanf("%d",&n3);
			for (i=0;i<n3;i++)
			{
				scanf("%d %lf",&x,&y);
				l3=addLast(l3,x,y);
			}
			q=l3;
			l3=l3->next;
			while (l3)
			{
				if ((l3->timestamp-l3->prev->timestamp >= 100)&&(l3->timestamp-l3->prev->timestamp <= 1000))
				{
					l3->timestamp=(l3->timestamp+l3->prev->timestamp)/2;
					l3->data=(l3->data+l3->prev->data)/2;
				}
				l3=l3->next;
			}
			printf("%d\n",n3);
			printList(q);
			deleteList(q);
		}
		if (strcmp(argv[j],"--c")==0)
		{
			printf("eee");
		}
		if (strncmp(argv[j],"--st",4)==0)
		{
			printf("aaa");
		}
	}
	//printf("%d\n",nr_final);
	//printList(list);
}