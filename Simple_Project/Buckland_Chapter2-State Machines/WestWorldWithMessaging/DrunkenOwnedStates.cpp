#include "DrunkenOwnedStates.h"
#include "fsm/State.h"
#include "Drunken.h"
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


//------------------------------------------------------------------------CalmState

CalmState* CalmState::Instance() // Crée l'instance static de l'état calme
{
	static CalmState instance;

	return &instance;
}

void CalmState::Enter(Drunken* pDrunken)
{
}

void CalmState::Execute(Drunken* pDrunken)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	pDrunken->DrinkAWhiskey(); // Boit un whisky
	cout << "\n" << GetNameOfEntity(pDrunken->ID()) << ": " << "It's so good that it makes me alive!";

	if (pDrunken->isDrunken()) // Si l'agent est saoul, alors il change d'état
	{
		pDrunken->GetFSM()->ChangeState(InstableTalk::Instance());
	}
	else // Reboit jusqu'à être saoul
	{
		pDrunken->GetFSM()->ChangeState(CalmState::Instance());
	}
}


void CalmState::Exit(Drunken* pDrunken) // Sortie de l'état
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pDrunken->ID()) << ": " << "Why empty the bottle is? Hic";
}


bool CalmState::OnMessage(Drunken* pDrunken, const Telegram& msg) // Réception des messages
{
	SetTextColor(BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Couleurs des messages

	switch (msg.Msg) // Switch des diffèrents messages reçus
	{
	case Msg_HiFriendIWillDrink: // Message du mineur pour dire bonjour

		cout << "\nMessage handled by " << GetNameOfEntity(pDrunken->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pDrunken->ID())
			<< ": Good! It will taste better. Good luck for your work and Bye!";

		// Message pour dire au mineur de partir du saloon et de retourner travailler
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, // delai du message
			pDrunken->ID(),         // ID de l'émetteur
			ent_Miner_Bob,          // ID du destinataire
			Msg_GoodLuck,			// Message
			NO_ADDITIONAL_INFO);


		if (pDrunken->isDrunken()) // Si l'agent est saoul, alors il change d'état
		{
			pDrunken->GetFSM()->ChangeState(InstableTalk::Instance());
		}
		else // Reboit jusqu'à être saoul
		{
			pDrunken->GetFSM()->ChangeState(CalmState::Instance()); // Reste calme puisqu'il n'est pas saoul
		}

		return true;
	}

	return false;
}


//------------------------------------------------------------------------InstableTalk

InstableTalk* InstableTalk::Instance() // Crée l'instance static de l'état instable
{
	static InstableTalk instance;

	return &instance;
}

void InstableTalk::Enter(Drunken* pDrunken)
{
}

void InstableTalk::Execute(Drunken* pDrunken)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pDrunken->ID()) << ": " << "Hic!";
	pDrunken->SoberUp(); // Désaoule, pour revenir à l'état calme

	if (pDrunken->GiveDrunkLevel() <= 0)
		pDrunken->GetFSM()->ChangeState(CalmState::Instance()); // Change l'état, une fois qu'il à désaoulé
}


void InstableTalk::Exit(Drunken* pDrunken)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	cout << "\n" << GetNameOfEntity(pDrunken->ID()) << ": " << "What time is it ? I don't remember anything."; // Message de sortie de l'état querelle
}


bool InstableTalk::OnMessage(Drunken* pDrunken, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Couleurs messages

	switch (msg.Msg) // Message qu'il reçoit lorsqu'il est saoul
	{
	case Msg_HiFriendIWillDrink: // Cas où le mineur lui dit bonjour

		cout << "\nMessage handled by " << GetNameOfEntity(pDrunken->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pDrunken->ID())
			<< ": Go back to your home! My beer tastes so bad with you around!";

		// Message pour dire au mineur de partir du saloon
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, // delai du message
			pDrunken->ID(),        // ID de l'émetteur
			ent_Miner_Bob,            // ID du destinataire
			Msg_GoBackToYourHome,   // Message
			NO_ADDITIONAL_INFO);

		pDrunken->GetFSM()->ChangeState(InstableTalk::Instance()); // Reste en état de saoul

		return true;

	case Msg_YourAreAlreadyDrunk: // Deuxième message reçu du mineur

		cout << "\nMessage handled by " << GetNameOfEntity(pDrunken->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pDrunken->ID())
			<< ": Don't judge me and leave me alone. You are good for nothing! Hic!"
			<< " ... Like I will care about your opinion.";

		pDrunken->GetFSM()->ChangeState(CalmState::Instance()); // Passe à l'état calm car la discussion s'est terminée

		return true;
	}

	return false;
}