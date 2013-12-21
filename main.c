#include <stdlib.h>
#include <SDL/SDL.h>
#include <stdbool.h>
#define HAUTEUR 400
#define LARGEUR 600
#define LARGEURHEROSPIXEL 24
#define HAUTEURHEROSPIXEL 30

enum {HAUT, BAS, GAUCHE, DROITE};
char tableau_ecran[HAUTEUR][LARGEUR];

struct s_pos{int ligne; int colonne;};
typedef struct s_pos t_pos;

struct s_ecran_de_jeu{char** ecran; int hauteur; int largeur; t_pos positionHeros;};     //creation de la structure qui va former l'écran
typedef struct s_ecran_de_jeu * t_ecran_de_jeu;



//==========================================================//
//                     malloc matrice                       //
//==========================================================//


t_ecran_de_jeu create_ecran_de_jeu(int hauteur, int largeur, int posHerosX, int posHerosY)                              //malloc de la matrice
{
	t_ecran_de_jeu matrice = (t_ecran_de_jeu)malloc (sizeof(struct s_ecran_de_jeu));
	matrice->ecran = (char**)malloc(hauteur*sizeof(char*));
	int i;
	for(i=0; i<largeur; i++)
	{
		matrice->ecran[i]=(char*)malloc(largeur*sizeof(char));
	}
	matrice->hauteur = hauteur;
	matrice->largeur = largeur;
	matrice->positionHeros.ligne = posHerosY;
	matrice->positionHeros.colonne = posHerosX;
	fprintf(stderr,"largeur[%d]\n",i);
	fprintf(stderr,"hauteur[%d]",hauteur);
	return matrice;
}


//==========================================================//
//                   variables globales                     //
//==========================================================//

      SDL_Event event;
      SDL_Surface * ecran = NULL;
      SDL_Surface * Heros = NULL;
      SDL_Surface * myMap = NULL;

      SDL_Rect positionHeros;
      SDL_Rect positionMap;

//==========================================================//
//                     initMatriceVide                      //
//==========================================================//

void initMatrice(t_ecran_de_jeu matrice)                                                     //initialisation de la matrice
{
      for (int i=0; i<HAUTEUR; i++)
      {
            for (int j = 0; j<LARGEUR; j++)
            {
                  if (positionHeros.x == j && positionHeros.y == i)
                  {
                        matrice->ecran[i][j] = 'H';
                  }
                  else
                  {
                  matrice->ecran[i][j] = ' ';
                  }
            }
      }
}

//==========================================================//
//                    Lecture matrice                       //
//==========================================================//


void LectureMatrice(t_ecran_de_jeu matrice, SDL_Surface* ecran)
{
      SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
      //SDL_BlitSurface(myMap, NULL, ecran, &positionMap);
      for(int i = 0; i<HAUTEUR; i++)
      {
            for (int j = 0; j<LARGEUR; j++)
            {
                  if (matrice->ecran[i][j] == 'H')
                  {
                        positionHeros.x = j;
                        positionHeros.y = i;
                        SDL_BlitSurface(Heros, NULL, ecran, &positionHeros);
                  }
            }
      }
      SDL_Flip(ecran);
}

//==========================================================//
//                      vérifier poussee                    //
//==========================================================//


bool verifierPoussee(t_ecran_de_jeu matrice, int direction, t_pos positionHeros, int deplacement)
{
      if (direction == GAUCHE)
      {
            if (positionHeros.colonne < 1+deplacement)
            {
                  return false;
            }
            else if (matrice->ecran[positionHeros.ligne][positionHeros.colonne-1] == ' ')
            {
                  return true;
            }
      }
      if (direction == DROITE)
      {
            if (positionHeros.colonne > LARGEUR-2-deplacement-LARGEURHEROSPIXEL)            // parce que la matrice est définie sur [0-(LARGEUR-1)][0-(HAUTEUR-1)]
            {                                                                               // LARGEURHEROSPIXEL pour pas que le heros dépasse l'écran
                  return false;
            }
            else if (matrice->ecran[positionHeros.ligne][positionHeros.colonne+1] == ' ')
            {
                  return true;
            }
      }
      if (direction == HAUT)
      {
            if (positionHeros.ligne < 1+deplacement)
            {
                  return false;
            }
            else if (matrice->ecran[positionHeros.ligne-1][positionHeros.colonne] == ' ')
            {
                  return true;
            }
      }
      if (direction == BAS)
      {
            if (positionHeros.ligne > HAUTEUR -2-deplacement-HAUTEURHEROSPIXEL)                 // HAUTEURHEROSPIXEL pour pas que le heros dépasse l'écran
            {
                  return false;
            }
            else if (matrice->ecran[positionHeros.ligne+1][positionHeros.colonne] == ' ')
            {
                  return true;
            }
      }
      return false;
}

void replacementHeros(t_ecran_de_jeu matrice, int direction, int nb)
{
      if (direction == BAS)
      {
            matrice->positionHeros.ligne += nb;
      }
      if (direction == HAUT)
      {
            matrice->positionHeros.ligne -= nb;
      }
      if (direction == GAUCHE)
      {
            matrice->positionHeros.colonne -= nb;
      }
      if (direction == DROITE)
      {
            matrice->positionHeros.colonne += nb;
      }
      for (int i=0; i<HAUTEUR; i++)
      {
            for (int j=0; j<LARGEUR; j++)
            {
                  if ((matrice->positionHeros.ligne == i) && (matrice->positionHeros.colonne == j))
                  {
                        matrice->ecran[i][j] = 'H';
                  }
                  else
                  {
                        matrice->ecran[i][j] = ' ';
                  }

            }
      }
}


//==========================================================//
//                            main                          //
//==========================================================//



int main ( int argc, char** argv )
{
      positionHeros.x = 200;
      positionHeros.y = 300;
      positionMap.x = 0;
      positionMap.y = 0;
      t_ecran_de_jeu matrice;
      matrice = create_ecran_de_jeu(HAUTEUR, LARGEUR, positionHeros.x, positionHeros.y);
      initMatrice(matrice);
      int continuer = 1;
       if (SDL_INIT_VIDEO == -1)
      {
                  fprintf(stderr, "erreur d'initialisatiion : %s\n", SDL_GetError());
                  exit(EXIT_FAILURE);
      }

      ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
      SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
      Heros = SDL_LoadBMP("link.bmp");
      myMap = SDL_LoadBMP("sol4.bmp");
      SDL_Flip(ecran);
      int nbDeplacement =  1;
      Uint8 *keystates = SDL_GetKeyState( NULL );
      while(continuer)
      {
            SDL_PollEvent(&event);
            switch (event.type)
            {
                  case SDL_QUIT : continuer = 0;break;
                  case SDL_KEYDOWN :
                  switch (event.key.keysym.sym)
                  {
                        case SDLK_ESCAPE : continuer = 0;break;
                        default : break;
                  }
            }

            if (keystates[SDLK_UP])                                     // les keystats permettent le déplacement en diagonal.
            {
                  if (verifierPoussee(matrice, HAUT,matrice->positionHeros, nbDeplacement))
                  {
                        replacementHeros(matrice, HAUT, nbDeplacement);
                  }
            }
            if (keystates[SDLK_DOWN])
            {
                  if (verifierPoussee(matrice, BAS, matrice->positionHeros, nbDeplacement))
                  {
                        replacementHeros(matrice, BAS, nbDeplacement);
                  }
            }
            if (keystates[SDLK_RIGHT])
            {
                  if (verifierPoussee(matrice, DROITE, matrice->positionHeros, nbDeplacement))
                  {
                        replacementHeros(matrice, DROITE, nbDeplacement);
                  }
            }
            if (keystates[SDLK_LEFT])
            {
                  if (verifierPoussee(matrice, GAUCHE, matrice->positionHeros, nbDeplacement))
                  {
                        replacementHeros(matrice, GAUCHE, nbDeplacement);
                  }
            }




            LectureMatrice(matrice, ecran); // Affiche la matrice telle qu'elle est
      }

    return 0;
}
