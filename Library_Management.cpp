#include<bits/stdc++.h>

using namespace std;

int days_in_month_arr[12]={31,28,31,30,31,30,31,31,30,31,30,31};

class User;

void space(int n)
{
	for (int i=0;i<n;i++)
	{
		cout<<" ";
	}
	cout<<"|";
}

struct Time {
    int d, m, y;
};

int days_in_month(int month,int year)
{
	if(month==1)
	{
		if(year%4==0)
		{
			if(year%100==0)
			{
				if(year%400==0)
				{
					return 29;
				}
				else
					return 28;
			}
			else
				return 29;
		}
	}
	return days_in_month_arr[month];
}

Time* add_days(int days,Time date)
{
	Time* due_date=(Time*)malloc(sizeof(Time));
	int d,m,y;
	due_date->d=date.d+days;
	due_date->m=date.m;
	due_date->y=date.y;
	while(due_date->d>days_in_month(date.m-1,date.y))
	{
		due_date->d=due_date->d-days_in_month(date.m-1,date.y);
		due_date->m=date.m+1;
		if(due_date->m>12)
		{
			due_date->m=1;
			due_date->y++;
		}
	}
	return due_date;
}

#include"book.h"
#include"user.h"


int User::give_due_time()
{
	if(Professor * u = dynamic_cast<Professor *>(this))
		return 60;
	else if (Student * u = dynamic_cast<Student*>(this))
		return 30;
}

void Book::book_request(User* user)
{
	int a;
	if(Professor * u = dynamic_cast<Professor *>(user))
		u->list_books.push_back(this);
	else if (Student * u = dynamic_cast<Student*>(user))
	{
		if (u->list_books.size()>=5)
		{
			cout<<"You Already have Borrowed 5 Books :("<<endl;
			return;
		}
		else
			u->list_books.push_back(this);
	}
	this->issued_by=user->id;
	time_t now=time(0);
	tm *ltm = localtime(&now);
	this->issue_time=mktime(ltm); 
	Time t={ltm->tm_mday,ltm->tm_mon+1,ltm->tm_year+1900};
	this->issue_date=t;
	return;
}

User_databse users;
Book_database books;
User * cur_user;

void Professor::clear_fine_amount()
{
	this->fine_amt=0;
	for(auto itr :this->list_books)
	{
		time_t now=time(0);
		// time_t cur=(time_t)(10);
		int difference= now-itr->issue_time;
		if (difference/(24*60*60)>60)
		{
			itr->return_book();
		}
	}
}
void Student::clear_fine_amount()
{
	this->fine_amt=0;
	for(auto itr :this->list_books)
	{
		time_t now=time(0);
		// time_t cur=(time_t)(10);
		int difference= now-itr->issue_time;
		if (difference/(24*60*60)>60)
		{
			itr->return_book();
		}
	}
}
void Book::return_book()
{
	User* user=users.search(this->issued_by);
	int i=0;
	if (user->post=="Professor")
	{
		Professor* p=dynamic_cast<Professor*>(user);
		for(auto itr : p->list_books)
		{
			if (itr->isbn==this->isbn)
			{
				p->list_books.erase(p->list_books.begin()+i);
				break;
			}
			i++;
		}
	}
	else
	{
		user=users.search(this->issued_by);
		Student* s=dynamic_cast<Student*>(user);
		for(auto itr : s->list_books)
		{
			if (itr->isbn==this->isbn)
			{
				s->list_books.erase(s->list_books.begin()+i);
				break;
			}
			i++;
		}
	}
	Time t={1,1,1};
	this->issue_date=t;
	this->issued_by="-1";
	cout<<"Book "<<this->isbn<< " Returned"<<endl;
	return;
}


Time Book::show_due_date()
{
	User * user = users.search(this->issued_by);
	int days=user->give_due_time();
	Time *due_date= add_days(days,this->issue_date);
	return *(due_date);
}


string get_login()
{
	string password;
	string id;
	cout<<"Enter Your ID: ";
	cin>>id;
	cout<<"Enter your Password: ";
	cin>>password;

	cur_user=users.search(id);
	if (cur_user==NULL)
		return "logged out";
	else
		return cur_user->post;

}

void display_stud_menu(User* cur)
{
	cout<<"WElCOME  "<<cur->name<<endl;
	cout<<"1:  See all Books."<<endl;
	cout<<"2:  List issued Books."<<endl;
	cout<<"3:  Check availibilty of books."<<endl;
	cout<<"4:  Return Book."<<endl;
	cout<<"5:  Check dues"<<endl;
	cout<<"6:  Logout\n";
	cout<<"Enter choice : ";
}

void display_prof_menu(User* cur)
{
	cout<<"WElCOME  "<<cur->name<<endl;
	cout<<"1:  See all Books."<<endl;
	cout<<"2:  List issued Books."<<endl;
	cout<<"3:  Check availibilty of books."<<endl;
	cout<<"4:  Return Book."<<endl;
	cout<<"5:  Check dues"<<endl;
	cout<<"6:  Logout\n";
	cout<<"Enter choice : ";
}

void display_lib_menu(User* cur)
{
	cout<<"WElCOME  "<<cur->name<<endl;
	cout<<"1:  Display All Books in Library."<<endl;
	cout<<"2:  List issued Books by User."<<endl;
	cout<<"3:  Modify Databases"<<endl;
	cout<<"4:  Display Database"<<endl;
	cout<<"5:  Clear Dues for User"<<endl;
	cout<<"6:  Show Due Date for a Book"<<endl;
	cout<<"7:  Logout\n";
	cout<<"Enter choice : ";
}

void modify_books(User* user)
{
	system("cls");
	cout<<"Welcome  "<<user->name<<endl;
	cout<<"1. Add Books."<<endl;
	cout<<"2. Update Book Details."<<endl;
	cout<<"3. Delete Book."<<endl;
	cout<<"Enter choice: ";
	char choice;
	cin>>choice;
	if (choice=='1')
	{
		string title,author,publication,isbn;
		cout<<"Enter Book name: ";
		getline(cin>>ws,title);
		cout<<"Enter Author name: ";
		getline(cin>>ws,author);
		cout<<"Enter Name of Publication: ";
		getline(cin>>ws,publication);
		cout<<"Enter 13-digit ISBN code: ";
		cin>>isbn;
		books.add(title,author,publication,isbn,"-1",(time_t)(0));
	}
	else if (choice=='2')
	{
		string isbn;
		cout<<"Enter 13-digit ISBN code: ";
		cin>>isbn;
		Book *b;
		b=books.search(isbn);
		string title,author,publication;
		cout<<"Enter Book name: ";
		getline(cin>>ws,title);
		cout<<"Enter Author name: ";
		getline(cin>>ws,author);
		cout<<"Enter Name of Publication: ";
		getline(cin>>ws,publication);
		books.update(title,author,publication,isbn);
	}
	else if (choice=='3')
	{
		cout<<"Enter ISBN of the book to be deleted"<<endl;
		string isbn;
		cin>>isbn;
		books.Delete(books.search(isbn));
	}
	else
		modify_books(user);
	return;
}

void modify_users(User * user)
{
	system("cls");
	cout<<"Welcome  "<<user->name<<endl;
	cout<<"1. Add Users."<<endl;
	cout<<"2. Update User Details."<<endl;
	cout<<"3. Delete Users."<<endl;
	cout<<"4. Go Back to Previous Menu"<<endl;
	cout<<"Enter choice: ";
	char choice;
	cin>>choice;
	system("cls");
	switch(choice)
	{
		case '1':
		{
			string name,password,post;
			string id;
			cout<<"Enter User name: ";
			getline(cin>>ws,name);
			cout<<"Enter Designation of User: ";
			getline(cin>>ws,post);
			cout<<"Enter password: ";
			getline(cin>>ws,password);
			id=1000001+users.list_users.size()+1;
			for (long int i=1000001;i<1000001+users.list_users.size()+1;i++)
			{
				if (users.search(to_string(i))==NULL)
					id=to_string(i);
			}
			users.add(name,password,post,id);
			break;
		}
		case '2':
		{
			string name,password,post;
			string id;
			cout<<"Enter Id: ";
			cin>>id;
			User * user;
			user=users.search(id);
			if(user!=NULL)
			{
				cout<<"Enter User name: ";
				getline(cin>>ws,name);
				cout<<"Enter new password: ";
				getline(cin>>ws,password);
				cout<<"Enter Designation of the user: ";
				getline(cin>>ws,post);
				users.update(name,password,post,id);
			}
			else
			{
				cout<<"User not found";
				modify_users(user);
			}
			break;
		}
		case '3':
		{
			string id;
			cout<<"Enter Id of user to be deleted: ";
			cin>>id;
			users.Delete(id,cur_user);
			break;
		}
		case '4':
		{
			return;
		}
		default : modify_users(user);
	}
}

void modify_databases(User * user)
{
	cout<<"Welcome  "<<user->name<<endl;
	cout<<"1. Modify User Database."<<endl;
	cout<<"2. Modify Book Database."<<endl;
	cout<<"3. Go Back to Main Menu."<<endl;
	cout<<"Enter choice: ";
	char choice;
	cin>>choice;
	if(choice=='1')
		modify_users(user);
	else if(choice=='2')
		modify_books(user);
	else if(choice=='3')
		return;
	else
		modify_databases(user);
	return; 
}

void check_availibility(User * user)
{
	books.display(0);
	cout<<"Welcome "<<user->name<<" Enter the id of the Book You wanna check availibility: "<<endl;
	string isbn;
	cin>>isbn;
	Book* b;
	b=books.search(isbn);
	if (b->issued_by=="-1")
	{
		char choice;
		cout<<"Book Available to Issue , Proceed to Issue Book? (y/n): ";
		cin>>choice;
		if(choice=='Y' || choice=='y')
		{
			b->book_request(user);
			return;
		}		
		else if (choice=='N' || choice=='n')
		{
			system("cls");
			check_availibility(user);
			return;
		}
	}
}

void read_data()
{
	vector<string> row;
	string line,word;
	fstream file("users.csv", ios::in);
	if(file.is_open())
	{
		while(getline(file, line))
		{
			row.clear();
 
			stringstream str(line);
 
			while(getline(str, word, ','))
				row.push_back(word);
			users.add(row[0],row[1],row[2],row[3]);
		}
	}
	file.close();

	fstream file2("books.csv", ios::in);
	if(file2.is_open())
	{
		while(getline(file2, line))
		{
			row.clear();
 
			stringstream str(line);
 
			while(getline(str, word, ','))
				row.push_back(word);
			books.add(row[0],row[1],row[2],row[3],row[4],(time_t)stoi(row[5]));
		}
	}
	file2.close();
	for(auto itr :books.list_books)
	{
		if (itr->issued_by!="-1")
		{
			if(users.search(itr->issued_by)->post=="Professor")
			{
				Professor* prof= dynamic_cast<Professor* >(users.search(itr->issued_by));
				prof->list_books.push_back(itr);
			}
			else
			{
				Student* stud = dynamic_cast<Student*>(users.search(itr->issued_by));
				stud->list_books.push_back(itr);
			}
		}
	}
	return;
}

void write_data()
{
	ofstream file1;
	file1.open("users.csv", ofstream::out | ofstream::trunc);

	for(auto itr: users.list_users)
	{
		file1<<itr->name<<',';
		file1<<itr->password<<',';
		file1<<itr->post<<',';
		file1<<itr->id;
		if (itr->id!=users.list_users[users.list_users.size()-1]->id)
			file1<<endl;
	}
	file1.close();

	file1.open("books.csv", ofstream::out | ofstream::trunc);

	for(auto itr: books.list_books)
	{
		file1<<itr->title<<',';
		file1<<itr->author<<',';
		file1<<itr->publication<<',';
		file1<<itr->isbn<<',';
		file1<<itr->issued_by<<',';
		file1<<itr->issue_time;
		if (itr->isbn!=books.list_books[books.list_books.size()-1]->isbn)
			file1<<endl;
	}
	file1.close();

}

int main()
{
	read_data();
	int a;
	while(1)
	{
		cout<<"Welcome User, to Your Library! Here login to get started.\n\n";
		string login_status="logged out";
		login_status=get_login();
		while(1)
		{
			write_data();
			if(login_status!="logged out" && login_status!="logged-out")
			{
				system("cls");
				if(login_status=="Professor")
					display_prof_menu(cur_user);
				else if (login_status=="Librarian")
					display_lib_menu(cur_user);
				else
					display_stud_menu(cur_user);
				int choice;
				cin>>choice;
				system("cls");
				switch(choice)
				{
					cout<<choice;
					case 1:	{
								
								books.display();
								char c;
								cout<<"Enter a character to continue";
								cin>>c;
								break;
							}

					case 2:	{
								if (login_status=="Librarian")
								{
									string id;
									cout<<"Enter id of User: ";
									cin>>id;
									if (Professor* prof=dynamic_cast<Professor*>(users.search(id)))
									{
										if (prof->list_books.size()==0)
										{
											string a;
											cout<<"No Books Issued"<<endl;
											cout<<"Enter a character to continue.";
											cin>>a;
											break;
										}
										prof->display_issued();
									}
									else
									{
										Student* stud=dynamic_cast<Student*>(users.search(id));
										if (stud->list_books.size()==0)
										{
											string a;
											cout<<"No Books Issued"<<endl;
											cout<<"Enter a character to continue.";
											cin>>a;
											break;
										}
										stud->display_issued();
									}

								}
								else
								{
									cur_user->display_issued();
								}
								char c;
								cout<<"Enter a character to continue";
								cin>>c;
								break;
							}

					case 3:	{
								if (login_status=="Librarian")
								{
									modify_databases(cur_user);
								}
								else
								{
									check_availibility(cur_user);
								}
								char c;
								cout<<"Enter a character to continue";
								cin>>c;
								break;
								
							}

					case 4:	{
								if(login_status=="Librarian")
								{
									users.display();
									books.display(0);
									char c;
									cin>>c;
								}
								else
								{
									string isbn;
									if (cur_user->post=="Student")
									{
										Student* user= dynamic_cast<Student*>(cur_user);
										user->display_issued();
									}
									else
									{
										Professor * user= dynamic_cast<Professor *>(cur_user);
										user->display_issued();
									}
									cout<<"Enter ISBN of the book:";
									cin>>isbn;
									Book* book = books.search(isbn);
									if (book!=NULL)
										books.search(isbn)->return_book();
									else
										cout<<"Book Not Found\n";
									char c;
									cout<<"Enter a character to continue";
									cin>>c;
									break;
								}
								break;
							}

					case 5: {
								if (cur_user->post!="Librarian")
								{
									if (cur_user->post=="Student")
									{
										Student* user= dynamic_cast<Student*>(cur_user);
										user->calculate_fine();
										cout<<"The Fine amount in your name is : Rs"<<user->fine_amt<<" .";
									}
									else
									{
										Professor * user= dynamic_cast<Professor *>(cur_user);
										user->calculate_fine();
										cout<<"The Fine amount in your name is : Rs"<<user->fine_amt<<" .";
									}
									char c;
									cout<<"Enter a character to continue";
									cin>>c;
									break;
								}
								else
								{
									string id;
									cout<<"Enter id of the user to clear fine of :"<<endl;
									cin>>id;
									User* user=users.search(id);
									if (cur_user->post=="Student")
									{
										Student* stud= dynamic_cast<Student*>(user);
										stud->clear_fine_amount();
									}
									else
									{
										Professor* prof =dynamic_cast<Professor*>(user);
										prof->clear_fine_amount();
									}
									char c;
									cout<<"Enter a character to continue";
									cin>>c;
								}
								break;
							}	

					case 6:	{
								if (cur_user->post=="Librarian")
								{
									string id;
									cout<<"Enter the ISBN of the Book: ";
									cin>>id;
									Book* book= books.search(id);
									if(book==NULL)
									{
										cout<<"Book not Found";
										break;
									}
									Time t =book->show_due_date();
									if(t.y==1)
									{
										cout<<"Book Not Issued\n";
									}
									cout<< t.d<<"/"<<t.m<<"/"<<t.y<<endl;;
									char c;
									cout<<"Enter a character to continue";
									cin>>c;
									break;
								}
								login_status="logged-out";
								break;
							}
					case 7: {
								login_status="logged-out";
								break;
							}
				}
			}
			else
				break;
		}
	}


}