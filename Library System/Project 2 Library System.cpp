//Library System (Project #2).

//Libraries and namespaces:
#include<iostream>
#include<string>
#include<string.h>
#include<algorithm>
using namespace std;

//Global Variable declaration:
const int MAX_BOOKS = 120, MAX_USERS = 100;

struct book //Struct containing the properties and methods of book objects.
{
	int id, quantity;
	string title;
	
	book()
	{
		id = 0;
		quantity = 0;
		title = "";
	}
	
	book(string name, int newId, int newQuantity)
	{
		title = name;
		id = newId;
		quantity = newQuantity;
	}
};

//Global Functions Definitions:
bool sortBooksById(book &a, book &b) //This function must be passed as an argument in a .sort() function in order to sort books by their IDs.
{
	return (a.id < b.id);
}

bool sortBooksByName(book &a, book &b) //This function must be passed as an argument in a .sort() function in order to sort books alphabetically.
{
	return a.title < b.title;
}

struct user //Struct containing the properties and methods of user objects.
{
	int id;
	string name;
	string borrowing;

	user()
	{
		id = 0;
		name = "";
		borrowing = "";
	}

	user(int newId, string newName)
	{
		id = newId;
		name = newName;
	}	

	void borrowB(book &b)
	{
		borrowing = b.title;
	}

	string returnB()
	{
		string b = borrowing;
		borrowing = "";
		return b;
	}

};

bool sortUsersByName(user &a, user &b) //This function must be passed as argument in a .sort() function in order to sort users alphabetically.
{
	return a.name < b.name;	
}

struct librarySystem //Struct where all the features of the Library System are defined.
{
	book books[MAX_BOOKS];
	user users[MAX_USERS];
	int currentBooks, currentUsers;
	
	librarySystem() //Default Constructor
	{
		currentBooks = 0;
		currentUsers = 0;
		run();
	}
	
	void run() //Run Program Function.
	{
		short option;
		do
		{
			option = menu();
			switch(option)
			{
				case 1: //Adds a new Book. Done.
					addBook(); 
					break;
				case 2: //Searches books by a prefix. Done.
					searchByPrefix();
					break;
				case 3:
					searchUsersBorrowingBook();
					break;
				case 4: //Prints books by ID. Done.
					printById();
					break;
				case 5: //Prints books by name. Done.
					printByName();
					break;
				case 6: //Adds an user. Done.
					addUser();
					break;
				case 7:
					borrowBook();
					break;
				case 8:
					returnBook();
					break;
				case 9:
					printAllUsers();
					break;
				case 10:
					cout << "\nBye!";
					break;
				default:
					cout << "That option does not exist.";
					break;
			}
		}while(option != 10);
		
	}
	
	short menu() //Shows the option menu and returns the entered option. Done.
	{
		short opt;
		
		cout << "\n1) Add Book.\n2) Search Book by prefix.\n3) Print who borrowed book by name.\n4) Print Library (ID).\n5) Print Library (name).";
		cout << "\n6) Add user.\n7) User Borrow Book.\n8) User Return Book.\n9) Print all Users.\n10) Exit.\nEnter your option: ";
		cin >> opt;
		
		return opt;
	}
	
	void addBook() //Adds a new book to the library. Done.
	{
		if(currentBooks == MAX_BOOKS)
		{
			cout << "FULL LIBRARY\n";
			return;
		}
		
		string name;
		int id, quantity;
		
		cout << "Enter the name of the book: ";
		cin.ignore(); getline(cin, name);
		
		cout << "Enter the id and quantity: ";
		cin >> id >> quantity;
		
		books[currentBooks] = book(name, id, quantity);
		currentBooks++;
	}
	
	void addUser() //Adds a new user to the system. Done.
	{
		if(currentUsers == MAX_USERS)
		{
			cout << "USER DATABASE FULL\n";
			return;
		}
		
		string name;
		int id;
		
		cout << "Enter the new user's name: ";
		cin.ignore(); getline(cin, name);
		
		cout << "Enter their id: ";
		cin >> id;
		
		users[currentUsers] = user(id, name);
		currentUsers++;
	}
	
	void printById() //Prints books by their ID. 
	{
		std::sort(books, books+currentBooks, sortBooksById);
		for(int i = 0; i < currentBooks; i++)
			cout << "ID: " << books[i].id << " Title: " << books[i].title << " QT: " << books[i].quantity << endl;
		
		cout << endl;
	}
	
	void searchByPrefix()
	{
		if(currentBooks == 0)
		{
			cout << "No books available\n";
			return;
		}
		
		string query;
		int matches = 0;
		book matchedBooks[MAX_BOOKS];
		
		cout << "Enter the query: ";
		cin.ignore(); getline(cin, query);
		
		for(int i = 0; i < MAX_BOOKS; i++)
		{
			bool isMatch = 1;
			for(int j = 0; j < query.length(); j++)
			{
				if(query[j] == books[i].title[j])
					continue;
				else
				{
					isMatch = 0;
					break;
				}
			}
			
			if(isMatch)
			{
				matchedBooks[matches] = books[i];
				matches++;
			}
			
		}
		cout << matches << " matches found.\n";
		if(matches != 0)
			for(int i = 0; i < matches; i++)
				cout << matchedBooks[i].id << " " <<  matchedBooks[i].title << endl;
	}
	
	void printByName() //Prints books by their name.
	{
		std::sort(books, books+currentBooks, sortBooksByName);
		for(int i = 0; i < currentBooks; i++)
			cout << "ID: " << books[i].id << " Title: " << books[i].title << " QT: " << books[i].quantity << endl;
		
		cout << endl;
	}
	
	void searchUsersBorrowingBook()
	{
		std::sort(users, users+currentUsers, sortUsersByName);
		for(int i = 0; i < currentUsers; i++)
			if(users[i].borrowing.compare("") != 0)
				cout << "(" << users[i].id << ") " << users[i].name << ". Borrowing: " << users[i].borrowing;	
			
	}
	
	void borrowBook()
	{
		if(currentUsers == 0)
		{
			cout << "There are no users registered.";
			return;
		}
		string userName;
		int bookID, indexB, indexU;
		
		bool valid = 0;
		do
		{
			cout << "Enter the user's name: ";
			cin.ignore(); getline(cin, userName);
			
			for(int i = 0; i < currentUsers; i++)
				if(userName.compare(users[i].name) == 0)
				{
					indexU = i;
					valid = 1;
					break;
				}
			
			if(!valid)
				cout << "Invalid name.";
				
		}while(!valid);
		
		if(users[indexU].borrowing.compare("") != 0)
		{
			cout << "The user is already borrowing a book.";
			return;
		}
		
		do
		{
			valid = 0;
			cout << "Enter the ID of the book: ";
			cin >> bookID;
			
			for(int i = 0; i < currentBooks; i++)
			{
				if(bookID == books[i].id && books[i].quantity != 0)
				{
					indexB = i;
					valid = 1;
					break;
				}
				else if(bookID == books[i].id && books[i].quantity == 0)
				{
					cout << "There are no copies available.";
					return;
				}
			}
				
			
			if(!valid)
				cout << "Invalid ID.";
				
		}while(!valid);
		
		users[indexU].borrowB(books[indexB]);
		books[indexB].quantity--;
				
	}
	
	void returnBook()
	{
		string nameU, returningBook;
		bool valid;
		
		bool v = 0;
		do
		{
			cout << "Enter the user's name: ";
			cin.ignore(); getline(cin, nameU);
			
			for(int i = 0; i < currentUsers; i++)
				if(nameU.compare(users[i].name) == 0)
				{
					returningBook = users[i].returnB();
					v = 1;
					break;
				}
			
			if(!v)
				cout << "Invalid name.";
				
		}while(!v);

		for(int i = 0; i < currentBooks; i++)
		{
			if(returningBook.compare(books[i].title) == 0)
				books[i].quantity++;
		}
	}
	
	void printAllUsers()
	{
		std::sort(users, users+currentUsers, sortUsersByName);
		for(int i = 0; i < currentUsers; i++)
			cout << "ID: " << users[i].id << " Name: " << users[i].name << endl;
		
		cout << endl;
	}
	
};

int main(void) //Main Function.
{
	librarySystem();
}
