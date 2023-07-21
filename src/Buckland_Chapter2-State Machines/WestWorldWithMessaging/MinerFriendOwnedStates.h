#ifndef DRUNKEN_OWNED_STATES_H
#define DRUNKEN_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   MinerFriendOwnedStates.h
//
//  Desc:   All states of MinerFriend class
//
//------------------------------------------------------------------------
#include "fsm/State.h"

class MinerFriend;
struct Telegram;

//------------------------------------------------------------------------
//
// He's calm until he reaches his alcohol limit. When Bob is in the saloon, 
// he'll talk to him calmly. When he reaches his alcohol limit, he'll go 
// into the UnstableState.
//
//------------------------------------------------------------------------
class CalmState : public State<MinerFriend>
{
private:

	CalmState() {}

	CalmState(const CalmState&);
	CalmState& operator=(const CalmState&);

public:

	static CalmState* Instance();

	virtual void Enter(MinerFriend* pMinerFriend);

	virtual void Execute(MinerFriend* pMinerFriend);

	virtual void Exit(MinerFriend* pMinerFriend);

	virtual bool OnMessage(MinerFriend* agent, const Telegram& msg);
};


//------------------------------------------------------------------------
//
// He's drunk and will try to quarrel with Bob until he sobers up or the 
// discussion ends. Then he will return to his calm state.
//
//------------------------------------------------------------------------
class InstableTalk : public State<MinerFriend>
{
private:

	InstableTalk() {}

	InstableTalk(const InstableTalk&);
	InstableTalk& operator=(const InstableTalk&);

public:

	static InstableTalk* Instance();

	virtual void Enter(MinerFriend* pMinerFriend);

	virtual void Execute(MinerFriend* pMinerFriend);

	virtual void Exit(MinerFriend* pMinerFriend);

	virtual bool OnMessage(MinerFriend* agent, const Telegram& msg);
};

#endif