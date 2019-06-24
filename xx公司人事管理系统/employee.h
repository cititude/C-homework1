#include<iostream>
#include<string>
#include<vector>
using namespace std;

enum Sex
{
	Male=0,
	Female=1,
	nSex=2
};

enum Vocation
{
	Salesman=0,
	Manager=1,
	Technician=2,
	Salemanager=3,
	nVocation=4
};

class employee
{
public:
	employee(string name1="null",Sex sex1=Male,Vocation vocation1=Salesman,int grade1=1);
	employee(employee &e);
	virtual ~employee();
	virtual void upgrade()=0;	// upgrade
	virtual void degrade()=0;	//degrade
	static int nemployee; //total number of employees 
	virtual double calculate_month_wage()=0;
	static void set_nemployee(int n);
	static long long largest_id;	//ever-existed largest id
	virtual void print_info(ostream& out)=0;
	bool judgename(string othername);	//判断该职工姓名是否为othername
	bool judgeid(long long otherid);	//判断该职工id是否为otherid
	void revise(string rename,Sex resex);
	friend void promote(employee* e);
	void set_id(long long new_id);
	friend bool cmp1(employee* a,employee*b);//按照相同职业在一起，同职业内部等级排序的方式排序
	friend bool cmp2(employee* a,employee*b);//按照编号排序 
	friend void sort1(vector<employee*>* ve,int n);	//按照相同职业在一起，同职业内部等级排序的方式排序 
	friend void sort2(vector<employee*>* ve,int n);	//按照编号排序 
	friend istream& operator>>(istream& in,employee** e);//将信息存入到*e对应的指针所指向的地址中 
	Vocation vocation;
protected:
	string name;
	Sex sex;
	long long id;	//individual number 
	int grade;		//from 1 to 5,higher is better
};

class salesman:virtual public employee
{
public:
	salesman(string name1="null",Sex sex1=Male,Vocation vocation1=Salesman,int grade1=1);
	salesman(salesman& s);
	~salesman();
	void upgrade();
	void degrade();
	friend ostream& operator<<(ostream& out, salesman& s);
	friend istream& operator>>(istream& in, salesman& s);
	double calculate_month_wage();
	friend class salemanager;
	virtual	void print_info(ostream& out);
protected:
	double bonus_per_month;
};

class manager:virtual public employee
{
public:
	manager(string name1="null",Sex sex1=Male,Vocation vocation1=Manager,int grade1=1);
	manager(manager& m);
	~manager();
	void upgrade();
	void degrade();
	friend ostream& operator<<(ostream& out,manager& m);
	friend istream& operator>>(istream& in,manager& m);
	double calculate_month_wage();
	friend class salemanager;
	virtual void print_info(ostream& out);
protected:
	double wage_per_month;	
};

class technician:virtual public employee
{
public:
	technician(string name1="null",Sex sex1=Male,Vocation vocation1=Technician,int grade1=1);
	technician(technician& t);
	~technician();
	void upgrade();
	void degrade();
	double calculate_month_wage();
	friend ostream& operator<<(ostream& out, technician& t);
	friend istream& operator>>(istream& in, technician& t);
	void print_info(ostream& out);
protected:
	double wage_per_hour;//default working hours: 8 per day
};

class salemanager:virtual public salesman,virtual public manager
{
public:
	salemanager(string name1="null",Sex sex1=Male,Vocation vocation1=Salemanager,int grade1=1);
	salemanager(salesman& s);
	salemanager(manager& m);
//	salemanager(salemanager& sm);  
	~salemanager();
	void upgrade();
	void degrade();
	double calculate_month_wage();
	friend ostream& operator<<(ostream& out,salemanager& sm);
	friend istream& operator>>(istream& in,salemanager& sm);
	void print_info(ostream& out);
private:
		
};

class myException
{
public:
	myException(const char* str):exception(str){}
	~myException(){};
	const char* what()const;
private:
	string exception;
};

