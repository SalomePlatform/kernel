// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  File   : DataIdFilter.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
//
/*   Module Filtre
 *   -------------
 *
 *   Implemente les fonctions de filtrage et conversion d'un port de DATASTREAM
 *
 *   Rappel des fonctions du Filtrage:
 *   --------------------------------
 *
 *   Dans une communication de type DATASTREAM, le destinataire indique à l'avance la liste
 *   des instances qu'il veut recevoir, c'est à dire celles qui lui sont nécessaires.
 *   Il indique pour cela la liste des 'times' et la liste des 'tags' qui
 *   caractérisent les instances désirées.
 *   Ces deux listes sont indépendantes. Toute instance dont les paramètres 'time' et
 *   'tag' figurent dans la liste des 'times' et respectivement dans la liste des
 *   'tags' est désirée par le destinataire.
 *   Par la suite, une telle instance sera acceptée par le port-DATASTREAM. Les autres
 *   seront rejetées.
 *
 *   Le filtrage consiste à limiter les valeurs possibles du paramètre TIME ou TAG (un
 *   entier). La liste des valeurs possibles est décrite sous la forme d'une liste de
 *   valeurs ou de séquences arithmétiques de valeurs.
 *   Exemple: 
 *     La liste 1; 3; 30:34; 40:50:2 autorise les valeurs 1 et 3 et toutes les valeurs
 *     comprises entre 30 et 34 inclus et toutes les valeurs de la séquence 40 à 50
 *     inclus par pas de 2, c'est à dire 40, 42, ... 50.
 *   On appelle règle élémentaire de filtrage celle spécifiant un élément de la liste
 *   des valeurs autorisées: soit une seule valeur, soit une séquence de valeurs. Une
 *   séquence de valeurs est spécifiée par sa valeur de départ, sa valeur de fin et
 *   son pas. Le filtrage est donc défini par une suite de règles de filtrage.
 *   La fonction élémentaire de configuration du filtrage sert à spécifier une règle
 *   de filtrage.
 *
 *   Rappels des fonctions de conversion:
 *   -----------------------------------
 *
 *   La conversion est intimement liée au filtrage car seules les valeurs passant le
 *   filtre sont converties. La conversion n'est pas obligatoire. Une valeur de TIME ou TAG
 *   entrante peut ne pas être convertie. Elle garde alors sa valeur et est gardée
 *   telle quelle pour l'objet destinataire.
 *   DATASTREAM peut associer une règle de conversion à chaque règle élémentaire de
 *   filtrage.
 *   La conversion consiste à changer:
 *     - un valeur de TIME ou TAG en une valeur différente
 *     - une séquence de valeurs en une autre séquence de valeurs de même taille
 *       (ex: 30:40 en 300:400:10)
 *   Mais la conversion permet aussi de transformer:
 *     - une valeur de TIME ou TAG unique en une séquence de valeurs (les données entrantes sont
 *       alors duppliquées et à chaque fois que l'objet destinataire réclame une donnée
 *       de la séquence, il reçoit en fait une copie de la donnée reçue une seule fois)
 *
 *     - une séquence de valeurs en une valeur unique (alors, chaque donnée entrante
 *       associée à un TIME ou TAG de la séquence correspond à une donnée unique pour le
 *       destinataire: seule la dernière reçue est la donnée valide)
 *
 */

#include <vector>
#include <iostream>

// Pour l'utilisation de "vector" de la STL
// Classe filtre_elementaire
//
// Implémente une structure de donnée décrivant un filtre élémentaire
// sur le paramètre TIME ou TAG; c'est
//    - soit une valeur entière unique
//    - soit une séquence arithmétique de valeurs
//
class filtre_elementaire
{
public:
    int len;    // Longueur de séquence ou 1 pour une valeur unique
    int debut;  // Début de la séquence ou valeur pour une valeur unique
    int fin;    // Fin de la séquence
    int pas;    // Pas de la séquence

    // Constructeur par défaut
    filtre_elementaire() {}
    
    // Création d'un filtre élémentaire pour une valeur unique
    filtre_elementaire(int valeur)
    {
        this->len = 1;
        this->debut = valeur;
    }

    // Création d'un filtre élémentaire pour une séquence de valeurs entières
    // Le pas par défaut est 1
    filtre_elementaire (int _debut, int _fin, int _pas=1)
    {
        this->debut = _debut;
        this->len = (_fin - _debut) / _pas;
        if (this->len > 0)
        {
            this->fin = _debut + _pas * this->len;  // Calcule la vrai borne de fin
            this->pas = _pas;
            this->len += 1;   // Compte les bornes et non les intervalles
        }
        else  // erreur de spécification: on ne prend que la première valeur
            this->len = 1;
    }

    // Constructeur par copie
    filtre_elementaire (filtre_elementaire &_f)
    {
      this->len = _f.len;
      this->debut = _f.debut;
      this->fin = _f.fin;
      this->pas = _f.pas;
    }
};

// Classe filtre_conversion
//
// Implémente le filtrage et la conversion du paramètre TIME ou TAG
// des données reçues par un port DATASTREAM.
//
// Mode d'emploi:
//      1) Création d'un objet
//      2) Configuration de cet objet par passage de paramètres
//         de filtage et de conversion
//      3) A la création d'un port DATASTREAM, on passe au constructeur
//         deux objets 'filtre_conversion', l'un pour le TIME, l'autre pour le TAG.
//      4) A l'utilisation du port DATASTREAM, celui-ci appelle la méthode
//         "applique_filtre_conversion" pour opérer
//
class filtre_conversion
{
private:
    // Structure de données décrivant une conversion élémentaire:
    // un filtre élementaire
    // et un pointeur éventuel vers les paramètres de conversion associés
    class conversion_elementaire
    {
    public :
        // Data
        filtre_elementaire filtre;
        filtre_elementaire * p_convers;

        // Constructeur
        conversion_elementaire() {}

        // Constructeur par copie d'un objet non modifie (const)
        conversion_elementaire (const conversion_elementaire& _ce)
        {
            *this = _ce;
        }
        // Remarque: le Constructeur par copie d'un objet existe par defaut mais sans le modificateur 'const'
        //           et l'emploi d'un objet comme element dans un vecteur oblige d'avoir un tel const-copy-constructor.
    };

    // Données de configuration de filtrage et conversion:
    //    une table de filtres élémentaires
    //    avec leurs données de conversion associées éventuelles
    std::vector<conversion_elementaire> config;

public:
    // Constructeur: juste une allocation mémoire initiale
    filtre_conversion() {}

    // Destructeur:
    // réclamer la mémoire utilisée par tous les éléments du vecteur config
    ~filtre_conversion()
    {
        std::vector<conversion_elementaire>::iterator i;
        for (i = this->config.begin(); i != this->config.end(); i ++)
        {
            delete (*i).p_convers;
        }
    }

    // Configuration partielle par ajout d'un filtre élémentaire
    bool config_elementaire (filtre_elementaire& _f)
    {
//    cout << "ajout config_elementaire 1  " << this << endl;
        conversion_elementaire conv_elem;
        
        conv_elem.filtre = _f;
        conv_elem.p_convers = NULL;

        // Ajoute cette conversion/filtrage elementaire a la liste
        this->config.push_back (conv_elem);
    
//    vector<conversion_elementaire>::iterator i;
//    cout << "liste apres ajout:" << endl;
//    for (i = this->config.begin(); i != this->config.end(); i ++)
//    {
//        cout << "config elem   " << endl;
//        cout << "filtre: len, debut, fin, pas " << (*i).filtre.len << " " << (*i).filtre.debut << " " << (*i).filtre.fin << " " << (*i).filtre.pas << endl;
//    }
        
        return true;
    }
    
    // Configuration partielle par ajout d'un filtre élémentaire
    // et sa conversion associée
    //
    // Retourne false si les param de conversion sont incompatibles avec le filtre élémentaire.
    // La configuration partielle est alors refusée.
    //
    bool config_elementaire (filtre_elementaire& _f, filtre_elementaire& _conv)
    {
//    cout << "ajout config_elementaire 2  " << this << endl;
    
        if (_f.len == 1 || _conv.len == 1 || _f.len == _conv.len)
        {
            conversion_elementaire conv_elem;
            conv_elem.filtre = _f;
            conv_elem.p_convers = new filtre_elementaire(_conv);

            // Ajoute cette conversion/filtrage elementaire a la liste
            this->config.push_back (conv_elem);
    
//    vector<conversion_elementaire>::iterator i;
//    cout << "liste apres ajout:" << endl;
//    for (i = this->config.begin(); i != this->config.end(); i ++)
//    {
//        cout << "config elem   " << endl;
//        cout << "filtre: len, debut, fin, pas " << (*i).filtre.len << " " << (*i).filtre.debut << " " << (*i).filtre.fin << " " << (*i).filtre.pas << endl;
//    }
        
            return true;
        }
        else
        {
            // Filtre et conversion incompatibles
            return false;
        }
    }

    // applique_filtre_conversion: Opération du filtre et de la conversion
    template <typename T > T applique_filtre_conversion (T valeur_initiale, std::vector<T>& liste_conversions) const;
};



// filtre_conversion::applique_filtre_conversion: Opération du filtre et de la conversion
//
// Etant donné une valeur entière (de TIME ou de TAG), cette méthode détermine :
//   - si cette valeur passe le filtre
//   - dans le cas où une conversion existe, la liste des valeurs de conversion
//     qui correspondent à la valeur initiale
//
// Dans tous les cas, cette méthode retourne une liste de valeurs.
// Dans le cas où il n'y a pas de conversion, cette liste a une longueur 1
// et ne contient que la valeur initiale.
//
// Paramètre d'entrée :         la valeur initiale (integer)
//
// Paramètre de sortie :        la liste des valeurs après conversion (vector<int>)
//
// Valeur de retour :           la longueur de la liste
//     si cette longueur est 0, c'est que la valeur initiale ne passe pas le filtre
//
template <typename T>
T filtre_conversion::applique_filtre_conversion (T valeur_initiale, std::vector<T>& liste_conversions) const
{
    // Part d'une liste vierge
    liste_conversions.clear();

//    cout << "config applique_filtre_conversion " << this << endl;
    
    // Balaye tous les éléments de configuration
    // et cherche pour chacun d'eux si la valeur initiale est présente parmi les valeurs filtrées

    // Pour tous les éléments de configuration du filtrage/conversion
    std::vector<conversion_elementaire>::const_iterator i;
    for (i = config.begin(); i != config.end(); i ++)
    {

//    cout << "config elem   " << endl;
//    cout << "filtre: len, debut, fin, pas " << (*i).filtre.len << " " << (*i).filtre.debut << " " << (*i).filtre.fin << " " << (*i).filtre.pas << endl;
    
        bool si_passe_filtre = false;

        // Si la longueur du filtre est 1
        if ((*i).filtre.len == 1) {
          // Si la valeur initiale correspond à la valeur du filtre
          if ((*i).filtre.debut == valeur_initiale)
            si_passe_filtre = true;
        } else  {
          // Si la valeur initiale est dans la séquence des valeurs du filtre
          //   la valeur est comprise dans les bornes [debut,fin]
          //   et sa distance du début de la séquence est modulo le pas
          if (  ((*i).filtre.fin - valeur_initiale >= 0) == (valeur_initiale - (*i).filtre.debut >= 0)
                &&  (valeur_initiale - (*i).filtre.debut) % (*i).filtre.pas == 0  ) {
            si_passe_filtre = true;
          }
        }

        // Si la valeur initiale passe le filtre
        if (si_passe_filtre) {
          //    cout << "config: filtre passe    " << endl;
            
          // Si il y a une conversion à effectuer
          if ((*i).p_convers != NULL) {

            // Si la longueur du filtre est 1
            if ((*i).filtre.len == 1) {

              // Si la longueur des paramètres de conversion est aussi 1
              if ((*i).p_convers->len == 1) {
                // Ajoute la valeur de conversion à la liste des valeurs après conversion
                liste_conversions.push_back ((*i).p_convers->debut);
              } else {
                // Ajoute la séquence de conversion à la liste des valeurs après conversion
                for (int s = (*i).p_convers->debut; s != (*i).p_convers->fin; s += (*i).p_convers->pas) {
                  liste_conversions.push_back (s);
                }
                liste_conversions.push_back ((*i).p_convers->fin);
              }

            } else {
              // Le filtre est une séquence qui est convertie en une autre séquence de même longueur
              // Choisit la valeur au rang désiré dans la séquence de conversion
              int rang = (valeur_initiale - (*i).filtre.debut) / (*i).filtre.pas;

              int valeur_convertie = (*i).p_convers->debut + rang * (*i).p_convers->pas;

              // Ajoute cette valeur à la liste des valeurs après conversion
              liste_conversions.push_back (valeur_convertie);
            }
          } else {
            // Ajoute la valeur initiale telle-quelle à la liste des valeurs après conversion
            liste_conversions.push_back (valeur_initiale);
          }
        }
    }

    return liste_conversions.size();
}
