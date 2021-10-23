#include "Text.h"
#include <string>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

Text:: Text(string t) {
    txt = t;
    //count = new noeud(26);
    for (int i = 0; i < 6; i++) {
        count[i].sdonnee = char(i + 97);
    }
    cout << "no prb1" << endl;
    for (unsigned int i = 0; i < txt.size(); i++) {
        count[int(txt[i]) - 97].sfreq++;
    }
    cout << "no prb2" << endl;
    noeud* a;
    for (int i = 0; i < 6; i++) {
        a = &count[i];
        a->sdroit = NULL;
        a->sgauche = NULL;
        d_queue.push(a);
    }
    head = new noeud;
    cout << "no prb3" << endl;
}


void Text::assign_code(noeud* child, vector<bool>* b) {
    if (child == NULL) {
        cout << "brr" << endl;
        return;
    }
    
    if (child->sgauche == NULL && child->sdroit == NULL && child->sdonnee!=0)
    {
        cout << "wsolet" << endl;
        code[int(child->sdonnee) - 97]=*b;
        return;
    }
    else if(child->sgauche != NULL || child->sdroit != NULL) {
        //cout << "tyb" << endl;
        if (child->sgauche != NULL) {
            b->push_back(1);
            assign_code(child->sdroit, b);
            b->pop_back();
        }
        if(child->sgauche != NULL){
        b->push_back(0);
        assign_code(child->sgauche, b);
        }
    }
    return;
}
void Text::Huffman_Tree_constructor() {
    noeud* NodesOfLeafs;
    NodesOfLeafs = new noeud(26);
    noeud mix, mux;
    noeud puf;
    while (d_queue.size() > 1) {
        
        mix.sfreq = d_queue.top()->sfreq;
        mix.sdonnee = d_queue.top()->sdonnee;
        mix.sgauche = d_queue.top()->sgauche;
        mix.sdroit = d_queue.top()->sdroit;
        d_queue.pop();
        mux.sfreq = d_queue.top()->sfreq;
        mux.sdonnee = d_queue.top()->sdonnee;
        mux.sgauche = d_queue.top()->sgauche;
        mux.sdroit = d_queue.top()->sdroit;
        d_queue.pop();
        puf.sfreq = mix.sfreq + mux.sfreq;
        if (mix.sfreq < mux.sfreq) {
            puf.sgauche = &mix;
            puf.sdroit = &mux;
            //adjust_depth(puf.sdroit);
            //adjust_depth(puf.sgauche);
        }
        else {
            puf.sdroit = &mix;
            puf.sgauche = &mux;
            //adjust_depth(puf.sdroit);
            //adjust_depth(puf.sgauche);
        }
        d_queue.push(&puf);
       
    }
    
    head->sfreq = d_queue.top()->sfreq;
    head->sdonnee = d_queue.top()->sdonnee;
    head->sgauche = d_queue.top()->sgauche;
    head->sdroit = d_queue.top()->sdroit;
    cout << "Hey" << endl;
    vector<bool> l;
    l.push_back(0);
    assign_code(head->sgauche, &l);
    cout << "Hmm" << endl;
   // assign_code(head->sdroit, 1);

}