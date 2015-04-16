#include <string>
#include <iostream>
#include "Domain.h"
using namespace std;

const int inputMaxLen = 128;

int main(int argc, char** argv)
{
	if (argc == 3 && argv[1] == string("add"))
	{
		// apwm add <name> - Add a new domain
		string name = argv[2];
		char user[inputMaxLen], pass[inputMaxLen];

		// Get master password
		//TODO

		// Ask user for username and password
		cout << "Enter username for " << name << ": ";
		cin.getline(user, inputMaxLen);
		cout << "Enter password for " << name << ": ";
		cin.getline(pass, inputMaxLen);

		// Add this domain
		Domain new_domain(name, user, pass);
	}
	else if (argc == 3 && argv[1] == string("get"))
	{
		// apwm get <name> - Get username och password for given domain

		// Get master password
		//TODO

		// Get domain and print username and password to screen
		Domain* domain = Domain::getDomain(argv[2]);

		if (domain != nullptr)
		{
			cout << "Username: " << domain->getName() << endl
				<< "Password: " << domain->getPass() << endl;
			delete domain;
		}
		else
		{
			cout << "No such domain." << endl;
		}
	}
	else if (argc == 2 && argv[1] == string("list"))
	{
		// apwm list - List all available domains
		int count;
		string* domains = Domain::listDomains(count);
		if (domains != nullptr)
		{
			for (int i = 0; i < count; i++)
			{
				cout << domains[i] << endl;
			}

			delete[] domains;
		}
	}
	else
	{
		// No valid arguments. Print usage message and exit.
		cout << "Andreas' password manager v1. Usage: " << endl << "apwm add <name>" << endl
			<< "apwm get <name" << endl << "apwm list" << endl;
		exit(0);
	}
}