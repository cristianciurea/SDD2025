#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int cod; //4B
	char* titlu; //4B
	float* vectPreturi; //4B
	int nrPreturi;
} carte;

typedef struct
{
	carte info; //informatie utila 12B
	struct nodLD* next, * prev;// 2 pointeri de legatura 4B x 2 = 8B
} nodLD;

nodLD* initializareNod(carte c)
{
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	//nou->info = c;
	nou->info.cod = c.cod;
	nou->info.titlu = (char*)malloc((strlen(c.titlu) + 1) * sizeof(char));
	strcpy(nou->info.titlu, c.titlu);
	nou->info.nrPreturi = c.nrPreturi;
	nou->info.vectPreturi = (float*)malloc(nou->info.nrPreturi * sizeof(float));
	for (int i = 0; i < c.nrPreturi; i++)
		nou->info.vectPreturi[i] = c.vectPreturi[i];
	nou->next = nou;
	nou->prev = nou;
	return nou;
}

nodLD* inserareNod1(nodLD* capLD, nodLD** coadaLD, carte c)
{
	nodLD* nou = initializareNod(c);
	if (capLD == NULL)
	{
		capLD = nou;
		*coadaLD = nou;
	}
	else
	{
		/*nodLD* temp = capLD;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou;
		nou->prev = temp;
		*coadaLD = nou;*/
		(*coadaLD)->next = nou;
		nou->prev = *coadaLD;
		*coadaLD = nou;
		(*coadaLD)->next = capLD;
		capLD->prev = *coadaLD;
	}
	return capLD;
}

void inserareNod2(nodLD** capLD, nodLD** coadaLD, carte c)
{
	nodLD* nou = initializareNod(c);
	if (*capLD == NULL)
	{
		*capLD = nou;
		*coadaLD = nou;
	}
	else
	{
		nodLD* temp = *capLD;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou;
		nou->prev = temp;
		*coadaLD = nou;
		(*coadaLD)->next = *capLD;
		(*capLD)->prev = *coadaLD;
	}
}

void traversareLista(nodLD* capLD)
{
	nodLD* temp = capLD;
	while (temp->next != capLD)
	{
		printf("\nCod = %d, Titlu = %s, Nr. preturi = %d",
			temp->info.cod, temp->info.titlu, temp->info.nrPreturi);
		for (int i = 0; i < temp->info.nrPreturi; i++)
			printf(" Pret = %5.2f", temp->info.vectPreturi[i]);
		temp = temp->next;
	}
	printf("\nCod = %d, Titlu = %s, Nr. preturi = %d",
		temp->info.cod, temp->info.titlu, temp->info.nrPreturi);
	for (int i = 0; i < temp->info.nrPreturi; i++)
		printf(" Pret = %5.2f", temp->info.vectPreturi[i]);
}

void traversareListaInvers(nodLD* coadaLD)
{
	nodLD* temp = coadaLD;
	while (temp->prev != coadaLD)
	{
		printf("\nCod = %d, Titlu = %s, Nr. preturi = %d",
			temp->info.cod, temp->info.titlu, temp->info.nrPreturi);
		for (int i = 0; i < temp->info.nrPreturi; i++)
			printf(" Pret = %5.2f", temp->info.vectPreturi[i]);
		temp = temp->prev;
	}
	printf("\nCod = %d, Titlu = %s, Nr. preturi = %d",
		temp->info.cod, temp->info.titlu, temp->info.nrPreturi);
	for (int i = 0; i < temp->info.nrPreturi; i++)
		printf(" Pret = %5.2f", temp->info.vectPreturi[i]);
}

void dezalocareLista(nodLD* capLD)
{
	nodLD* temp = capLD;
	while (temp->next != capLD)
	{
		nodLD* aux = temp->next;
		free(temp->info.titlu);
		free(temp->info.vectPreturi);
		free(temp);
		temp = aux;
	}
	free(temp->info.titlu);
	free(temp->info.vectPreturi);
	free(temp);
}

void salvareCartiVector(nodLD* capLD, carte* vect, int* nrElem, int prag)
{
	nodLD* temp = capLD;
	while (temp->next != capLD)
	{
		if (temp->info.nrPreturi >= prag)
		{
			vect[*nrElem] = temp->info;
			(*nrElem)++;
		}
		temp = temp->next;
	}
	if (temp->info.nrPreturi >= prag)
	{
		vect[*nrElem] = temp->info;
		(*nrElem)++;
	}
}

void stergeCarteDupaTitlu(nodLD** capLD, nodLD** coadaLD, char* titluDeSters)
{
	//caz 1
	if (strcmp((*capLD)->info.titlu, titluDeSters) == 0)
	{
		nodLD* temp = *capLD;
		*capLD = (*capLD)->next;
		(*capLD)->prev = *coadaLD;
		(*coadaLD)->next = *capLD;
		free(temp->info.titlu);
		free(temp->info.vectPreturi);
		free(temp);
		return;
	}
	else //caz 2
		if (strcmp((*coadaLD)->info.titlu, titluDeSters) == 0)
		{
			nodLD* temp = *coadaLD;
			*coadaLD = (*coadaLD)->prev;
			(*coadaLD)->next = *capLD;
			(*capLD)->prev = *coadaLD;
			free(temp->info.titlu);
			free(temp->info.vectPreturi);
			free(temp);
			return;
		}
		else //caz 3
		{
			nodLD* temp = *capLD;
			while (temp != NULL)
			{
				if (strcmp(temp->info.titlu, titluDeSters) == 0)
				{
					nodLD* anterior = temp->prev;
					nodLD* urmator = temp->next;
					anterior->next = urmator;
					urmator->prev = anterior;
					free(temp->info.titlu);
					free(temp->info.vectPreturi);
					free(temp);
					return;
				}
				else
					temp = temp->next;
			}
		}
}

void main()
{
	int nrCarti;
	carte c;
	nodLD* capLD = NULL, * coadaLD = NULL;
	char buffer[20];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrCarti);
	for (int i = 0; i < nrCarti; i++)
	{
		fscanf(f, "%d", &c.cod);
		fscanf(f, "%s", buffer);
		c.titlu = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(c.titlu, buffer);
		fscanf(f, "%d", &c.nrPreturi);
		c.vectPreturi = (float*)malloc(c.nrPreturi * sizeof(float));
		for (int i = 0; i < c.nrPreturi; i++)
			fscanf(f, "%f", &c.vectPreturi[i]);
		capLD = inserareNod1(capLD, &coadaLD, c);
		free(c.titlu);
		free(c.vectPreturi);
	}
	fclose(f);

	traversareLista(capLD);
	printf("\n--------------Traversare inversa--------------\n");
	traversareListaInvers(coadaLD);

	printf("\n--------------Vector--------------\n");
	carte* vect = (carte*)malloc(nrCarti * sizeof(carte));
	int nrElem = 0;
	salvareCartiVector(capLD, vect, &nrElem, 3);
	for (int i = 0; i < nrElem; i++)
	{
		printf("\nCod = %d, Titlu = %s, Nr. preturi = %d",
			vect[i].cod, vect[i].titlu, vect[i].nrPreturi);
		for (int j = 0; j < vect[i].nrPreturi; j++)
			printf(" Pret = %5.2f", vect[i].vectPreturi[j]);
	}
	free(vect);

	printf("\n--------------Stergere--------------\n");
	stergeCarteDupaTitlu(&capLD, &coadaLD, "Baltagul");
	traversareLista(capLD);
	printf("\n--------------Traversare inversa--------------\n");
	traversareListaInvers(coadaLD);

	dezalocareLista(capLD);
}