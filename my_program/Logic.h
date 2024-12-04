#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <oc_math.h>

#define N 8  // rozmiar planszy 8x8
#define GRACZ_X 'X' //0
#define GRACZ_O 'O' //1
#define PUSTE '.'
#define DAMKA_X 'D'  // Damka gracza X
#define DAMKA_O 'd'  // Damka gracza O

// Struktura przechowująca stan gry
typedef struct  {
    char plansza[N][N];
    char gracz;
    int x;
    int y;
    bool isActive;
}Stan ;


typedef struct
{
    uint8_t Color : 1;
    uint8_t Empty : 1;
    uint8_t Damka : 1;
    uint8_t Active : 1;
} FieldState_t;// __attribute__((packed));

typedef struct  {
    FieldState_t plansza[N][N];
    bool isBlack;
    bool isActive;
} State ;

// Funkcja sprawdzająca, czy gracz ma obowiązkowe bicie na planszy
bool czy_gracz_ma_obowiazkowe_bicie(Stan* stan) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (stan->plansza[i][j] == stan->gracz || stan->plansza[i][j] == (stan->gracz == GRACZ_X ? DAMKA_X : DAMKA_O)) {
                // Sprawdzamy bicia dla pionków gracza X i O
                if (stan->gracz == GRACZ_X || stan->gracz == GRACZ_O) {
                    if (i - 2 >= 0 && j - 2 >= 0 && stan->plansza[i - 1][j - 1] == (stan->gracz == GRACZ_O ? GRACZ_X : GRACZ_O) && stan->plansza[i - 2][j - 2] == PUSTE) return true;
                    if (i - 2 >= 0 && j + 2 < N && stan->plansza[i - 1][j + 1] == (stan->gracz == GRACZ_O ? GRACZ_X : GRACZ_O) && stan->plansza[i - 2][j + 2] == PUSTE) return true;
                    if (i + 2 < N && j - 2 >= 0 && stan->plansza[i + 1][j - 1] == (stan->gracz == GRACZ_O ? GRACZ_X : GRACZ_X) && stan->plansza[i + 2][j - 2] == PUSTE) return true;
                    if (i + 2 < N && j + 2 < N && stan->plansza[i + 1][j + 1] == (stan->gracz == GRACZ_O ? GRACZ_X : GRACZ_X) && stan->plansza[i + 2][j + 2] == PUSTE) return true;
                }
            }
        }
    }
    return false;
}

// Funkcja sprawdzająca, czy można wykonać ruch z pola (x, y) dla danego gracza
bool czy_mozna_ruch(int x, int y, Stan* stan) {
    if (x < 0 || x >= N || y < 0 || y >= N) return false;  // Sprawdzamy, czy współrzędne są w granicach planszy
    if (stan->plansza[x][y] != stan->gracz && stan->plansza[x][y] != (stan->gracz == GRACZ_X ? DAMKA_X : DAMKA_O)) return false;  // Sprawdzamy, czy na danym polu znajduje się pionek gracza

    // Sprawdzamy, czy gracz ma obowiązkowe bicie
    bool ma_obowiazkowe_bicie = czy_gracz_ma_obowiazkowe_bicie(stan);

    // Jeśli gracz ma obowiązkowe bicie, ruch musi być biciem
    if (ma_obowiazkowe_bicie) {
        if (stan->gracz == GRACZ_X || stan->gracz == GRACZ_O) {
            if (x - 2 >= 0 && (y - 2 >= 0 || y + 2 < N)) {
                if ((stan->plansza[x - 1][y - 1] == GRACZ_O || stan->plansza[x - 1][y + 1] == GRACZ_O) && stan->plansza[x - 2][y - 2] == PUSTE) return true;
            }
        }
        return false;
    }

    // Sprawdzamy zwykłe ruchy (bez bicia)
    if (stan->gracz == GRACZ_X || stan->gracz == GRACZ_O) {
        if (x - 1 >= 0 && (y - 1 >= 0 || y + 1 < N)) {
            if (stan->plansza[x - 1][y - 1] == PUSTE || stan->plansza[x - 1][y + 1] == PUSTE) {
                return true;
            }
        }
    }
    else if (stan->gracz == GRACZ_O || stan->gracz == GRACZ_X) {
        if (x + 1 < N && (y - 1 >= 0 || y + 1 < N)) {
            if (stan->plansza[x + 1][y - 1] == PUSTE || stan->plansza[x + 1][y + 1] == PUSTE) {
                return true;
            }
        }
    }

    return false;  // Ruch jest niedozwolony
}

// Funkcja sprawdzająca, czy można wykonać ruch z pola (x1, y1) na pole (x2, y2)
bool czy_mozna_ruch_na_pole(int x1, int y1, int x2, int y2, Stan* stan) {
    // Sprawdzamy, czy współrzędne są w granicach planszy
    if (x1 < 0 || x1 >= N || y1 < 0 || y1 >= N || x2 < 0 || x2 >= N || y2 < 0 || y2 >= N) {
        return false;
    }

    // Sprawdzamy, czy na polu początkowym jest pionek danego gracza
    if (stan->plansza[x1][y1] != stan->gracz && stan->plansza[x1][y1] != (stan->gracz == GRACZ_X ? DAMKA_X : DAMKA_O)) {
        return false;
    }

    // Sprawdzamy, czy na polu docelowym jest puste miejsce
    if (stan->plansza[x2][y2] != PUSTE) {
        return false;
    }

    // Sprawdzamy, czy gracz ma obowiązkowe bicie
    bool ma_obowiazkowe_bicie = czy_gracz_ma_obowiazkowe_bicie(stan);

    // Jeśli gracz ma obowiązkowe bicie, ruch musi być biciem
    if (ma_obowiazkowe_bicie) {
        if (stan->gracz == GRACZ_X || stan->gracz == GRACZ_O) {
            if (x2 == x1 - 2 && (y2 == y1 - 2 || y2 == y1 + 2)) {
                if (stan->plansza[x1 - 1][y1 - 1] == GRACZ_O || stan->plansza[x1 - 1][y1 + 1] == GRACZ_O) {
                    return true;
                }
            }
        }
        return false;
    }

    // Sprawdzamy zwykłe ruchy (bez bicia)
    if (stan->gracz == GRACZ_X || stan->gracz == GRACZ_O) {
        if (x2 == x1 - 1 && (y2 == y1 - 1 || y2 == y1 + 1)) {
            return true;
        }
    }
    else if (stan->gracz == GRACZ_O || stan->gracz == GRACZ_X) {
        if (x2 == x1 + 1 && (y2 == y1 - 1 || y2 == y1 + 1)) {
            return true;
        }
    }

    return false;  // Ruch jest niedozwolony
}

// Funkcja wykonująca ruch danego gracza
bool wykonaj_ruch(Stan* stan, int x1, int y1, int x2, int y2) {
    // Sprawdzamy, czy ruch jest dozwolony
    if (!czy_mozna_ruch_na_pole(x1, y1, x2, y2, stan)) {
        printf("Ruch z pola (%d, %d) na pole (%d, %d) jest niedozwolony!\n", x1, y1, x2, y2);
        return false;  // Ruch nie jest dozwolony
    }

    // Wykonaj ruch (przesuwamy pionek)
    stan->plansza[x2][y2] = stan->plansza[x1][y1];  // Przenosimy pionek na nowe pole
    stan->plansza[x1][y1] = PUSTE;            // Zostawiamy stare pole puste

    // Jeśli ruch był biciem, usuwamy zbity pionek
    if (abs(x2 - x1) == 2) {
        int x_srodka = (x1 + x2) / 2;
        int y_srodka = (y1 + y2) / 2;
        stan->plansza[x_srodka][y_srodka] = PUSTE;  // Zbijamy pionek
    }

    // Sprawdzamy, czy pionek gracza dotarł do ostatniego rzędu i zamieniamy go w damkę
    if (stan->gracz == GRACZ_X && x2 == 0) {
        stan->plansza[x2][y2] = DAMKA_X;
    }
    if (stan->gracz == GRACZ_O && x2 == N - 1) {
        stan->plansza[x2][y2] = DAMKA_O;
    }

    return true;  // Ruch został wykonany
}

void ruch(int x, int y, Stan* stan)
{
    if (czy_mozna_ruch(x, y, stan))
    {
        stan->x = x;
        stan->y = y;
    }
    else if (stan->x != -1 && stan->y != -1)
    {
        if (wykonaj_ruch(stan, stan->x, stan->y, x, y))
        {
            stan->isActive = false;
            stan->x = -1;
            stan->y = -1;
        }
    }
}

void mapowanie(const Stan* stan, State* state) {
    state->isBlack = stan->gracz == GRACZ_X;
    state->isActive = stan->isActive;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            FieldState_t field = {0};

            char pole = stan->plansza[i][j];

            // Mapowanie kolorów:
            // Gracz 'X' -> Color 0
            // Gracz 'O' -> Color 1
            if (pole == 'X') {
                field.Color = 0;  // Gracz X
            } else if (pole == 'O') {
                field.Color = 1;  // Gracz O
            } else {
                field.Color = 0;  // Brak gracza (puste pole)
            }

            // Sprawdzanie, czy pole jest puste
            field.Empty = (pole == '.' ? 1 : 0);

            field.Damka = 0;

            // Zakładając, że damka jest oznaczana jako 'D' lub 'd' (np. 'DX' dla damki gracza X)
            if ((pole == 'X' || pole == 'O') &&
                (stan->plansza[i][j] == 'D' || stan->plansza[i][j] == 'd')) {
                field.Damka = 1;  // Damka
            }

            // Wartość Active
            field.Active = 0;

            // Mapowanie pola do nowej struktury
            state->plansza[i][j] = field;
        }
    }

    if(stan->x != -1 && stan->y != -1)
    {
        state->plansza[stan->x][stan->y].Active = 1;
    }
}

// Funkcja pomocnicza do wyświetlania planszy
void wypisz_plansze(Stan* stan) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%c ", stan->plansza[i][j]);
        }
        printf("\n");
    }
}

// Funkcja kopiująca zawartość jednego obiektu Stan do drugiego
void kopiuj_stan(Stan* zrodlo, Stan* cel) {
    // Kopiowanie planszy
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cel->plansza[i][j] = zrodlo->plansza[i][j];
        }
    }

    cel->isActive = true;
}

char* convertTab(char tab [8][8])
{

    static char t[64];
    int index = 0;

    for (int i = 0; i < 8; ++i) {

        for (int j = 0; j < 8; ++j) {

            t[index] = tab[i][j];

            ++index;

        }

    }

    return t;
}


void refreshBoard(char* tab, Stan* cel)
{
    for (int i = 0; i <8; ++i) {
        for (int j = 0; j < 8; ++j) {
            cel->plansza[i][j] = tab[i * 8 + j];
        }
    }

    cel->isActive = true;
}
