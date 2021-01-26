/////////Allan and Abdoulaye Project///////////////////
/////////Théorie des langages ////////////////////////
/////////////////////////////////////////////////////


#include <stdio.h>  /* Pour les Entrers/Sorties */
#include <stdlib.h> /* Pour l'Allocation dynamique et autre */
#include <string.h> /* Pour les fonctions de manipulation de chaines */
#include <ctype.h>  /* Pour la fonction isdigit() */
#include <assert.h>


typedef struct token{
	int type;/* 0=operateur.  1=entier.  2=paranthese_ouvrante, 3=paranthese_fermante*/
	int data; //stocke l'entier correspondant ou le code ascii du caractère si ce n'est pas un entier
}token;;

//la liste de token
typedef struct List_token{
	token tok;
	struct List_token *suiv;
}*list_token;

//la structure qui répresente l'arbre du token
typedef struct noeud{
	token t;
	struct noeud* filsG;
	struct noeud* filsD;
}NOEUD;


//Structure de données représentant un arbre et une pile
//n.....: la valeur du maillon (i.e. ici un noeud pour l'arbre)
//suiv.....: un pointeur vers le prochain maillon(un maillon est composé du noeud et ses fils)
typedef struct arbre{
	NOEUD *n;
	struct arbre *suiv;
}*arbre_token,Arbre;

/**********************SIGNATURE DES FONCTIONS*************************/

///////////////////////Prototypes Pour les Arbres//////////////////////////////////////////////////////

int arbre_to_int(list_token l);//donne le resultat de l'expression

NOEUD *creerNoeud(token t); //permet de créer un noeud

NOEUD *creeFilsGauche(NOEUD *n,token t); //crée un fils gauche

NOEUD *creeFilsDroit(NOEUD *n,token t); //crée un fils droit

arbre_token remplirArbreToken(list_token l); //crée la forêt d'arbre à l'aide de la liste token

arbre_token empiler(arbre_token a,token t);//empile un token dans la pile d'arbre

Arbre* depiler(arbre_token *a); //retourne l'élement de tête de la pile

Arbre* sommet( arbre_token p );//le sommet de la pile

/////////////////Prototype Pour tous ce qui concerne le token et sa liste de token/////////////////////

list_token creerListToken(); //crée un liste de token

int isEmpty(list_token l); //verifie si la liste de token est vide 

int IsOperator ( list_token l ); // Verifie si une valeur est un opérateur 

int IsOperand ( list_token l );//Verifie si c'est un operande c'est à dire un nombre 

int IsParenthesis(list_token l); //verifie si c'est une parenthèse ouvrante ou fermante

list_token ajoute_token_fin(list_token l, token t); //ajoute un token à la fin de la liste

void affiche_liste_token(list_token l); //affiche la liste de token

char *string_sans_espace(char *chaine,char *string); //élémine les espaces dans une chaine

list_token string_to_token(char *string); //transforme une chaie en list_token

int nbr_occurence_char(char* string,char c); //return le nombre du caractère 'c' figurant dans la chaine

int expre_Arth_Bien_Forme(char* string); //verifie si l'expression arithmetique est bien formée

list_token ajout_token_debut(list_token l,token t); //ajoute un token au debut d'une liste

token pop_token_to_head(list_token *l); //cette fonction return l'élément de la tête de la liste(considéré comme une pile)

list_token transforme_list_token_to_postfixe_list(list_token l); //cette transforme la liste du token initialement en infix en postfix

list_token libere_liste(list_token l); //libération de la liste de token

list_token inverseList(list_token l);//inverse une liste

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/****************************FIN DE PROTOTYPES************************************/

////////////////////////FONCTION PRINCIPALE//////////////////////////////////////////////
int main(int argc,char** argv)
{
	char* chaine = malloc(sizeof(char)*50);
	
	printf("\t\t**********PROJET THEORIE DES LANGAGES***********\n\n\n");
	list_token l;
	l = creerListToken();
	
	if(argc < 2){printf("printf argument sur la ligne de commande manquant\n");exit(1);}
	
	//recuperation de l'expression sur la ligne de commande
	//en ignorant les espaces
	for(int i=1;i<argc;i++)
	{
		
		chaine = string_sans_espace(chaine,argv[i]);
	}
	
	//transformation de la chaine recupérer en liste token
	l = string_to_token(chaine);
	
	//Stop directement le programme si l'expression est incorrecte
	if(expre_Arth_Bien_Forme(chaine)!=0){
		int ret = expre_Arth_Bien_Forme(chaine);
		if(ret == -1)
		{
			printf("\tL'expression :  ");
			affiche_liste_token(l);
			printf("\test l'exicalement Incorrecte!\n\n\n");
		}
		else if(ret == -2)
		{
			printf("\tL'expression :  ");
			affiche_liste_token(l);
			printf("\test Syntaxiquement Incorrecte!\n\n\n");
		}
	return 0;
	}
	//si l'expression est correcte
	printf("\tL'expression :  ");
	affiche_liste_token(l);
	printf("\test syntaxiquement et lexicalement correcte!\n\n\n");
	
	//transformation de la liste token initialement en infix
	//en liste de token postfix
	l = transforme_list_token_to_postfixe_list(l);
	//inverse la liste de token pour faciliter l'évaluation
	l = inverseList(l);
	printf("\tVoici l'expression postfixée correspondante : ");
	affiche_liste_token(l);
	printf("\n");
	
	//affichage du résultat
	printf("\n\tLe résultat de l'évaluation de l'expression arithmetique est : %d\n\n\n",arbre_to_int(l));
	
	libere_liste(l);
	free(chaine);

	return 0;
}

/////////////////////////FIN DE LA FONCTION PRINCIPALE//////////////////////////////////



list_token creerListToken(){return NULL;}


//verifie si la liste de token est vide 
int isEmpty(list_token l){
	
	if(l==NULL)
		return 1;//si la liste est vide 
		
	return 0;//si la liste n'est pas vide
}


// Verifie si une valeur est un opérateur 
int IsOperator ( list_token l )
{
   if( l->tok.type == 0 )
         return 1;  /* c'est un operateur */

   return 0;  /* ce n'est pas un operateur */
}


//Verifie si c'est un operande c'est à dire un nombre 
int IsOperand ( list_token l )
{
   if( l->tok.type == 1 )
         return 1;  /* c'est un operande */

   return 0;  /* ce n'est pas un operande */
}

//verifie si c'est une parenthèse ouvrante ou fermante
int IsParenthesis(list_token l)
{
	if(l->tok.type == 2)
	{
		return 2;
	}
	else if(l->tok.type == 3)
	{
		return 3;
	}
	return 0;
}

//ajoute un token à la fin de la liste
list_token ajoute_token_fin(list_token l, token t)
{
	list_token new = malloc(sizeof(list_token));
	new->tok.type = t.type;
	new->tok.data = t.data;
	new->suiv = NULL;
	if(isEmpty(l))
	{
		return new;
	}
	list_token debut = l;
	//on place le pointeur sur le dernier element de la liste
	while(l->suiv)
	{
		l = l->suiv;
	}
	l->suiv = new;
	return debut;
}

//affiche la liste de token
void affiche_liste_token(list_token l)
{
	
	if(isEmpty(l))
	{
		printf("La liste est vide \n");
		return;
	}
	while(l)
	{
		//si c'est un nombre
		if(IsOperand(l) == 1)
		{
			printf("%d",l->tok.data);
		}
		//si ce n'est pas un nombre
		else if(IsParenthesis(l) == 2 || IsOperator(l) == 1)
		{
			printf("%c",l->tok.data);
		}
		else
		{
			printf("%c",l->tok.data);
		}
		l = l->suiv;
	}
}


//élémine les espaces dans une chaine
char *string_sans_espace(char *chaine,char *string)
{
	for(int i=0;i<strlen(string);i++)
	{
		if(string[i] !=' ')
		{
			sprintf(chaine,"%s%c",chaine,string[i]);
		}
	}
	
	return chaine;
}


//transforme une chaie en list_token
list_token string_to_token(char *string){
	//debut
	list_token l=creerListToken();
	token t;
	//boucle permettant de parcourir toute la chaine
	for(int i=0;i<strlen(string);i++)
	{
		//si un caractère est un entier
		if(string[i] >= '0' && string[i] <= '9')
		{
			char *ch = malloc(sizeof(int));
			//tant que les caractères suivant un entier sont des entier aussi
			while(i<strlen(string) && (string[i] >= '0' && string[i] <='9'))
			{
				sprintf(ch,"%s%c",ch,string[i]);
				i++;
			}
			//ajoute l'entier converti dans la liste
			t.type =1; t.data = atoi(ch);
			l=ajoute_token_fin(l,t);
			i--;
			free(ch);
		}
		else if(string[i] == '(' )
		{
			//ajoute le code ascii du caractère '('
			t.type =2; t.data = (int)string[i];
			l=ajoute_token_fin(l,t);
		}
		else if( string[i] == ')')
		{
			//ajoute le code ascii du caractère ')'
			t.type =3; t.data = (int)string[i];
			l=ajoute_token_fin(l,t);
		}
		else if(string[i] == '+' || string[i] == '-' || string[i] == '*' || string[i] == '/')
		{
			//ajoute le code ascii du caractère correspondant
			t.type =0; t.data = (int)string[i];
			l=ajoute_token_fin(l,t);
		}
			
	}
	return l;
}

//le nombre fois qu'y figure un caractère
int nbr_occurence_char(char* string,char c)
{
	int nbr_occ=0;
		for(int i=0;i<strlen(string);i++)
		{
			if(string[i] == c){nbr_occ++;}
		}
	return nbr_occ;
}


//teste si une expression est lexicalement ou syntaxiquement correcte
int expre_Arth_Bien_Forme(char* string){
	
	//tous les symbole possible de notre expression
	char symbole[17] = "0123456789+-*/()";
	int i;
	/**Vérification lexicial , est ce que les symbole utilisés sont ceux autorisés**/

   for( i=0; i < strlen(string); i++ )
   {
      /* Verifie si le caractère y figure*/
		if(strchr(symbole,(int)string[i])==NULL)
		{
			return -1;
		}
   }
   
   /**Verification syntaxique**/
   
   //verification d'égalité entre '(' et ')'
   if(nbr_occurence_char(string,'(') != nbr_occurence_char(string,')'))
   {
	   return -2;
   }
   
   //verification de la présence d'un opérateur au debut d'un expresion ou à la fin
   for(i =0;i<strlen(string);i++)
   {
	   if( string[i] == '+' || string[i] == '/' || string[i] == '*' || string[i] == '-' )
	   {
		   if(i == 0 || i == strlen(string) - 1)
		   {
			   return -2;
		   }
		   else if(i != 0 || i != strlen(string) - 1)
		   {
			   if( string[i+1] == '+' || string[i+1] == '/' || string[i+1] == '*' || string[i+1] == '-' )
			   {
					return -2;
			   }
			   if( !( isdigit(string[i+1]) || string[i+1] == '(' ) )
				{
					return -2;  
				}
				if( !( isdigit(string[i-1]) || string[i-1] == '('  || string[i-1] ==')') )
				{
					return -2;  
				}
				
			}
		  }
		  /* pour le caractere '(' */
      if( string[i] == '(' )
      {
         // s'il est au debut
         if( i == 0 )
         {
            // il doit être succeder par un nombre
            if( ! (isdigit(string[i+1]) || string[i+1]=='(' ) )
            {
               return -2;
            }
         }
         //s'il est à la fin problème
         else if( i == strlen(string)-1 )
         {
            return -2;
         }
         //sinon
         else
         {
            if( ! ( (isdigit(string[i+1]) || string[i+1]=='(')  &&
                    (string[i-1]=='*' || string[i-1]=='-'|| string[i-1]=='/' ||
                     string[i-1]=='+' ||  string[i-1]=='(') ) )
            {
              return -2;
            }
         }
      }
      //si le caractère est ')'
      if( string[i] == ')' )
      {
         //s'il est à la fin 
         if( i == strlen(string) -1 )
         {
            //il doit être précéder par un entier ou un ')'
            if( ! ( isdigit(string[i-1]) || string[i-1]==')' ) )
            {
              return -2;
            }
         }
         //s'il est au debut erreur
         else if( i == 0 )
         {
            return -2;
         }
         //s'il est ailleur dans l'expression
         else
         {
            if( ! ( (isdigit(string[i-1]) || string[i-1]==')') &&
                    (string[i+1]=='*' || string[i+1]=='-'|| string[i+1]=='/' ||
                     string[i+1]=='+' || string[i+1]==')') ) )
            {
				return -2;
            }
         }
      }
      
	}
	return 0;
}

//cette fonction return l'élément de la tête de la liste(considéré comme une pile)
token pop_token_to_head(list_token *l)
{
	token t;
	t.type = (*l)->tok.type;
	t.data = (*l)->tok.data;
	(*l) = (*l)->suiv;
	return t;
}

//ajoute un token au debut d'une liste
list_token ajout_token_debut(list_token l,token t)
{  
	list_token new = malloc(sizeof(list_token));
	new->tok.type = t.type;
	new->tok.data = t.data;
	new->suiv = l;
	l = new;
	return new;
}


//cette transforme la liste du token initialement en infix en postfix
list_token transforme_list_token_to_postfixe_list(list_token l)
{
	list_token postfix,tmp;
	postfix = creerListToken();
	tmp = creerListToken();
	while(l!=NULL)
	{
		if(l->tok.type == 2 || l->tok.type == 0 )
		{
			tmp = ajout_token_debut(tmp,l->tok);
			l = l->suiv;
		}
		else if(l->tok.type == 1)
		{
			postfix = ajout_token_debut(postfix,l->tok);
			l = l->suiv;
		}
		else if(l->tok.type == 3)
		{
			l = l->suiv;
			//compteur permettant de connaitre le nombre de paranthèse ouvrante rencontré
			int compChar=0;
			if(tmp != NULL)
			{
				token d;
				while(compChar < 2 && tmp != NULL)
				{
					d = pop_token_to_head(&tmp);
					if(d.type == 2)
					{
						compChar++;
					}
					else
					{
						postfix = ajout_token_debut(postfix,d);
					}
				
				}
				if(compChar == 2)
				{
					tmp = ajout_token_debut(tmp,d);
				}
			}
				
			
			
		}
	}
	if(tmp != NULL)
	{
		token d;
		while(tmp != NULL)
		{
			d = pop_token_to_head(&tmp);
			postfix = ajout_token_debut(postfix,d);
				
		}
	}
	return postfix;
}

//fonction inverse
list_token inverseList(list_token l)
{
	list_token tmp=creerListToken();
	while(l!=NULL)
	{
		tmp = ajout_token_debut(tmp,l->tok);
		l = l->suiv;
	}
	
	return tmp;
}

//libere la liste de token
list_token libere_liste(list_token l)
{
	list_token tmp;
	while(l)
	{
		tmp = l->suiv;
		free(l);
		l = tmp;
	}
	return NULL;
}


NOEUD *creerNoeud(token t)
{
	NOEUD *s = (NOEUD*)malloc(sizeof(NOEUD));
	s->filsG = NULL;
	s->filsD = NULL;
	s->t = t;
	
return s;
}


NOEUD *creeFilsGauche(NOEUD *n,token t)
{
	NOEUD *nouveau = creerNoeud(t);
	n->filsG = nouveau;
	return nouveau;
}

NOEUD *creeFilsDroit(NOEUD *n,token t)
{
	NOEUD *nouveau = creerNoeud(t);
	n->filsD = nouveau;
	return nouveau;
}

arbre_token empiler(arbre_token a,token t)
{
	// Création d'un noeud de l'arbre
	NOEUD *Noeud = creerNoeud(t);
	assert( Noeud != NULL );
	// Création du maillon
	Arbre* m = (Arbre*) malloc( sizeof(arbre_token) );
	assert( m != NULL );
	m->n = Noeud; 
	m->suiv = a;
	// Affectation du noeud d'arbre en valeur du maillon
	a = m;
	return a;
}

Arbre* depiler(arbre_token *a)
{
		if ( *a != NULL ){
		Arbre* tete = *a;
		(*a) = (*a)->suiv;
		//printf("tete = %d\n",tete->n->t.data);
		return tete;
	}

	return NULL;
}

Arbre* sommet( arbre_token p ){

	if ( p != NULL )
		return p;

	return NULL;
}


//fonction qui évalue l'expression arithmetique
int arbre_to_int(list_token l)
{
	token resultat,t1,t2; 
	resultat.data = 0; resultat.type = 1;
	list_token tmp=NULL;
	
	if(isEmpty(l)==1){printf("liste de token vide\n");exit(-1);}
	
	while(l!=NULL)
	{
		if(IsOperand(l) == 1)
		{
			tmp = ajout_token_debut(tmp,l->tok);
		}
		else if(IsOperator(l)==1)
		{		
				t1 = pop_token_to_head(&tmp);
				t2 = pop_token_to_head(&tmp);
				
				if(l->tok.data == 43)
				{
					resultat.data = t2.data + t1.data;
					tmp = ajout_token_debut(tmp,resultat);
				}
				else if(l->tok.data == 45)
				{
					resultat.data = t2.data - t1.data;
					tmp = ajout_token_debut(tmp,resultat);
				}
				else if(l->tok.data == 42)
				{
					resultat.data = t2.data * t1.data;
					tmp = ajout_token_debut(tmp,resultat);
				}
				else if(l->tok.data == 47)
				{
					resultat.data = t2.data / t1.data;
					tmp = ajout_token_debut(tmp,resultat);
				}
		}
	l = l->suiv;
	}
	return resultat.data;
	
}

/*
Arbre* remplirArbreToken(list_token l)
{
	printf("je rentre\n");
	arbre_token p=NULL;
	if(l == NULL){printf("liste de token vide\n");exit(-1);}
	
	while(l!=NULL)
	{
		printf("je suis dans la while\n");
		if(l->tok.type == 1)
		{
			printf("je suis dans if1\n");
			printf("%d\n",l->tok.data);
			p=empiler(p,l->tok);
			printf("%d de la pile\n",p->n->t.data);
			l=l->suiv;
		}
		else if(l->tok.type == 0 )
		{
			printf("je suis dans elseif\n");
			//depilage des 2 premier élément de la pile d'arbre
			Arbre* maillon1 = depiler(&p);
			printf("%d\n",maillon1->n->t.data);
			Arbre* maillon2 = depiler(&p);
			printf("%d\n",maillon2->n->t.data);
			//création d'un élément de type arbre pour l'operateur
			Arbre* maillon=(Arbre*)(sizeof(Arbre));
			maillon->suiv = NULL;
			printf("maillon\n");
			NOEUD* Noeud =(NOEUD*)(sizeof(NOEUD));
			Noeud->t = l->tok;
			maillon->n = Noeud;
			printf("maillon\n");
			//récupération des fils du maillon créer
			Noeud->filsG = maillon2->n;
			Noeud->filsD = maillon1->n;
			
			//Empilement du maillon
			Arbre* teteArbre = sommet(p);
			
			if ( teteArbre == NULL )
			{
					p = maillon;
			} else 
			{
				maillon->suiv = teteArbre;
				p = maillon;
			}
		l= l->suiv;	
		free(maillon1);				
		free(maillon2);
		}
	}
	return p;
	
}
* */





