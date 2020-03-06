#ifndef DRUNKEN_H
#define DRUNKEN_H
//------------------------------------------------------------------------
//
//  Name:   Drunken.h
//
//  Desc:   Une classe définissant l'agent saoul
//
//------------------------------------------------------------------------
#include <string>
#include <cassert>
#include <iostream>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "DrunkenOwnedStates.h"
#include "fsm/StateMachine.h"

template <class entity_type> class State;
struct Telegram;

const int DrunkLevel = 5; // Quantité maximal avant qu'il soit saoul et soit dans un état instable


class Drunken : public BaseGameEntity
{
private:

	StateMachine<Drunken>* m_pStateMachine;

	location_type         m_Location; // Localisation du saoulard

	int                   m_iDrunken; // Plus la valeur est haute, plus il est "saoul"


public:

	Drunken(int id) :m_Location(saloon), // Il se trouve toujours au saloon
		m_iDrunken(0), // Il n'est pas saoul de base
		BaseGameEntity(id)
	{
		m_pStateMachine = new StateMachine<Drunken>(this);
		m_pStateMachine->SetCurrentState(CalmState::Instance());
	}

	~Drunken() { delete m_pStateMachine; }

	void Update();

	virtual bool  HandleMessage(const Telegram& msg); // Permet de gérer les messages


	StateMachine<Drunken>* GetFSM()const { return m_pStateMachine; } // Donne la MEF de l'agent


	// Renvoie un tableau des diffèrents états de l'agent 
	// à l'utilisateur pour qu'il choisisse l'état initial
	void GiveStateName(std::vector<std::string>& list)
	{
		list.push_back("CalmState");
		list.push_back("InstableTalk");
	};


	// L'utilisateur définie l'état initial de l'agent
	bool  UserSetInitialState(const int& initialState)
	{
		switch (initialState)
		{
		case 0:
			m_pStateMachine->SetCurrentState(CalmState::Instance());
			return true;

		case 1:
			m_pStateMachine->SetCurrentState(InstableTalk::Instance());
			return true;
		}

		return false;
	}

	//-------------------------------------------------------------accessors
	location_type Location()const { return m_Location; } // Demande de localisation

	bool          isDrunken()const; // Renvoie true s'il est saoul
	int 		  GiveDrunkLevel() { return m_iDrunken; } // Donne le taux actuel du saoulard
	void          DrinkAWhiskey() { ++m_iDrunken; } // Boit un Whisky
	void          SoberUp() { if (0 < m_iDrunken) --m_iDrunken; } // Désaoule
};
#endif