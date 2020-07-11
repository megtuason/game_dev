#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main(){

    int l;
    int w;
    int n;

    cout << "Enter grid settings: " << endl;

    for (int i = 0; i < 3; i++){
        if (i == 0){
            cin >> l;
        } else if (i == 1){
            cin >> w;
        } else {
            cin >> n;
        }
    }

    if (l < 3 || w > 5 || n < 1 || n > 10){
        cout << "Invalid Input!" << endl;
    } else {
        int grid[l][w];
        cout << "Enter grid numbers:" << endl;
        for (int i = 0; i < l; i++){
            for (int j = 0; j < w; j++){
                cin >> grid[i][j];
            }
        }

        int target[2];
        int count = 0;
        while (count != n){
            for(int i = 0; i < 2; i++){
                if (i == 0){
                    cin >> target[0];
                }
                else if (i == 1){
                    cin >> target[1];
                }
            }
            cout << "TARGET: " << target[0] << target[1] << endl;
            int row = target[0];
            int col = target[1];
            
            if (grid[row][col] == 0){
                grid[row][col] = 1;
            } else{
                grid[row][col] = 0;
            }


            if (row != 0){
                if (grid[row-1][col] == 0){
                    grid[row-1][col] = 1;
                } else{
                    grid[row-1][col] = 0;
                }
            } else {
                int newrow = l-1;
                if (grid[newrow][col] == 0){
                    grid[newrow][col] = 1;
                } else{
                    grid[newrow][col] = 0;
                }
            }
            if (row != (l-1)){
                if (grid[row+1][col] == 0){
                    grid[row+1][col] = 1;
                } else{
                    grid[row+1][col] = 0;
                }
            } else {
                if (grid[0][col] == 0){
                    grid[0][col] = 1;
                } else{
                    grid[0][col] = 0;
                }
            }

            if (col != 0){
                if (grid[row][col-1] == 0){
                    grid[row][col-1] = 1;
                } else{
                    grid[row][col-1] = 0;
                }
            } else {
                int newcol = l-1;
                if (grid[row][newcol] == 0){
                    grid[row][newcol] = 1;
                } else{
                    grid[row][newcol] = 0;
                }
            }
            if (col != (l-1)){
                if (grid[row][col+1] == 0){
                    grid[row][col+1] = 1;
                } else{
                    grid[row][col+1] = 0;
                }
            } else {
                if (grid[row][0] == 0){
                    grid[row][0] = 1;
                } else{
                    grid[row][0] = 0;
                }
            }

            for (int i = 0; i < l; i++){
                for (int j = 0; j < w; j++){
                    cout << grid[i][j] << " ";
                }
                cout << endl;
            }
            count++;
        }
    }
    return 0;
}