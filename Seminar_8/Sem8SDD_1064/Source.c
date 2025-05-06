#include<malloc.h>
#include<stdio.h>

typedef struct
{
	int zi, luna, an;
} data;

typedef struct
{
	int cod;
	char* companie;
	float pret;
	data dataZbor;
} zbor;

typedef struct
{
	zbor inf;
	struct nodArb* stanga, * dreapta;
} nodArb;

nodArb* creareNod(zbor z, nodArb* st, nodArb* dr)
{
	nodArb* nou = (nodArb*)malloc(sizeof(nodArb));
	nou->inf.cod = z.cod;
	nou->inf.companie = (char*)malloc((strlen(z.companie) + 1) * sizeof(char));
	strcpy(nou->inf.companie, z.companie);
	nou->inf.pret = z.pret;
	nou->inf.dataZbor.zi = z.dataZbor.zi;
	nou->inf.dataZbor.luna = z.dataZbor.luna;
	nou->inf.dataZbor.an = z.dataZbor.an;
	nou->stanga = st;
	nou->dreapta = dr;
	return nou;
}

nodArb* inserareNod(nodArb* rad, zbor z)
{
	if (rad != NULL)
	{
		if (z.cod < rad->inf.cod)
		{
			rad->stanga = inserareNod(rad->stanga, z);
			return rad;
		}
		else
			if (z.cod > rad->inf.cod)
			{
				rad->dreapta = inserareNod(rad->dreapta, z);
				return rad;
			}
			else
				return rad;
	}
	else
		return creareNod(z, NULL, NULL);
}

//RSD
void preordine(nodArb* rad)
{
	if (rad != NULL)
	{
		printf("\nCod=%d, Companie=%s, Pret=%5.2f, Data=%d/%d/%d",
			rad->inf.cod, rad->inf.companie, rad->inf.pret,
			rad->inf.dataZbor.zi, rad->inf.dataZbor.luna, rad->inf.dataZbor.an);
		preordine(rad->stanga);
		preordine(rad->dreapta);
	}
}

//SRD
void inordine(nodArb* rad)
{
	if (rad != NULL)
	{
		inordine(rad->stanga);
		printf("\nCod=%d, Companie=%s, Pret=%5.2f, Data=%d/%d/%d",
			rad->inf.cod, rad->inf.companie, rad->inf.pret,
			rad->inf.dataZbor.zi, rad->inf.dataZbor.luna, rad->inf.dataZbor.an);
		inordine(rad->dreapta);
	}
}

//SDR
void postordine(nodArb* rad)
{
	if (rad != NULL)
	{
		postordine(rad->stanga);
		postordine(rad->dreapta);
		printf("\nCod=%d, Companie=%s, Pret=%5.2f, Data=%d/%d/%d",
			rad->inf.cod, rad->inf.companie, rad->inf.pret,
			rad->inf.dataZbor.zi, rad->inf.dataZbor.luna, rad->inf.dataZbor.an);
	}
}

void dezalocare(nodArb* rad)
{
	//preordine
	/*if (rad != NULL)
	{
		nodArb* st = rad->stanga;
		nodArb* dr = rad->dreapta;
		free(rad->inf.companie);
		free(rad);
		dezalocare(st);
		dezalocare(dr);
	}*/
	//postordine
	if (rad != NULL)
	{
		dezalocare(rad->stanga);
		dezalocare(rad->dreapta);
		free(rad->inf.companie);
		free(rad);
	}
}

void salvareZboruriVector(nodArb* rad, zbor* vect, int* nr, int anPrag)
{
	if (rad != NULL)
	{
		if (rad->stanga==NULL && rad->dreapta==NULL && rad->inf.dataZbor.an == anPrag)
		{
			vect[*nr].cod = rad->inf.cod;
			vect[*nr].companie = (char*)malloc((strlen(rad->inf.companie) + 1) * sizeof(char));
			strcpy(vect[*nr].companie, rad->inf.companie);
			vect[*nr].pret = rad->inf.pret;
			vect[*nr].dataZbor.zi = rad->inf.dataZbor.zi;
			vect[*nr].dataZbor.luna = rad->inf.dataZbor.luna;
			vect[*nr].dataZbor.an = rad->inf.dataZbor.an;
			(*nr)++;
		}
		salvareZboruriVector(rad->stanga, vect, nr, anPrag);
		salvareZboruriVector(rad->dreapta, vect, nr, anPrag);
	}
}

void sumaPreturilor(nodArb* rad, float* suma)
{
	if (rad != NULL)
	{
		*suma += rad->inf.pret;
		sumaPreturilor(rad->stanga, suma);
		sumaPreturilor(rad->dreapta, suma);
	}
}

void main()
{
	nodArb* rad = NULL;
	zbor z;
	char buffer[20];
	int nrZboruri;
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrZboruri);
	for (int i = 0; i < nrZboruri; i++)
	{
		fscanf(f, "%d", &z.cod);
		fscanf(f, "%s", buffer);
		z.companie = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(z.companie, buffer);
		fscanf(f, "%f", &z.pret);
		fscanf(f, "%d", &z.dataZbor.zi);
		fscanf(f, "%d", &z.dataZbor.luna);
		fscanf(f, "%d", &z.dataZbor.an);
		rad = inserareNod(rad, z);
		free(z.companie);
	}
	fclose(f);

	printf("\n---------------------\n");
	preordine(rad);
	printf("\n---------------------\n");
	inordine(rad);
	printf("\n---------------------\n");
	postordine(rad);

	printf("\n---------------------\n");

	zbor* vect = (zbor*)malloc(nrZboruri * sizeof(zbor));
	int nr = 0;
	salvareZboruriVector(rad, vect, &nr, 2023);
	for(int i=0;i<nr;i++)
		printf("\nCod=%d, Companie=%s, Pret=%5.2f, Data=%d/%d/%d",
			vect[i].cod, vect[i].companie, vect[i].pret,
			vect[i].dataZbor.zi, vect[i].dataZbor.luna, vect[i].dataZbor.an);

	for (int i = 0; i < nr; i++)
		free(vect[i].companie);
	free(vect);

	float suma = 0;
	sumaPreturilor(rad, &suma);
	printf("\nSuma preturilor = %5.2f", suma);

	dezalocare(rad);
}