//
// Created by farouk on 19-03-17.
//

#ifndef TP2_ARBREAVL_H
#define TP2_ARBREAVL_H

#include <algorithm>
#include <iostream>
#include <vector>
#include "assert.h"

using namespace std;

template<class C, class V>
struct Noeud {
    Noeud( const C &, const V & );

    C clef;
    V valeur;
    Noeud *gauche;
    Noeud *droite;
    int equilibre;
};


template<class C, class V>
class ArbreAVL {

public:
    //Méthodes externes
    ArbreAVL();

    void inserer( const C &x, const V &y );

    Noeud<C, V> *rechercher( C x );

private:

    Noeud<C, V> *racine;

    //Méthodes internes
    bool inserer( const C &x, const V &y, Noeud<C, V> *&noeud );

    Noeud<C, V> *rechercher( C x, Noeud<C, V> *&n );

    void rotationGaucheDroite( Noeud<C, V> *&racinesousarbre );

    void rotationDroiteGauche( Noeud<C, V> *&racinesousarbre );


};

/**
 * Initialisation du noeud
 * @tparam C
 * @tparam V
 * @param c
 * @param v
 */
template<class C, class V>
Noeud<C, V>::Noeud( const C &c, const V &v )
        : clef( c ), valeur( v ), equilibre( 0 ), gauche( NULL ), droite( NULL ) {
}

template<class C, class V>
ArbreAVL<C, V>::ArbreAVL()
        : racine( NULL ) {
}


/**
 * Méthode interne retournant un booléan selon l'état de l'augmentation de l'hauteur de l'arbre
 * @tparam C
 * @tparam V
 * @param x
 * @param y
 * @param noeud
 * @return bool
 */
template<class C, class V>
bool ArbreAVL<C, V>::inserer( const C &x, const V &y, Noeud<C, V> *&noeud ) {
    if ( noeud == NULL ) {
        noeud = new Noeud<C, V>( x, y );
        return true;
    }
    if ( x < noeud->clef ) {
        if ( inserer( x, y, noeud->gauche )) {
            noeud->equilibre--;
            if ( noeud->equilibre == 0 ) return false;
            if ( noeud->equilibre == -1 ) return true;
            assert( noeud->equilibre == -2 );
            if ( noeud->gauche->equilibre == 1 )
                rotationDroiteGauche( noeud->gauche );
            rotationGaucheDroite( noeud );
        }
        return false;
    } else if ( noeud->clef < x ) {

        if ( inserer( x, y, noeud->droite )) {
            noeud->equilibre++;
            if ( noeud->equilibre == 0 ) return false;
            if ( noeud->equilibre == 1 ) return true;
            assert( noeud->equilibre == 2 );
            if ( noeud->droite->equilibre == -1 )
                rotationGaucheDroite( noeud->droite );
            rotationDroiteGauche( noeud );
        }

        return false;
    } else if ( noeud->clef == x ) {

        cerr << "L'identificateur: " << x << " existe déjà !" << endl;
        exit( -1 );
    }

    noeud->clef = x;  // Mise à jour
    noeud->valeur = y;
    return false;
}


/**
 * C'est une méthode interne qui retourne un pointeur du noeud s'il existe dans l'arbre sinon elle retourne NULL
 * @param x
 * @param n
 * @return Noeud< C, V>*
 */
template<class C, class V>
Noeud<C, V> *ArbreAVL<C, V>::rechercher( C x, Noeud<C, V> *&n ) {

    if (( n != nullptr ) && ( x > n->clef )) {

        n = n->droite;
        rechercher( x, n );

    } else if (( n != nullptr ) && ( x < n->clef )) {

        n = n->gauche;
        rechercher( x, n );

    }

    return ( n );

}


/**
 * Méthode publique d'insertion à utiliser dans le main.cpp
 * @param n
 */
template<class C, class V>
void ArbreAVL<C, V>::inserer( const C &x, const V &y ) {

    inserer( x, y, racine );

}

/**
 * Méthode publique de recherche retournant un pointeur du noeud à utiliser dans le main.cpp
 * @tparam C
 * @tparam V
 * @param x
 * @return
 */
template<class C, class V>
Noeud<C, V> *ArbreAVL<C, V>::rechercher( C x ) {

    Noeud<C, V> *temp = racine;
    return ( rechercher( x, temp ));

}


/**
 * Méthode interne effectuant la rotation simple gauche-droite
 * @tparam C
 * @tparam V
 * @param racinesousarbre
 */
template<class C, class V>
void ArbreAVL<C, V>::rotationGaucheDroite( Noeud<C, V> *&racinesousarbre ) {

    Noeud<C, V> *temp = racinesousarbre->gauche;
    int ea = temp->equilibre;
    int eb = racinesousarbre->equilibre;
    int neb = 1 + eb + max( 0, -ea );
    int nea = ea + max( 0, neb ) + 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->gauche = temp->droite;
    temp->droite = racinesousarbre;
    racinesousarbre = temp;

}


/**
 * Méthode interne effectuant la rotation simple droite-gauche
 * @tparam C
 * @tparam V
 * @param racinesousarbre
 */
template<class C, class V>
void ArbreAVL<C, V>::rotationDroiteGauche( Noeud<C, V> *&racinesousarbre ) {

    Noeud<C, V> *temp = racinesousarbre->droite;
    int eb = temp->equilibre;
    int ea = racinesousarbre->equilibre;
    int nea = ea - 1 - max( 0, eb );
    int neb = eb - 1 + min( 0, nea );

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->droite = temp->gauche;
    temp->gauche = racinesousarbre;
    racinesousarbre = temp;


}


#endif //TP2_ARBREAVL_H
