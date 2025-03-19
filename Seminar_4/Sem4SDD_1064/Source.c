#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int zi, luna, an;
} dataReceptionare;

typedef struct
{
	dataReceptionare data;
	char* expeditor;
	float dimensiune;
} mesaj;

typedef struct
{
	mesaj info; //20B
	struct nodStiva* next; //4B
} nodStiva;

typedef struct
{
	mesaj info; //20B
	struct nodCoada* next; //4B
} nodCoada;

//fct de inserare nod in stiva
void push(nodStiva** varf, mesaj m)
{
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->info.data.zi = m.data.zi;
	nou->info.data.luna = m.data.luna;
	nou->info.data.an = m.data.an;
	nou->info.expeditor = (char*)malloc((strlen(m.expeditor) + 1) * sizeof(char));
	strcpy(nou->info.expeditor, m.expeditor);
	nou->info.dimensiune = m.dimensiune;
	nou->next = NULL;
	if (*varf == NULL)
		*varf = nou;
	else
	{
		nou->next = *varf;
		*varf = nou;
	}
}

//fct pentru extragere nod din stiva
int pop(nodStiva** varf, mesaj* m)
{
	if (*varf == NULL)
		return -1;
	else
	{
		(*m).data.zi = (*varf)->info.data.zi;
		(*m).data.luna = (*varf)->info.data.luna;
		(*m).data.an = (*varf)->info.data.an;
		(*m).expeditor = (char*)malloc((strlen((*varf)->info.expeditor) + 1) * sizeof(char));
		strcpy((*m).expeditor, (*varf)->info.expeditor);
		(*m).dimensiune = (*varf)->info.dimensiune;
		nodStiva* temp = *varf;
		*varf = (*varf)->next;
		free(temp->info.expeditor);
		free(temp);
		return 0;
	}
}

void afisareStiva(nodStiva* varf)
{
	nodStiva* temp = varf;
	while (temp)
	{
		printf("\nData = %d/%d/%d, Expeditor = %s, Dimensiune = %5.2f",
			temp->info.data.zi, temp->info.data.luna, temp->info.data.an,
			temp->info.expeditor, temp->info.dimensiune);
		temp = temp->next;
	}
}

void dezalocareStiva(nodStiva* varf)
{
	mesaj m;
	while (pop(&varf, &m) == 0)
		free(m.expeditor);
}

//fct de inserare nod in coada
void put(nodCoada** prim, nodCoada** ultim, mesaj m)
{
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->info.data.zi = m.data.zi;
	nou->info.data.luna = m.data.luna;
	nou->info.data.an = m.data.an;
	nou->info.expeditor = (char*)malloc((strlen(m.expeditor) + 1) * sizeof(char));
	strcpy(nou->info.expeditor, m.expeditor);
	nou->info.dimensiune = m.dimensiune;
	nou->next = NULL;
	if (*prim == NULL || *ultim == NULL)
	{
		*prim = nou;
		*ultim = nou;
	}
	else
	{
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

//fct de extragere nod din coada
int get(nodCoada** prim, nodCoada** ultim, mesaj* m)
{
	if (*prim != NULL && *ultim != NULL)
	{
		(*m).data.zi = (*prim)->info.data.zi;
		(*m).data.luna = (*prim)->info.data.luna;
		(*m).data.an = (*prim)->info.data.an;
		(*m).expeditor = (char*)malloc((strlen((*prim)->info.expeditor) + 1) * sizeof(char));
		strcpy((*m).expeditor, (*prim)->info.expeditor);
		(*m).dimensiune = (*prim)->info.dimensiune;
		nodCoada* temp = *prim;
		*prim = (*prim)->next;
		free(temp->info.expeditor);
		free(temp);
		return 0;
	}
	if (*prim == NULL)
	{
		*ultim = NULL;
		return -1;
	}
}

void afisareCoada(nodCoada* prim)
{
	nodCoada* temp = prim;
	while (temp)
	{
		printf("\nData = %d/%d/%d, Expeditor = %s, Dimensiune = %5.2f",
			temp->info.data.zi, temp->info.data.luna, temp->info.data.an,
			temp->info.expeditor, temp->info.dimensiune);
		temp = temp->next;
	}
}

void dezalocareCoada(nodCoada *prim, nodCoada *ultim)
{
	mesaj m;
	while (get(&prim, &ultim, &m) == 0)
		free(m.expeditor);
}

void conversieStivaCoada(nodStiva* varf, nodCoada** prim, nodCoada** ultim)
{
	mesaj m;
	while (pop(&varf, &m) == 0)
		put(prim, ultim, m);
}

void determinareMesajeDimensiune(nodCoada* prim, nodCoada* ultim, mesaj* vect, int* nr, float prag)
{
	mesaj m;
	while(get(&prim, &ultim, &m)==0)
		if (m.dimensiune >= prag)
		{
			vect[*nr] = m;
			(*nr)++;
		}
}

void main()
{
	int nrMesaje;
	nodStiva* varf = NULL;
	nodCoada* prim = NULL, * ultim = NULL;
	mesaj m;
	char buffer[30];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrMesaje);
	for (int i = 0; i < nrMesaje; i++)
	{
		fscanf(f, "%d", &m.data.zi);
		fscanf(f, "%d", &m.data.luna);
		fscanf(f, "%d", &m.data.an);
		fscanf(f, "%s", buffer);
		m.expeditor = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(m.expeditor, buffer);
		fscanf(f, "%f", &m.dimensiune);
		//inserare in stiva
		push(&varf, m);
		//inserare in coada
		//put(&prim, &ultim, m);
		free(m.expeditor);
	}
	fclose(f);

	printf("\n-------Stiva-------\n");

	afisareStiva(varf);

	//dezalocareStiva(varf);

	printf("\n-------Coada-------\n");

	conversieStivaCoada(varf, &prim, &ultim);

	afisareCoada(prim);

	//dezalocareCoada(prim, ultim);

	printf("\n-------Vector-------\n");

	mesaj* vect = (mesaj*)malloc(nrMesaje * sizeof(mesaj));
	int nr = 0;
	determinareMesajeDimensiune(prim, ultim, vect, &nr, 200);
	for(int i=0;i<nr;i++)
		printf("\nData = %d/%d/%d, Expeditor = %s, Dimensiune = %5.2f",
			vect[i].data.zi, vect[i].data.luna, vect[i].data.an,
			vect[i].expeditor, vect[i].dimensiune);
	for (int i = 0; i < nr; i++)
		free(vect[i].expeditor);
	free(vect);
}
