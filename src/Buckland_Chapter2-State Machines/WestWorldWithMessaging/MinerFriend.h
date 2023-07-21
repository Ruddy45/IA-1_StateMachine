#ifndef DRUNKEN_H
#define DRUNKEN_H
//------------------------------------------------------------------------
//
//  Name:   MinerFriend.h
//
//  Desc:   Class definie Bob Friend.
//
//------------------------------------------------------------------------
#include <string>
#include <cassert>
#include <iostream>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "MinerFriendOwnedStates.h"
#include "fsm/StateMachine.h"

template <class entity_type> class State;
struct Telegram;

// Max quantity of alcool to swich in drunk state
const int MAX_ALCOHOL_LEVEL = 5;


class MinerFriend : public BaseGameEntity
{
private:

	StateMachine<MinerFriend>* m_pStateMachine;

	location_type         m_Location;

	int                   m_iCurrentAlcoholLevel;


public:

	// At first, he is always at the saloon with all his mind.
	MinerFriend(int id) :m_Location(saloon),
		m_iCurrentAlcoholLevel(0),
		BaseGameEntity(id)
	{
		m_pStateMachine = new StateMachine<MinerFriend>(this);
		m_pStateMachine->SetCurrentState(CalmState::Instance());
	}

	~MinerFriend() { delete m_pStateMachine; }

	void Update();

	virtual bool  HandleMessage(const Telegram& msg);


	StateMachine<MinerFriend>* GetFSM() const { return m_pStateMachine; }

	std::vector<std::string> GetStatesNames();

	bool  SetInitialState(int initialState);

	//-------------------------------------------------------------accessors
	location_type Location()const { return m_Location; } // Demande de localisation

	bool          IsDrunk() const { return MAX_ALCOHOL_LEVEL <= m_iCurrentAlcoholLevel;	}
	int 		  GetAlcoholLevel() { return m_iCurrentAlcoholLevel; }
	void          DrinkAWhiskey() { ++m_iCurrentAlcoholLevel; }
	void          SoberUp();
};
#endif