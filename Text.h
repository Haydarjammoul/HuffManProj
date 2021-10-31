//Haydar JAMMOUL & Antonin Grigoire
#pragma once

#include <cstddef>
#include <vector>
#include <queue>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

//! Classe Text contenant les méthodes afin de réaliser un codage Huffman
class Text
{

    //! Structure représentant une paire lettre/fréquence
    struct hufMap {

        //! Charactère, lettre
        char h_key;

        //! Entier, fréquence de la lettre
        int h_fr;

        //! Constructeur principal de la structure
        hufMap() {

            h_key = char(0);
            h_fr = 0;
        }
    };

    //! Structure représentant un noeud de l'arbre binaire construit par le codage Huffman
    struct noeud {

        //! Charactère, lettre du noeud
        char s_donnee;

        //! Fréquence, fréquence de la lettre
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

    //! Structure contenant une surcharge de l'opérateur ()
    struct compare {

        //!Surcharge de l'opérateur ()
        /*!
            L'opérateur sruchargé permet de comparer 2 noeuds entre eux,
            retourne vrai si et seulement si la fréquence du noeud opérande gauche
            est plus grande que celle du noeud opérande droite
        */
        bool operator () (noeud* gauche, noeud* droit) {

            return gauche->s_freq > droit->s_freq;
        }
    };

    //! Chaîne de charactères, texte à coder
    string m_texte;

    //! Queue de noeuds, contient la liste des noeuds de l'arbre
    /*!
        Utilise la structure "compare" et la surcharge de l'opérateur () pour trier les noeuds
    */
    priority_queue <noeud*, vector<noeud*>, compare> m_queue;

    //! Pointeur de noeud vers un tableau des noeuds créés
    noeud* m_noeuds;

    //! Pointeur de noeud, racine de l'arbre binaire
    noeud* m_racine;

    //! Tableau de vecteurs de booléens, pour contenir les codes binaires des lettres
    vector<bool> m_codes[256];

    //
    //---Méthodes---
    //

    //! Méthode pour assigner un code binaire à son noeud
    /*!
      \param 1 pointeur de noeud
      \param 2 pointeur vecteur de booléens
    */
    void assignerCode(noeud*, vector<bool>*);

    //! Méthode pour écrire les codes binaires des lettres dans un fichier
    /*!
        Cette fonction écrit un mappage des fréquences à l'aide de la structure hufMap pour
        être capable par la suite de reconstruire la table pour décoder
        Chaque charactère sera encodé dans un fichier txt avec son code binaire
        avec des paquets de 8 bits en 1 octect de char
    */
    int ecrireFichierBinaire();

    //! Méthode pour lire le fichier texte d'entrée à coder
    /*!
      \param 1 référence de la variable dans laquelle retourner la texte
    */
    string& lireFichierEntree(string&);

    //! Créer le fichier crypté/codé à partir du fichier binaire de codage
    void encodage();

    //! Retourne le fils gauche ou droite du noeud selon le bit
    /*!
        \param 1 noeud de départ du parcours
        \param 2 bit pour trouver le bon fils
    */
    noeud* parcourirArbre(noeud*, bool);


public:

    //! Méthode du codage Huffman avec création de l'arbre binaire correspondant
    void codageHuffman();

    //! Constructeur principal de la classe Text
    Text();

    //! Méthode pour décoder le fichier crypté à l'aide du codage
    void Decode();

    //! Méthode pour lire le fichier binaire du codage et reconstruire l'arbre associé
    int lireBinaireEtReconstruireArbre();

    //! Destructeur
    ~Text();
};