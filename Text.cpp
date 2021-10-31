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

//Haydar JAMMOUL & Antonin Grigoire
using namespace std;

Text::Text() {

    m_texte = lireFichierEntree(m_texte);

    //Table ASCII : 256 charactères possibles, donc 256 noeuds possibles
    m_noeuds = new noeud[256];

    for (unsigned int i = 0; i < 256; i++) {

        m_noeuds[i].s_donnee = char(i);
    }

    for (unsigned int i = 0; i < m_texte.size(); i++) {

        m_noeuds[int(m_texte[i])].s_freq++;
    }


    for (unsigned int i = 0; i < 256; i++) {

        if (m_noeuds[i].s_freq > 0) {

            m_queue.push((m_noeuds + i));
        }
    }
    cout << "no pb" << endl;
    m_racine = new noeud;
}

Text::~Text() {
}


string& Text::lireFichierEntree(string& texteEntree) {

    std::ifstream fichier("custdata.txt");

    stringstream buffer;

    buffer << fichier.rdbuf();

    texteEntree = buffer.str();

    return texteEntree;
}

void Text::assignerCode(noeud* fils, vector<bool>* bits) {

    if (fils == NULL) {

        return;
    }

    if (fils->s_gauche != NULL || fils->s_droit != NULL) {

        if (fils->s_droit != NULL) {

            bits->push_back(1);
            assignerCode(fils->s_droit, bits);
            bits->pop_back();
        }

        if (fils->s_gauche != NULL) {

            bits->push_back(0);
            assignerCode(fils->s_gauche, bits);
            bits->pop_back();
        }
    }
    else {

        cout << "\nFrequence de : [ " << fils->s_donnee << " ] = " << fils->s_freq << endl;

        cout << "Code assigné " << "(" << bits->size() << " bits) : ";

        for (vector<bool>::iterator it = bits->begin(); it != bits->end(); ++it) {

            m_codes[int(fils->s_donnee)].push_back(*it);
            cout << m_codes[int(fils->s_donnee)].back();
        }

        cout << endl;

        return;
    }
    return;
}

int Text::ecrireFichierBinaire() {

    int size;
    size = sizeof(struct hufMap);

    //Création du pointeur de fichier pour le fichier de sortie binaire Mapdb
    FILE* binaire;

    //Mapdb pour Map database
    binaire = fopen("Mapdb", "wb");

    if (binaire == NULL) {

        cout << "\nImpossible d'ouvrir le fichier Mapdb" << endl;
        return -1;
    }

    hufMap tmp_hufmap;

    for (unsigned int j = 0; j < 256; j++) {

        tmp_hufmap.h_key = char(j);
        tmp_hufmap.h_fr = m_noeuds[j].s_freq;
        fwrite(&tmp_hufmap, size, 1, binaire);
    }

    fclose(binaire);

    return 1;
}


void Text::encodage() {

    cout << "\n";

    char c = char(0);

    ofstream fichier("Encrypted.txt");

    //j est le flag pour indiquer le nombre de bits stockés dans le char
    int j = 0;

    cout << "Le code sera écrit dans le fichier Encrypted.txt" << endl;

    for (unsigned int k = 0; k < m_texte.size(); k++) {

        char raw = m_texte[k];

        //cout << "Charactère encodé : " << raw << endl;

        for (vector<bool>::iterator it = m_codes[int(raw)].begin(); it != m_codes[int(raw)].end(); it++) {

            if (*it == 1)
            {

                //"Set" le bit correct avec les méthodes ET et SHIFT
                c |= 1 << (7 - j);
            }

            j++;

            if (j == 8) {

                //Ecriture dans le fichier
                fichier << c;

                //Reset
                c = char(0);
                j = 0;
            }
        }
    }

    //Ecriture du charactère restant 
    if (j != 0) fichier << c;

    fichier.close();

    cout << "Encodage du texte terminé !" << endl;

    ecrireFichierBinaire();

    return;
}

void Text::codageHuffman() {

    cout << "Construction de l'arbre..." << endl;

    noeud* noeuds;
    noeuds = new noeud[256];

    noeud* min1, * min2;

    int i = 0;

    while (m_queue.size() != 1) {

        min1 = m_queue.top();
        m_queue.pop();
        min2 = m_queue.top();
        m_queue.pop();

        noeuds[i].s_freq = min1->s_freq + min2->s_freq;
        noeuds[i].s_gauche = min1;
        noeuds[i].s_droit = min2;

        m_queue.push(&noeuds[i]);

        i++;
    }
    i--;

    cout << "\nAssignation des codes binaires : Début" << endl;
    cout << "cool" << endl;
    vector<bool>* codes_gauche;
    codes_gauche = new vector<bool>;
    codes_gauche->push_back(0);
    assignerCode((noeuds + i)->s_gauche, codes_gauche);

    cout << "\nAssignation des codes binaires : Moitié gauche terminée" << endl;

    vector<bool>* codes_droite = new vector<bool>;
    codes_droite->push_back(1);
    assignerCode((noeuds + i)->s_droit, codes_droite);

    cout << "\nAssignation des codes binaires : Fin" << endl;

    encodage();
}

int Text::lireBinaireEtReconstruireArbre() {

    cout << "\n";

    ifstream fichier("Mapdb", ios::out | ios::binary);

    if (!fichier) {

        cout << "Impossible d'ouvrir le fichier Mapdb pour lire la structure !" << endl;

        return 1;
    }

    hufMap temp;

    m_noeuds = new noeud[256];

    //Nettoyage de la queue
    while (!m_queue.empty()) {

        m_queue.pop();
    }

    for (unsigned int i = 0; i < 256; i++)
    {

        fichier.read((char*)&temp, sizeof(hufMap));

        m_noeuds[i].s_donnee = temp.h_key;
        m_noeuds[i].s_freq = temp.h_fr;

        if (m_noeuds[i].s_freq > 0) {

            m_queue.push((m_noeuds + i));
        }
    }

    fichier.close();

    if (!fichier.good()) {

        cout << "Erreur de lecture pour le fichier Mapdb !" << endl;
        return 1;
    }

    cout << "Lecture de la structure dans Mapdb !" << endl;

    //for (unsigned int i = 0; i < 256; i++) {

      //  cout << "Char : " << m_noeuds[i].s_donnee << " " << "Fréquence : " << m_noeuds[i].s_freq << endl;
    //}

    cout << "\nReconstruction de l'arbre" << endl;

    noeud* noeuds;
    noeuds = new noeud[256];
    noeud* min1, * min2;

    int i = 0;
    while (m_queue.size() != 1) {
        min1 = m_queue.top();
        m_queue.pop();
        min2 = m_queue.top();
        m_queue.pop();

        noeuds[i].s_freq = min1->s_freq + min2->s_freq;
        noeuds[i].s_gauche = min1;
        noeuds[i].s_droit = min2;

        m_queue.push(&noeuds[i]);
        i++;
    }
    i--;

    cout << "\nReconstruction de l'arbre : début" << endl;

    vector<bool>* gauche;
    gauche = new vector<bool>;
    gauche->push_back(0);
    m_racine = (noeuds + i);

    if (m_racine->s_droit == NULL && m_racine->s_gauche == NULL) {

        cout << "Erreur : La racine n'a pas d'enfant !"; return -1;
    }

    assignerCode((noeuds + i)->s_gauche, gauche);

    cout << "\nReconstruction de l'arbre : 50%" << endl;

    vector<bool>* droite = new vector<bool>;
    droite->push_back(1);
    assignerCode((noeuds + i)->s_droit, droite);

    cout << "\nReconstruction de l'arbre terminé !" << endl;

    return 0;
}

Text::noeud* Text::parcourirArbre(noeud* parent, bool bit) {

    if (bit == 0 && parent->s_gauche != NULL)
        return parent->s_gauche;

    if (bit == 1 && parent->s_droit != NULL)
        return parent->s_droit;

    cout << "\n(Fonction parcourirArbre) Erreur de parcours de l'arbre !" << endl;

    return NULL;
}

void Text::Decode() {

    lireBinaireEtReconstruireArbre();

    cout << "\nDécodage ! \n";

    FILE* fp_input, * fp_out;

    fp_input = fopen("Encrypted.txt", "r");

    if (fp_input == NULL) {

        cout << "\nImpossible d'ouvrir le fichier Encrypted.txt pour l'entrée du décodage !" << endl;
        return;
    }

    fp_out = fopen("Decoded.txt", "wb");

    if (fp_out == NULL) {

        cout << "\nImpossible d'ouvrir le fichier Decoded.txt pour la sortie du décodage" << endl;
        return;
    }

    //Pointeur de position dans le texte pour lire jusqu'à la fin (EOF)
    noeud* pos = m_racine;

    char temp;

    char comp = char(0);

    bool b;

    int N = 0;

    while (!feof(fp_input)) {

        string code = "";

        //Lecture du fichier input
        fscanf(fp_input, "%c", &temp);

        //Recherche des codes dans temp
        //Chaque bit retraité sera utilisé pour parcourir l'arbre
        //N est l'index pour dire quel bit est lu dans temp
        while (N < 8) {

            comp = char(97 + (temp >> (7 - N) & 1));
            N++;

            //comp is byte with NULL value; 
            //we will read each bit of temp in comp using Logic AND  to 0000 0001
            //after shifting temp to the bit we want to check we make the and
            // to the char we add 97 se we are able to make char a if the detectedbitis 0
            // and bif the bit is 1 comp is equal to char(98)='b'
            //if the bit we are checking is 1 comp='b' else ='a' 
            //this is why we added 97 to com 
            if (comp == 'a') {
                b = 0;
            }
            else {
                b = 1;
            }


            if (pos == NULL) {

                cout << "ERROR1";
                return;
            }
            pos = parcourirArbre(pos, b);

            if (pos == NULL) {

                cout << "ERROR2";
                return;
            }

            if (pos->s_gauche == NULL && pos->s_droit == NULL) {

                cout << "Lettre détectée :" << " " << pos->s_donnee << endl;

                fwrite(&(pos->s_donnee), sizeof(char), 1, fp_out);

                pos = m_racine;

                code = "";
            }

            comp = char(0);
        }

        N = 0;
    }

    fclose(fp_input);
    fclose(fp_out);
}