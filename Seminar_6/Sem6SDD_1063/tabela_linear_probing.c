#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int cod;
	char* marca;
	float pret;
} masina;

typedef struct
{
	masina** vect;
	int nrElem;
} hashT;

int functieHash(hashT tabela, int cheie)
{
	return cheie % tabela.nrElem;
}

void inserareHash(hashT tabela, masina* m)
{
	int poz = functieHash(tabela, (*m).cod);
	if (tabela.vect[poz] == NULL)
		tabela.vect[poz] = m; //shallow copy intre 2 pointeri
	else
	{
		int index = 1;
		while (poz + index < tabela.nrElem)
		{
			if (tabela.vect[poz + index] == NULL)
			{
				poz += index;
				tabela.vect[poz] = m;
				break;
			}
			else
				index++;
		}
	}
}

void traversareHash(hashT tabela)
{
	for(int i=0;i<tabela.nrElem;i++)
		if (tabela.vect[i] != NULL)
		{
			printf("\nPozitie=%d", i);
			printf("\nCod=%d, Marca=%s, Pret=%5.2f",
				tabela.vect[i]->cod, tabela.vect[i]->marca,
				tabela.vect[i]->pret);
		}
}

void dezalocareHash(hashT tabela)
{
	for(int i=0;i<tabela.nrElem;i++)
		if (tabela.vect[i] != NULL)
		{
			free(tabela.vect[i]->marca);
			free(tabela.vect[i]);
		}
	free(tabela.vect);
}

int stergereHash(hashT tabela, int codCautat)
{
	int poz = functieHash(tabela, codCautat);
	if (tabela.vect[poz] == NULL)
		return -1;
	else
		if (tabela.vect[poz]->cod == codCautat)
		{
			free(tabela.vect[poz]->marca);
			free(tabela.vect[poz]);
			tabela.vect[poz] = NULL;
		}
		else
		{
			int index = 1;
			while (poz + index < tabela.nrElem)
			{
				if (tabela.vect[poz + index]->cod == codCautat)
				{
					poz += index;
					free(tabela.vect[poz]->marca);
					free(tabela.vect[poz]);
					tabela.vect[poz] = NULL;
					break;
				}
				else
					index++;
			}
		}
	return poz;
}

void main()
{
	hashT tabela;
	tabela.nrElem = 23;
	tabela.vect = (masina**)malloc(tabela.nrElem * sizeof(masina*));
	for (int i = 0; i < tabela.nrElem; i++)
		tabela.vect[i] = NULL;

	int nrMasini;
	masina* m;
	char buffer[20];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrMasini);
	for (int i = 0; i < nrMasini; i++)
	{
		m = (masina*)malloc(sizeof(masina));
		fscanf(f, "%d", &m->cod);
		fscanf(f, "%s", buffer);
		m->marca = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(m->marca, buffer);
		fscanf(f, "%f", &m->pret);
		inserareHash(tabela, m);
	}
	fclose(f);

	traversareHash(tabela);

	printf("\n------------------------------\n");

	stergereHash(tabela, 47);

	traversareHash(tabela);

	dezalocareHash(tabela);
}