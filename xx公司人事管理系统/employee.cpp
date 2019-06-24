#include"employee.h"

#include<windows.h>
#include<fstream>
#include<algorithm>
#include<strstream>

const int working_hour=100; //技术人员的默认每月工作时间
double turnover=200000;	//月营业总额，用于计算分红 
const double tech_default_wage=260;
const double mana_default_wage=12000;
const double sale_default_bonus=0.05;

vector<employee*>workers;
int nworkers;

employee::employee(string name1,Sex sex1,Vocation vocation1,int grade1):
	name(name1),sex(sex1),vocation(vocation1),grade(grade1)
{
	nemployee++;
	largest_id++;
	id=largest_id;
}

employee::employee(employee& e)
{
	name=e.name;
	sex=e.sex;
	vocation=e.vocation;
	id=e.id;
	grade=e.grade;
}

employee::~employee()
{
	nemployee--;
}

void employee::set_nemployee(int n)
{
	nemployee=n;
}
 
bool employee::judgename(string othername)
{
	if(name==othername)return true;
	else return false;
}

bool employee::judgeid(long long otherid)
{
	if(id==otherid)return true;
	return false;
}

void employee::revise(string rename,Sex resex)
{
	name=rename;
	sex=resex;
	return;
}

void promote(employee* e)
{
	salemanager* newsalemanager=new salemanager(e->name,e->sex,e->vocation,e->grade);
	workers.push_back(newsalemanager);
	return;
}

void employee::set_id(long long new_id)
{
	id=new_id;
}

bool cmp1(employee* a,employee*b)
{
	if(a->vocation<b->vocation)return false;
	else if(a->grade<b->grade&&a->vocation==b->vocation)return false;
	return true;
}

bool cmp2(employee* a,employee*b)
{
	if(a->id<b->id)return false;
	else return true;
}

void sort1(vector<employee*>* ve,int n)
{
	for(int i=1;i<n;i++)
		for(int j=0;j<=n-i-1;j++)
		{
			if(cmp1((*ve)[j],(*ve)[j+1]))
			{
				employee* tmp=(*ve)[j];
				(*ve)[j]=(*ve)[j+1];
				(*ve)[j+1]=tmp;
			}
		}
	return;
}

void sort2(vector<employee*>* ve,int n)
{
	for(int i=1;i<n;i++)
		for(int j=0;j<=n-i-1;j++)
		{
			if(cmp2((*ve)[j],(*ve)[j+1]))
			{
				employee* tmp=(*ve)[j];
				(*ve)[j]=(*ve)[j+1];
				(*ve)[j+1]=tmp;
			}
		}
	return;
}

istream& operator>>(istream& in,employee** e)
{
	string tmp;
	string name,strsex,strvocation;
	Sex sex;
	Vocation vocation;
	long long id;
	int grade;
	in>>tmp>>name;
	in>>tmp>>strsex;
	in>>tmp>>strvocation;
	in>>tmp>>id;
	in>>tmp>>grade;
	if(strsex=="男")sex=Male;
	else sex=Female;
	if(strvocation==string("营销员"))
	{
		*e=new salesman(name,sex,Vocation(0),grade);
	}
	else if(strvocation==string("经理"))
	{
		*e=new manager(name,sex,Vocation(1),grade);
	}
	else if(strvocation==string("技术工"))
	{
		*e=new technician(name,sex,Vocation(2),grade);
	}
	else if(strvocation==string("销售经理"))
	{
		*e=new salemanager(name,sex,Vocation(3),grade);
	}
	(*e)->set_id(id);
	employee::largest_id=max(id,employee::largest_id-1);
	return in;
}
//salesman
salesman::salesman(string name1,Sex sex1,Vocation vocation1,int grade1):employee(name1,sex1,vocation1,grade1)
{
	bonus_per_month=sale_default_bonus+(grade-1)*0.01;
}

salesman::salesman(salesman& s):employee(s.name,s.sex,s.vocation,s.grade),bonus_per_month(sale_default_bonus+(grade-1)*0.01)
{}

salesman::~salesman(){}

void salesman::upgrade()
{
	if(grade==5)return;
	grade++;
	bonus_per_month+=0.01;
}

void salesman::degrade()
{
	if(grade==1)return;
	grade--;
	bonus_per_month-=0.01;
}

double salesman::calculate_month_wage()
{
	return turnover*bonus_per_month;
}


ostream& operator<<(ostream& out,salesman& s)
{
	out<<"姓名: "<<s.name<<' ';
	out<<"性别: ";
	switch(s.sex)
	{
		case 0:
			out<<"男 ";
			break;
		case 1:
			out<<"女 ";
			break;
		default:
			break;
	}
	out<<"职业: "<<"营销员 ";
	out<<"编号: "<<s.id<<' ';
	out<<"等级: "<<s.grade<<' ';
	out<<"月提成率: "<<s.bonus_per_month;
	out<<endl;
	return out;
}

void salesman::print_info(ostream& out)
{
	out<<"姓名: "<<name<<' ';
	out<<"性别: ";
	switch(sex)
	{
		case 0:
			out<<"男 ";
			break;
		case 1:
			out<<"女 ";
			break;
		default:
			break;
	}
	out<<"职业: "<<"营销员 ";
	out<<"编号: "<<id<<' ';
	out<<"等级: "<<grade<<' ';
	out<<"月提成率: "<<bonus_per_month<<' ';
	out<<"月工资总额: "<<calculate_month_wage();
	out<<endl;
}

istream& operator>>(istream& in, salesman& s)
{
	int ssex,svocation;
	in>>s.name>>ssex>>svocation>>s.id>>s.grade;
	s.sex=(Sex)ssex;
	s.vocation=(Vocation)svocation;
	s.bonus_per_month=sale_default_bonus+0.01*(s.grade-1);
	return in;
}


//manager

manager::manager(string name1,Sex sex1,Vocation vocation1,int grade1):employee(name1,sex1,vocation1,grade1)
{
	wage_per_month=mana_default_wage+(grade-1)*1000;
}

manager::manager(manager& m):employee(m.name,m.sex,m.vocation,m.grade),wage_per_month(mana_default_wage+(grade-1)*1000)
{
	
}

manager::~manager(){}

void manager::upgrade()
{
	if(grade==5)return;
	grade++;
	wage_per_month+=1000;
}

void manager::degrade()
{
	if(grade==1)return;
	grade--;
	wage_per_month-=1000;
}

double manager::calculate_month_wage()
{
	return wage_per_month;
}


ostream& operator<<(ostream& out,manager& m)
{
	out<<"姓名: "<<m.name<<' ';
	out<<"性别: ";
	switch(m.sex)
	{
		case 0:
			out<<"男 ";
			break;
		case 1:
			out<<"女 ";
			break;
		default:
			break;
	}
	out<<"职业: "<<"经理 ";
	out<<"编号: "<<m.id<<' ';
	out<<"等级: "<<m.grade<<' ';
	out<<"月薪: "<<m.wage_per_month;
	out<<endl;
	return out;
}

void manager::print_info(ostream& out)
{
	out<<"姓名: "<<name<<' ';
	out<<"性别: ";
	switch(sex)
	{
		case 0:
			out<<"男 ";
			break;
		case 1:
			out<<"女 ";
			break;
		default:
			break;
	}
	out<<"职业: "<<"经理 ";
	out<<"编号: "<<id<<' ';
	out<<"等级: "<<grade<<' ';
	out<<"月薪: "<<wage_per_month<<' ';
	out<<"月工资总额: "<<calculate_month_wage();
	out<<endl;
}

istream& operator>>(istream& in, manager& s)
{
	int ssex,svocation;
	in>>s.name>>ssex>>svocation>>s.id>>s.grade;
	s.sex=(Sex)ssex;
	s.vocation=(Vocation)svocation;
	s.wage_per_month=mana_default_wage+(s.grade-1)*1000;
	return in;
}



//technician
technician::technician(string name1,Sex sex1,Vocation vocation1,int grade1):employee(name1,sex1,vocation1,grade1)
{
	wage_per_hour=tech_default_wage+(grade-1)*20;
}

technician::technician(technician& t):employee(t.name,t.sex,t.vocation,t.grade),wage_per_hour(tech_default_wage+(grade-1)*20)
{
	
}

technician::~technician(){}

void technician::upgrade()
{
	if(grade==5)return;
	grade++;
	wage_per_hour+=20;
}

void technician::degrade()
{
	if(grade==1)return;
	grade--;
	wage_per_hour-=20;
}

double technician::calculate_month_wage()
{
	return wage_per_hour*working_hour;
}


ostream& operator<<(ostream& out,technician& t)
{
	out<<"姓名: "<<t.name<<' ';
	out<<"性别: ";
	switch(t.sex)
	{
		case 0:
			out<<"男 ";
			break;
		case 1:
			out<<"女 ";
			break;
		default:
			break;
	}
	out<<"职业: "<<"技术工 ";
	out<<"编号: "<<t.id<<' ';
	out<<"等级: "<<t.grade<<' ';
	out<<"时薪: "<<t.wage_per_hour<<' ';
	out<<"月工资总额: "<<t.calculate_month_wage();
	out<<endl;
	return out;
}

void technician::print_info(ostream& out)
{
	out<<"姓名: "<<name<<' ';
	out<<"性别: ";
	switch(sex)
	{
		case 0:
			out<<"男 ";
			break;
		case 1:
			out<<"女 ";
			break;
		default:
			break;
	}
	out<<"职业: "<<"技术工 ";
	out<<"编号: "<<id<<' ';
	out<<"等级: "<<grade<<' ';
	out<<"时薪: "<<wage_per_hour<<' ';
	out<<"月工资总额: "<<calculate_month_wage();
	out<<endl;
}

istream& operator>>(istream& in, technician& s)
{
	int ssex,svocation;
	in>>s.name>>ssex>>svocation>>s.id>>s.grade;
	s.sex=(Sex)ssex;
	s.vocation=(Vocation)svocation;
	s.wage_per_hour=tech_default_wage+(s.grade-1)*20;
	return in;
}

salemanager::salemanager(string name1,Sex sex1,Vocation vocation1,int grade1):employee(name1,sex1,vocation1,grade1),
	manager(name1,sex1,vocation1,grade1),salesman(name1,sex1,vocation1,grade1){}
	
salemanager::salemanager(salesman& s):employee(s.name,s.sex,Salemanager,s.grade),
	manager(s.name,s.sex,Salemanager,s.grade),salesman(s.name,s.sex,Salemanager,s.grade){}

salemanager::salemanager(manager& s):employee(s.name,s.sex,Salemanager,s.grade),
	manager(s.name,s.sex,Salemanager,s.grade),salesman(s.name,s.sex,Salemanager,s.grade){}

salemanager::~salemanager(){}

void salemanager::upgrade()
{
	if(grade==5)return;
	grade++;
	bonus_per_month+=0.01;
	wage_per_month+=1000;
	return;
}

void salemanager::degrade()
{
	if(grade==1)return;
	grade--;
	bonus_per_month-=0.01;
	wage_per_month-=1000;
	return;
}

double salemanager::calculate_month_wage()
{
	return turnover*bonus_per_month+wage_per_month;
}

ostream& operator<<(ostream& out,salemanager& sm)
{
	out<<"姓名: "<<sm.name<<' ';
	out<<"性别: ";
	switch(sm.sex)
	{
		case 0:
			out<<"男 ";
			break;
		case 1:
			out<<"女 ";
			break;
		default:
			break;
	}
	out<<"职业: "<<"销售经理 ";
	out<<"编号: "<<sm.id<<' ';
	out<<"等级: "<<sm.grade<<' ';
	out<<"月薪: "<<sm.wage_per_month<<' ';
	out<<"月提成率： "<<sm.bonus_per_month<<' '; 
	out<<"月工资总额: "<<sm.calculate_month_wage();
	out<<endl;
	return out;	
}

void salemanager::print_info(ostream& out)
{
	out<<"姓名: "<<name<<' ';
	out<<"性别: ";
	switch(sex)
	{
		case 0:
			out<<"男 ";
			break;
		case 1:
			out<<"女 ";
			break;
		default:
			break;
	}
	out<<"职业: "<<"销售经理 ";
	out<<"编号: "<<id<<' ';
	out<<"等级: "<<grade<<' ';
	out<<"月薪: "<<wage_per_month<<' ';
	out<<"月提成率： "<<bonus_per_month<<' ';
	out<<"月工资总额: "<<calculate_month_wage();
	out<<endl;
}

istream& operator>>(istream& in, salemanager& s)
{
	int ssex,svocation;
	in>>s.name>>ssex>>svocation>>s.id>>s.grade;
	s.sex=(Sex)ssex;
	s.vocation=(Vocation)svocation;
	s.wage_per_month=mana_default_wage+(s.grade-1)*1000;
	s.bonus_per_month=sale_default_bonus+0.01*(s.grade-1);
	return in;
}


const char* myException::what()const
{
	return exception.c_str();
}

void show_screen()
{
	system("cls");
	std::cout<<"++++++++++++++++++++++++++++++++++++"<<std::endl;
	std::cout<<"       xxx公司人事管理系统"<<std::endl;
	std::cout<<std::endl;
	std::cout<<"++++++++++++++++++++++++++++++++++++"<<std::endl;
	std::cout<<std::endl; 
	std::cout<<"####################################"<<std::endl; 
	std::cout<<std::endl;
	std::cout<<"       主菜单                       "<<std::endl;
	std::cout<<"   1. 信息录入                      "<<std::endl;
	std::cout<<"   2. 信息查询                      "<<std::endl;
	std::cout<<"   3. 信息修改                      "<<std::endl;
	std::cout<<"   4. 信息转存                      "<<std::endl;
	std::cout<<"   5. 保存并退出界面                "<<std::endl;
	std::cout<<"    请选择序号（1-5）               "<<std::endl;
	std::cout<<"####################################"<<std::endl; 
}






int employee::nemployee=0;
long long employee::largest_id=2019000000;
void init()	throw(myException)//从文件中读入信息并作初始化 
{
	workers.clear();
	char s[200];
	ifstream infile("employee_info.txt",ios::in);
	if(!infile)throw(myException("无法加载数据！请确定相关文件是否损坏"));
	while(infile.getline(s,200))
	{
		istrstream strin(s,sizeof(s));
		employee* newworker;
		strin>>&newworker;
		workers.push_back(newworker);
	}
}

void data_input()
{
	system("cls");
	string name;
	Sex sex;
	char c;
	Vocation vocation;
	int grade;
	string tmp;
	cout<<"姓名(请勿出现空格)："; 
	cin>>name;
	cout<<"性别：\n(输入0表示男性，1表示女性)\n"; 
	cin>>tmp;
	while(tmp.length()!=1||(tmp[0]!='0'&&tmp[0]!='1'))
	{
		cout<<"输入数据格式或范围有误，请重新输入！\n";
		cin>>tmp; 
	}
	if(tmp[0]=='0')sex=Male;
	else sex=Female;
	cout<<"职业：\n";
	cout<<"(输入0表示营销员，1表示经理，2表示技术工人，3表示销售经理)\n"; 
	cin>>tmp;
	while(tmp.length()!=1||tmp[0]<'0'||tmp[0]>'3')
	{
		cout<<"输入数据格式或范围有误，请重新输入！\n";
		cin>>tmp; 
	}
	switch(tmp[0])
	{
		case '0':vocation=Salesman;break;
		case '1':vocation=Manager;break;
		case '2':vocation=Technician;break;
		case '3':vocation=Salemanager;break;
		default:break;
	}
	cout<<"等级（1-5）："; 
	cin>>tmp;
	while(tmp.length()!=1||tmp[0]<='0'||tmp[0]>='6')
	{
		cout<<"输入数据格式或范围有误，请重新输入！\n";
		cin>>tmp; 
	}
	grade=tmp[0]-'0';
	employee* new_worker=NULL;
	switch(vocation)
	{
		case Salesman:
			new_worker=new salesman(name,sex,vocation,grade);
			break;
		case Manager:
			new_worker=new manager(name,sex,vocation,grade);
			break;
		case Technician:
			new_worker=new technician(name,sex,vocation,grade);
			break;
		case Salemanager:
			new_worker=new salemanager(name,sex,vocation,grade);
			break;		
	}
	cout<<"信息输入成功！即将添加以下信息：\n";
	new_worker->print_info(cout); 
	cout<<"确认添加该职工信息请输入1，否则请输入0\n";
	cin>>tmp;
	while(tmp.length()!=1||tmp[0]<'0'||tmp[0]>'1')
	{
		cout<<"输入数据格式或范围有误，请重新输入！\n";
		cin>>tmp; 
	}
	if(tmp[0]=='0')
	{
		cout<<"即将退出数据录入界面";
		Sleep(3000);
		return;
	}
	workers.push_back(new_worker);
	cout<<"添加用户信息成功！\n"; 
	system("pause");
	Sleep(3000);
	return;
}

void data_query()
{
	system("cls");
	cout<<"请选择查询索引：1.姓名 2.编号 ";
	int search_index;
	cin>>search_index;
	while(search_index<=0||search_index>=3)
	{
		cout<<"输入数据格式或范围有误，请重新输入！\n";
		cin>>search_index;
	}
	if(search_index==1)
	{
		bool flag=false;
		cout<<"请输入所查询对象姓名：";
		string name;
		cin>> name;
		for(int i=0;i<workers.size();i++)
		{
			if(workers[i]==NULL)continue;	//该指针对应人员信息可能已被删除 
			if(workers[i]->judgename(name))
			{
				workers[i]->print_info(cout);
				flag=true;
			}
		}
		if(flag==false)cout<<"所查询对象不存在!\n";
	}
	else
	{
		bool flag=false;
		cout<<"请输入所查询对象编号：";
		long long id;
		cin>> id;
		for(int i=0;i<workers.size();i++)
		{
			if(workers[i]==NULL)continue;
			if(workers[i]->judgeid(id))
			{
				workers[i]->print_info(cout);
				flag=true;
			}
		}
		if(flag==false)cout<<"所查询对象不存在!\n";
	}
	system("pause"); 
	Sleep(1000);
}

void data_revise()
{
	system("cls");
	cout<<"请输入您想进行的操作编号：1.晋升或降级 2.提拔销售经理 3.删除员工信息 4. 修改基本信息\n";
	int index;
	cin>>index;
	while(index<=0||index>=5)
	{
		cout<<"输入数据格式或范围有误，请重新输入！\n";
		cin>>index;	
	}
	if(index==1)
	{
		cout<<"请输入被晋升者编号:\n";
		long long id;
		cin>>id;
		bool flag=false;
		int i=0;
		for(i=0;i<workers.size();i++)
		{
			if(workers[i]==NULL)continue;
			if(workers[i]->judgeid(id))
			{
				flag=true;
				break;
			}
		}
		if(flag==false)
		{
			cout<<"该对象不存在!\n";
		}
		else
		{
			workers[i]->print_info(cout);
			cout<<"请输入所提高等级数（修改后等级不得超过5级，不得低于1级,使用负数表示降低等级）：\n";
			int upg;
			cin>>upg;
			if(upg>=0)for(int j=0;j<upg;j++)workers[i]->upgrade();
			else for(int j=0;j>upg;j--)workers[i]->degrade();
			cout<<"操作成功！当前该人员信息：\n";
			workers[i]->print_info(cout);
			system("pause"); 
		}		
	}
	if(index==2)
	{
		cout<<"请输入被提拔为销售经理者编号:\n";
		long long id;
		cin>>id;
		bool flag=false;
		int i=0;
		for(i=0;i<workers.size();i++)
		{
			if(workers[i]==NULL)continue;
			if(workers[i]->judgeid(id))
			{
				flag=true;
				break;
			}
		}
		if(flag==false)
		{
			cout<<"该对象不存在!\n";
		}
		else
		{
			workers[i]->print_info(cout);
			cout<<"确认提拔为销售经理请输入1，否则输入0\n";
			int upg;
			cin>>upg;
			if(upg==1)
			{
				if(workers[i]->vocation!=Salesman&&workers[i]->vocation!=Manager)
				{
					cout<<"此人不是营销员或经理，不能提拔为销售经理";
					system("pause"); 
				}
				else
				{
					promote(workers[i]);
					delete workers[i];
					workers[i]=NULL;
					cout<<"操作成功！当前该人员信息：\n";
					workers[workers.size()-1]->print_info(cout);
					system("pause"); 
				}
				
			}
		}	
	}
	if(index==3)
	{
		cout<<"请输入被移除者编号:\n";
		long long id;
		cin>>id;
		bool flag=false;
		int i=0;
		for(i=0;i<workers.size();i++)
		{
			if(workers[i]==NULL)continue;
			if(workers[i]->judgeid(id))
			{
				flag=true;
				break;
			}
		}
		if(flag==false)
		{
			cout<<"该对象不存在!\n";
		}
		else
		{
			workers[i]->print_info(cout);
			cout<<"确认删除该人员信息请输入1，否则输入0\n";
			int upg;
			cin>>upg;
			if(upg==1)
			{
				delete workers[i];
				workers[i]=NULL;
				cout<<"已删除\n";
				system("pause");
			}
			else
			{
				cout<<"即将退出该界面\n"; 
			}
		}		
	}
	if(index==4)
	{
		cout<<"请输入被修改对象编号:\n";
		long long id;
		cin>>id;
		bool flag=false;
		int i=0;
		for(i=0;i<workers.size();i++)
		{
			if(workers[i]==NULL)continue;
			if(workers[i]->judgeid(id))
			{
				flag=true;
				break;
			}
		}
		if(flag==false)
		{
			cout<<"该对象不存在!\n";
		}
		else
		{
			workers[i]->print_info(cout);
			cout<<"请输入修改后的姓名：";
			string rename;
			cin>> rename;
			cout<<"请输入修改后的性别对应编号（0表示男性，1表示女性）：";
			string tmp;
			cin>>tmp;
			Sex resex;
			while(tmp.length()!=1||(tmp[0]!='0'&&tmp[0]!='1'))
			{
				cout<<"输入数据格式或范围有误，请重新输入！\n";
				cin>>tmp; 
			}
			if(tmp[0]=='0')resex=Male;
			else resex=Female; 
			workers[i]->revise(rename,resex);
			cout<<"修改成功！\n";
			system("pause"); 
		}
	}
	Sleep(3000);
}

void data_copy()throw(myException)
{
	system("cls");
	string filename1;
	cout<<"请输入新建文本文件文件名（不包括.txt)\n";
	cin>>filename1;
	string filename=filename1+string(".txt");
	ofstream outfile(filename,ios::out);
	if(!outfile)throw(myException("无法新建文件并转存相应信息"));
	vector<employee*> tmp;
	for(int i=0;i<workers.size();i++)
	{
		if(workers[i]==NULL)continue;
		else tmp.push_back(workers[i]);
	}
	cout<<"请选择信息排序方式（1表示按照职业和等级排序，2表示按照编号排序）：";
	string ttmp;
	cin>>ttmp;
	while(ttmp.length()!=1||ttmp[0]<='0'||ttmp[0]>'2')
	{
		cout<<"输入数据格式或范围有误，请重新输入！\n";
		cin>>ttmp; 
	}
	void (*f)(vector<employee*>* ve,int n);
	if(ttmp[0]=='1')f=sort1;
	else f=sort2;
	f(&tmp,tmp.size());
	for(int i=0;i<tmp.size();i++)
	{
		tmp[i]->print_info(outfile);
	}
	outfile.close();
	cout<<"转存成功！即将返回主界面";
	Sleep(3000);
	return; 
}

void quit()throw(myException)
{
	system("cls");
	cout<<"文件已保存！\n"; 
	cout<<"Goodbye and Good Luck!";
	Sleep(3000); 
	ofstream outfile("employee_info.txt",ios::out);
	if(!outfile)throw(myException("信息保存时发生错误!"));
	vector<employee*> tmp;
	for(int i=0;i<workers.size();i++)
	{
		if(workers[i]==NULL)continue;
		else tmp.push_back(workers[i]);
	}
	sort2(&tmp,tmp.size());
	for(int i=0;i<tmp.size();i++)
	{
		tmp[i]->print_info(outfile);
	}
	outfile.close();
	return;
}

int main()
{
	show_screen();
	try
	{
		init();
		while(true)                  
		{
			int flag;
			string tmp;
			cin>>tmp;
			flag=tmp[0]-'0';
			system("cls");
			switch(flag)
			{
				case 1:
					data_input();
					show_screen();
					break;
				case 2:
					data_query();
					show_screen();
					break;
				case 3:
					data_revise();
					show_screen();
					break;
				case 4:
					data_copy();
					show_screen();
					break;
				case 5:
					quit(); 
					return 0;
				default:
					std::cout<<"输入格式错误！请重试\n";
					Sleep(3000);
					show_screen();
			}
		}		
	}
	catch(myException &e)
	{
		system("cls");
		cout<<e.what();
		system("pause");
		return 0;
	}	
}
