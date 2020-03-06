#ifndef DRUNKEN_OWNED_STATES_H
#define DRUNKEN_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   DrunkenOwnedStates.h
//
//  Desc:   Tous les étas de la classe Drunken
//
//------------------------------------------------------------------------
#include "fsm/State.h"


class Drunken;
struct Telegram;


//------------------------------------------------------------------------
//
//  Il est calme jusqu'à ce qu'il ait atteint sa limite d'alcool. Boit de 
//	l'alcool dans cet état. Quand le mineur est dans le saloon, il va lui parler calmement
//	Quand il atteint sa limite d'alcool, il va aller dans l'état InstableState.
//
//------------------------------------------------------------------------
class CalmState : public State<Drunken>
{
private:

	CalmState() {}

	CalmState(const CalmState&);
	CalmState& operator=(const CalmState&);

public:

	static CalmState* Instance();

	virtual void Enter(Drunken* drunken);

	virtual void Execute(Drunken* drunken);

	virtual void Exit(Drunken* drunken);

	virtual bool OnMessage(Drunken* agent, const Telegram& msg);
};


//------------------------------------------------------------------------
//
//  Il est saoul et il va chercher à se quereller avec le mineur jusqu'à ce que le Dunken
//	désaoule ou que la discussion se termine. Quand il sera désaoulé ou que la 
//	discussion avec le mineur est terminée, il reviendra à l'état calme.
//
//------------------------------------------------------------------------
class InstableTalk : public State<Drunken>
{
private:

	InstableTalk() {}

	InstableTalk(const InstableTalk&);
	InstableTalk& operator=(const InstableTalk&);

public:

	static InstableTalk* Instance();

	virtual void Enter(Drunken* drunken);

	virtual void Execute(Drunken* drunken);

	virtual void Exit(Drunken* drunken);

	virtual bool OnMessage(Drunken* agent, const Telegram& msg);
};

#endif