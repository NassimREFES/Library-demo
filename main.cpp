//------------------------------------------------------------------------------
//---------------------------------------------------------------------------
//------------------------------------------------------------------------
//----------------------------
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
//----------------------------
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::exception;
using std::runtime_error;
using std::string;
using std::ostream;
using std::istream;
using std::vector;
//----------------------------
//------------------------------------------------------------------------

void error(string s)
{
    throw runtime_error(s);
}

void keep_win_open(string s)
{
    cin.clear();
    string l;
    while(cin>>l && l!=s){}
    return;
}
//------------------------------------------------------------------------
class Date{
    public:
        enum Month{
            jan=1,feb,mar,apr,may,jun,jul,aug,sep,oct,nov,dec
        };
        class Invalid{};
        Date(int,Month,int);
        Date();
        int year () const {return y;}
        Month month() const {return m;}
        int day() const {return d;}

        void add_day(int);
        void add_month(int);
        void add_year(int);
    private:
    int y;
    Month m;
    int d;
};

//------------------------------------------------------------------------

static const Date& default_date()
{
    static const Date dd(2001,Date::jan,1);
    return dd;
}

//------------------------------------------------------------------------

bool is_date(int,Date::Month,int);
Date::Date(int yy,Month mm,int dd)
        :y(yy) , m(mm) , d(dd)
{
    if (!is_date(yy,mm,dd)) throw Invalid();
}

//------------------------------------------------------------------------

Date::Date()
    :y(default_date().year()),m(default_date().month()),d(default_date().day()) {}

//------------------------------------------------------------------------

bool is_leap(int y)
{
    if (y%4) return false;
	if (y%100==0 && y%400) return false;
	return true;
}

//------------------------------------------------------------------------

void Date::add_day(int n)
{
    while(n>0){
        int days_in_month=31;
        switch(m){
        case 2:
            days_in_month= (is_leap(year()))?29:28;
            break;
        case 4: case 6: case 9: case 11:
            days_in_month=30;
            break;
        }
        if(m==12 && d==31)
            ++y;
        if(d==days_in_month){
            m=(m==Date::dec) ? Date::jan : Month(m+1);
            d=0;
        }
    }
    ++d;
    --n;

}

//------------------------------------------------------------------------

void Date::add_month(int n)
{
    while(n>0){
        switch(m){
        case 12:
            m = Date::jan;
            ++y;
            break;
        default:
            m = Month(m+n);
            break;
        }
        --n;
    }

}

//------------------------------------------------------------------------

void Date::add_year(int n)
{
    y += n;
}

//------------------------------------------------------------------------

bool is_date(int y,Date::Month m,int d)
{
    if(y<0 || y>2200) return false;
    int days_in_month=31;
    switch(m){
    case 2:
        days_in_month= (is_leap(y))?29:28;
        break;
    case 4: case 6: case 9: case 11:
        days_in_month=30;
        break;
    }
    if(days_in_month<=0 || d>days_in_month) return false;
    return true;
}

//------------------------------------------------------------------------

ostream& operator<<(ostream& os,const Date& d)
{
    return os << '(' << d.year() << '/' << d.month() << '/' << d.day() << ')';
}

istream& operator>>(istream& in,Date& dt)
{
    int y,m,d;
    char slash1,slash2;
    in >> y >> slash1 >> m >> slash2 >> d;
    dt=Date(y,Date::Month(m),d);
    return in;
}
//------------------------------------------------------------------------

bool operator==(const Date& a,const Date& b)
{
    return a.year()==b.year() && a.month()==b.month() && a.day()==b.day();
}

//------------------------------------------------------------------------

bool operator!=(const Date& a,const Date& b)
{
    return !(a!=b);
}

//------------------------------------------------------------------------

class Book {
public:
    enum Genre{
        fiction,nonfiction,periodcal,biography,children,crime,horror,history,poetry,medical,adventure,religion,romance,nogenre
    };
    struct ISBN{
        int n1,n2,n3;
        char c;
        ISBN(int,int,int,char);
        ISBN();
        class Bad_isbn{};
    };
    Book(ISBN,string,string,Date,Genre);
    Book();
    void check_in();
    void check_out();
    void is_chekeck();
    ISBN isbn_r() const {return n;}
    string title_r() const {return title;}
    string author_r() const {return author;}
    int genre_r() const {return genre;}
    Date copy_r() const {return copyright;}
private:
    ISBN n;
    string title;
    string author;
    Date copyright;
    Genre genre;
    bool checked;
};

//------------------------------------------------------------------------

Book::ISBN::ISBN(int num1,int num2,int num3,char c)
    : n1(num1), n2(num2), n3(num3), c(c)
    {
        if(num1<0 || num2<0 || num3<0 || !isalpha(c) && !isdigit(c))
            throw Bad_isbn();
    }

//------------------------------------------------------------------------

Book::ISBN::ISBN()
    : n1(0),n2(0),n3(0),c('0') {}

//------------------------------------------------------------------------

Book::Book(ISBN n1,string s1,string s2,Date d,Genre g)
    : n(ISBN(n1)), title(s1), author(s2), copyright(Date(d)), genre(g), checked(true) { }

//------------------------------------------------------------------------
static const Book& no_book();
Book::Book()
    : n(no_book().n), title(no_book().title), author(no_book().author), copyright(no_book().copyright), genre(no_book().genre), checked(false) { }

//------------------------------------------------------------------------

static const Book& no_book()
{
    static const Book bk(Book::ISBN(),"","",Date(),Book::nogenre);
    return bk;
}

//------------------------------------------------------------------------

void Book::check_in()
{
    if(checked) error("Book already checked in");
    checked = true;
    cout << title << "by : " << author << " checking in.\n";
}

//------------------------------------------------------------------------

void Book::check_out()
{
    if(!checked) error("Book already checked out");
    checked = false;
    cout << title << "by : " << author << " checking out.\n";
}

//------------------------------------------------------------------------


void Book::is_chekeck()
{
    if(checked)
        cout << title << "by : " << author << " is available.\n";
    else
        cout << title << "by : " << author << " is available.\n";
}

//------------------------------------------------------------------------

istream& operator>>(istream& input,Book::ISBN& n)
{
    int n1,n2,n3;
    char c1,c2,c3,c;
    input >> n1 >> c1 >> n2 >> c2 >> n3 >> c3 >> c;
    if (!input ||(c1!='-'||c2!='-'||c3!='-')) error("input ISBN fail");
    n=Book::ISBN(n1,n2,n3,c);
    return input;
}

istream& operator>>(istream& input,Book& b)
{
    Book::ISBN n;
    string s1,s2;
    Date d;
    int i;

    input >> n >> s1 >> s2 >> d >> i;
    b=Book(n,s1,s2,d,Book::Genre(i));
    return input;
}

//------------------------------------------------------------------------



//------------------------------------------------------------------------
//------------------------------------------------------------------------

bool operator==(const Book::ISBN& a,const Book::ISBN& b)
{
    return (a.n1==b.n1)&&(a.n2==b.n2)&&(a.n3==b.n3)&&(a.c==b.c);
}

bool operator!=(const Book::ISBN& a,const Book::ISBN& b)
{
    return !(a==b);
}

//------------------------------------------------------------------------

ostream& operator<<(ostream& out,const Book::ISBN& n)
{
    return out << n.n1 << '-' << n.n2
               << '-' << n.n3 << '-' << n.c << '\n';
}

ostream& operator<<(ostream& out,const Book& b)
{
    return out << b.title_r() << '\n' << b.author_r() << '\n' << b.isbn_r() ;
}

bool operator==(const Book& a,const Book& b)
{
    return a.isbn_r()==b.isbn_r() && a.title_r()==b.title_r() && a.author_r()==b.author_r()
            && a.copy_r()==b.copy_r() && a.genre_r()==b.genre_r();
}

//------------------------------------------------------------------------

class Patron {
public:
    Patron(string,int,double);
    Patron();
    string u_name() const {return user_name;}
    int card_n() const {return card_number;}
    double fee() const {return fees;}
    void set_fee(double);
private:
    string user_name;
    int card_number;
    double fees;
};

//------------------------------------------------------------------------

Patron::Patron(string s,int n,double)
    : user_name(s), card_number(n), fees(0)
{
    if (n<0) error("Invalid profile");
}

//------------------------------------------------------------------------

Patron::Patron()
    : user_name(""), card_number(0), fees(0) { }

//------------------------------------------------------------------------

void Patron::set_fee(double price)
{
    fees = price;
}

//------------------------------------------------------------------------

bool is_fee(const Patron& p)
{
    return (p.fee()>0) ? true:false;
}

//------------------------------------------------------------------------

istream& operator>>(istream& in,Patron& p)
{
    string user_name;
    int card_number;
    in >> user_name >> card_number ;
    p=Patron(user_name,card_number,0);
    return in;
}

bool operator==(const Patron& a,const Patron& b)
{
    return a.u_name()==b.u_name() && a.card_n()==b.card_n();
}

//------------------------------------------------------------------------

class Library{
public:
    struct Transaction{
        Book bk;
        Patron ptrn;
        Date dt;
        Transaction(Book,Patron,Date);
    };
    void add_book_lib();
    void add_patrons();
    void check_out(const Book&,const Patron&);
    vector<string> fee_patrons();
private:
    vector<Book>books;
    vector<Patron>patrons;
    vector<Transaction>trans;
};

//------------------------------------------------------------------------
Library::Transaction::Transaction(Book b,Patron p,Date d)
    : bk(b), ptrn(p), dt(d) { }

//------------------------------------------------------------------------
void Library::add_book_lib()
{
    Book b;
    cout << "ISBN Title Author Copyright-date Genre (0-0-0-0 to stop):\n";
    while(cin >> b && b.title_r()!="NoMoRe" && b.author_r()!="NoMoRe"){
        for (int i = 0 ; i < books.size() ; ++i)
            if (books[i].isbn_r()==b.isbn_r()) error("ISBN duplicated");
        books.push_back(b);
    }
}
//------------------------------------------------------------------------

void Library::add_patrons()
{
    Patron p;
    cout << "User name card number (NoMoRe to stop):\n";
    while(cin >> p && p.u_name()!="NoMoRe"){
        for (int i = 0 ; i < patrons.size() ; ++i)
            if (patrons[i].card_n()==p.card_n()) error("ISBN duplicated");
        patrons.push_back(p);
    }
}

//------------------------------------------------------------------------

bool is_book_in_lib(const vector<Book>& bks,const Book& b,int& index)
{
    bool ok = false;
    for(int i = 0 ; i < bks.size() ; ++i){
        if(bks[i]==b) {
            index = i;
            ok = true;
            break;
        }
    }
    return ok;
}

bool is_user_in_lib(const vector<Patron>& pts,const Patron& p,int& index)
{
    bool ok = false ;
    for(int i = 0 ; i < pts.size() ; ++i){
        if(pts[i]==p) {
            ok = true;
            index = i;
            break;
        }
    }
    return ok;
}

//------------------------------------------------------------------------

void Library::check_out(const Book& b,const Patron& p)
{
    int index1 =0;
    int index2 =0;
    if(!is_book_in_lib(books,b,index1)) error("book not in library");
    if(!is_user_in_lib(patrons,p,index2)) error("not a member");
    if(is_fee(patrons[index2])) error("pay your fees asshole");
    books[index1].check_out();

    cout << "Enter the current date please : ";
    Date d;
    cin >> d;
    Library::Transaction t(b,p,d);
    trans.push_back(t);
}

//------------------------------------------------------------------------

vector<string> Library::fee_patrons()
{
    vector<string> f;
    for(int i = 0 ; i < patrons.size() ; ++i){
        if(is_fee(patrons[i]))
            f.push_back(patrons[i].u_name());
    }
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

int main()
try{


    return 0;
}
catch(Date::Invalid)
{
    cerr << "Error : invalid Date." << endl;
}
catch(exception& err)
{
    cerr << "Error : " << err.what() << endl;
    return 1;
}
catch(...)
{
    cerr << "Unknown error!" << endl;
    return 2;
}
//------------------------------------------------------------------------
//---------------------------------------------------------------------------
//------------------------------------------------------------------------------
