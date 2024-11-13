# Table des matieres

- [Balises](#balises)
- [Doxygen](#doxygen)
- [Concretement pour le projet](#concretement-pour-le-projet)

# Doxygen

## Utilite

- Générer une documentation exhaustive et structurée de manière automatique.
- Maintenir une documentation à jour en fonction des changements dans le code.
- Faciliter la collaboration entre développeurs en rendant le code plus lisible.
- Permettre à d'autres développeurs de comprendre rapidement le fonctionnement du code.

## Comment l utiliser

1. Installer Doxygen : Téléchargez et installez Doxygen depuis le site officiel.
2. Créer un fichier de configuration : Dans le terminal, exécutez doxygen -g pour générer un fichier de configuration nommé Doxyfile.
3. Configurer Doxyfile : Ouvrez le fichier Doxyfile pour y configurer les options de documentation (comme les chemins de fichiers, formats de sortie, etc.).
4. Commenter le code : Utilisez les balises Doxygen dans le code (voir ci-dessous) pour documenter les classes, fonctions, et variables.
5. Générer la documentation : Dans le terminal, exécutez doxygen Doxyfile pour générer la documentation.

# Concretement pour le projet

On va devoir dockeriser l utilisation de doxygen avec un petit dockerfile du genre:  
```Dockerfile
FROM alpine:latest
RUN apk add --no-cache doxygen graphviz
WORKDIR /docs
COPY Doxyfile /docs/Doxyfile
COPY src /docs/src
CMD ["doxygen", "Doxyfile"]
```  

1. On va devoir analyser le code source en C pour extraire les signatures[^1] de fonctions, variables gloables, structures... On va pouvoir se renseigner sur ce qu est la bibliotheque **clang.cindex** en python.

2. On va devoir generer des commentaires avec ces signatures. On va envoyer une requete au LLM pour chaque element avec un petit prompt ou je pense on demandera les bonnes balises en fonction de la signature. Donc un @brief/@details pour une fonction avec evidemment ses @param, @return... Puis evidemment inserer ces commentaires au bon endroit en faisant en sorte qu ils aient le bon format pour doxygen.

3. Integrer Doxygen pour generer la documentation

4. Faire des tests

5. Sinon pour deployer la documentation en ligne, il faut creer un fichier **.github/workflows/doxygen.yml** dans le depot pour generer une github action[^2] puis configurer le fichier de l action github. Ensuite il faudra que l utilisateur active github pages en selectionnant la page cree par l action.

# Balises

1. [Balises de documentation générales](#balises-de-documentation-générales)
2. [Balises de documentation de fichier et de groupe](#balises-de-documentation-de-fichier-et-de-groupe)
3. [Balises de documentation de classes et de structures](#balises-de-documentation-de-classes-et-de-structures)
4. [Balises pour les fonctions et les méthodes](#balises-pour-les-fonctions-et-les-méthodes)
5. [Balises pour les variables](#balises-pour-les-variables)
6. [Balises pour les commentaires de code](#balises-pour-les-commentaires-de-code)
7. [Balises de navigation et de référence](#balises-de-navigation-et-de-référence)
8. [Balises pour l'organisation de la documentation](#balises-pour-lorganisation-de-la-documentation)
9. [Balises pour les notes et avertissements](#balises-pour-les-notes-et-avertissements)
10. [Balises pour les tableaux, listes et images](#balises-pour-les-tableaux-listes-et-images)
11. [Balises diverses](#balises-diverses)
12. [Table des matieres](#table-des-matieres)

## Balises de documentation générales

[Balises](#balises)  
-    @brief : Résumé rapide, souvent utilisé en introduction.
-    @details : Fournit une description détaillée, souvent placée après @brief.
-    @author : Indique l’auteur du fichier ou de la fonction.
-    @version : Version du fichier, de la fonction ou de la classe.
-    @date : Date de création ou de modification.
-    @since : Indique depuis quelle version la fonctionnalité existe.
-    @deprecated : Signale qu'une fonctionnalité est obsolète ou ne devrait plus être utilisée.

## Balises de documentation de fichier et de groupe
[Balises](#balises)  
-    @file : Documentation d'un fichier.
-    @defgroup : Définir un groupe de fonctions ou de classes (pour organiser la documentation).
-    @ingroup : Associe une fonction ou classe à un groupe défini avec @defgroup.
-    @addtogroup : Ajoute des éléments dans un groupe existant.
-    @namespace : Documentation d'un espace de noms.
-    @internal : Marque une section réservée pour les développeurs internes.

## Balises de documentation de classes et de structures
[Balises](#balises)  
-    @class : Documente une classe ou une structure.
-    @struct : Documente une structure.
-    @union : Documente une union.
-    @interface : Documente une interface.
-    @enum : Documente une énumération.
-    @typedef : Documente un type défini.
-    @def : Documente une macro
-    @protocol : Utilisé pour documenter un protocole (surtout en Objective-C).

## Balises pour les fonctions et les méthodes
[Balises](#balises)  
-    @param : Documentation d'un paramètre de la fonction (par exemple @param name Description).
-    @return : Description de la valeur de retour.
-    @retval : Documente les valeurs spécifiques de retour avec une explication.
-    @throws ou @exception : Documente les exceptions lancées par la fonction.
-    @deprecated : Indique qu'une fonction est obsolète.

## Balises pour les variables
[Balises](#balises)  
-    @var : Documente une variable.
-    @const : Documente une constante.

## Balises pour les commentaires de code
[Balises](#balises)  
-    @code ... @endcode : Utilisé pour inclure des exemples de code.
-    @snippet : Insère un extrait de code à partir d’un fichier source externe.
-    @dontinclude : Exclut des portions d'un fichier lors de l’inclusion dans la documentation.
-    @include : Inclut un fichier ou une portion de code.
-    @line : Inclut une ligne spécifique de code.
-    @skip ... @until : Sauter du texte jusqu'à un certain point dans la documentation.

## Balises de navigation et de référence
[Balises](#balises)  
-    @see : Référence vers d'autres éléments de la documentation.
-    @ref : Crée une référence vers une autre section.
-    @anchor : Définit une ancre pour référencer une section.
-    @copydoc : Copie la documentation d'un autre élément.
-    @copybrief : Copie seulement le résumé d'un autre élément.

## Balises pour l'organisation de la documentation
[Balises](#balises)  
-    @section et @subsection : Crée une section ou sous-section dans la documentation.
-    @subsubsection : Crée un niveau de sous-section supplémentaire.
-    @paragraph : Crée un paragraphe dans la documentation.
-    @subparagraph : Crée un sous-paragraphe.

## Balises pour les notes et avertissements
[Balises](#balises)  
-    @note : Ajoute une note explicative.
-    @warning : Ajoute un avertissement.
-    @attention : Met en évidence une section à lire attentivement.
-    @todo : Marque un élément à compléter ou améliorer.

## Balises pour les tableaux, listes et images
[Balises](#balises)  
-    @image : Insère une image.
-    @table : Crée un tableau de données.
-    @li : Élément de liste.
-    @arg : Ajoute un argument dans une liste de descriptions.

## Balises diverses
[Balises](#balises)  
-    @mainpage : Définit la page d’accueil de la documentation.
-    @page : Crée une page dédiée à un sujet spécifique.
-    @cond ... @endcond : Conditionne l'inclusion de certaines parties du code dans la documentation.
-    @nosubgrouping : Désactive le regroupement automatique des membres de classes.

[^1]: **signatures** : L extraction de signatures de fonctions par exemple revient a trouver la ligne qui definit la fonction sans inclure le corps de la fonction.
[^2]: **github action**: Une GitHub Action est un workflow automatisé dans GitHub, conçu pour exécuter des tâches spécifiques en réponse à des événements, comme un push ou une pull request par exemple. Les GitHub Actions permettent d’intégrer l'intégration continue (CI) et le déploiement continu (CD) directement dans vos projets GitHub, facilitant l’automatisation des tests, la génération de documentation, le déploiement de projets, et bien plus encore.