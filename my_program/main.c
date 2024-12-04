/** ****************************************************************************************************************************************
 *
 * @file       main.c
 *
 * @brief      Main file of the my_program program
 *
 * @author     marcin s - (Created on: 2024-12-02 - 12:11:12) 
 *
 * @note       Copyright (C) 2024 marcin s <marcin@s.com>
 *
 *                This program is free software; you can redistribute it and/or modify
 *                it under the terms of the GNU General Public License as published by
 *                the Free Software Foundation; either version 2 of the License, or
 *                (at your option) any later version.
 *
 *                This program is distributed in the hope that it will be useful,
 *                but WITHOUT ANY WARRANTY; without even the implied warranty of
 *                MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *                GNU General Public License for more details.
 *
 *                You should have received a copy of the GNU General Public License
 *                along with this program; if not, write to the Free Software
 *                Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************************************************************************************/

#include <oc_stdio.h>
#include "Logic.h"
#include "komunikacja.h"

//==========================================================================================================================================
/**
 * The main entry of the application
 *
 * @param Argc      Argument counter
 * @param Argv      Arguments array
 *
 * @return result
 */
//==========================================================================================================================================

    Stan stan = {
      {
          {'O', '.', 'O', '.', 'O', '.', 'O', '.'},
          {'.', 'O', '.', 'O', '.', 'O', '.', 'O'},
          {'O', '.', 'O', '.', 'O', '.', 'O', '.'},
          {'.', '.', '.', '.', '.', '.', '.', '.'},
          {'.', '.', '.', '.', '.', '.', '.', '.'},
          {'.', 'X', '.', 'X', '.', 'X', '.', 'X'},
          {'X', '.', 'X', '.', 'X', '.', 'X', '.'},
          {'.', 'X', '.', 'X', '.', 'X', '.', 'X'}
      },
      GRACZ_X,  // Gracz X zaczyna grę
          -1,
          -1,
          true
      };

 void getState(void* data, size_t dataSize)
 {
     refreshBoard((char*)data , &stan);
 }

int main( int Argc , char ** Argv )
{

    printf("Czy host 1- tak  0- nie");
    int a;
    scanf("%d", &a);

    if(a == 1)
    {
        stan.gracz = GRACZ_X;
        stan.isActive = true;
    }
    else
    {
        stan.gracz = GRACZ_O;
        stan.isActive = false;
    }

      startUart();

      wypisz_plansze(&stan);

      while (true)
      {


          // Sprawdzamy, czy z pola (2, 1) można wykonać ruch dla gracza 'X'

          if (!stan.isActive)
            {
              printf("Czekaj");
                readState(getState);
            }

          wypisz_plansze(&stan);

          int x, y;

          printf("Gracz %c :\n", stan.gracz);

          scanf("%d", &x);
          scanf("%d", &y);

          ruch(x, y, &stan);
          printf("Zaznaczone pole (%d, %d).\n", stan.x, stan.y);

          if (!stan.isActive)
            {
                char* tosend = convertTab(stan.plansza);
                sendState(tosend, sizeof(tosend));
            }
      }

      return 0;
}
