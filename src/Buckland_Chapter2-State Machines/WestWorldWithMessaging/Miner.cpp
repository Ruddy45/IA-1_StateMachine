#include "Miner.h"

bool Miner::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}

void Miner::Update()
{
  SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);

  m_iThirst += 1;
  
  m_pStateMachine->Update();
}

std::vector<std::string> Miner::GetStatesNames()
{
	return {
		"EnterMineAndDigForNugget",
		"VisitBankAndDepositGold",
		"GoHomeAndSleepTilRested",
		"QuenchThirst",
		"EatStew"
	};
}

bool Miner::SetInitialState(int initialState)
{
	switch (initialState)
	{
	case 0:
		m_pStateMachine->SetCurrentState(EnterMineAndDigForNugget::Instance());
		return true;

	case 1:
		m_pStateMachine->SetCurrentState(VisitBankAndDepositGold::Instance());
		return true;

	case 2:
		m_pStateMachine->SetCurrentState(GoHomeAndSleepTilRested::Instance());
		return true;

	case 3:
		m_pStateMachine->SetCurrentState(QuenchThirst::Instance());
		return true;

	case 4:
		m_pStateMachine->SetCurrentState(EatStew::Instance());
		return true;
	}

	return false;
}

void Miner::AddToGoldCarried(const int val)
{
  m_iGoldCarried += val;

  if (m_iGoldCarried < 0) m_iGoldCarried = 0;
}

void Miner::AddToWealth(const int val)
{
  m_iMoneyInBank += val;

  if (m_iMoneyInBank < 0) m_iMoneyInBank = 0;
}

bool Miner::Thirsty()const
{
  if (m_iThirst >= ThirstLevel){return true;}

  return false;
}

bool Miner::Fatigued()const
{
  if (m_iFatigue > TirednessThreshold)
  {
    return true;
  }

  return false;
}
