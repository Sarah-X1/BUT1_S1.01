#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h> // pour qsort

#pragma warning (disable:4996)
enum { MAXNOM = 31, MAXETU = 100, MAXABS = 800 }; // nbre de caracteres du nom, nbre d'étudiants, nbre d'absence max : 40jourmax x 2 (am et pm) x 100 (nbre etudiants) = 800 
enum { MAXENTREE = 100 }; // nbre caracteres qu'on va écrire 
enum { DEMI = 3 }; // am ou pm
enum { MAXCAR = 51 }; // nbre de caractere pour justificatif
enum { ACCEPTEE = 3 }; // ok ou ko
enum { MAXNJ = 3 }; //le nbre maximum de jour qui suit l'absence pour justifié avant de ne plus être justifiable
enum Etat { NONJUSTIFIEE = 0, ATTENTEJUSTIFICAT = 1, ATTENTEVALIDATION = 2, JUSTIFIEE = 3 }; //etat d'une absence
enum { VIDE = -1 }; //chaine vide
enum { NONTROUVEE = 0, TROUVEE = 1 };
enum { JOURCOMMENCE = 1, JOURFINI = 40 }; //num jour minimum est 1 et num jour maximum de l'absence est 40
enum { ABSETU_DEFAILLANT = 5 }; //nbre d'absence max avant d'être defaillant

/*XXXXXXXXXXXXXXXXXXXX structure Etudiant XXXXXXXXXXXXXXXXX*/

typedef struct {
	int id;
	char NomEtu[MAXNOM];
	int NumGrp;//Numero de groupe de l'étudiant
} Etudiant;

/*XXXXXXXXXXXXXXXXXXXXX structure Absence XXXXXXXXXXXXXXXXXXXX*/

typedef struct {
	int id; //id de l'étudiant
	unsigned int NumJour;
	char demiJour[DEMI];
	int idAbs; // identifiant de l'absence
	unsigned JourJusti;//Jour du depôt de la justification
	char explication[MAXCAR]; // la justification par l'élève de l'absence 
	unsigned int etat; //Etat de l'absence
	char JustiAcceptee[ACCEPTEE]; //décision du directeur : valider ou invalider une justification
} Absence;

/*XXXXXXXXXXXXXXXXXXXXXX Fonction Inscription C1 
XXXXXXXXXXXXXXXXXXXXXXXX*/

void inscription(Etudiant* tabEtu, unsigned int* compEtu);

/*XXXXXXXXXXXXXXXXXXXXXXX Fonction Absence C2
XXXXXXXXXXXXXXXXXXXXXXXXX*/

void absence(Absence* tabAbs, Etudiant* tabEtu, unsigned int* compAbs, unsigned int* compEtu);

/*XXXXXXXXXXXXXXXXXXXXXXXX Sous Fonction Compare (trie Etudiant)
XXXXXXXXXXXXXXXXXXXXXXXXXX*/

int compare(const void* a, const void* b);

/*XXXXXXXXXXXXXXXXXXXXXXXXX Fonction Etudiants C3
XXXXXXXXXXXXXXXXXXXXXXXXXXX*/

void etudiants(Etudiant* tabEtu, unsigned int* compEtu, Absence* tabAbs, unsigned int* compAbs);

/*XXXXXXXXXXXXXXXXXXXXXXXXXXX Fonction Justificatif C4
XXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

void justificatif(Absence* tabAbs, unsigned int* compAbs);

/*XXXXXXXXXXXXXXXXXXXXXXXXXXXX Sous Fonction Compare2 (trie Etudiant + Absence)
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

int compare2(const void* a, const void* b);

/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXX Fonction Validations C5
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

void validations(Absence* tabAbs, unsigned int* compAbs, Etudiant* tabEtu, unsigned int* compEtu);

/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX Fonction Validation C6
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

void validation(Absence* tabAbs, unsigned int* compAbs);

/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX Sous Fonction Compare3 (trie Absence)
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

int compare3(const void* a, const void* b);

/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX Fonction Etudiant C7
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

void etudiant(Etudiant* tabEtu, unsigned int* compEtu, Absence* tabAbs, unsigned int* compAbs);

/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX Fonction Defaillants C8
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

void defaillants(Absence* tabAbs, unsigned int* compAbs, Etudiant* tabEtu, unsigned int* compEtu);

/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX Fonction MAIN XXXXXXXXXXXXXXXXXXXXXXXXXXX*/

int main() {
	Etudiant etud[MAXETU];
	unsigned int compteurEtudiant = 0;
	Absence absences[MAXABS];
	unsigned int compteurAbsence = 0;
	char entrée[MAXENTREE];	//tableau car chaine de caractère
	while (1) {
		scanf("%s", entrée);
		if (strcmp(entrée, "inscription") == 0) {
			inscription(etud, &compteurEtudiant);
		}
		else if (strcmp(entrée, "absence") == 0) {
			absence(absences, etud, &compteurAbsence, &compteurEtudiant);
		}
		else if (strcmp(entrée, "etudiants") == 0) {
			etudiants(etud, &compteurEtudiant, absences, &compteurAbsence);
		}
		else if (strcmp(entrée, "justificatif") == 0) {
			justificatif(absences, &compteurAbsence);
		}
		else if (strcmp(entrée, "validations") == 0) {
			validations(absences, &compteurAbsence, etud, &compteurEtudiant);
		}
		else if (strcmp(entrée, "validation") == 0) {
			validation(absences, &compteurAbsence);
		}
		else if (strcmp(entrée, "etudiant") == 0) {
			etudiant(etud, &compteurEtudiant, absences, &compteurAbsence);
		}
		else if (strcmp(entrée, "defaillants") == 0) {
			defaillants(absences, &compteurAbsence, etud, &compteurEtudiant);
		}
		else if (strcmp(entrée, "exit") == 0) { 
			return 0; //quitte le programme
		}
	}
}

/*XXXXXXXXXXXXXXXXXXXXXX Fonction Inscription C1
XXXXXXXXXXXXXXXXXXXXXXXX*/

void inscription(Etudiant* tabEtu, unsigned int* compEtu) { // void car fonction ne retourne rien
	char NomEtu[MAXNOM];
	int NumGrp;
	scanf("%s%d", NomEtu, &NumGrp); //pas de & pour NomEtu car tableau deja converti en pointeur
	//assert(strlen(NomEtu) <= 30) 
	for (unsigned int i = 0; i < *compEtu; ++i) {
		if (strcmp(NomEtu, tabEtu[i].NomEtu) == 0 && NumGrp == tabEtu[i].NumGrp) {
			printf("Nom incorrect\n");
			return; // nom incorrect donc sort
		}
	}
	tabEtu[*compEtu].id = *compEtu + 1;
	strcpy(tabEtu[*compEtu].NomEtu, NomEtu);
	tabEtu[*compEtu].NumGrp = NumGrp;
	++(*compEtu);
	printf("Inscription enregistree (%u)\n", *compEtu);
}

/*XXXXXXXXXXXXXXXXXXXXXXX Fonction Absence C2
XXXXXXXXXXXXXXXXXXXXXXXXX*/

void absence(Absence* tabAbs, Etudiant* tabEtu, unsigned int* compAbs, unsigned int* compEtu) {
	int id;
	unsigned int NumJour;
	char demiJour[DEMI];
	scanf("%d%u%s", &id, &NumJour, demiJour);
	for (unsigned int i = 0; i < *compAbs; ++i) {
		if (id == tabAbs[i].id && NumJour == tabAbs[i].NumJour && strcmp(demiJour, tabAbs[i].demiJour) == 0) { //si les entrées sont déjà dans le tabAbs alors l'absence est connue
			printf("Absence deja connue\n");
			return;
		}
	}
	for (unsigned int j = 0; j < *compEtu; ++j) {
		if (id == tabEtu[j].id) { //si id de l'étudiant existe dans tabEtu alors c un etudiant bien inscrit
			if (NumJour >= JOURCOMMENCE && NumJour <= JOURFINI) {
				if (strcmp(demiJour, "am") == 0 || (strcmp(demiJour, "pm") == 0)) {
					tabAbs[*compAbs].idAbs = *compAbs + 1; //on stock le détail de l'absence entrées par l'utilisateur dans tabAbs
					tabAbs[*compAbs].id = tabEtu[j].id;
					tabAbs[*compAbs].NumJour = NumJour;
					strcpy(tabAbs[*compAbs].demiJour, demiJour);
					tabAbs[*compAbs].explication[0] = VIDE;
					tabAbs[*compAbs].JustiAcceptee[0] = VIDE;
					tabAbs[*compAbs].etat = ATTENTEJUSTIFICAT;
					++(*compAbs);
					printf("Absence enregistree [%u]\n", *compAbs);
					return;
				}
				else {
					printf("Demi-journee incorrecte\n");
					return;
				}
			}
			else {
				printf("Date incorrecte\n");
				return;
			}
		}
	}
	printf("Identifiant incorrect\n");
	return;
}


/*XXXXXXXXXXXXXXXXXXXXXXXX Sous Fonction Compare
XXXXXXXXXXXXXXXXXXXXXXXXXX*/

int compare(const void* a, const void* b) {
	//conversion
	const Etudiant* p1 = (const Etudiant*)a;
	const Etudiant* p2 = (const Etudiant*)b;
	if ((*p1).NumGrp < (*p2).NumGrp)
		return -1; // groupe de p1 est plus petit que groupe de p2
	if ((*p1).NumGrp > (*p2).NumGrp)
		return 1; // groupe de p1 est plus grand que groupe de p2
	// ici on sait que : groupe de p1 == groupe de p2, ils sont dans le même groupe 
	return strcmp((*p1).NomEtu, (*p2).NomEtu); // on compare les 2 noms
}

/*XXXXXXXXXXXXXXXXXXXXXXXXXX Fonction Etudiants C3
XXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

void etudiants(Etudiant* tabEtu, unsigned int* compEtu, Absence* tabAbs, unsigned int* compAbs) {
	unsigned int JourCourant;
	scanf("%u", &JourCourant);
	if (JourCourant < JOURCOMMENCE) { //si le JourCourant est inférieur à 1
		printf("Date incorrecte\n");
		return;
	}
	else if (*compEtu == 0) { //s'il n'y aucun etudiant inscrit à ce jour
		printf("Aucun inscrit\n");
		return;
	}
	else {
		qsort(tabEtu, *compEtu, sizeof(tabEtu[0]), compare); //trie Etudiants
		for (unsigned int i = 0; i < *compEtu; ++i) { //pour chaque etudiant
			unsigned int nbreAbs = 0;
			for (unsigned int j = 0; j < *compAbs; ++j) {
				if (tabAbs[j].NumJour <= JourCourant && tabAbs[j].id == tabEtu[i].id) {
					nbreAbs++;
				}
			}
			printf("(%d) %s %d %d\n", tabEtu[i].id, tabEtu[i].NomEtu, tabEtu[i].NumGrp, nbreAbs);
		}
	}
}

/*XXXXXXXXXXXXXXXXXXXXXXXX Fonction Justificatif C4
XXXXXXXXXXXXXXXXXXXXXXXXXX*/

void justificatif(Absence* tabAbs, unsigned int* compAbs) {
	int idAbs;
	unsigned int JourJusti;
	char explication[MAXCAR];
	scanf("%u%u%[^\n]", &idAbs, &JourJusti, explication); //%[^\n] pour inclure les espaces de l'explication
	for (unsigned int i = 0; i < *compAbs; ++i) {
		if (tabAbs[i].idAbs == idAbs) {  // l'idAbs connu
			if (JourJusti >= JOURCOMMENCE && JourJusti >= tabAbs[i].NumJour) { // si numjour supérieur ou égale à 1 et numjour après la date absence ou à la date d'absence
				//printf("entre"); pour voir si ça rentre dans la condition
				if (tabAbs[i].explication[0] == VIDE) { // explication est vide donc l'absence pas encore justifiee
					tabAbs[i].JourJusti = JourJusti; // Enregistrer le numéro de jour de la justification
					strcpy(tabAbs[i].explication, explication);// copie l'explication dans Enregistrer l'explication
					printf("Justificatif enregistre\n");
					if (JourJusti - tabAbs[i].NumJour > MAXNJ) { // si justificatif absence + de trois jours de l'absence //nbre magique
						tabAbs[i].etat = NONJUSTIFIEE; // non-justifiee
						//printf("%d\n", tabAbs[i].etat);
						return;
					}
					else {
						tabAbs[i].etat = ATTENTEVALIDATION; // en cours de traitement
						//printf("%d\n", tabAbs[i].etat);
						return;
					}
					return;
				}
				else {
					printf("Justificatif deja connu\n"); //si une justification existe déjà pour cet identifiant d'absence
					return;
				}
			}
			else {
				printf("Date incorrecte\n");
				return;
			}
		}
	}
	printf("Identifiant incorrect\n");
	return;
}

/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX Sous Fonction Compare2
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

int compare2(const void* a, const void* b) {
	//conversion
	const Absence* p1 = (const Absence*)a;
	const Absence* p2 = (const Absence*)b;
	if ((*p1).id < (*p2).id) //trier par numero d’´etudiant croissant
		return -1; // id de p1 est plus petit que id de p2
	if ((*p1).id > (*p2).id)
		return 1; // id de p1 est plus grand que id de p2
	// Si les id sont égaux : meme etudiant, comparaison par NumJour (chronologie)
	if ((*p1).NumJour < (*p2).NumJour)
		return -1;  // absence de p1 vient avant absence de p2
	if ((*p1).NumJour > (*p2).NumJour)
		return 1;   // absence de p1 vient après celle de p2 
	// Si les id et NumJour sont égaux, on compare par demi-journée
	return strcmp((*p1).demiJour, (*p2).demiJour);
}

/*XXXXXXXXXXXXXXXXXXXXXXXXXXX Fonction Validations C5
XXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

void validations(Absence* tabAbs, unsigned int* compAbs, Etudiant* tabEtu, unsigned int* compEtu) {
	unsigned int validationTrouvee = NONTROUVEE; //si validation trouvé
	qsort(tabAbs, *compAbs, sizeof(tabAbs[0]), compare2);
	for (unsigned i = 0; i < *compAbs; ++i) {
		if (tabAbs[i].etat == ATTENTEVALIDATION) { // si l'absence est en cours de traitement
			for (unsigned int j = 0; j < *compEtu; ++j) {
				if (tabAbs[i].id == tabEtu[j].id) { //jointure par l'id
					printf("[%d] (%d) %s %d %u/%s (%s)\n", tabAbs[i].idAbs, tabAbs[i].id, tabEtu[j].NomEtu, tabEtu[j].NumGrp, tabAbs[i].NumJour, tabAbs[i].demiJour, tabAbs[i].explication + 1); // tabJusti[i].explication + 1 car on ne veux pas d'espace entre ( et l'explication
					validationTrouvee = TROUVEE; // une absence en attente de validation a ete trouvee
				}
			}
		}
	}
	if (validationTrouvee == NONTROUVEE) { // Si aucune absence en attente de validation n'a ete trouvee
		printf("Aucune validation en attente\n");
		return;
	}
}

/*XXXXXXXXXXXXXXXXXXXXXXXXXXX Fonction Validation C6
XXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

void validation(Absence* tabAbs, unsigned int* compAbs) {
	int idAbsence;
	char JustiAcceptee[ACCEPTEE];
	scanf("%d%s", &idAbsence, JustiAcceptee);
	for (unsigned int i = 0; i < *compAbs; ++i) {
		if (tabAbs[i].idAbs == idAbsence) { //si l’idAbsence est une absence connue et en attente de validation
			//printf("%d\n", tabAbs[i].etat); //verifie si l'etat est en attente pour voir s'il rentre dans la condition 
			if (tabAbs[i].JustiAcceptee[0] == VIDE) {
				//printf("%d\n", tabAbs[i].etat);
				if (tabAbs[i].etat == ATTENTEVALIDATION) {
					if (strcmp(JustiAcceptee, "ok") == 0) {
						strcpy(tabAbs[i].JustiAcceptee, JustiAcceptee); //copie caractères de JustiAcceptee dans tabAbs
						tabAbs[i].etat = JUSTIFIEE; // l'absence est justifiee
						printf("Validation enregistree\n");
						return;
					}
					else if (strcmp(JustiAcceptee, "ko") == 0) {
						strcpy(tabAbs[i].JustiAcceptee, JustiAcceptee);
						tabAbs[i].etat = NONJUSTIFIEE; // l'absence est non-justifiee
						printf("Validation enregistree\n");
						return;
					}
					else { // le cas où (strcmp(JustiAcceptee, "ok") != 0 && strcmp(JustiAcceptee, "ko" != 0)) 
						printf("Code incorrect\n");
						return;
					}
				}
				else {
					printf("Identifiant incorrect\n"); //si autre etat qu'en attente de validation
					return;
				}
			}
			else {
				printf("Validation deja connue\n");
				return;

			}
		}
	}
	printf("Identifiant incorrect\n");
	return;
}

/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXX Sous Fonction Compare3
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

int compare3(const void* a, const void* b) {
	//conversion
	const Absence* abs1 = (const Absence*)a;
	const Absence* abs2 = (const Absence*)b;
	if ((*abs1).NumJour < (*abs2).NumJour)
		return -1;  // absence 1 vient avant absence 2
	if ((*abs1).NumJour > (*abs2).NumJour)
		return 1;   // absence 1 vient après absence 2
	// Si NumJour sont égaux, on compare par demi-journée
	return strcmp((*abs1).demiJour, (*abs2).demiJour);
}

/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX Fonction Etudiant C7
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

void etudiant(Etudiant* tabEtu, unsigned int* compEtu, Absence* tabAbs, unsigned int* compAbs) {
	int idEtu;
	unsigned int JourCourant;
	int nonjusti = 0;
	int attentejust = 0;
	int justi = 0;
	int attentevalid = 0;
	scanf("%d%u", &idEtu, &JourCourant);
	if (JourCourant < JOURCOMMENCE) {
		printf("Date incorrecte\n");
		return;
	}
	qsort(tabAbs, *compAbs, sizeof(tabAbs[0]), compare3); //on trie par absence de l'étudiant
	for (unsigned int i = 0; i < *compEtu; ++i) { //pour chaque etudiant
		if (tabEtu[i].id == idEtu) { //si idEtu est un identifiant d'un etudiant inscrit
			unsigned int nbreAbs = 0;
			for (unsigned int j = 0; j < *compAbs; ++j) {
				if (tabAbs[j].NumJour <= JourCourant && tabAbs[j].id == tabEtu[i].id) {
					nbreAbs++;
					//printf("idAbs : %d etat : %d", tabAbs[j].id, tabAbs[j].etat);
					//printf("(%d) etat = %d\n", tabAbs[j].idAbs, tabAbs[j].etat);
					//printf("%d", tabAbs[j].etat);

					if (JourCourant - tabAbs[j].NumJour <= MAXNJ && tabAbs[j].JustiAcceptee == VIDE && tabAbs[j].explication[0] == VIDE) { //l'absence ne dépasse pas 3 jours sans justificat, elle est donc en attente de justification
						tabAbs[j].etat = ATTENTEJUSTIFICAT;
						//printf("la");
					}
					else if (tabAbs[j].etat == ATTENTEJUSTIFICAT && JourCourant - tabAbs[j].NumJour > MAXNJ) { // ATTENTEJUSTIF JOUR ABSENCE > 3 JOUR
						tabAbs[j].etat = NONJUSTIFIEE;
						//printf("go");
					}
					else if (JourCourant - tabAbs[j].NumJour > MAXNJ && JourCourant < tabAbs[j].JourJusti) {
						tabAbs[j].etat = NONJUSTIFIEE;
					}
					else if (JourCourant < tabAbs[j].JourJusti && tabAbs[j].explication[0] != VIDE) { // y a une justification après le JourCourant
						tabAbs[j].etat = ATTENTEJUSTIFICAT;
						//printf("lo");
					}
					else if (JourCourant >= tabAbs[j].JourJusti && tabAbs[i].explication[0] != VIDE) {// y a une justification avant le JourCourant 
						if (tabAbs[j].JourJusti - tabAbs[j].NumJour <= MAXNJ) {
							if (tabAbs[j].JustiAcceptee[0] == VIDE)
								tabAbs[j].etat = ATTENTEVALIDATION;
							else if (strcmp(tabAbs[j].JustiAcceptee, "ok") == 0)
								tabAbs[j].etat = JUSTIFIEE;
							else if (strcmp(tabAbs[j].JustiAcceptee, "ko") == 0)
								tabAbs[j].etat = NONJUSTIFIEE;
						}
						else
							tabAbs[j].etat = NONJUSTIFIEE;
					}
					else if (JourCourant - tabAbs[j].NumJour <= MAXNJ && tabAbs[j].etat == NONJUSTIFIEE && tabAbs[i].JustiAcceptee == VIDE) {
						tabAbs[j].etat = ATTENTEJUSTIFICAT;
						//printf("hello");
					}
					else if (JourCourant - tabAbs[j].NumJour <= MAXNJ && strcmp(tabAbs[i].JustiAcceptee, "ko") == 0 && tabAbs[i].JourJusti > JourCourant) {
						tabAbs[j].etat = ATTENTEJUSTIFICAT;
						//printf("po");
					}

					else if (tabAbs[j].JourJusti > JourCourant && tabAbs[j].etat == ATTENTEVALIDATION) {
						tabAbs[j].etat = ATTENTEJUSTIFICAT;
						//printf("pa");
					}
					if (tabAbs[j].etat == ATTENTEJUSTIFICAT) {
						attentejust++;
					}
					else if (tabAbs[j].etat == NONJUSTIFIEE) {
						nonjusti++;
					}
					else if (tabAbs[j].etat == JUSTIFIEE) {
						justi++;
					}
					else {
						attentevalid++;
					}
				}
			}
			//printf("attenteJ : %d \nnonJ : %d\nJ : %d\nattenteV : %d\n", attentejust, nonjusti, justi, attentevalid);
			printf("(%d) %s %d %d\n", tabEtu[i].id, tabEtu[i].NomEtu, tabEtu[i].NumGrp, nbreAbs);
			if (attentejust > 0) {
				printf("- En attente justificatif\n");
				for (unsigned int j = 0; j < *compAbs; ++j) {
					if (tabAbs[j].etat == ATTENTEJUSTIFICAT && tabAbs[j].id == tabEtu[i].id && tabAbs[j].NumJour <= JourCourant) {
						printf("  [%d] %u/%s\n", tabAbs[j].idAbs, tabAbs[j].NumJour, tabAbs[j].demiJour);
						--attentejust;
					}
				}
			}
			if (attentevalid > 0) {
				printf("- En attente validation\n");
				for (unsigned int j = 0; j < *compAbs; ++j) {
					if (tabAbs[j].etat == ATTENTEVALIDATION && tabAbs[j].id == tabEtu[i].id && tabAbs[j].NumJour <= JourCourant) {
						printf("  [%d] %u/%s", tabAbs[j].idAbs, tabAbs[j].NumJour, tabAbs[j].demiJour);
						--attentevalid;
						if (tabAbs[j].explication[0] != VIDE && JourCourant >= tabAbs[j].JourJusti) { //si une justification existe déjà pour cet identifiant d'absence
							printf(" (%s)\n", tabAbs[j].explication + 1);
						}
						else {
							printf("\n"); //pour retour à la ligne si pas d'explication antérieur à mettre
						}
					}
				}
			}
			if (justi > 0) {
				printf("- Justifiees\n");
				for (unsigned int j = 0; j < *compAbs; ++j) {
					if (tabAbs[j].etat == JUSTIFIEE && tabAbs[j].id == tabEtu[i].id && tabAbs[j].NumJour <= JourCourant) {
						printf("  [%d] %u/%s", tabAbs[j].idAbs, tabAbs[j].NumJour, tabAbs[j].demiJour);
						--justi;
						if (tabAbs[j].explication[0] != VIDE && JourCourant >= tabAbs[j].JourJusti) { //si une justification existe déjà pour cet identifiant d'absence
							printf(" (%s)\n", tabAbs[j].explication + 1);
						}
						else {
							printf("\n"); //pour retour à la ligne si pas d'explication antérieur à mettre
						}
					}
				}
			}
			if (nonjusti > 0) {
				printf("- Non-justifiees\n");
				for (unsigned int j = 0; j < *compAbs; ++j) {
					if (tabAbs[j].etat == NONJUSTIFIEE && tabAbs[j].id == tabEtu[i].id && tabAbs[j].NumJour <= JourCourant) {
						printf("  [%d] %u/%s", tabAbs[j].idAbs, tabAbs[j].NumJour, tabAbs[j].demiJour);
						--nonjusti;
						if (tabAbs[j].explication[0] != VIDE && JourCourant >= tabAbs[j].JourJusti) { //si une justification existe déjà pour cet identifiant d'absence
							printf(" (%s)\n", tabAbs[j].explication + 1);
						}
						else {
							printf("\n"); //pour retour à la ligne si pas d'explication antérieur à mettre
						}
					}
				}
			}
			return; //plus d'absence donc sort
		}
	}
	printf("Identifiant incorrect\n");
	return;
}

/*XXXXXXXXXXXXXXXXXXXXXXX Fonction Defaillants C8
XXXXXXXXXXXXXXXXXXXXXXXXX*/

void defaillants(Absence* tabAbs, unsigned int* compAbs, Etudiant* tabEtu, unsigned int* compEtu) {
	int JourCourant;
	unsigned int defaillantTrouvee = NONTROUVEE;
	scanf("%d", &JourCourant);
	qsort(tabEtu, *compEtu, sizeof(tabEtu[0]), compare);
	if (JourCourant < JOURCOMMENCE) {
		printf("Date incorrecte\n");
		return;
	}
	else {
		for (unsigned int i = 0; i < *compEtu; ++i) {
			unsigned int compAbsEtu = 0; //compte absence pour chaque etudiant individuellement
			unsigned int nbJNonJusti = 0; //compte absence non justifiée d'un etudiant
			for (unsigned int j = 0; j < *compAbs; ++j) {
				if (tabEtu[i].id == tabAbs[j].id && tabAbs[j].NumJour <= JourCourant) {
					if (tabAbs[j].etat == ATTENTEJUSTIFICAT && JourCourant - tabAbs[j].NumJour > MAXNJ) {
						//tabAbs[j].etat = NONJUSTIFIEE;
						++nbJNonJusti;
					}
					//else if (JourCourant - tabAbs[j].NumJour <= MAXNJ) {
						//tabAbs[j].etat = ATTENTEJUSTIFICAT;
					//}
					if (tabAbs[j].etat == NONJUSTIFIEE) {
						++nbJNonJusti;
					}
					++compAbsEtu;
				}
			}
			if (nbJNonJusti >= ABSETU_DEFAILLANT) { //étudiant a plus de 5 absences non-justifiiées à ce jour
				printf("(%d) %s %d %u\n", tabEtu[i].id, tabEtu[i].NomEtu, tabEtu[i].NumGrp, compAbsEtu);
				defaillantTrouvee = TROUVEE;
			}
		}
	}
	if (defaillantTrouvee == NONTROUVEE) {
		printf("Aucun defaillant\n");
		return; // aucun étudiant defaillant trouvé sort 
	}
}

