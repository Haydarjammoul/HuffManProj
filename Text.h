#pragma once
#include <cstddef>
#include <cstddef>
#include <vector>
#include <queue>
#include <string>
#include <iostream>
using namespace std;
class Text
{
    struct noeud {
        unsigned int depth;
        char sdonnee; // caractere a coder
        int sfreq; // frequence du caractere
        noeud *sgauche; // f i l s gauche
        noeud *sdroit; // f i l s droit
        noeud(char d =char(0), int f = 0) {
            sgauche = NULL;
            sdroit = NULL;
            this->sdonnee = d;
            this->sfreq = f;
            depth = 0;
        }
    };
    struct compare {
        bool operator () (noeud *gauche, noeud *droit) {
            return gauche->sfreq > droit->sfreq;
        }
    };
   
    string txt;
    //int* count;
    priority_queue <noeud *, vector<noeud*>, compare> d_queue;
    noeud count[6];
    noeud* head;
    vector<bool> code[6];

    //void adjust_depth(noeud*);
    //bool isLeaf(noeud*);
    void assign_code(noeud*, vector<bool>*);

public:
    void Huffman_Tree_constructor();
    Text(string);
   // ~Text() {  delete count; delete code; }
};

