#include "MinersWife.h"

bool MinersWife::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}

void MinersWife::Update()
{
  //set text color to green
  SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
 
  m_pStateMachine->Update();
}

std::vector<std::string> MinersWife::GetStatesNames()
{
	return {
		"WifesGlobalState",
		"DoHouseWork",
		"VisitBathroom",
		"CookStew"
	};
}

bool MinersWife::SetInitialState(int initialState)
{
	{
		switch (initialState)
		{
		case 0:
			m_pStateMachine->SetCurrentState(WifesGlobalState::Instance());
			return true;

		case 1:
			m_pStateMachine->SetCurrentState(DoHouseWork::Instance());
			return true;

		case 2:
			m_pStateMachine->SetCurrentState(VisitBathroom::Instance());
			return true;

		case 3:
			m_pStateMachine->SetCurrentState(CookStew::Instance());
			return true;
		}

		return false;
	}
}
