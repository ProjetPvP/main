#include <stdlib.h>
#include <SDL/SDL.h>
#include <stdbool.h>
#define HAUTEUR 400
#define LARGEUR 600
#define LARGEURHEROSPIXEL 24
#define HAUTEURHEROSPIXEL 30
#define LARGEURMINE 30
#define prout 1

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
	for(i=0; i<hauteur; i++)
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
      SDL_Surface * mine = NULL;

      SDL_Rect positionHeros;
      SDL_Rect positionMap;
      SDL_Rect positionMine;

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
                        matrice->ecran[i-40][j-50] = 'M';
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
      SDL_BlitSurface(myMap, NULL, ecran, &positionMap);
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
                  if (matrice->ecran[i][j] == 'M')
                  {
                        positionMine.x = j;
                        positionMine.y = i;
                        SDL_BlitSurface(mine, NULL, ecran, &positionMine);
                  }
            }
      }
      SDL_Flip(ecran);
}

//==========================================================//
//                      takedamage                          //
//==========================================================//


bool takedamage(t_ecran_de_jeu matrice, int direction, t_pos positionHeros, int deplacement)
{
      if (direction == GAUCHE)
      {
            if (positionHeros.colonne-1 == 'm' )
            exit(EXIT_SUCCESS);
      }
      else if (direction == DROITE)
      {
            if (positionHeros.colonne+1 == 'm' )
            exit(EXIT_SUCCESS);
      }
      else if (direction == HAUT)
      {
            if (positionHeros.ligne-1 == 'm' )
            exit(EXIT_SUCCESS);
      }
      else if (direction == BAS)
      {
            if (positionHeros.ligne+1 == 'm' )
            exit(EXIT_SUCCESS);
      }
      return false;
}
//==========================================================//
//                      Hitbox                              //
//==========================================================//

void hitbox(t_ecran_de_jeu matrice)
{
      if(matrice->positionHeros.colonne<1)
      {
            if (matrice->positionHeros.ligne<1)
            {
                  for(int i=matrice->positionHeros.ligne;i<matrice->positionHeros.ligne+HAUTEURHEROSPIXEL; i++)
                  {
                        matrice->ecran[i][matrice->positionHeros.colonne] = 'h';
                  }
            }
      }
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
      else if (direction == DROITE)
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
      else if (direction == HAUT)
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
      else if (direction == BAS)
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
      takedamage(matrice, direction, positionHeros, deplacement);
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
                        for(int i=0; i<HAUTEURHEROSPIXEL; i++)
                        {
                              matrice->ecran[matrice->positionHeros.ligne+i+1][matrice->positionHeros.colonne] = 'h';
                              matrice->ecran[matrice->positionHeros.ligne+i+1][matrice->positionHeros.colonne+LARGEURHEROSPIXEL] = 'h';
                        }
                        for(int i=0; i<LARGEURHEROSPIXEL; i++)
                        {
                              matrice->ecran[matrice->positionHeros.ligne][matrice->positionHeros.colonne+i+1] = 'h';
                              matrice->ecran[matrice->positionHeros.ligne+HAUTEURHEROSPIXEL][matrice->positionHeros.colonne+i+1] = 'h';
                        }
                  }
                  else
                  {
                        if(matrice->ecran[i][j] != 'M')
                        {
                              matrice->ecran[i][j] = ' ';
                        }
                        else
                        {
                              for(int i=0; i<LARGEURMINE; i++)
                              {
                                    matrice->ecran[positionMine.x][matrice->positionHeros.colonne+i+1] = 'm';
                                    matrice->ecran[matrice->positionHeros.ligne+HAUTEURHEROSPIXEL][matrice->positionHeros.colonne+i+1] = 'm';
                              }
                        }
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
      fprintf(stderr,"c'est bon initmatrice");
      int continuer = 1;
       if (SDL_INIT_VIDEO == -1)
      {
                  fprintf(stderr, "erreur d'initialisation : %s\n", SDL_GetError());
                  exit(EXIT_FAILURE);
      }
      fprintf(stderr,"c'est bon sdl_init");

      ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);

      fprintf(stderr,"c'est bon setvideomode");

      if (SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255) == 0))
      {
            fprintf(stderr,"c'bon fillrect");
      }

      fprintf(stderr, "pas de problème");
      Heros = SDL_LoadBMP("link.bmp");
      SDL_SetColorKey(Heros, SDL_SRCCOLORKEY, SDL_MapRGB(Heros->format, 255, 0, 0));
      myMap = SDL_LoadBMP("sol4.bmp");
      mine = SDL_LoadBMP("nvMine.bmp");
      SDL_SetColorKey(mine, SDL_SRCCOLORKEY, SDL_MapRGB(mine->format, 255, 0, 0));
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
                  }break;
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
