#include "MinerFriend.h"

bool MinerFriend::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}


void MinerFriend::Update()
{
	// Set dialog color for Miner Friend
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	
	// Update current State
	m_pStateMachine->Update();
}

std::vector<std::string> MinerFriend::GetStatesNames()
{
	return std::vector<std::string>({ "CalmState", "InstableTalk" });
};

bool MinerFriend::SetInitialState(int initialState)
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

void MinerFriend::SoberUp()
{
	if (0 < m_iCurrentAlcoholLevel)
	{
		--m_iCurrentAlcoholLevel;
	}
}