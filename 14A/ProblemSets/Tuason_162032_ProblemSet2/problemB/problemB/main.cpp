//
//  main.cpp
//  problemB
//
//  Created by Meg Tuason on 19/09/2019.
//  Copyright © 2019 Meg Tuason. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

#define CPS 5

int main(int argc, const char * argv[]) {
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
    
    string word;
    cout << "Enter word: ";
    cin >> word;
    
    i = 0;
    bool found = false;
    string line;
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
                        line = "up";
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
                        line = "down";
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
                        line = "left";
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
                        line = "right";
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
                        line = "upleft";
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
                        line = "upright";
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
                        line = "lowleft";
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
                        line = "lowright";
                    }
                }
            }
        }
        
    }
    if(found == true){
        i = 0;
        for (int r = 0; r < N; r++){
            for (int c = 0; c < N; c++){
                string s(1, word[i]);
                if(grid[r][c] == s){
                    i+= 1;
                    grid[r][c] = ("&" + s);
                    if(line == "up"){
                        int curr = r-1;
                        while(i < word.length()){
                            string s(1, word[i]);
                            if (grid[curr][c] == s){
                                grid[curr][c] = ("&" + s);
                                i+= 1;
                                curr -= 1;
                            }
                        }
                    } else if (line == "down"){
                        int curr = r+1;
                        while(i < word.length()){
                            string s(1, word[i]);
                            if (grid[curr][c] == s){
                                grid[curr][c] = ("&" + s);
                                i+= 1;
                                curr += 1;
                            }
                        }
                        
                    } else if (line == "left"){
                        int curr = c-1;
                        while(i < word.length()){
                            string s(1, word[i]);
                            if (grid[r][curr] == s){
                                grid[curr][c] = ("&" + s);
                                i+= 1;
                                curr -= 1;
                            }
                        }
                    } else if (line == "right"){
                        int curr = c+1;
                        while(i < word.length()){
                            string s(1, word[i]);
                            if (grid[r][curr] == s){
                                grid[curr][c] = ("&" + s);
                                i+= 1;
                                curr += 1;
                            }
                        }
                        
                    } else if (line == "upright"){
                        int currR = r-1;
                        int currC = c+1;
                        while(i < word.length()){
                            string s(1, word[i]);
                            if (grid[currR][currC] == s){
                                grid[currR][currC] = ("&" + s);
                                i+= 1;
                                currR -= 1;
                                currC += 1;
                            }
                        }
                    } else if (line == "upleft"){
                        int currR = r-1;
                        int currC = c-1;
                        while(i < word.length()){
                            string s(1, word[i]);
                            if (grid[currR][currC] == s){
                                grid[currR][currC] = ("&" + s);
                                i+= 1;
                                currR -= 1;
                                currC -= 1;
                            }
                        }
                    } else if (line == "lowleft"){
                        int currR = r+1;
                        int currC = c-1;
                        while(i < word.length()){
                            string s(1, word[i]);
                            if (grid[currR][currC] == s){
                                grid[currR][currC] = ("&" + s);
                                i+= 1;
                                currR += 1;
                                currC -= 1;
                            }
                        }
                        
                    } else if (line == "lowright"){
                        int currR = r+1;
                        int currC = c+1;
                        while(i < word.length()){
                            string s(1, word[i]);
                            if (grid[currR][currC] == s){
                                grid[currR][currC] = ("&" + s);
                                i+= 1;
                                currR += 1;
                                currC += 1;
                            }
                        }
                    }
                }
            }
        }
        
    }
    string out = "";
    for(int r=0; r < N; r++){
        for (int c=0; c < N; c++){
            out += grid[r][c] + " ";
        }
        out += '\n';
    }
    
    sf::RenderWindow window(sf::VideoMode(1000,1000), "Problem B");
    sf::Font font;
    sf::Text whole[out.length()];
    sf::Text text;
    
    if (!font.loadFromFile("/Users/Meg/Desktop/school/fur/179.14/ProblemSets/Tuason_162032_ProblemSet2/problemB/problemB/res/fonts/FontFile.ttf")){
        cout << "help" << endl;
    }
    text.setFont(font);
    text.setCharacterSize(50);
//    text.setPosition(400, 400);
    
    string newout;
    float x = 400;
    float y = 400;
    for (int i = 0; i < out.length(); i++){
        string s(1, out[i]);
        if (s == "&"){
            text.setString(out[i+1]);
            text.setFillColor(sf::Color::Red);
            text.setPosition(x, y);
            x+=30;
            whole[i] = text;
            i = i + 1;
        }else if (out[i] == '\n'){
            text.setString(out[i]);
            text.setFillColor(sf::Color::White);
            x = 400;
            y+=100;
            text.setPosition(0, y);
            whole[i] = text;
        }else{
            text.setString(s);
            text.setFillColor(sf::Color::White);
            text.setPosition(x, y);
            x+=30;
            whole[i] = text;
        }
    }
    
    
    while(window.isOpen()){
        sf::Event event;
        while( window.pollEvent( event ) ){
            switch( event.type ){
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }
        window.clear();
        for(int i = 0; i < out.length();i++){
            window.draw(whole[i]);
        }
        window.display();
        
    }
    return 0;
}
