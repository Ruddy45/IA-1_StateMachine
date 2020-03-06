#include <fstream>
#include <time.h>
#include <vector> // Pour utiliser les tableaux dynamiques

#include "Locations.h"
#include "Miner.h"
#include "MinersWife.h"
#include "Drunken.h" // Nouvel Agent
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"

using namespace std;
std::ofstream os;


// Affiche les différents agents disponibles pour définir un état initial
void SelectAgent(vector<BaseGameEntity*>& listBaseGameEntity, unsigned int& indexAgentChoose, vector<string>& listState)
{
	do
	{
		cout << "Saisissez le numero de l'agent auquel vous voulez definir l'etat initial : \n";

		for (unsigned int i = 0; i < listBaseGameEntity.size(); ++i)
			cout << i << " : " << GetNameOfEntity(listBaseGameEntity[i]->ID()) << "\n"; // Affiche le nom des agents

		cin >> indexAgentChoose; // Demande sur l'agent à définir l'état initial

		if (indexAgentChoose < 0 || listBaseGameEntity.size() <= indexAgentChoose) // Vérifie les données de l'utilisateurs
			cout << "Vous n'avez pas entre la bonne valeur.\n";

	} while (indexAgentChoose < 0 || listBaseGameEntity.size() <= indexAgentChoose); // Boucle jusqu'à ce que l'utilisateur donne les bonnes informations

	cout << "\nVous avez choisie " << GetNameOfEntity(listBaseGameEntity[indexAgentChoose]->ID()) << "\n\n";

	listBaseGameEntity[indexAgentChoose]->GiveStateName(listState); // Récupère les états de l'agent sélectionné
}

// Affiche et prend la sélection de l'utilisateur des diffèrents états
void SelectInitialState(vector<string>& listState, unsigned int& indexStateChoose, vector<BaseGameEntity*>& const listBaseGameEntity, unsigned int& const indexAgentChoose)
{
	do
	{
		cout << "Choisissez l'etat initial voulu :\n";

		for (unsigned int i = 0; i < listState.size(); ++i)
			cout << i << " : " << listState[i] << "\n"; // Affiche les diffèrents noms d'états disponibles

		cin >> indexStateChoose; // Prend l'état choisi par l'utilisateur

		if (indexStateChoose < 0 || listState.size() <= indexStateChoose) // Vérifie les données de l'utilisateurs
			cout << "Vous n'avez pas entre la bonne valeur.\n";

	} while (indexStateChoose < 0 || listState.size() <= indexStateChoose);

	listBaseGameEntity[indexAgentChoose]->UserSetInitialState(indexStateChoose); // Définie à l'agent son état initial
}

int main()
{
	//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
	os.open("output.txt");
#endif

	//seed random number generator
	srand((unsigned)time(NULL));

	//create a miner
	Miner* Bob = new Miner(ent_Miner_Bob);

	//create his wife
	MinersWife* Elsa = new MinersWife(ent_Elsa);

	Drunken* Patrick = new Drunken(ent_Patrick); // Création du nouvel agent Soulard

	//register them with the entity manager
	EntityMgr->RegisterEntity(Bob);
	EntityMgr->RegisterEntity(Elsa);
	EntityMgr->RegisterEntity(Patrick); // Enregistrement du nouvel agent

	// Liste des entités agents, utilisés pour définir l'état initial
	vector<BaseGameEntity*> listBaseGameEntity;

	// Stocke toutes les entités pour les utilisés pour choisir l'état initial
	listBaseGameEntity.push_back(Bob);
	listBaseGameEntity.push_back(Elsa);
	listBaseGameEntity.push_back(Patrick);

	// Contient le nom des diffèrents états de l'agent choisie
	vector<string> listState;

	unsigned int indexAgentChoose; // Index de l'agent choisi par l'utilisateur
	unsigned int indexStateChoose; // Etat initial de l'agent choisi par l'utilisateur

	//------------------------------------------------------------------------
	/* Demande à l'utilisateur sur quel agent, 
	il veut définir l'état initial puis demande qu'elle état */
	//------------------------------------------------------------------------
	SelectAgent(listBaseGameEntity, indexAgentChoose, listState);
	SelectInitialState(listState, indexStateChoose, listBaseGameEntity, indexAgentChoose);

	//------------------------------------------------------------------------
	/*run Bob, Elsa and Patrick through a few Update calls*/
	//------------------------------------------------------------------------
  for (int i = 0; i < 30; ++i)
  { 
    Bob->Update();
    Elsa->Update();
	Patrick->Update(); // Nouvelle Agent

    //dispatch any delayed messages
    Dispatch->DispatchDelayedMessages();

    Sleep(800);
  }

  //tidy up
  delete Bob;
  delete Elsa;
  delete Patrick; // Supprime l'agent

  //wait for a keypress before exiting
  PressAnyKeyToContinue();


  return 0;
}