#include <iostream>
#include <cstring>
using namespace std;

int main(){
    int N;
    cout << "Enter number of Grid Size: ";
    cin >> N;
    string grid[N][N];
    string input;
    cout << "Enter string input: ";
    cin >> input;
    int i = 0;
    bool help = true;
    while(help){
        for(int r=0; r < N; r++){
            for (int c=0; c < N; c++){
                if (i == input.length()){
                    help = false;
                    break;
                } else{
                    grid[r][c] = input[i];
                    i++;
                }
            }
        }
    }
    cout << "==========" <<endl;

    for(int r=0; r < N; r++){
        for (int c=0; c < N; c++){
            cout << grid[r][c];
        }
        cout << endl;
    }

    cout << "==========" <<endl;
    
    string word;
    cout << "Enter word: ";
    cin >> word;
    
    string newgrid[N][N];
    for (int r = 0; r < N; r++){
        for (int c = 0; c < N; c++){
            newgrid[r][c] = grid[r][c];
        }
    }

    i = 0;
    bool found = false;
    for (int r = 0; r < N; r++){
        for (int c = 0; c < N; c++){
            string s(1, word[i]);
            if(grid[r][c] == s){
                i+= 1;
                //check up
                if (r != 0){
                    int curr = r-1;
                    while(i < word.length()){
                        if(curr < 0){
                            i = 1;
                            break;
                        }
                        string s(1, word[i]);
                        if (grid[curr][c] == s){
                            i+= 1;
                            curr -= 1;
                        }else{
                            i = 1;
                            break;
                        }
                    }
                    if(i == word.length()){
                        found = true;
                    }
                }
                //check down
                if (r != (N-1) && !found){
                    int curr = r+1;
                    while(i < word.length()){
                        if(curr == N){
                            i = 1;
                            break;
                        }
                        string s(1, word[i]); 
                        if (grid[curr][c] == s){
                            i+= 1;
                            curr += 1;
                        }else{
                            i = 1;
                            break;
                        }
                    }
                    if(i == word.length()){
                        found = true;
                    }
                }
                
                //check left
                if (c != 0 && !found){
                    int curr = c-1;
                    while(i < word.length()){
                        if (curr < 0){
                            i = 1;
                            break;
                        }
                        string s(1, word[i]);
                        if (grid[r][curr] == s){
                            i+= 1;
                            curr -= 1;
                        }else{
                            i = 1;
                            break;
                        }
                    }
                    if(i == word.length()){
                        found = true;
                    }
                }
                
                //check right
                if (c != (N-1) && !found){
                    int curr = c+1;
                    while(i < word.length()){
                        if (curr == N){
                            i = 1;
                            break;
                        }
                        string s(1, word[i]);
                        if (grid[r][curr] == s){
                            i+= 1;
                            curr += 1;
                        }else{
                            i = 1;
                            break;
                        }
                    }
                    if(i == word.length()){
                        found = true;
                    }
                }
                //check diag upleft
                if (r != 0 && c != 0 && !found){
                    int currR = r-1;
                    int currC = c-1;
                    while(i < word.length()){
                        if (currR < 0 || currC < 0){
                            i = 1;
                            break;
                        }
                        string s(1, word[i]);
                        if (grid[currR][currC] == s){
                            i+= 1;
                            currR -= 1;
                            currC -= 1;
                        }else{
                            i = 1;
                            break;
                        }
                    }
                    if(i == word.length()){
                        found = true;
                    }
                }

                //check diag upright
                if (r != 0 && c != (N-1) && !found){
                    int currR = r-1;
                    int currC = c+1;
                    while(i < word.length()){
                        if(currR < 0 || currC == N){
                            i = 1;
                            break;
                        }
                        string s(1, word[i]);
                        if (grid[currR][currC] == s){
                            i+= 1;
                            currR -= 1;
                            currC += 1;
                        }else{
                            i = 1;
                            break;
                        }
                    }
                    if(i == word.length()){
                        found = true;
                    }
                }
                //check diag lowleft
                if (r != (N-1) && c != 0 && !found){
                    int currR = r+1;
                    int currC = c-1;
                    while(i < word.length()){
                        if(currR == N || currC < 0){
                            i = 1; 
                            break;
                        }
                        string s(1, word[i]);
                        if (grid[currR][currC] == s){
                            i+= 1;
                            currR += 1;
                            currC -= 1;
                        }else{
                            i = 1;
                            break;
                        }
                    }
                    if(i == word.length()){
                        found = true;
                    }
                }
                
                //check diag lowright
                if (r != (N-1) && c != (N-1) && !found){
                    int currR = r+1;
                    int currC = c+1;
                    while(i < word.length()){
                        if(currR == N || currC == N){
                            i = 1;
                            break;
                        }
                        string s(1, word[i]);
                        if (grid[currR][currC] == s){
                            i+= 1;
                            currR += 1;
                            currC += 1;
                        }else{
                            i = 1;
                            break;
                        }
                    }
                    if(i == word.length()){
                        found = true;
                    }
                }
            }
        }
        
    }
    if(found == true){
        cout << "YAY" << endl;
    } else{
        cout << "SHEMS" << endl;
    }
    return 0;
}