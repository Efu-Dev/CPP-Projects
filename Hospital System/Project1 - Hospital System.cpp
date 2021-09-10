// Project 1 — Hospital System
// Diego Faria

#include<iostream>
#include<string>
using namespace std;

//Global Variables:
string patientList[15][5][2] = {{{""}}};
string patientQueue[75];
int countPatient[15] = {0};

//Function Declaration:
void printMenu();
void addPatient();
void showPatients();
void nextPatient();
void addList(string name, int spec, bool urgent);
bool isFull(int spec);

int main(void)
{

	//Local Variables:
	int option;

	do{
		printMenu();
		cin >> option;

		switch(option){
			case 1:
				addPatient();
				break;

			case 2:
				showPatients();
				break;

			case 3:
				nextPatient();
				break;

			case 4:
				break;

			default:
				cout << "\nThat option does not exist";
				break;

		}

		cout << endl << endl;

	}while(option != 4);

}

void printMenu()
{
	cout << "Option Menu: \n1) Add new patient.\n2) Show Patients.\n3) Get next patient.\n4) Exit.\n";
	cout << "Enter your option: ";
}

void addList(string name, int spec, bool urgent)
{
	if(urgent)
	{
		if(patientList[spec][0][0] != "")
			for(int i = 3; i >= 0; i--)
			{
				patientList[spec][i+1][0] = patientList[spec][i][0];
				patientList[spec][i+1][1] = patientList[spec][i][1];
			}

		patientList[spec][0][0] = name;
		patientList[spec][0][1] = "urgent";

	}
	else
		for(int i = 0; i < 5; i++)
			if(patientList[spec][i][0] == "")
			{
				patientList[spec][i][0] = name;
				patientList[spec][i][1] = "regular";
				break;
			}

	countPatient[spec]++;
}

bool isFull(int spec){
	bool full = true;

	for(int i = 0; i < 5; i++)
		if(patientList[spec][i][0] == "")
		{
			full = false;
			break;
		}


	return full;
}

void addPatient()
{
	int specialization;
	string name;
	bool urgent;

	do
	{
		cout << "Enter specialization (0 - 14): ";
		cin >> specialization;

		if(specialization >= 15 || specialization < 0)
		{
			cout << "Invalid input";
			continue;
		}


		if(isFull(specialization))
		{
			cout << "Specialization full.";
			return;
		}

	} while(specialization >= 15 || specialization < 0);

	cout << "Enter the patient's name: ";
	cin.ignore();
	getline(cin, name);

	cout << "Is it urgent? (0 = no, 1 = yes): ";
	cin >> urgent;

	addList(name, specialization, urgent);

}


void showPatients(){
	int spec;
	cout << "Enter specialization (0-14): ";
	cin >> spec;

	if(patientList[spec][0][0] == "")
		cout << "There are no patients in this specialization";
	else
		for(int i = 0; i < 5; i++)
		{
			if(patientList[spec][i][0] == "")
				break;

			cout << 1+i << ". " << patientList[spec][i][0] << " " << patientList[spec][i][1] << endl;
		}

}

void nextPatient(){
	int spec;
	cout << "Enter specialization: ";
	cin >> spec;

	if( patientList[spec][0][0] == "")
	{
		cout << "There is no one waiting.";
		return;
	}

	cout <<  patientList[spec][0][0] << "'s turn.";
	patientList[spec][0][0] = "";
	patientList[spec][0][1] = "";

	string auxname, auxstat;
	for(int i = 1; i <= countPatient[spec]; i++)
	{
		patientList[spec][i-1][0] = patientList[spec][i][0];
		patientList[spec][i-1][1] = patientList[spec][i][1];
	}

	countPatient[spec]--;
}
