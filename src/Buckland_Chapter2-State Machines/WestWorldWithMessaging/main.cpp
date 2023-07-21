#include <fstream>
#include <time.h>
#include <vector>

#include "Locations.h"
#include "Miner.h"
#include "MinersWife.h"
#include "MinerFriend.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"

using namespace std;
std::ofstream os;

/// <summary>
/// Get selection command to set initial state in one character.
/// </summary>
/// <param name="characters">List of all characters / entity in the world.</param>
/// <returns>Character selected by the user.</returns>
BaseGameEntity* GetSelectedCharacter(const vector<BaseGameEntity*>& characters)
{
	bool bValidChoice = false;
	int chooseIndex;

	do
	{
		cout << "Enter the agent number for which you want to define the initial state : \n";

		for (auto i = 0; i < characters.size(); ++i)
			cout << i << " : " << GetNameOfEntity(characters[i]->ID()) << "\n";

		cin >> chooseIndex;

		bValidChoice = (0 <= chooseIndex && chooseIndex < characters.size());
		if (!bValidChoice)
			cout << "Incorrect Value, rety ^^ \n";

	// Loop until a character is chosen
	} while (!bValidChoice);

	cout << "\nYou choose " << GetNameOfEntity(characters[chooseIndex]->ID()) << "\n\n";

	return characters[chooseIndex];
}

/// <summary>
/// Set initial state based on user inputs.
/// </summary>
/// <param name="selectedCharacter"></param>
void SelectInitialState(BaseGameEntity* selectedCharacter)
{
	bool bValidChoice = false;
	int chooseIndex;

	auto stateList (selectedCharacter->GetStatesNames());

	do
	{
		cout << "Choose the initial state :\n";

		for (auto i = 0; i < stateList.size(); ++i)
			cout << i << " : " << stateList[i] << "\n";

		cin >> chooseIndex;

		bValidChoice = (0 <= chooseIndex && chooseIndex < stateList.size());
		if (!bValidChoice)
			cout << "Incorrect Value, rety ^^ \n";
	
	// Loop until a character is chosen
	} while (chooseIndex < 0 || stateList.size() <= chooseIndex);

	// Define initial state
	selectedCharacter->SetInitialState(chooseIndex);
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

	MinerFriend* Patrick = new MinerFriend(ent_Patrick);

	//register them with the entity manager
	EntityMgr->RegisterEntity(Bob);
	EntityMgr->RegisterEntity(Elsa);
	EntityMgr->RegisterEntity(Patrick);

	// Add all characters in character selection list.
	vector<BaseGameEntity*> characterSelectionList {Bob, Elsa, Patrick};

	//------------------------------------------------------------------------
	/* Debug Utils : Choose your character and it's initial state */
	//------------------------------------------------------------------------
	BaseGameEntity* selectedCharacter = GetSelectedCharacter(characterSelectionList);
	SelectInitialState(selectedCharacter);
	selectedCharacter = nullptr;

	//------------------------------------------------------------------------
	/*run Bob, Elsa and Patrick through a few Update calls*/
	//------------------------------------------------------------------------
  for (int i = 0; i < 30; ++i)
  { 
    Bob->Update();
    Elsa->Update();
	Patrick->Update();

    //dispatch any delayed messages
    Dispatch->DispatchDelayedMessages();

    Sleep(800);
  }

  //tidy up
  delete Bob;
  delete Elsa;
  delete Patrick;

  //wait for a keypress before exiting
  PressAnyKeyToContinue();


  return 0;
}