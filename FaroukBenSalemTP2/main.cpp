#include <iostream>
#include <vector>
#include "Type.h"
#include "ArbreAVL.h"
#include<fstream>
#include <algorithm>

using namespace std;


/**
 * Fonction qui lit un fichier et qui met son contenu dans un vecteur
 * @param nomFichierFAROUUUUUUUK PAUUUUUL !!!!!
 * @return texte paul
 */
vector<string> lireFichier( string nomFichier ){

    ifstream file( nomFichier );


    vector<string> texte;

    if (file.is_open()){

        string str;

        while ( getline( file, str ) ){

            texte.push_back(str );

        }

        file.close();

    }

    return texte;
}

/**
 *  Procédure qui enlève les espaces dans un texte donné
 * @param texte
 */
void enleverEspace( string &texte ) {

    //nettoyer un texte des espaces
    int i = 0;
    while( ( i < texte.size() )  ){

        if( isspace( texte[ i ] ) ){

            // parcourir et supprimer tous les espaces

            texte.erase( texte.begin() + i );

            i = 0;
        }

        else

            ++ i;

    }

}

/**
 * Procédure qui vérifie si la séquence d'identificateurs d'un type donné contient des doublons
 * @param vecteur
 */
void detecterDoublonElment(vector<string> vecteur){

    sort( vecteur.begin(), vecteur.end() );

    for (int i = 0; i < vecteur.size() - 1; ++ i) {

        if ( vecteur.at( i ) == vecteur.at( i + 1 )){

            cerr << " L'élément: " << vecteur.at( i ) << " existe déjà dans la sequence ! " << endl;

            exit( - 1 );
        }

    }
}

/**
 * Procédure qui vérifie s'il existe une redondance de clause, si oui on enlève les redondances
 * @param vecteur
 */
void detecterDoublonClause( vector< vector< string > > &vecteur ){

    for (int i = 0; i < vecteur.size()  ; ++i) {

        for (int j = i + 1; j < vecteur.size() ; ++j) {

            if( vecteur.at( i ) == vecteur.at( j ))

                vecteur.erase( vecteur.begin() + j  );
        }



        }

    }

/**
 * Fonction qui prend en parametre une chaine de caractère délimitée par des virgules et qui retoune un vecteur de la meme chaine sans la virgule
 * @param seq
 * @return tabId
 */
vector<string> construireSeq(string& seq) {

    vector<string> tabId;
    string delimiteur = ",";
    string nouveauId;
    int pos_delimiteur = 0;

    while (pos_delimiteur != string::npos) {

        pos_delimiteur = seq.find(delimiteur);

        nouveauId = seq.substr(0, pos_delimiteur);

        enleverEspace( nouveauId );

        //Erreur si l'identificateur est un nom réservé
        if ( nouveauId == "foncteur" or nouveauId == "type" ){

            cerr << "un id ne peut pas être un mot réservé"<< endl;
            exit( - 1 );
        }

        tabId.push_back(nouveauId);

        seq.erase(0, pos_delimiteur + delimiteur.length());

    }

    return tabId;
}

/**
 * Fonction retournant un objet de nature Type
 * @param texte
 * @return Type
 */
Type construireType( string texte ){


    //position avant le signe =
    int posNom2 = texte.find( "=" ) - 1;
    //position apres "type "
    int posNom1 = texte.find( "type ") + 5;
    string nomType = texte.substr( posNom1, posNom2 - posNom1 );

    //position apres accolade ouvrante
    int posId1 = texte.find( "{" ) + 1;
    //position avant accolade fermante
    int posId2 = texte.find( "}" );
    string seqId = texte.substr( posId1, posId2 - posId1 );
    vector<string> tabSeq = construireSeq(seqId);
    detecterDoublonElment(tabSeq);
    //Instanciation de la classe Type
    Type* T = new Type( nomType, tabSeq );

    return *T;

}


/**
 *  Fonction retournant un arbre de type
 * @param fichier
 * @return arbreType
 */
ArbreAVL< string, vector< string > > construireArbreType( vector<string> fichier ){
    ArbreAVL< string, vector< string > > arbreType;
    for ( int i = 0; i < fichier.size(); ++i ) {

        if( fichier.at( i ).find( "type ") != string::npos ){

            Type T = construireType( fichier.at( i ) );
            arbreType.inserer( T.getnom(), T.getseqID());

        }

    }

    return arbreType;

}



/**
 * Fonction retournant un arbre de foncteur
 * @param fichier
 * @param arbreType
 * @return
 */
ArbreAVL<string, vector<vector<string>>> construireArbreFoncteur(vector<string> fichier, ArbreAVL< string, vector< string > > arbreType) {

    ArbreAVL< string, vector< vector < string > > > arbreFoncteur;
    vector<string> tabType;
    vector<string> tabId;
    vector< vector< string > > tabClause ;
    string nomFoncteur;

    //debut boucle
    int i = 0;
    while (  i < fichier.size() ) {

        if( fichier.at( i ).find( "foncteur ") != string::npos ){

            tabClause.clear();
            //position avant le ::
            int posNom2 = fichier.at(i ).find( "::" ) - 1;
            //position apres "foncteur "
            int posNom1 = fichier.at(i ).find( "foncteur ") + 9;
            nomFoncteur = fichier.at(i ).substr( posNom1, posNom2 - posNom1 );

            if( arbreType.rechercher( nomFoncteur ) != NULL){

                cerr << " On ne peut pas avoir un nom commun entre les types et les foncteurs " << endl;
                exit( - 1 );
            }

            //sequence de type
            int posType1 = fichier.at(i ).find( ":: " ) + 3;
            int posType2 = fichier.at(i ).size() -1 ;
            string seqType = fichier.at(i ).substr( posType1, posType2 - posType1 );
            tabType = construireSeq( seqType );

            //verifier si les types existent déjà
            for (int k = 0; k < tabType.size() ; ++k) {

                if ( arbreType.rechercher( tabType.at( k ) ) == nullptr ){

                    cerr << "Type introuvable !" <<  tabType.at( k ) << " " << endl;
                    exit( - 1 );
                }

            }

            //sequence de clauses, vecteur de vecteur
            int j = i + 1;
            while ( ( j < fichier.size() ) && ( fichier.at( j ).find( "foncteur ") == string::npos )
                        && ( fichier.at( j ).find( "?") == string::npos )
                        && ( fichier.at( j ).find( "type ") == string::npos )  ){

                    string clausePrimaire = fichier.at( j );
                    //position apres parenthèse ouvrante
                    int posId1 = clausePrimaire.find( "(" ) + 1;
                    //position avant parenthèse fermante
                    int posId2 = clausePrimaire.find( ")" );
                    string seqId = clausePrimaire.substr( posId1, posId2 - posId1 );
                    tabId = construireSeq( seqId );

                    // verifier si l'id existe dans la liste des id du type correspondant
                    for (int k = 0; k < tabId.size(); ++ k) {
                        string type = tabType.at( k );
                        string id = tabId.at( k );
                        bool trouve = ( find( ( arbreType.rechercher( type ) -> valeur.begin() ), ( arbreType.rechercher( type ) -> valeur.end() ), id )
                                != ( arbreType.rechercher( type )->valeur .end() ) );
                        if( ! trouve ){

                            cerr << "identificateur introuvable ! " << endl;
                            exit( - 1 );
                        }


                    }

                    //vecteur de vecteur de id
                    tabClause.push_back( tabId );
                    //On élimine les redondances des clauses
                    detecterDoublonClause( tabClause );

                    ++ j;

            }

            arbreFoncteur.inserer( nomFoncteur, tabClause );

            i = j;

        }

        else
        ++i;

    }

    return arbreFoncteur;
}

/**
 * Reuqete s? avec s le nom du type
 * @param nom
 * @param arbre
 * @return
 */
vector< string > requeteElement( string nom, ArbreAVL< string, vector< string > >  arbre ){

    vector< string > tabElement;
    string elmt;

    if( arbre.rechercher( nom ) != NULL ){

        tabElement = arbre.rechercher( nom ) -> valeur;

        cout << "{";

        for (int j = 0; j < tabElement.size() ; ++j) {

            elmt += tabElement.at( j ) + ", ";
        }

        elmt.erase( elmt.length() - 2);
        cout << elmt << "}" << endl;

    }

    return tabElement;

}

/**
 * Requete f? avec f est le nom du foncteur
 * @param nom
 * @param arbre
 * @return
 */
vector< vector< string > > requeteClause( string nom, ArbreAVL< string, vector< vector < string > > > arbre ){

    vector< vector< string > > tabClause;
    string clause = "";

    if(  arbre.rechercher( nom ) != NULL ){

        tabClause = arbre.rechercher( nom ) -> valeur;

        for (int i = 0; i < tabClause.size() ; ++i) {

            cout << "( ";

            for (int j = 0; j < tabClause.at( i ).size(); ++j) {

                clause += tabClause.at( i ).at( j ) + ", ";

            }

            clause.erase( clause.end()- 2);
            cout << clause <<  ")"<< endl;

            clause = "";

        }

    }

    return tabClause;

}


/**
 * Requete f(e1,...,?...,en) avec f nom du foncteur et e1,...,en est une séquence d'id
 * @param clause
 * @param nom
 * @param arbre
 * @return
 */
vector<string> extraireElement(string clause, string nom, ArbreAVL<string, vector<vector<string>>> arbre) {

    vector< string > tabElmt;

    vector< string > tabId;

    int pos = 0;

    string elmt;

    if( arbre.rechercher( nom ) == NULL )

        cout << "Foncteur de nom : " << nom <<  " introuvable !" << endl;

    else{

        //position apres parenthèse ouvrante
        int posId1 = clause.find( "(" ) + 1;
        //position avant parenthèse fermante
        int posId2 = clause.find( ")" );

        //construire table id
        string seqId = clause.substr( posId1, posId2 - posId1 );
        tabId = construireSeq( seqId );

        //extraire position "?"
        vector< string >::iterator it = find( tabId.begin(), tabId.end(), "?");
        pos = distance( tabId.begin(), it );

        //Acces à la clause à partir de pos
        cout << "{";
        for (int i = 0; i < arbre.rechercher( nom ) -> valeur .size() ; ++i) {

            //Variable booléenne qui cherche si une clause donnée est identique à la clause passée dans la requête
            bool identique = true;

            int j = 0;
            while( ( identique ) && ( j < tabId.size() - 1 ) ){

                if( ( tabId[ j ] != arbre.rechercher( nom ) -> valeur .at( i ).at( j ) ) && tabId[ j ] != "?" )

                    identique = false;

                ++ j;
            }

            if( identique ){

                elmt += ( arbre.rechercher( nom ) -> valeur .at( i ).at( pos ) ) + ", ";

            }

        }

        elmt.erase( elmt.length() - 2);
        cout << elmt << "}" << endl;

    }

    return tabElmt;
}

/**
 * Procédure qui englobe le traitement de toutes les requetes
 * @param fichier
 * @param arbreT
 * @param arbreF
 */
void analyseRequete( vector< string > fichier, ArbreAVL<string, vector<string> > arbreT,
                    ArbreAVL<string, vector<vector<string> > > arbreF ){

    string nom;
    vector< string > tabElmt;
    vector< vector< string > > tabClause;
    for ( int i = 0; i < fichier.size(); ++i ) {

        if( fichier.at( i ).find( "?") != string::npos ){

            if( fichier.at( i ).find( "(" ) == string::npos ){

                //construction nom type ou foncteur
                int posNom = fichier.at(i ).find( "?") ;
                nom = fichier.at(i ).substr( 0, posNom);
                enleverEspace( nom );

                //requetes s? ou f?
                tabElmt = requeteElement( nom, arbreT);

                tabClause = requeteClause( nom, arbreF );

                if( ( tabElmt.size() == 0 ) && ( tabClause.size() == 0 ) )
                    cout << "Le type ou le foncteur n'existe pas dans la base de connaissance !" << endl;


            }

            //requete valeurs (x,x,?,x)
            else
            {
                int posNom = fichier.at(i ).find( "(") ;
                nom = fichier.at(i ).substr( 0, posNom);
                enleverEspace( nom );
                extraireElement( fichier.at( i ), nom, arbreF );
            }

        }
    }
}




int main( int argc, char * argv[] )
{
    //On construit les arbres
    vector< string > fichier = lireFichier( argv[ 1 ] );
    ArbreAVL< string, vector<string> > arbreT = construireArbreType( fichier );
    ArbreAVL< string, vector<vector<string>> > arbreFoncteur = construireArbreFoncteur( fichier, arbreT );

    //On lit les requêtes par redirection du fichier les rassemblant
    vector< string > requetes;
    string commande;
    string ligne;
    do{
        getline( cin, ligne );
        requetes.push_back(ligne);

    }while( !cin.eof() );

    //Finalement on analyse les requêtes
    analyseRequete( requetes, arbreT, arbreFoncteur );

    return 0;
}
