using namespace std;

class Book
{
public:
	string title,author,publication,isbn;
	time_t issue_time;
	Time issue_date;

	string issued_by;



	void book_request(User* user);
	Time show_due_date();
	void return_book();
};

class Book_database
{
public:
	vector<Book *> list_books;

	void display(int is_issued=0)
	{
		cout<<"List of Books in our Library:\n";
		cout<<"          Name          |       ISBN       |       Author       |     Publication     |\n";
		for(auto itr : list_books)
		{
			if (is_issued==1 && itr->issued_by!="-1")
			{
				continue;
			}
			else if (is_issued==2 and itr->issued_by!="-1" or is_issued==0 or is_issued==1)
			{
				cout<<itr->title;
				space(24-itr->title.size());
				cout<<itr->isbn<<" |";
				cout<<itr->author;
				space(20-itr->author.size());
				cout<<itr->publication;
				// space(21-itr->publication.size());
				cout<<endl;
			}
		}
		return;
	}
	void add(string title,string author,string publication,string isbn,string issued_by,time_t issue_time)
	{
		Book *b=new Book;
		b->issued_by=issued_by;
		b->issue_time=issue_time;
		if (int(issue_time)==0){
			Time t={1,1,1};
			b->issue_date=t;
		}
		else
		{
			tm *ltm = localtime(&issue_time);
			Time t={ltm->tm_mday,ltm->tm_mon+1,ltm->tm_year+1900};
			b->issue_date=t;
		}
		b->title=title;
		b->author=author;
		b->isbn=isbn;
		b->publication=publication;
		list_books.push_back(b);
	}
	void update(string title,string author, string publication,string isbn)
	{
		Book * book=this->search(isbn);
		book->title=title;
		book->author=author;
		book->publication=publication;
		return;
	}
	void Delete(Book* b)
	{
		string a;
		int i=0;
		for(auto itr : list_books)
		{
			if (itr->isbn==b->isbn)
			{
				if (itr->issued_by=="-1")
				{
					cout<<"Cant Delete";
					cin>>a;
					return;
				}
				list_books.erase(list_books.begin()+i);
				break;
			}
			i++;
		}

	}
	Book* search(string isbn)
	{
		for(auto itr : list_books)
		{
			if (itr->isbn==isbn)
				return itr;
		}
		return NULL;
	}
};
