#include "Drunken.h" // Ajoute le fichier en t�te de la classe

bool Drunken::HandleMessage(const Telegram& msg) // Ajoute la capacit� de re�evoir un message selon l'�tat
{
	return m_pStateMachine->HandleMessage(msg);
}


void Drunken::Update()
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Couleur des dialogues de l'agent
	
	m_pStateMachine->Update(); // Update de l'�tat courant
}


bool Drunken::isDrunken()const // Si l'agent est saoul ou non
{
	if (m_iDrunken >= DrunkLevel)
		return true;

	return false;
}
