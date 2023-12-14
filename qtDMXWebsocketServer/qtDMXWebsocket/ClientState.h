#pragma once
#include <qstring.h>

class ClientState
{
public:
	ClientState(); // Constructeur
	bool isAuthenticated(); // Méthode d'authentification
	void setAuthenticated(bool newAuthenticated, QString newUsername); // Changer l'authentification

private:
	bool authenticated;
	QString username;
};

