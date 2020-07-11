#include <iostream>

using namespace std;

int main()
{
  int w = 3;
  int h = 4;
  char spaces[h][w];
  //a is player
  int ah = 3;
  int aw = 2;
  //b is target
  int bh = 1;
  int bw = 1;
  char input = ' ';

  //initialization
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      spaces[i][j] = ' ';
    }
  }
  spaces[ah][aw] = 'A';
  spaces[bh][bw] = 'B';


  while (input != 'e')
  {
    //plot
    for (int i = 0; i < h; i++)
    {
      for (int j = 0; j < w; j++)
      {
        cout << spaces[i][j];
      }
      cout << endl;
    }

    while (input != 'w' && input != 'a' && input != 's' && input != 'd' && input != 'e')
    {
      cin >> input;
    }
    spaces[ah][aw] = ' ';


    if (input == 'w')
    {
       if (ah == 0)
       {
         ah = h;
       }
       ah--;
       spaces[ah][aw] = 'A';
    }
    else if (input == 'a')
    {
       if (aw == 0)
       {
         aw = w;
       }
       aw--;
       spaces[ah][aw] = 'A';
    }
    else if (input == 's')
    {
       ah++;
       if (ah == h)
       {
         ah = 0;
       }
       spaces[ah][aw] = 'A';
    }
    else if (input == 'd')
    {
       aw++;
       if (aw == w)
       {
         aw = 0;
       }
       spaces[ah][aw] = 'A';
    }
    else if (input == 'e')
    {
      break;
    }

    if (ah == bh && aw == bw)
    {
      break;
    }

    //reset
    input = ' ';
  }

  return 0;
}