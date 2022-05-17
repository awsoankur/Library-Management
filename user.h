using namespace std;

class User
{
	public:
	string name,password,post;
	string id;

	int give_due_time();
	virtual void display_issued()
	{
		cout<<"";
		return;
	}
	virtual void clear_fine_amount()
	{
		cout<<"no";
	}

};
class Professor : public User
{
public:
	int fine_amt;
	vector<Book*> list_books;

	void display_issued()
	{
		cout<<"List of Books in our Library:\n";
		cout<<"          Name          |       ISBN       |       Author       |     Publication     |        Due Date      \n";
		for(auto itr : list_books)
		{
			cout<<itr->title;
			space(24-itr->title.size());
			cout<<itr->isbn<<" |";
			cout<<itr->author;
			space(20-itr->author.size());
			cout<<itr->publication;
			space(21-itr->publication.size());
			Time t=itr->show_due_date();
			cout<<"      "<<t.d<<"/"<<t.m<<"/"<<t.y<<endl;
		}
	}
	
	int calculate_fine()
	{
		this->fine_amt=0;
		for(auto itr : this->list_books)
		{
			time_t now=time(NULL);
			int difference= now-itr->issue_time;
			if (difference/(24*60*60)>60)
			{
				this->fine_amt+=5*(difference/(24*60*60)-60);
			}
		}
	}
	virtual void clear_fine_amount();

};
class Student : public User
{
public:
	int fine_amt;
	vector<Book*> list_books;

	void display_issued()
	{
		cout<<"List of Books in our Library:\n";
		cout<<"               Name               |       ISBN       |           Author           |        Publication        |        Due Date      \n";
		for(auto itr : list_books)
		{
			cout<<itr->title;
			space(34-itr->title.size());
			cout<<itr->isbn<<" |";
			cout<<itr->author;
			space(28-itr->author.size());
			cout<<itr->publication;
			space(27-itr->publication.size());
			Time t=itr->show_due_date();
			cout<<"      "<<t.d<<"/"<<t.m<<"/"<<t.y<<endl;
		}
	}
	int calculate_fine()
	{
		this->fine_amt=0;
		for(auto itr : list_books)
		{
			time_t now=time(NULL);
			long long int difference= now-itr->issue_time;
			if (difference/(24*60*60)>30)
			{
				this->fine_amt+=2*(difference/(24*60*60)-30);
			}
		}
	}
	void clear_fine_amount();

};
class Librarian : public User
{

};





class User_databse
{
public:
	vector<User*> list_users;


	void add(string name,string password, string post,string id)
	{
		if(post=="Professor")
		{
			Professor*  p = new Professor;
			p->name=name;
			p->password=password;
			p->post=post;
			p->id=id;
			list_users.push_back(p);
		}
		else if(post=="Student")
		{
			Student *  p = new Student;
			p->name=name;
			p->password=password;
			p->post=post;
			p->id=id;
			list_users.push_back(p);
		}
		else
		{
			Librarian*  p = new Librarian;
			p->name=name;
			p->password=password;
			p->post=post;
			p->id=id;
			list_users.push_back(p);
		}
	}
	void update(string name,string password, string post,string id)
	{
		User * user=this->search(id);
		user->name=name;
		user->password=password;
		user->post=post;
		return;
	}
	void Delete(string id,User* cur)
	{
		string a;
		if(cur->id==id)
		{
			cout<<"Cannot Delete Self";
			return;
		}
		int i=0;
		for(auto itr : list_users)
		{
			if (itr->id==id)
			{
				if (itr->post=="Professor") 
				{
					if ((dynamic_cast<Student*>(itr))->list_books.size()>0)
					{
						cout<<"Cannot Delete User";
						cin>>a;
						return;
					}
				}
				else if (itr->post=="Student")
				{
					if ((dynamic_cast<Student*>(itr))->list_books.size()>0)
					{
						cout<<"Cannot Delete User";
						cin>>a;
						return;
					}
				}
				list_users.erase(list_users.begin()+i);
				break;
			}
			i++;
		}
		return;
	}
	User* search(string id)
	{
		for(auto itr : list_users)
		{
			if(itr->id==id)
				return itr;
		}
		return NULL;
	}
	void display()
	{
		cout<<"List of Users registered in our Library:\n";
		cout<<"          Name          |   ID   |    Designation    |     Password     \n";
		for(auto itr : list_users)
		{
			cout<<itr->name;
			space(24-itr->name.size());
			cout<<itr->id<<" |";
			cout<<itr->post;
			space(19-itr->post.size());
			cout<<itr->password<<endl;
		}
		return;
	}
};
