#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int cod;
	char* titlu;
	int nrAutori;
	float pret;
} carte;

typedef struct
{
	int BF;
	carte inf; //16B
	struct nodArb* stanga, * dreapta; //8B
} nodArb;

nodArb* creareNod(carte c, nodArb* st, nodArb* dr)
{
	nodArb* nou = (nodArb*)malloc(sizeof(nodArb));
	nou->inf.cod = c.cod;
	nou->inf.titlu = (char*)malloc((strlen(c.titlu) + 1) * sizeof(char));
	strcpy(nou->inf.titlu, c.titlu);
	nou->inf.nrAutori = c.nrAutori;
	nou->inf.pret = c.pret;
	nou->stanga = st;
	nou->dreapta = dr;
	return nou;
}

nodArb* inserareNod(nodArb* rad, carte c)
{
	if (rad != NULL)
	{
		if (c.cod < rad->inf.cod)
		{
			rad->stanga = inserareNod(rad->stanga, c);
			return rad;
		}
		else
			if (c.cod > rad->inf.cod)
			{
				rad->dreapta = inserareNod(rad->dreapta, c);
				return rad;
			}
			else
				return rad;
	}
	else
		return creareNod(c, NULL, NULL);
}

//RSD
void preordine(nodArb* rad)
{
	if (rad != NULL)
	{
		printf("\nCod=%d, Titlu=%s, Nr. autori=%d, Pret=%5.2f",
			rad->inf.cod, rad->inf.titlu, rad->inf.nrAutori, rad->inf.pret);
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
		printf("\nCod=%d, Titlu=%s, Nr. autori=%d, Pret=%5.2f",
			rad->inf.cod, rad->inf.titlu, rad->inf.nrAutori, rad->inf.pret);
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
		printf("\nCod=%d, Titlu=%s, Nr. autori=%d, Pret=%5.2f",
			rad->inf.cod, rad->inf.titlu, rad->inf.nrAutori, rad->inf.pret);
	}
}

void dezalocare(nodArb* rad)
{
	//preordine
	//if (rad != NULL)
	//{
	//	nodArb* st = rad->stanga;
	//	nodArb* dr = rad->dreapta;
	//	free(rad->inf.titlu);
	//	free(rad);
	//	dezalocare(st);
	//	dezalocare(dr);
	//}
	//postordine
	if (rad != NULL)
	{
		dezalocare(rad->stanga);
		dezalocare(rad->dreapta);
		free(rad->inf.titlu);
		free(rad);
	}
}

void numarCartiAutori(nodArb* rad, int* nr, int prag)
{
	if (rad != NULL)
	{
		if (rad->inf.nrAutori > prag)
			(*nr)++;
		numarCartiAutori(rad->stanga, nr, prag);
		numarCartiAutori(rad->dreapta, nr, prag);
	}
}

void salvareCartiVector(nodArb* rad, carte* vect, int* nr, float pretPrag)
{
	if (rad != NULL)
	{
		if (rad->stanga == NULL && rad->dreapta == NULL && rad->inf.pret > pretPrag)
		{
			vect[*nr].cod = rad->inf.cod;
			vect[*nr].titlu = (char*)malloc((strlen(rad->inf.titlu) + 1) * sizeof(char));
			strcpy(vect[*nr].titlu, rad->inf.titlu);
			vect[*nr].nrAutori = rad->inf.nrAutori;
			vect[*nr].pret = rad->inf.pret;
			(*nr)++;
		}
		salvareCartiVector(rad->stanga, vect, nr, pretPrag);
		salvareCartiVector(rad->dreapta, vect, nr, pretPrag);
	}
}

int maxim(int a, int b)
{
	int max = a;
	if (max < b)
		max = b;
	return max;
}

int inaltimeArbore(nodArb* rad)
{
	if (rad != NULL)
		return 1 + maxim(inaltimeArbore(rad->stanga),
			inaltimeArbore(rad->dreapta));
	else
		return 0;
}

nodArb* stergeRad(nodArb* rad)
{
	nodArb* aux = rad;
	if (aux->stanga != NULL)
	{
		rad = aux->stanga;
		if (aux->dreapta != NULL)
		{
			nodArb* temp = aux->stanga;
			while (temp->dreapta != NULL)
				temp = temp->dreapta;
			temp->dreapta = aux->dreapta;
		}
	}
	else
		if (aux->dreapta != NULL)
			rad = aux->dreapta;
		else
			rad = NULL;
	free(aux->inf.titlu);
	free(aux);
	return rad;
}

nodArb* stergeNod(nodArb* rad, int cheieDeSters)
{
	if (rad != NULL)
	{
		if (rad->inf.cod == cheieDeSters)
		{
			rad = stergeRad(rad);
			return rad;
		}
		else
			if (cheieDeSters < rad->inf.cod)
			{
				rad->stanga = stergeNod(rad->stanga, cheieDeSters);
				return rad;
			}
			else
			{
				rad->dreapta = stergeNod(rad->dreapta, cheieDeSters);
				return rad;
			}
	}
	else
		return NULL;
}

void calculBF(nodArb* rad)
{
	if (rad != NULL)
	{
		rad->BF = inaltimeArbore(rad->dreapta) - inaltimeArbore(rad->stanga);
		calculBF(rad->stanga);
		calculBF(rad->dreapta);
	}
}

nodArb* rotatie_dreapta(nodArb* rad)
{
	printf("\nRotatie dreapta\n");
	nodArb* nod1 = rad->stanga;
	rad->stanga = nod1->dreapta;
	nod1->dreapta = rad;
	rad = nod1;
	return rad;
}

nodArb* rotatie_stanga(nodArb* rad)
{
	printf("\nRotatie stanga\n");
	nodArb* nod1 = rad->dreapta;
	rad->dreapta = nod1->stanga;
	nod1->stanga = rad;
	rad = nod1;
	return rad;
}

nodArb* rotatie_dreapta_stanga(nodArb* rad)
{
	printf("\nRotatie dreapta-stanga\n");
	nodArb* nod1 = rad->dreapta;
	nodArb* nod2 = nod1->stanga;
	nod1->stanga = nod2->dreapta;
	nod2->dreapta = nod1;
	rad->dreapta = nod2->stanga;
	nod2->stanga = rad;
	rad = nod2;
	return rad;
}

nodArb* rotatie_stanga_dreapta(nodArb* rad)
{
	printf("\nRotatie stanga-dreapta\n");
	nodArb* nod1 = rad->stanga;
	nodArb* nod2 = nod1->dreapta;
	nod1->dreapta = nod2->stanga;
	nod2->stanga = nod1;
	rad->stanga = nod2->dreapta;
	nod2->dreapta = rad;
	rad = nod2;
	return rad;
}

nodArb* reechilibrare(nodArb* rad)
{
	calculBF(rad);
	nodArb* stanga = rad->stanga;
	nodArb* dreapta = rad->dreapta;
	if (rad->BF <= -2 && stanga->BF <= -1)
	{
		rad = rotatie_dreapta(rad);
		calculBF(rad);
	}
	else
		if (rad->BF >= 2 && dreapta->BF >= 1)
		{
			rad = rotatie_stanga(rad);
			calculBF(rad);
		}
		else
			if (rad->BF >= 2 && dreapta->BF <= -1)
			{
				rad = rotatie_dreapta_stanga(rad);
				calculBF(rad);
			}
			else
				if (rad->BF <= -2 && stanga->BF >= 1)
				{
					rad = rotatie_stanga_dreapta(rad);
					calculBF(rad);
				}
	return rad;
}

void main()
{
	nodArb* rad = NULL;
	carte c;
	int nrCarti;
	char buffer[30];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrCarti);
	for (int i = 0; i < nrCarti; i++)
	{
		fscanf(f, "%d", &c.cod);
		fscanf(f, "%s", buffer);
		c.titlu = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(c.titlu, buffer);
		fscanf(f, "%d", &c.nrAutori);
		fscanf(f, "%f", &c.pret);
		rad = inserareNod(rad, c);
		free(c.titlu);

		rad = reechilibrare(rad);
	}
	fclose(f);

	printf("\n-------preordine----\n");
	preordine(rad);
	printf("\n-------preordine stanga----\n");
	preordine(rad->stanga);
	printf("\n-------preordine dreapta----\n");
	preordine(rad->dreapta);
	printf("\nInaltime subarbore stang = %d", inaltimeArbore(rad->stanga));
	printf("\nInaltime subarbore drept = %d", inaltimeArbore(rad->dreapta));
	/*printf("\n-------inordine----\n");
	inordine(rad);
	printf("\n-------postordine----\n");
	postordine(rad);*/

	printf("\n----------------------\n");
	int nr = 0;
	numarCartiAutori(rad, &nr, 2);
	printf("\nNumar carti autori = %d", nr);

	printf("\n----------------------\n");
	carte* vect = (carte*)malloc(nrCarti * sizeof(carte));
	int nrC = 0;
	salvareCartiVector(rad, vect, &nrC, 110);
	for (int i = 0; i < nrC; i++)
		printf("\nCod=%d, Titlu=%s, Nr. autori=%d, Pret=%5.2f",
			vect[i].cod, vect[i].titlu, vect[i].nrAutori, vect[i].pret);

	for (int i = 0; i < nrC; i++)
		free(vect[i].titlu);
	free(vect);

	printf("\n----------------------\n");
	printf("\nInaltime arbore = %d", inaltimeArbore(rad));

	printf("\n-----------dupa stergere---------------\n");
	//rad = stergeRad(rad);
	rad = stergeNod(rad, 10);
	preordine(rad);
	printf("\nInaltime subarbore stang = %d", inaltimeArbore(rad->stanga));
	printf("\nInaltime subarbore drept = %d", inaltimeArbore(rad->dreapta));

	dezalocare(rad);
}