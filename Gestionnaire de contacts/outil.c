#include <stdio.h>   /* pour les entrées-sorties */
#include <string.h>  /* pour les manipulations de chaînes de caractères */
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include "rep.h"

#define VERSION 3.0
#define SQUELET
/**************************************************************************/
/* Compléter votre nom ici                                                */
/*   Nom : Maëse                 Prénom : Fabien                          */
/**************************************************************************/

extern bool modif;


/**********************************************************************/
/*  Ajout d'un contact dans le répertoire stocké en mémoire           */
/**********************************************************************/

int ajouter_un_contact_dans_rep(Repertoire* rep, Enregistrement enr)
{
#ifdef IMPL_TAB
	
	int idx;

	if (rep->nb_elts < MAX_ENREG)
	{
		idx = rep->nb_elts;
		rep->tab[idx] = enr;
		rep->nb_elts = rep->nb_elts + 1;

	}
	else {
		return(ERROR);
	}

	
#else
#ifdef IMPL_LIST

	bool inserted = false;
	if (rep->nb_elts == 0) {
		InsertElementAt(rep->liste, 0, enr);
		if (InsertElementAt(rep->liste, rep->liste->size, enr) ==1) {
			rep->nb_elts += 1;
			modif = true;
			rep->est_trie = true;
			return(OK);
		}

	}
	else {
		if (rep->nb_elts < MAX_ENREG) {
			int i = 0;
			// on regarde à quel indice (i) l'élément qu'on veut ajouter n'est plus inférieur aux autres, pour le placer au bon endroit

		/*	SingleLinkedListElem* elem = GetElementAt(rep->liste, 0);
			while ((elem != NULL) && (est_sup(elem->pers, enr) == true)) {
				i++;
				elem = elem->next;
			}*/
			
			for (int j = 0; j < rep->nb_elts; j++) {
				SingleLinkedListElem* elem = GetElementAt(rep->liste, j);					
				
				if (est_sup(elem->pers, enr)) {
					i++;
				}
			}

			InsertElementAt(rep->liste, i, enr);
			int ret = InsertElementAt(rep->liste, i, enr);
			if (ret == 1) {
				rep->liste->size += 1;
				return(OK);
			}
			
			else { return(ERROR); }
		}

	}
	

#endif
	
#endif


	return(OK);

} /* fin ajout */
  /**********************************************************************/
  /* supprime du répertoire l'enregistrement dont l'indice est donné en */
  /*   paramètre       et place modif = true                            */
  /**********************************************************************/
#ifdef IMPL_TAB
void supprimer_un_contact_dans_rep(Repertoire *rep, int indice) {

	
	if (rep->nb_elts >= 1)		/* s'il y a au moins un element ds le tableau */
	{						/* et que l'indice pointe a l'interieur */
		if (indice <= rep->nb_elts || indice>0) {

			for (int i = indice+1; i < rep->nb_elts; i++) {

				rep->tab[i-1] = rep->tab[i];			//on part du principe que les indices commencent à 1 : tab[0], le 1er contact, est à l'indice 1
			}
			
		}

		rep->nb_elts -= 1;		/* dans tous les cas, il y a un element en moins */
		modif = true;
	}

	return;
} /* fin supprimer */

#else
#ifdef IMPL_LIST
  /************************************************************************/
  /* supprime du répertoire l'enregistrement contenu dans le maillon elem */
  /*                   et fixe modif à vrai                              */
  /************************************************************************/
  // complet

	int supprimer_un_contact_dans_rep_liste(Repertoire *rep, SingleLinkedListElem *elem) {
	
	int ret=DeleteLinkedListElem(rep->liste, elem);
	if (ret == 1) {
		rep->nb_elts--;
		modif = true;
	}

	return (0); 
	}
#endif
#endif


  /**********************************************************************/
  /*  fonction d'affichage d'un enregistrement sur une ligne à l'écran  */
  /* ex Dupont, Jean                 0320304050                         */
  /**********************************************************************/
void affichage_enreg(Enregistrement enr)
{
	printf("\n%s, %s			%s", enr.nom, enr.prenom, enr.tel);


} /* fin affichage_enreg */
  /**********************************************************************/
  /*  fonction d'affichage d'un enregistrement avec alignement des mots */
  /* pour les listes                                                    */
  /* ex | Dupont                |Jean                  |0320304050      */
  /**********************************************************************/
void affichage_enreg_frmt(Enregistrement enr) {

	int lettres_nom = strlen(enr.nom);
	int lettres_prenom = strlen(enr.prenom);
	int num_tel = strlen(enr.tel);

	printf("\n|%s", enr.nom);
	if (lettres_nom < MAX_NOM) {
		for (int i = lettres_nom; i<= MAX_NOM; i++) {
			printf(" ");
		}
	}

	printf("|%s", enr.prenom);
	if (lettres_prenom < MAX_NOM) {
		for (int j = lettres_prenom; j <= MAX_NOM; j++) {
			printf(" ");
		}
	}

	printf("|%s", enr.tel);
	if (num_tel < MAX_TEL) {
		for (int j = lettres_prenom; j <= MAX_TEL; j++) {
			printf(" ");
		}
	}
		
			
	// comme fonction affichage_enreg, mais avec présentation alignées des infos
	

} /* fin affichage_enreg */


  /**********************************************************************/
  /* test si dans l'ordre alphabetique, un enregistrement est apres     */
  /* un autre                                                           */
  /**********************************************************************/
bool est_sup(Enregistrement enr1, Enregistrement enr2)
{
	
	unsigned int k = 0;
	
	// on met tous les noms et prénoms en majuscules 

	for(k=0; k<strlen(enr1.nom); k++){
		toupper(enr1.nom[k]);

	}

	for (k = 0; k < strlen(enr1.prenom); k++) {
		toupper(enr1.prenom[k]);
		
	}

	for (k = 0; k < strlen(enr2.nom); k++) {
		toupper(enr1.nom[k]);
	
	}

	for (k = 0; k < strlen(enr2.prenom); k++) {
		toupper(enr2.prenom[k]);
		
	}


	//les 2 variables qui serviront à comparer les noms et les prénoms 
	int ret1 = strcmp(enr1.nom, enr2.nom);
	int ret2 = strcmp(enr1.prenom, enr2.prenom); 


	if (ret1 == 0) {	//si les noms sont égaux

		if (ret2 < 0) {
			return(false);
		}
		else if (ret2 > 0) {
			return(true);
		}
		else {
			return(true); //choix arbitraire, si les deux personnes ont le même nom et prénom, l'ordre importe peu
		}

	}

	if (ret1 > 0) {
		return(true);
	}
	
	else{
		return(false);
	}
}
 
/*********************************************************************/
/*   Tri Alphabetique du tableau d'enregistrements                   */
/*********************************************************************/

void trier(Repertoire *rep)
{

#ifdef IMPL_TAB
	
	

	
	if (rep->est_trie == false) {
		//tri à bulles
		for (int i = 0; i < rep->nb_elts; i++) {
			for (int j = 0; j < rep->nb_elts; j++) {
				if (est_sup(rep->tab[i], rep->tab[j]) == false) {
					Enregistrement temp = rep->tab[i];
					rep->tab[i] = rep->tab[j];
					rep->tab[j] = temp;
				}
			} 
		}
	}

	
#else
#ifdef IMPL_LIST
	// ajouter code ici pour Liste
	// rien à faire !
	// la liste est toujours triée
#endif
#endif


	rep->est_trie = true;

} /* fin trier */

  /**********************************************************************/
  /* recherche dans le répertoire d'un enregistrement correspondant au  */
  /*   nom à partir de l'indice ind                                     */
  /*   retourne l'indice de l'enregistrement correspondant au critère ou*/
  /*   un entier négatif si la recherche est négative				    */
  /**********************************************************************/

int rechercher_nom(Repertoire* rep, char nom[], int ind)
{
	int i = ind;		    /* position (indice) de début de recherche dans tableau/liste rep */
	int ind_fin;			/* position (indice) de fin de tableau/liste rep */
	char tmp_nom[MAX_NOM];	/* 2 variables temporaires dans lesquelles */
	char tmp_nom2[MAX_NOM];	/* on place la chaine recherchee et la chaine lue dans le */
							/* tableau, afin de les convertir en majuscules et les comparer */
	bool trouve = false;

	ind_fin = rep->nb_elts - 1; // indice de fin à ne pas dépasser
	strncpy_s(tmp_nom, _countof(tmp_nom), nom, _TRUNCATE);  //copie le contenu d'une chaîne de caractère dans une autre



#ifdef IMPL_TAB           

	while ((i <= ind_fin) && (!trouve))
	{
		strncpy_s(tmp_nom2, _countof(tmp_nom2), rep->tab[i].nom, _TRUNCATE);
		
		if (strcmp(tmp_nom, tmp_nom2) == 0)
			trouve = true;
		else
		{
			// si pas trouvé, on passe au suivant
			i++;
		}
}


#else
#ifdef IMPL_LIST
	// ajouter code ici pour Liste
	for(int i =0; i<= ind_fin;i++){

		SingleLinkedListElem* elem= GetElementAt(rep->liste, i);
		int ret = strcmp(elem->pers.nom, nom);
		if (ret == 0) {
			trouve = true;
		}

	}
	

#endif
#endif

	return((trouve) ? i : -1);
} /* fin rechercher_nom */

  /*********************************************************************/
  /* Supprimer tous les caracteres non numériques de la chaine        */
  /*********************************************************************/
void compact(char *s){

	for (unsigned int i = 0; i < strlen(s); i++) {
		if ((s[i] < 48) || (s[i] > 57)) {
			for (unsigned int j=i; j < strlen(s); j++) {
				s[j - 1] = s[i];			
			}
			free(&s[strlen(s)-1]);

		}
	}

	return;
}

/**********************************************************************/
/* sauvegarde le répertoire dans le fichier dont le nom est passé en  */
/* argument                                                           */
/* retourne OK si la sauvegarde a fonctionné ou ERROR sinon           */
/**********************************************************************/
int sauvegarder(Repertoire *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err = fopen_s(&fic_rep, nom_fichier, "w");
#ifdef IMPL_TAB

	if (fic_rep == NULL) {

	}
	for (int i = 0; i < rep->nb_elts; i++) {

		fputs(rep->tab[i].nom, fic_rep);
		fputs("		", fic_rep);
		fputs(rep->tab[i].prenom, fic_rep);
		fputs("		", fic_rep);
		fputs(rep->tab[i].tel, fic_rep);
		fputs("\n", fic_rep);
	}
	
	
#else
#ifdef IMPL_LIST
	// ajouter code ici pour Liste
	for (int i = 0; i < rep->nb_elts; i++) {
		
		fputs(GetElementAt(rep->liste,i)->pers.nom, fic_rep);
		fputs("		", fic_rep);
		fputs(GetElementAt(rep->liste, i)->pers.prenom, fic_rep);
		fputs("		", fic_rep);
		fputs(GetElementAt(rep->liste, i)->pers.tel, fic_rep);
		fputs("\n", fic_rep);
	}
#endif
#endif

	return(OK);
} /* fin sauvegarder */


  /**********************************************************************/
  /*   charge dans le répertoire le contenu du fichier dont le nom est  */
  /*   passé en argument                                                */
  /*   retourne OK si le chargement a fonctionné et ERROR sinon         */
  /**********************************************************************/

int charger(Repertoire *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(Enregistrement);
	char buffer[sizeof(Enregistrement) + 1];
	int idx = 0;

	char *char_nw_line;
	
	_set_errno(0);
	if ( ((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0) || (fic_rep == NULL) )
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep) && (rep->nb_elts < MAX_ENREG))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
#ifdef IMPL_TAB
				if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].nom, MAX_NOM, SEPARATEUR) == OK)
				{
					idx++;							/* on saute le separateur */
					if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].prenom, MAX_NOM, SEPARATEUR) == OK)
					{
						idx++;
						if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].tel, MAX_TEL, SEPARATEUR) == OK)
							num_rec++;		/* element à priori correct, on le comptabilise */
					}
				}
#else
#ifdef IMPL_LIST
				// On s'inspire fortement de la partie tableaux déjà rédigée 
				SingleLinkedListElem temp;
				if (lire_champ_suivant(buffer, &idx, temp.pers.nom, MAX_NOM, SEPARATEUR) == OK)
				{
					idx++;							/* on saute le separateur */
					if (lire_champ_suivant(buffer, &idx, temp.pers.prenom, MAX_NOM, SEPARATEUR) == OK)
					{
						idx++;
						if (lire_champ_suivant(buffer, &idx, temp.pers.tel, MAX_TEL, SEPARATEUR) == OK) {
							InsertElementAt(rep->liste, num_rec, temp.pers);
							num_rec++;		/* element à priori correct, on le comptabilise */
						}
					}
				}								
#endif			
#endif			


			}

		}
		rep->nb_elts = num_rec;
		fclose(fic_rep);
		return(OK);
	}


} /* fin charger */