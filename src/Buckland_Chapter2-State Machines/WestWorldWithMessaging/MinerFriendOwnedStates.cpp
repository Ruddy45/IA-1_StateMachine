#include "MinerFriendOwnedStates.h"
#include "fsm/State.h"
#include "MinerFriend.h"
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

CalmState* CalmState::Instance()
{
	static CalmState instance;

	return &instance;
}

void CalmState::Enter(MinerFriend* pMinerFriend)
{
}

void CalmState::Execute(MinerFriend* pMinerFriend)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);


	pMinerFriend->DrinkAWhiskey();
	cout << "\n" << GetNameOfEntity(pMinerFriend->ID()) << ": " << "A good whiskey, it's so good that it makes me alive!";

	if (pMinerFriend->IsDrunk())
	{
		pMinerFriend->GetFSM()->ChangeState(InstableTalk::Instance());
	}
}


void CalmState::Exit(MinerFriend* pMinerFriend)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMinerFriend->ID()) << ": " << "Why empty the bottle is? Hic";
}


bool CalmState::OnMessage(MinerFriend* pMinerFriend, const Telegram& msg)
{
	SetTextColor(BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	// All messages supported by mineur friend.
	switch (msg.Msg)
	{
	case Msg_HiFriendIWillDrink:

		cout << "\nMessage handled by " << GetNameOfEntity(pMinerFriend->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pMinerFriend->ID())
			<< ": Good! It will taste better. Good luck for your work and Bye!";

		// Message to tell the miner to get out of the saloon and get back to work.
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
								pMinerFriend->ID(),
								ent_Miner_Bob,
								Msg_GoodLuck,
								NO_ADDITIONAL_INFO);


		if (pMinerFriend->IsDrunk())
		{
			pMinerFriend->GetFSM()->ChangeState(InstableTalk::Instance());
		}

		return true;
	}

	return false;
}


//------------------------------------------------------------------------InstableTalk

InstableTalk* InstableTalk::Instance()
{
	static InstableTalk instance;

	return &instance;
}

void InstableTalk::Enter(MinerFriend* pMinerFriend)
{
}

void InstableTalk::Execute(MinerFriend* pMinerFriend)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMinerFriend->ID()) << ": " << "Hic!";

	// Sober up 
	pMinerFriend->SoberUp();

	// Return to normal after fulling sobering up
	if (pMinerFriend->GetAlcoholLevel() <= 0)
	{
		pMinerFriend->GetFSM()->ChangeState(CalmState::Instance());
	}
}


void InstableTalk::Exit(MinerFriend* pMinerFriend)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	// End message at the end of Bob dispute.
	cout << "\n" << GetNameOfEntity(pMinerFriend->ID()) << ": " << "What time is it ? I don't remember anything.";
}


bool InstableTalk::OnMessage(MinerFriend* pMinerFriend, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); 

	switch (msg.Msg)
	{
	case Msg_HiFriendIWillDrink:

		cout << "\nMessage handled by " << GetNameOfEntity(pMinerFriend->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pMinerFriend->ID())
			<< ": Go back to your home! My beer tastes so bad with you around!";

		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
								pMinerFriend->ID(),
								ent_Miner_Bob,
								Msg_GoBackToYourHome,
								NO_ADDITIONAL_INFO);

		return true;

	case Msg_YourAreAlreadyDrunk:

		cout << "\nMessage handled by " << GetNameOfEntity(pMinerFriend->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pMinerFriend->ID())
			<< ": Don't judge me and leave me alone. You are good for nothing! Hic!"
			<< " ... Like I will care about your opinion.";

		// Return to normal
		pMinerFriend->GetFSM()->ChangeState(CalmState::Instance());

		return true;
	}

	return false;
}