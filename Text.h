//Haydar JAMMOUL & Antonin Grigoire
#pragma once

#include <cstddef>
#include <vector>
#include <queue>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

//! Classe Text contenant les m�thodes afin de r�aliser un codage Huffman
class Text
{

    //! Structure repr�sentant une paire lettre/fr�quence
    struct hufMap {

        //! Charact�re, lettre
        char h_key;

        //! Entier, fr�quence de la lettre
        int h_fr;

        //! Constructeur principal de la structure
        hufMap() {

            h_key = char(0);
            h_fr = 0;
        }
    };

    //! Structure repr�sentant un noeud de l'arbre binaire construit par le codage Huffman
    struct noeud {

        //! Charact�re, lettre du noeud
        char s_donnee;

        //! Fr�quence, fr�quence de la lettre
        int s_freq;

        //! Fils gauche du noeud
        noeud* s_gauche;

        //! Fils droit du noeud
        noeud* s_droit;

        //! Constructeur principal d'un noeud
        noeud(char d = char(0), int f = 0) {

            s_gauche = NULL;
            s_droit = NULL;
            this->s_donnee = d;
            this->s_freq = f;
        }
    };

    //! Structure contenant une surcharge de l'op�rateur ()
    struct compare {

        //!Surcharge de l'op�rateur ()
        /*!
            L'op�rateur srucharg� permet de comparer 2 noeuds entre eux,
            retourne vrai si et seulement si la fr�quence du noeud op�rande gauche
            est plus grande que celle du noeud op�rande droite
        */
        bool operator () (noeud* gauche, noeud* droit) {

            return gauche->s_freq > droit->s_freq;
        }
    };

    //! Cha�ne de charact�res, texte � coder
    string m_texte;

    //! Queue de noeuds, contient la liste des noeuds de l'arbre
    /*!
        Utilise la structure "compare" et la surcharge de l'op�rateur () pour trier les noeuds
    */
    priority_queue <noeud*, vector<noeud*>, compare> m_queue;

    //! Pointeur de noeud vers un tableau des noeuds cr��s
    noeud* m_noeuds;

    //! Pointeur de noeud, racine de l'arbre binaire
    noeud* m_racine;

    //! Tableau de vecteurs de bool�ens, pour contenir les codes binaires des lettres
    vector<bool> m_codes[256];

    //
    //---M�thodes---
    //

    //! M�thode pour assigner un code binaire � son noeud
    /*!
      \param 1 pointeur de noeud
      \param 2 pointeur vecteur de bool�ens
    */
    void assignerCode(noeud*, vector<bool>*);

    //! M�thode pour �crire les codes binaires des lettres dans un fichier
    /*!
        Cette fonction �crit un mappage des fr�quences � l'aide de la structure hufMap pour
        �tre capable par la suite de reconstruire la table pour d�coder
        Chaque charact�re sera encod� dans un fichier txt avec son code binaire
        avec des paquets de 8 bits en 1 octect de char
    */
    int ecrireFichierBinaire();

    //! M�thode pour lire le fichier texte d'entr�e � coder
    /*!
      \param 1 r�f�rence de la variable dans laquelle retourner la texte
    */
    string& lireFichierEntree(string&);

    //! Cr�er le fichier crypt�/cod� � partir du fichier binaire de codage
    void encodage();

    //! Retourne le fils gauche ou droite du noeud selon le bit
    /*!
        \param 1 noeud de d�part du parcours
        \param 2 bit pour trouver le bon fils
    */
    noeud* parcourirArbre(noeud*, bool);


public:

    //! M�thode du codage Huffman avec cr�ation de l'arbre binaire correspondant
    void codageHuffman();

    //! Constructeur principal de la classe Text
    Text();

    //! M�thode pour d�coder le fichier crypt� � l'aide du codage
    void Decode();

    //! M�thode pour lire le fichier binaire du codage et reconstruire l'arbre associ�
    int lireBinaireEtReconstruireArbre();

    //! Destructeur
    ~Text();
};