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
   // cout << "no prb1" << endl;
    for (unsigned int i = 0; i < txt.size(); i++) {
        count[int(txt[i]) - 97].sfreq++;
    }
    //cout << "no prb2" << endl;
    noeud* a;
    for (int i = 0; i < 6; i++) {
        d_queue.push((count+i));
    }
    cout << d_queue.top()->sfreq << endl;
    head = new noeud;
   // cout << "no prb3" << endl;
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
         if ((child)->sdroit != NULL)
         {
             b->push_back(1);
             assign_code((child)->sdroit, b);
             b->pop_back();
         }
         if ((child)->sgauche != NULL)
         {
             b->push_back(0);
             assign_code((child)->sgauche, b);
             b->pop_back();
         }
     }
     else
     {
             cout << "A code is assigned:"<<'(' << b->size()<<')' ;
             for (vector<bool>::iterator it = b->begin(); it != b->end(); ++it)
             {
                 code[int((child)->sdonnee) - 97].push_back(*it);
                 cout << code[int((child)->sdonnee) - 97].back();
             }
             cout << endl;
             
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
        cout << "encoding char:" << raw << endl;;
        for (std::vector<bool>::iterator it = code[int(raw)-97].begin(); it != code[ int(raw)-97].end(); it++) {
            cout << "it?" << endl;
            if (*it == 1)
            {
                //set the appropriate bit using the AND and SHIFT methods
                cs |= 1 << (7 - j);
            }
            j++;
            cout << j << endl;
            if (j == 8) {
                //write the char in file
                MyFile << cs;
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
    write_to_bin_file();
    return;
}

void Text::Huffman_Tree_constructor() {
    noeud* puf;
    (puf) = new noeud[6];
    noeud *mix, *mux;
    cout << "!!!!!!!!!!!!!!!!!!!" << endl;
    int i = 0;
    while (d_queue.size() != 1) {
        mix = d_queue.top();
        cout<<"our smallest element:"<< d_queue.top()->sfreq<<endl;
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
        cout << d_queue.size() << endl;
    }
    i--;
    cout << "our smallest element:" << d_queue.top()->sfreq << endl;
    vector<bool> *l;
    l = new vector<bool>;
    l->push_back(0);
    assign_code((puf+i)->sgauche, l);
    cout << "Half Way" << endl;
    vector<bool> *ll=new vector<bool>;
    ll->push_back(1);
    assign_code((puf + i)->sdroit, ll);
    cout << "DONE assigning codes" << endl;
  
    //cout<< write_to_bin_file();
    
    Encode_write();
    //cout << 1;

}

int Text::ReadBinFile_and_ReconstructTree() {
    ifstream rf("Mapdb", ios::out | ios::binary);
    if (!rf) {
        cout << "Cannot open file!" << endl;
        return 1;
    }
    Huf_map temp;
    count = new noeud[6];
    //we will clear our priority queue
    while (!d_queue.empty()) { d_queue.pop(); }
    cout << "size of queue" << d_queue.size() << endl;

    for (int i = 0; i < 6; i++)
    {
        rf.read((char*)&temp, sizeof(Huf_map));
        count[i].sdonnee = temp.h_key;
        count[i].sfreq = temp.h_fr;
        cout << "char:" << count[i].sdonnee << " " << count[i].sfreq << endl;
        d_queue.push((count + i));
    }
    rf.close();
    if (!rf.good()) {
        cout << "Error occurred at reading time!" << endl;
        return 1;
    }
    cout << "We will read our Map out Loud!" << endl;
    for (int i = 0; i < 6; i++) {
        cout << "Char: " << count[i].sdonnee <<" " 
            << "Fr: " << count[i].sfreq << endl;
    }
    
    cout << "Now we will startreconstructing our Tree" << endl;

    noeud* puf;
    (puf) = new noeud[6];
    noeud* mix, * mux;

    int i = 0;
    while (d_queue.size() != 1) {
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
    vector<bool>* l;
    l = new vector<bool>;
    l->push_back(0);
    head = (puf + i);
    if (head->sdroit == NULL && head->sgauche == NULL) { cout << "aprblome"; return -1; }
    assign_code((puf + i)->sgauche, l);
    cout << "Half Way" << endl;
    vector<bool>* ll = new vector<bool>;
    ll->push_back(1);
    assign_code((puf + i)->sdroit, ll);
    cout << "DONE constructing our Tree" << endl;
    //Store Head of our Tree
    return 0;
}

Text::noeud* Text::TraverseTree(noeud* parent, bool bit) {
    if (bit == 0 && parent->sgauche!=NULL)
        return parent->sgauche;
    if (bit == 1 && parent->sdroit != NULL)
        return parent->sdroit;
    cout << "PRBLM in TREE" << endl;
    return NULL;
}

void Text::Decode() {
    ReadBinFile_and_ReconstructTree();
    FILE* fp_input, * fp_out;
    fp_input = fopen("Encrypted.txt", "r");
    if (fp_input == NULL) {
        cout << "Could not open input file"
            << endl;
        //return -1;
    }
    fp_out = fopen("Decoded.txt", "wb");
    if (fp_out == NULL) {
        cout << "Could not open input file"
            << endl;
        return;
    }
    // Read next line from input text
    // file until EOF is reached
    noeud* pos = head;
    if (pos->sdroit == NULL && pos->sgauche == NULL) { cout << "bprblm"; return; }
    char temp;
    char comp = char(0);
    bool b; 
    int N = 0;
    while (!feof(fp_input)) {

        // Reading the text file
        fscanf(fp_input, "%c", &temp);
        cout << "we have took the following char:" << " " << temp << endl;
        //search for codes in temp
        //each retreated bit will be used to traverse the tree
        //N is index to tell which bit we are reading from temp
        while (N < 8) {
            comp = char(97 + (temp >> (7 - N) & 1));
            N++;
            //comp is byte with NULL value; 
            //we will read each bit of temp in comp using Logic AND  to 0000 0001
            //after shifting temp to the bit we want to check 
            //if the bit we are checking is 1 comp='b' else ='a' 
            //this is why we added 97 to com 
            if (comp == 'a')
                b = 0;
            else
                b = 1;
            cout << "detectected bit:" << b << endl;
            if (pos == NULL) { cout << "ERROR1"; return; }
            pos = TraverseTree(pos, b);
            if (pos == NULL) { cout << "ERROR2"; return; }
            if (pos->sgauche == NULL && pos->sdroit == NULL) {
                cout << "we have detected a code" << endl;
                cout << "we detected:" << " " << pos->sdonnee << endl;
                fwrite(&(pos->sdonnee), sizeof(char), 1, fp_out);
                pos = head;
            }
            comp = char(0);
        }
        N = 0;
    }
    // Close both the files
     fclose(fp_input);
     fclose(fp_out);
}
