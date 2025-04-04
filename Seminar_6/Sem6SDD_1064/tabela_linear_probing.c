#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int cod;
	char* nume;
	float medie;
} student;

typedef struct
{
	student** vect;
	int nrElem;
} hashT;

int functieHash(hashT tabela, int cheie)
{
	return cheie % tabela.nrElem;
}

void inserareHash(hashT tabela, student* s)
{
	int poz = functieHash(tabela, (*s).cod);
	if (tabela.vect[poz] == NULL)
		tabela.vect[poz] = s; //shallow copy intre 2 pointeri
	else
	{
		//coliziune
		int index = 1;
		while (poz + index < tabela.nrElem)
		{
			if (tabela.vect[poz + index] == NULL)
			{
				poz += index;
				tabela.vect[poz] = s;
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
			printf("\nPozitie = %d", i);
			printf("\nCod = %d, Nume = %s, Medie = %5.2f",
				tabela.vect[i]->cod, tabela.vect[i]->nume,
				tabela.vect[i]->medie);
		}
}

void dezalocareHash(hashT tabela)
{
	for(int i=0;i<tabela.nrElem;i++)
		if (tabela.vect[i] != NULL)
		{
			free(tabela.vect[i]->nume);
			free(tabela.vect[i]);
		}
	free(tabela.vect);
}

int stergereHash(hashT tabela, int codDeSters)
{
	int poz = functieHash(tabela, codDeSters);
	if (tabela.vect[poz] == NULL)
		return -1;
	else
		if (tabela.vect[poz]->cod == codDeSters)
		{
			free(tabela.vect[poz]->nume);
			free(tabela.vect[poz]);
			tabela.vect[poz] = NULL;
		}
		else
		{
			int index = 1;
			while (poz + index < tabela.nrElem)
			{
				if (tabela.vect[poz + index]->cod == codDeSters)
				{
					poz += index;
					free(tabela.vect[poz]->nume);
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
	tabela.vect = (student**)malloc(tabela.nrElem * sizeof(student*));
	for (int i = 0; i < tabela.nrElem; i++)
		tabela.vect[i] = NULL;

	int nrStud;
	student* s;
	char buffer[20];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrStud);
	for (int i = 0; i < nrStud; i++)
	{
		s = (student*)malloc(sizeof(student));
		fscanf(f, "%d", &s->cod);
		fscanf(f, "%s", buffer);
		s->nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s->nume, buffer);
		fscanf(f, "%f", &s->medie);
		inserareHash(tabela, s);
	}
	fclose(f);

	traversareHash(tabela);

	int poz = stergereHash(tabela, 70);
	printf("\nPozitia stearsa este %d", poz);

	traversareHash(tabela);

	dezalocareHash(tabela);
}