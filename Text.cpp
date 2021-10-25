#pragma warning(disable : 4996)
#include "Text.h"
#include <string>
#include <iostream>
#include <queue>
#include <vector>
#include <cstdio>
#include <map>
#include <fstream>
#include <sstream>
#include <cstddef>
using namespace std;
Text:: Text() {
    txt=read_from_txt_file(txt);
    //txt = t;
    count = new noeud[6];
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
        d_queue.push((count+i));
    }
    head = new noeud;
    cout << "no prb3" << endl;
}

string& Text::read_from_txt_file(string& in) {
    // Create File Pointers fp_input for
    // text file
    std::ifstream t("custdata.txt");
    stringstream buffer;
    buffer << t.rdbuf();
    in=buffer.str();
    return in;
}

void Text::assign_code(noeud* child, vector<bool>* b) {
    if ((child) == NULL) {
        cout << "brr" << endl;
        return;
    }
    cout << "Freq:" << endl;
    cout << child->sdonnee<<" "<< (child)->sfreq << endl;
     if((child)->sgauche != NULL || (child)->sdroit != NULL) {
            //cout << "tyb" << endl;
            b->push_back(1);
            assign_code((child)->sdroit, b);
            b->pop_back();
            b->push_back(0);
            assign_code((child)->sgauche, b);
     }
     else
     {
             cout << "A code is assigned" << endl;
             code[int((child)->sdonnee) - 97] = *b;
             return;
     }
    return;
}

int Text::write_to_bin_file() {
    int rec_size;
    rec_size = sizeof(struct Huf_map);
    // Create File Pointers fp_output for output binary file
    FILE* fp_output;
    fp_output = fopen("Mapdb", "wb");

    if (fp_output == NULL) {
        cout << "Could not open "
            << "output file" << endl;
        return -1;
    }
    Huf_map temp_to_insert;
    for (int j = 0; j < 6; j++) {
       // cout << char(97 + j) << " ";
        temp_to_insert.h_key = char(97 + j);
        temp_to_insert.h_fr = count[j].sfreq;
        fwrite(&temp_to_insert, rec_size, 1, fp_output);
        cout << "Freq map written to bin file Mapdb" << endl;
    }
    fclose(fp_output);
    //we have just wrote our frequency map as Huf_map struct to be able to reconstruct the table for decoding
    //we will now encode each char of our txt and write its code
    // each 8 bits are arranged in 1 Byte(char)and written to file
    return 1;
}


void Text::Encode_write() {
    
    char cs = char(0); //in Hex 0
    //j will be the flag to indicate number of bits stored in char
    ofstream MyFile("Encrypted.txt");
    int j = 0;
    cout << "code will be written if file Encrypted.txt" << endl;
    for (int k = 0; k < txt.size(); k++) {
        char raw = txt[k];
        cout << "encoding char:" << raw<<endl;
        for (int i = 0; !code[int(raw) - 97].empty(); i++) {
            if (code[int(raw) - 97].front() == 1)
            {
                //set the appropriate bit using the AND and SHIFT methods
                cs |= 1 << (7 - i - j);
            }
            code[int(raw) - 97].erase(code[int(raw) - 97].begin());
            j++;
            if (j == 8) {
                //write the char in file
                MyFile << cs;
                cout << j << endl;
                cout << "we wrote:" << cs << endl;
                cs = char(0);
                j = 0;
            }
        }
    }//write the last element is still in cs temperary variable 
    if (j != 0) MyFile << cs;
   // Close the file
    MyFile.close();
    cout << "sucessfully encoded your text" << endl;
    return;
}

void Text::Huffman_Tree_constructor() {
    noeud* puf;
    (puf) = new noeud[6];
    noeud *mix, *mux;

    int i = 0;
    while (d_queue.size() > 1) {
        mix = d_queue.top();
        d_queue.pop();
        mux = d_queue.top();
        d_queue.pop();
        
        //cout << mix->depth << " " << mux->depth << endl;
        
        (puf[i]).sfreq = mix->sfreq + mux->sfreq;
        (puf[i]).sgauche = mix;
        (puf[i]).sdroit = mux;
        //cout << puf[i].sfreq <<' ' << puf[i].sgauche->sfreq << ' ' << puf[i].sdroit->sfreq << endl;
        //cout << (puf + i) << ' ' << (puf + i)->sdroit << ' ' << (puf + i)->sgauche << endl<<endl;

        d_queue.push(&puf[i]);
        i++;
        //cout << i << endl;
    }
    i--;
    vector<bool> *l;
    l = new vector<bool>;
    l->push_back(0);
    assign_code((puf+i)->sgauche, l);
    cout << "Half Way" << endl;
    vector<bool> *ll=new vector<bool>;
    ll->push_back(1);
    assign_code((puf + i)->sdroit, ll);
    cout << "DONE assigning codes" << endl;
  
    cout<< write_to_bin_file();
    Encode_write();
    cout << 1;

}