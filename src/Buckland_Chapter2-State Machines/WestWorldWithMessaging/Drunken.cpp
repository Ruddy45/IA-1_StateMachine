#include "Drunken.h" // Ajoute le fichier en tête de la classe

bool Drunken::HandleMessage(const Telegram& msg) // Ajoute la capacité de reçevoir un message selon l'état
{
	return m_pStateMachine->HandleMessage(msg);
}


void Drunken::Update()
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Couleur des dialogues de l'agent
	
	m_pStateMachine->Update(); // Update de l'état courant
}


bool Drunken::isDrunken()const // Si l'agent est saoul ou non
{
	if (m_iDrunken >= DrunkLevel)
		return true;

	return false;
}
