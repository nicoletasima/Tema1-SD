#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h>


typedef struct List 										//definire lista dublu inlantuita 
{
	int timestamp;											
	double data;											
	struct List *next;										
	struct List *prev;
}*List;

List initList(int t, double d)									//functie de initialiare pentru lista 
{
	List l = (List)malloc(sizeof(struct List));					//se aloca memorie pentru nod
	l->timestamp = t;
	l->data = d;
	l->next = NULL;												
	l->prev = NULL;
	return l;
}	

void printList(List l)											//functie recursiva prin care se afiseaza nodurile listei incepand cu capul de lista
{
	if (l != NULL)												
	{
		printf("%d %.2lf\n",l->timestamp,l->data);
		printList(l->next);
	}
}

List addLast(List l, int t, double d)							//functie de adaugare a unui nod la sfarsitul unei liste
{
	List p = l;
	if (l == NULL)												//daca lista este vida atunci apelez functia de init pentru a o initializa
		l = initList(t,d);
	else
	{
		List new = initList(t,d);									//in caz contrar aloc memorie pentru un nod nou caruia ii atribui valorile date in antetul functiei
		while (p->next)											//caut capatul listei
			p = p->next;
		p->next = new;											
		new->prev = p;											
	}
	return l;
}

List deleteList(List l)											//functie de stergere completa a unei liste
{
	List k;
	while (l != NULL)												
	{
		k = l;													
		l = l->next;												
		free(k);												//se elibereaza memoria alocata anterior pentru nodul curent 
	}
	return l;													//returneaza lista vida
}

List sort(List l)												//functie de sortare a unei liste
{
	List p = l, q;													
	double aux;
	while (p->next != NULL)										
	{
		q = p->next;												
		while (q != NULL)
		{
			if (p->data > q->data)								
			{
				aux = p->data;									
				p->data = q->data;
				q->data = aux;
			}
		q = q->next;
		}
	p = p->next;
	}
	return l;													//functia returneaza lista ordonata
}
int find_integer(double d,int delta)							//functie care gaseste cel mai apropiat intreg mai mic decat o valoare data
{
	int sup = 0;
	if (d < 0)
	{
		while (d <= sup)										//daca numarul e negativ se fac scaderi succesive de lungime a intervalului
			sup = sup - delta;
		return sup;
	}
	else
	{
		while (d >= sup)										//daca numarul e pozitiv se aduna succesiv lungimea intervalui pana se ajunge la valoarea dorita  
			sup = sup + delta;
		return sup - delta;
	}

}

double calcul_w(int i,double k)									//functie care calculeaza coeficientul w care are are rolul de a scadea influenta valorilor
{
	double coef_scalare, suma = 0;
	int j;
	for (j=0;j<k;j++)
	{
		suma = (double)(suma+(j/(k-1))*(j/(k-1))*0.9+0.1);					//se calculeaza suma de la numitor 
	}
	coef_scalare =(double) (((i/(k-1))*(i/(k-1))*0.9+0.1)/suma);			//se calculeaza coeficientul w conform formulei 
	return coef_scalare; 
}

double fscalare(int timestamp,int left,int right)				//calculul coeficientului de scalare
{
	double C;
	C = (double)(timestamp - left)/(right - left);				
	return C;
}


int main(int argc,char *argv[])									//se dau argumentele in main pentru rulare
{
	int j,k = 5, n = 0, x = 0, i, SW = 1;									//declarare variabile 
	double y;
	List l = NULL;

	scanf("%d",&n);												//se citeste numarul de elemente din lista initiala 
	for (i=0;i<n;i++)											//se citesc elementele de la tastatura si se adauga intr-o lista folosind functia addLast definita anterior 
	{
		scanf("%d %lf",&x,&y);
		l = addLast(l,x,y);
	}
	
	for (j=0;j<argc;j++)										//se parcurg argumentele date in linia de comanda si in functie de ele se merge pe una din ramuri 
	{	
		if (strcmp(argv[j],"--e1") == 0)				//1.1 Eliminare de exceptii folosind metode statistice
		{
			int nl;														//declarare de varibile 
			double s, t, m, sigma;
			List new = NULL;

			new = addLast(new,l->timestamp,l->data);					//avand in vedere ca primele si ultimele k/2 elemente din lista se exclud, primele 2 se adauga inca de la inceput in lista finala
			new = addLast(new,l->next->timestamp,l->next->data);
			i = 2;
			l = l->next->next;
			nl = 2;

			while (i < n-2)												//parcurg lista pana la al treilea element de la sfarsit
			{
				s = 0;
				sigma = 0;
				t = 0;
				m = 0;
				s = l->data+l->prev->data+l->prev->prev->data+l->next->data+l->next->next->data;		//calculez suma datelor reale din sublista de k = 5 elemente
				m = (double)s/k;																		//se calculeaza media aritmetica
				t = (l->data - m)*(l->data - m)+(l->prev->data - m)*(l->prev->data - m)+(l->prev->prev->data - m)*(l->prev->prev->data - m)+(l->next->data - m)*(l->next->data - m)+(l->next->next->data - m)*(l->next->next->data - m);
				sigma = sqrt(t/k);											//calculul deviatiei standard
				if ((l->data > m-sigma)&&(l->data < m+sigma))				
				{
					new = addLast(new,l->timestamp,l->data);					
					nl++;													
				}

				l = l->next;													
				i++;

			}
			new = addLast(new,l->timestamp,l->data);							
			new = addLast(new,l->next->timestamp,l->next->data);
			n = nl+2;
			l = new;
															
		}
		if (strcmp(argv[j],"--e2") == 0)				//1.2 Eliminare de zgomote folsind filtre - 1.2.1 Filtrare mediana
		{
			List mediana = NULL, sublist;										//declarare variabile de tip List in care se va depunde lista finala, respectiv sublista ce va fi ordonata
			l = l->next->next;												//incepand cu al treilea element al listei pentru a putea crea ferestrele de k = 5 elemente 
			i = 2;

			while (i < n-2)
			{
				sublist = NULL;
				sublist = addLast(sublist,l->prev->prev->timestamp,l->prev->prev->data);			//se creeaza sublista de k = 5 elemente 
				sublist = addLast(sublist,l->prev->timestamp,l->prev->data);
				sublist = addLast(sublist,l->timestamp,l->data);
				sublist = addLast(sublist,l->next->timestamp,l->next->data);
				sublist = addLast(sublist,l->next->next->timestamp,l->next->next->data);
				sublist = sort(sublist);																		//se ordoneaza sublista 
				mediana = addLast(mediana,sublist->next->next->timestamp,sublist->next->next->data);			
				deleteList(sublist);																		//se sterge sublista pentru a putea fi folosita in continuare
				l = l->next;																					
				i++;
			}

			n = n-4;																							
			l = mediana;																					
			
		}
		if (strcmp(argv[j],"--e3") == 0)		//1.2.2 Filtrare folosind media aritmetica
		{
			
			double s2, m2;									//declarare de varibile pentru suma, respectiv medie aritmetica
			List average = NULL;
			l = l->next->next;								
			i = 2;

			while (i < n-2)									 
			{
				s2 = 0;
				m2 = 0;
				s2 = s2+l->data+l->prev->data+l->prev->prev->data+l->next->data+l->next->next->data;		//se calculeaza media aritmetica a elementelor din sublista
				m2 = s2/k;
				average = addLast(average,l->timestamp,m2);					//se adauga media aritmetica calculata intr-o noua lista
				l = l->next;
				i++;
			}
			n = n - 4;														
			l = average;
			
		}
		if (strcmp(argv[j],"--u") == 0)		//1.3 Uniformizarea frecventei in timp a datelor
		{
			
			List q;							//declarare lista auxiliara 
									
			q = l;
			l = l->next;						//se ignora primul element al listei deorece campul prev pointeaza spre NULL

			while (l)					
			{
				if ((l->timestamp-l->prev->timestamp >= 100)&&(l->timestamp-l->prev->timestamp <= 1000))	//verifica daca diferenta dintre timestamp-ul curent si cel precedent este cea corecta
				{
					l->timestamp = (l->timestamp+l->prev->timestamp)/2;	//se calculeaza media aritmetica dintre timestamp-uri si data
					l->data = (l->data+l->prev->data)/2;					
				}
				l = l->next;												
			}

			l = q;														
			
		}
		if (strcmp(argv[j],"--c") == 0)									//1.4 Completarea datelor
		{
			List right = NULL, left = NULL, gaps = NULL;					//declarare varibile utilizate
			int time, op = 3,  t1 = 0, t2 = 0, nr_elem = 0;
			double w0, w1, w2, f, p1, p2, v1 = 0, v2 = 0, C;

			w0 = calcul_w(0,3);											//calcul w cu ajutorul functiei,acesta fiind constant 
			w1 = calcul_w(1,3);
			w2 = calcul_w(2,3);

			gaps = addLast(gaps,l->timestamp,l->data);					//adauga in lista gaps primele 3 elemente care nu se iau in considerare
			gaps = addLast(gaps,l->next->timestamp,l->next->data);
			gaps = addLast(gaps,l->next->next->timestamp,l->next->next->data);
			i = 2;
			nr_elem = 3;			//variabile in care se retine numarul final al elementelor din noua lista
			l = l->next->next;

			while (i < n - 3)		//lista se parcurge pana la antepenultimul element al listei
			{
				if (l->next->timestamp - l->timestamp > 1000)				
				{
					if (op >= 3)												//varibila op verifica peste cate elemente se trece in lista curenta de la ultima completare 
					{
						left = addLast(left,l->prev->prev->timestamp,l->prev->prev->data);			//daca este mai mare decat 3 atunci lista left va avea toate elementele din l
						left = addLast(left,l->prev->timestamp,l->prev->data);
						left = addLast(left,l->timestamp,l->data);
						right = addLast(right,l->next->timestamp,l->next->data);
						right = addLast(right,l->next->next->timestamp,l->next->next->data);
						right = addLast(right,l->next->next->next->timestamp,l->next->next->next->data);
						op = 0;
					}

					if (op == 2)							//daca aceasta este egala cu 2 inseamna ca s-a trecut peste 2 elemente din lista si left va contine ultimul element adaugat in lista noua
					{
						left = addLast(left,t2,v2);
						left = addLast(left,l->prev->timestamp,l->prev->data);
						left = addLast(left,l->timestamp,l->data);
						right = addLast(right,l->next->timestamp,l->next->data);
						right = addLast(right,l->next->next->timestamp,l->next->next->data);
						right = addLast(right,l->next->next->next->timestamp,l->next->next->next->data);
						op = 0;
					}

					if (op == 1)		//daca op = 1 atunci exista 2 timestamp-uri consecutive care au diferenta mai mare ca 1000 si se retin ultimele valori calculate
					{
						left = addLast(left,t1,v1);
						left = addLast(left,t2,v2);
						left = addLast(left,l->timestamp,l->data);
						right = addLast(right,l->next->timestamp,l->next->data);
						right = addLast(right,l->next->next->timestamp,l->next->next->data);
						right = addLast(right,l->next->next->next->timestamp,l->next->next->next->data);
						op = 0;
					}

					time = l->timestamp;
					p1 = w0*left->data + w1*left->next->data + w2*left->next->next->data; 
					p2 = w0*right->next->next->data + w1*right->next->data + w2*right->data;

					while (l->next->timestamp - time > 200)						//cat timp diferenta dintre timestamp-ul urmator din l si cel adaugat este mai mare ca 200
					{
						C = fscalare((time+200),l->timestamp,l->next->timestamp);			//calcul factor de scalare
						f = (1 - C)*p1 + C*p2;												//calcul valoare data ce va fi adaugata in noua lista gaps
						v1 = v2;															
						v2 = f;
						time = time + 200;													
						t1 = t2;
						t2 = time;
						gaps = addLast(gaps,time,f);
						nr_elem++;
					}

					gaps = addLast(gaps,l->next->timestamp,l->next->data);
					nr_elem++;
				}
				else									//in cazul in care diferenta este mai mica ca o secunda se adauga elementul urmator in lista
				{
					gaps = addLast(gaps,l->next->timestamp,l->next->data);
					nr_elem++;
					op++;
				}

				l = l->next;
				op++;

				if ((left != NULL)&&(right != NULL))			//daca left si right au fost folosite, atunci ele trebuie sterse pentru adaugarea urmatoarelor elemente
				{
					left = deleteList(left);
					right = deleteList(right);
					left = NULL;
					right = NULL;
				}

				i++;
			}

		gaps = addLast(gaps,l->next->timestamp,l->next->data);				
		gaps = addLast(gaps,l->next->next->timestamp,l->next->next->data);
		nr_elem = nr_elem + 2;
		n = nr_elem;
		l = gaps;
		//gaps = deleteList(gaps);
		}
		if (strncmp(argv[j],"--st",4) == 0)							//1.5 Bonus: Statistici
		{	
			char c[10];												//declarare variabile 
			int m = 0, delta0, delta, gama, nr_aparitii;

			i = 4;

			while (argv[j][i])									//calcul lungime interval extras din argument 
			{
				c[m] = argv[j][i];
				m++;
				i++;
			}
			delta0 = atoi(c);								//tranformarea sirului caractere intr-un numar intreg
			l = sort(l);									//ordonare lista 
			gama = find_integer(l->data,delta0);			//gama ia valoarea capatului superior primului interval
			delta = gama + delta0;							//delta reprezinta capatul inferior al intervalului

			while (l->next != NULL)							
			{
				nr_aparitii = 0;						//numara aparitiile elementelor din fiecare interval

				while ((l->next != NULL)&&(l->data >= gama)&&(l->data <=delta))
				{
					nr_aparitii++;
					l = l->next;
				}

				if ((l->next != NULL)&&(l->data >= gama)&&(l->data <= delta))		
					nr_aparitii++;

				if (nr_aparitii > 0)
					printf("[%d, %d] %d\n",gama,delta,nr_aparitii);

				gama = delta;						//creste valoarea intervalului cu delta0 
				delta = delta0 + delta;

			}
			nr_aparitii = 0;

			while ((l != NULL)&&(l->next == NULL))			//caz separat pentru ultimul element al listei 
			{
				if ((l->data >= gama)&&(l->data <= delta))
				{
					nr_aparitii++;
					l=l->next;
					printf("[%d, %d] %d\n",gama,delta,nr_aparitii);
				}

				gama = delta;
				delta = delta0 + delta;
			}

			SW = 0;
		}
	}

	if (SW == 1)
	{
		printf("%d\n",n);
		printList(l);
	}
	l = deleteList(l);
}


