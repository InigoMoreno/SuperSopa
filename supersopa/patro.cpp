#include "patro.h"
#include <iostream>
#include <algorithm>

int costat[8][2]={{0,1}, {0,-1},
                  {1,0},{-1,0},
                 {1,-1},{-1,1},{-1,-1},{1,1}};

Patro::Patro()
{
}

Patro::Patro(vector<string> dictionary, vector<vector<char> > board)
{
    this->_board =  vector<vector<char>>( board.size()+2 , vector<char> ( board.size()+2 , '-') );
    for(int i =0; i<board.size();++i){
        for(int j=0; j<board.size(); ++j ){
            _board[i+1][j+1]= board[i][j];
        }
    }
    initDictionary(dictionary);

}


void Patro::initDictionary(vector<string> dictionary)
{
    //sort(dictionary.begin(),dictionary.end());
    //indirectament al crear l'arbre ja ho estarem ordenant en temps O(#lletres)
    this->arrel.caracter = '+';
    arrel.acaba= false;
    arrel.fills.clear();
    for(string paraula : dictionary){
        cerr << paraula << endl;
        insereix(paraula,&this->arrel);
        cerr << "Ok" << endl;
    }
    cerr << "inicialitzacio del diccionary Ok!" <<endl;
}

//envoltem el board amb el signe -
void Patro::initBoard(vector<vector<char> > board)
{
    this->_board =  vector<vector<char>>( board.size()+2 , vector<char> ( board.size()+2 , '-') );
     for(int i =0; i<board.size();++i){
         for(int j=0; j<board.size(); ++j ){
             _board[i+1][j+1]= board[i][j];
         }
     }
}
//cost = NxNx cost(cercapatro)
void Patro::solve()
{
    for(int i =0; i<_board.size()-2;++i){
        for(int j=0; j<_board.size()-2; ++j ){
            cercapatro(i+1,j+1,arrel.fills[_board[i+1][j+1]]);
        }
    }
}

//O(n) = 1 + 8*O(n-1)
//cost en cas pitjor
void Patro::cercapatro(int i, int j, Patro::arbre* node){
    if (node != NULL){
        if(node->acaba)guardaidestrueix("",node);
        for(int k=0; k<8; ++k){
            cercapatro( i+costat[k][0] , j+costat[k][1] ,node->fills[ _board[i+costat[k][0]][j+costat[k][1]] ]);
        }
    }
}

void Patro::guardaidestrueix(string paraula, Patro::arbre* node){
    (node->utilitzen)--;
    for(pair<const char, Patro::arbre*> fills : node->fills){
        if(fills.second->utilitzen == 0) node->fills[fills.first]= NULL;
    }
    if (node->caracter == '+'){
        this->_solucio.push_back(paraula);
    } else {
        guardaidestrueix(node->caracter+paraula, node->pare);
    }

}

void Patro::printSolution()
{
    for(string sol : this->_solucio){
        cout << sol << endl;
    }
}

void Patro::insereix(string paraula, Patro::arbre* node)
{
    if (paraula.size() > 0) {
        char inici = paraula.front();
        paraula.erase(0,1);
        auto it = node->fills.find(inici);
        if ( it == node->fills.end() ){
           //el node no esta creat
           arbre* nou = new arbre();
           nou->caracter = inici;
           nou->pare = node;
           node->fills[inici] = nou;
           it = node->fills.find(inici);
        }
        arbre* fill = it->second;
        insereix(paraula,fill);
        (node->utilitzen)++;
    } else node->acaba = true;
}
