#include "ClientState.h"

// [Constructeur]
ClientState::ClientState()
{
	authenticated = false;
	username = nullptr; // nullptr
}

// [Méthode pour savoir si le client est connecter]
bool ClientState::isAuthenticated()
{
	if (authenticated == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// [Méthode pour changer l'authentification et l'username]
void ClientState::setAuthenticated(bool newAuthenticated, QString newUsername)
{
	this->authenticated = newAuthenticated;
	this->username = newUsername;
}