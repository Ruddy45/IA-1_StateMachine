#include "MinerOwnedStates.h"
#include "fsm/State.h"
#include "Miner.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif


//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance()
{
  static EnterMineAndDigForNugget instance;

  return &instance;
}


void EnterMineAndDigForNugget::Enter(Miner* pMiner)
{
  SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
  //if the miner is not already located at the goldmine, he must
  //change location to the gold mine
  if (pMiner->Location() != goldmine)
  {
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Walkin' to the goldmine";

    pMiner->ChangeLocation(goldmine);
  }
}


void EnterMineAndDigForNugget::Execute(Miner* pMiner)
{  
  //Now the miner is at the goldmine he digs for gold until he
  //is carrying in excess of MaxNuggets. If he gets thirsty during
  //his digging he packs up work for a while and changes state to
  //gp to the saloon for a whiskey.
  pMiner->AddToGoldCarried(1);

  pMiner->IncreaseFatigue();

  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Pickin' up a nugget";

  //if enough gold mined, go and put it in the bank
  if (pMiner->PocketsFull())
  {
    pMiner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
  }

  if (pMiner->Thirsty())
  {
    pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
  }
}


void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
       << "Ah'm leavin' the goldmine with mah pockets full o' sweet gold";
}


bool EnterMineAndDigForNugget::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

//------------------------------------------------------------------------methods for VisitBankAndDepositGold

VisitBankAndDepositGold* VisitBankAndDepositGold::Instance()
{
  static VisitBankAndDepositGold instance;

  return &instance;
}

void VisitBankAndDepositGold::Enter(Miner* pMiner)
{  
  //on entry the miner makes sure he is located at the bank
  if (pMiner->Location() != bank)
  {
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Goin' to the bank. Yes siree";

    pMiner->ChangeLocation(bank);
  }
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
  //deposit the gold
  pMiner->AddToWealth(pMiner->GoldCarried());
    
  pMiner->SetGoldCarried(0);

  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
       << "Depositing gold. Total savings now: "<< pMiner->Wealth();

  //wealthy enough to have a well earned rest?
  if (pMiner->Wealth() >= ComfortLevel)
  {
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
         << "WooHoo! Rich enough for now. Back home to mah li'lle lady";
      
    pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());      
  }

  //otherwise get more gold
  else 
  {
    pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }
}


void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Leavin' the bank";
}


bool VisitBankAndDepositGold::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}
//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested

GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance()
{
  static GoHomeAndSleepTilRested instance;

  return &instance;
}

void GoHomeAndSleepTilRested::Enter(Miner* pMiner)
{
  if (pMiner->Location() != shack)
  {
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Walkin' home";

    pMiner->ChangeLocation(shack); 

    //let the wife know I'm home
    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pMiner->ID(),        //ID of sender
                              ent_Elsa,            //ID of recipient
                              Msg_HiHoneyImHome,   //the message
                              NO_ADDITIONAL_INFO);
  }
}

void GoHomeAndSleepTilRested::Execute(Miner* pMiner)
{ 
  //if miner is not fatigued start to dig for nuggets again.
  if (!pMiner->Fatigued())
  {
     cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
          << "All mah fatigue has drained away. Time to find more gold!";

     pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }

  else 
  {
    //sleep
    pMiner->DecreaseFatigue();

    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "ZZZZ... ";
  } 
}

void GoHomeAndSleepTilRested::Exit(Miner* pMiner)
{ 
}


bool GoHomeAndSleepTilRested::OnMessage(Miner* pMiner, const Telegram& msg)
{
   SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

   switch(msg.Msg)
   {
   case Msg_StewReady:

     cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID()) 
     << " at time: " << Clock->GetCurrentTime();

     SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

     cout << "\n" << GetNameOfEntity(pMiner->ID()) 
          << ": Okay Hun, ahm a comin'!";

     pMiner->GetFSM()->ChangeState(EatStew::Instance());
      
     return true;

   }//end switch

   return false; //send message to global message handler
}

//------------------------------------------------------------------------QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
  static QuenchThirst instance;

  return &instance;
}

void QuenchThirst::Enter(Miner* pMiner)
{
  if (pMiner->Location() != saloon)
  {    
    pMiner->ChangeLocation(saloon);

    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Boy, ah sure is thusty! Walking to the saloon";
  }
}

void QuenchThirst::Execute(Miner* pMiner)
{
	pMiner->BuyAndDrinkAWhiskey(); // Achète et boit un whisky

	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "That's mighty fine sippin' liquer";

	// Message pour dire au saoulard que bob est au saloon
	Dispatch->DispatchMessage(0.001, // delai du message
		pMiner->ID(),        // ID de l'émetteur
		ent_Patrick,            // ID du destinataire
		Msg_HiFriendIWillDrink,   // Message
		NO_ADDITIONAL_INFO);

	pMiner->GetFSM()->ChangeState(TalkWithDrunken::Instance()); // Boit un verre pour ensuite discutter avec le saoulard selon son état
}


void QuenchThirst::Exit(Miner* pMiner)
{
cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": Feelin' good again";
}

bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------TalkWithDrunken

TalkWithDrunken* TalkWithDrunken::Instance() // Crée l'instance static de l'état querelle
{
	static TalkWithDrunken instance;

	return &instance;
}

void TalkWithDrunken::Enter(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": Hello!";
}

void TalkWithDrunken::Execute(Miner* pMiner)
{
}

void TalkWithDrunken::Exit(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY); // Couleurs messages
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": Bye! And don't drink too much!";
}

bool TalkWithDrunken::OnMessage(Miner* pMiner, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Couleurs messages

	switch (msg.Msg) // Message qu'il reçoit de l'agent Drunken
	{
	case Msg_GoodLuck: // Message reçu si l'autre agent n'est pas saoul

		cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pMiner->ID())
			<< ": Thanks, have a nice day!";

		pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance()); // Retourne à la mine

		return true;

	case Msg_GoBackToYourHome: // Message reçu si l'autre agent est saoul

		cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pMiner->ID())
			<< ": Oh God... Your are already drunk at this hour. You are hopeless...";

		// Message pour dire au saoulard, qu'il est déjà saoul à cette heure
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, // delai du message
			pMiner->ID(),        // ID de l'émetteur
			ent_Patrick,            // ID du destinataire
			Msg_YourAreAlreadyDrunk,   // Message
			NO_ADDITIONAL_INFO);

		pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance()); // Retourne à la mine

		return true;
	}

	return false;
}

//------------------------------------------------------------------------EatStew

EatStew* EatStew::Instance()
{
	static EatStew instance;

	return &instance;
}


void EatStew::Enter(Miner* pMiner)
{
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Smells Reaaal goood Elsa!";
}

void EatStew::Execute(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Tastes real good too!";

  // Modification pour permettre de choisir cet état initial car pas d'état précédent
  // Va à la mine si le Mineur est fatigué ou qu'il n'y a pas d'état précédent au eat stew
  // sinon reviens à la maison
  if (!pMiner->Fatigued() || !pMiner->GetFSM()->PreviousState())
  {
	  pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }
  else // S'il l'état précédent existe et qu'il n'est pas fatigué
  {
	  pMiner->GetFSM()->RevertToPreviousState();
  }
}

void EatStew::Exit(Miner* pMiner)
{ 
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Thankya li'lle lady. Ah better get back to whatever ah wuz doin'";
}


bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}