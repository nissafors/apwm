#include <string>
#include <iostream>
#include <vector>
#include "Domain.h"
using namespace std;

const int inputMaxLen = 128;
const string path = "apwd.txt";

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
		Domain domain(name, user, pass);
		domain.writeDomain(path);
	}
	else if (argc == 3 && argv[1] == string("get"))
	{
		// apwm get <name> - Get username och password for given domain

		// Get master password
		//TODO

		// Get domain and print username and password to screen
		Domain domain;
		if (domain.readDomain(argv[2], path))
		{
			cout << "Username: " << domain.getUser() << endl
				<< "Password: " << domain.getPass() << endl;
		}
		else
		{
			cout << "Domain not found." << endl;
		}
	}
	else if (argc == 2 && argv[1] == string("list"))
	{
		// apwm list - List all available domains
		vector<string> domains = Domain::listDomains(path);
		if (domains.size() > 0)
		{
			for each (string domain in domains)
			{
				cout << domain << endl;
			}
		}
		else
		{
			cout << "No domains found.";
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