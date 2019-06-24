#include"employee.h"

#include<windows.h>
#include<fstream>
#include<algorithm>
#include<strstream>

const int working_hour=100; //������Ա��Ĭ��ÿ�¹���ʱ��
double turnover=200000;	//��Ӫҵ�ܶ���ڼ���ֺ� 
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
	if(strsex=="��")sex=Male;
	else sex=Female;
	if(strvocation==string("Ӫ��Ա"))
	{
		*e=new salesman(name,sex,Vocation(0),grade);
	}
	else if(strvocation==string("����"))
	{
		*e=new manager(name,sex,Vocation(1),grade);
	}
	else if(strvocation==string("������"))
	{
		*e=new technician(name,sex,Vocation(2),grade);
	}
	else if(strvocation==string("���۾���"))
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
	out<<"����: "<<s.name<<' ';
	out<<"�Ա�: ";
	switch(s.sex)
	{
		case 0:
			out<<"�� ";
			break;
		case 1:
			out<<"Ů ";
			break;
		default:
			break;
	}
	out<<"ְҵ: "<<"Ӫ��Ա ";
	out<<"���: "<<s.id<<' ';
	out<<"�ȼ�: "<<s.grade<<' ';
	out<<"�������: "<<s.bonus_per_month;
	out<<endl;
	return out;
}

void salesman::print_info(ostream& out)
{
	out<<"����: "<<name<<' ';
	out<<"�Ա�: ";
	switch(sex)
	{
		case 0:
			out<<"�� ";
			break;
		case 1:
			out<<"Ů ";
			break;
		default:
			break;
	}
	out<<"ְҵ: "<<"Ӫ��Ա ";
	out<<"���: "<<id<<' ';
	out<<"�ȼ�: "<<grade<<' ';
	out<<"�������: "<<bonus_per_month<<' ';
	out<<"�¹����ܶ�: "<<calculate_month_wage();
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
	out<<"����: "<<m.name<<' ';
	out<<"�Ա�: ";
	switch(m.sex)
	{
		case 0:
			out<<"�� ";
			break;
		case 1:
			out<<"Ů ";
			break;
		default:
			break;
	}
	out<<"ְҵ: "<<"���� ";
	out<<"���: "<<m.id<<' ';
	out<<"�ȼ�: "<<m.grade<<' ';
	out<<"��н: "<<m.wage_per_month;
	out<<endl;
	return out;
}

void manager::print_info(ostream& out)
{
	out<<"����: "<<name<<' ';
	out<<"�Ա�: ";
	switch(sex)
	{
		case 0:
			out<<"�� ";
			break;
		case 1:
			out<<"Ů ";
			break;
		default:
			break;
	}
	out<<"ְҵ: "<<"���� ";
	out<<"���: "<<id<<' ';
	out<<"�ȼ�: "<<grade<<' ';
	out<<"��н: "<<wage_per_month<<' ';
	out<<"�¹����ܶ�: "<<calculate_month_wage();
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
	out<<"����: "<<t.name<<' ';
	out<<"�Ա�: ";
	switch(t.sex)
	{
		case 0:
			out<<"�� ";
			break;
		case 1:
			out<<"Ů ";
			break;
		default:
			break;
	}
	out<<"ְҵ: "<<"������ ";
	out<<"���: "<<t.id<<' ';
	out<<"�ȼ�: "<<t.grade<<' ';
	out<<"ʱн: "<<t.wage_per_hour<<' ';
	out<<"�¹����ܶ�: "<<t.calculate_month_wage();
	out<<endl;
	return out;
}

void technician::print_info(ostream& out)
{
	out<<"����: "<<name<<' ';
	out<<"�Ա�: ";
	switch(sex)
	{
		case 0:
			out<<"�� ";
			break;
		case 1:
			out<<"Ů ";
			break;
		default:
			break;
	}
	out<<"ְҵ: "<<"������ ";
	out<<"���: "<<id<<' ';
	out<<"�ȼ�: "<<grade<<' ';
	out<<"ʱн: "<<wage_per_hour<<' ';
	out<<"�¹����ܶ�: "<<calculate_month_wage();
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
	out<<"����: "<<sm.name<<' ';
	out<<"�Ա�: ";
	switch(sm.sex)
	{
		case 0:
			out<<"�� ";
			break;
		case 1:
			out<<"Ů ";
			break;
		default:
			break;
	}
	out<<"ְҵ: "<<"���۾��� ";
	out<<"���: "<<sm.id<<' ';
	out<<"�ȼ�: "<<sm.grade<<' ';
	out<<"��н: "<<sm.wage_per_month<<' ';
	out<<"������ʣ� "<<sm.bonus_per_month<<' '; 
	out<<"�¹����ܶ�: "<<sm.calculate_month_wage();
	out<<endl;
	return out;	
}

void salemanager::print_info(ostream& out)
{
	out<<"����: "<<name<<' ';
	out<<"�Ա�: ";
	switch(sex)
	{
		case 0:
			out<<"�� ";
			break;
		case 1:
			out<<"Ů ";
			break;
		default:
			break;
	}
	out<<"ְҵ: "<<"���۾��� ";
	out<<"���: "<<id<<' ';
	out<<"�ȼ�: "<<grade<<' ';
	out<<"��н: "<<wage_per_month<<' ';
	out<<"������ʣ� "<<bonus_per_month<<' ';
	out<<"�¹����ܶ�: "<<calculate_month_wage();
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
	std::cout<<"       xxx��˾���¹���ϵͳ"<<std::endl;
	std::cout<<std::endl;
	std::cout<<"++++++++++++++++++++++++++++++++++++"<<std::endl;
	std::cout<<std::endl; 
	std::cout<<"####################################"<<std::endl; 
	std::cout<<std::endl;
	std::cout<<"       ���˵�                       "<<std::endl;
	std::cout<<"   1. ��Ϣ¼��                      "<<std::endl;
	std::cout<<"   2. ��Ϣ��ѯ                      "<<std::endl;
	std::cout<<"   3. ��Ϣ�޸�                      "<<std::endl;
	std::cout<<"   4. ��Ϣת��                      "<<std::endl;
	std::cout<<"   5. ���沢�˳�����                "<<std::endl;
	std::cout<<"    ��ѡ����ţ�1-5��               "<<std::endl;
	std::cout<<"####################################"<<std::endl; 
}






int employee::nemployee=0;
long long employee::largest_id=2019000000;
void init()	throw(myException)//���ļ��ж�����Ϣ������ʼ�� 
{
	workers.clear();
	char s[200];
	ifstream infile("employee_info.txt",ios::in);
	if(!infile)throw(myException("�޷��������ݣ���ȷ������ļ��Ƿ���"));
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
	cout<<"����(������ֿո�)��"; 
	cin>>name;
	cout<<"�Ա�\n(����0��ʾ���ԣ�1��ʾŮ��)\n"; 
	cin>>tmp;
	while(tmp.length()!=1||(tmp[0]!='0'&&tmp[0]!='1'))
	{
		cout<<"�������ݸ�ʽ��Χ�������������룡\n";
		cin>>tmp; 
	}
	if(tmp[0]=='0')sex=Male;
	else sex=Female;
	cout<<"ְҵ��\n";
	cout<<"(����0��ʾӪ��Ա��1��ʾ����2��ʾ�������ˣ�3��ʾ���۾���)\n"; 
	cin>>tmp;
	while(tmp.length()!=1||tmp[0]<'0'||tmp[0]>'3')
	{
		cout<<"�������ݸ�ʽ��Χ�������������룡\n";
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
	cout<<"�ȼ���1-5����"; 
	cin>>tmp;
	while(tmp.length()!=1||tmp[0]<='0'||tmp[0]>='6')
	{
		cout<<"�������ݸ�ʽ��Χ�������������룡\n";
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
	cout<<"��Ϣ����ɹ����������������Ϣ��\n";
	new_worker->print_info(cout); 
	cout<<"ȷ����Ӹ�ְ����Ϣ������1������������0\n";
	cin>>tmp;
	while(tmp.length()!=1||tmp[0]<'0'||tmp[0]>'1')
	{
		cout<<"�������ݸ�ʽ��Χ�������������룡\n";
		cin>>tmp; 
	}
	if(tmp[0]=='0')
	{
		cout<<"�����˳�����¼�����";
		Sleep(3000);
		return;
	}
	workers.push_back(new_worker);
	cout<<"����û���Ϣ�ɹ���\n"; 
	system("pause");
	Sleep(3000);
	return;
}

void data_query()
{
	system("cls");
	cout<<"��ѡ���ѯ������1.���� 2.��� ";
	int search_index;
	cin>>search_index;
	while(search_index<=0||search_index>=3)
	{
		cout<<"�������ݸ�ʽ��Χ�������������룡\n";
		cin>>search_index;
	}
	if(search_index==1)
	{
		bool flag=false;
		cout<<"����������ѯ����������";
		string name;
		cin>> name;
		for(int i=0;i<workers.size();i++)
		{
			if(workers[i]==NULL)continue;	//��ָ���Ӧ��Ա��Ϣ�����ѱ�ɾ�� 
			if(workers[i]->judgename(name))
			{
				workers[i]->print_info(cout);
				flag=true;
			}
		}
		if(flag==false)cout<<"����ѯ���󲻴���!\n";
	}
	else
	{
		bool flag=false;
		cout<<"����������ѯ�����ţ�";
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
		if(flag==false)cout<<"����ѯ���󲻴���!\n";
	}
	system("pause"); 
	Sleep(1000);
}

void data_revise()
{
	system("cls");
	cout<<"������������еĲ�����ţ�1.�����򽵼� 2.������۾��� 3.ɾ��Ա����Ϣ 4. �޸Ļ�����Ϣ\n";
	int index;
	cin>>index;
	while(index<=0||index>=5)
	{
		cout<<"�������ݸ�ʽ��Χ�������������룡\n";
		cin>>index;	
	}
	if(index==1)
	{
		cout<<"�����뱻�����߱��:\n";
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
			cout<<"�ö��󲻴���!\n";
		}
		else
		{
			workers[i]->print_info(cout);
			cout<<"����������ߵȼ������޸ĺ�ȼ����ó���5�������õ���1��,ʹ�ø�����ʾ���͵ȼ�����\n";
			int upg;
			cin>>upg;
			if(upg>=0)for(int j=0;j<upg;j++)workers[i]->upgrade();
			else for(int j=0;j>upg;j--)workers[i]->degrade();
			cout<<"�����ɹ�����ǰ����Ա��Ϣ��\n";
			workers[i]->print_info(cout);
			system("pause"); 
		}		
	}
	if(index==2)
	{
		cout<<"�����뱻���Ϊ���۾����߱��:\n";
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
			cout<<"�ö��󲻴���!\n";
		}
		else
		{
			workers[i]->print_info(cout);
			cout<<"ȷ�����Ϊ���۾���������1����������0\n";
			int upg;
			cin>>upg;
			if(upg==1)
			{
				if(workers[i]->vocation!=Salesman&&workers[i]->vocation!=Manager)
				{
					cout<<"���˲���Ӫ��Ա�����������Ϊ���۾���";
					system("pause"); 
				}
				else
				{
					promote(workers[i]);
					delete workers[i];
					workers[i]=NULL;
					cout<<"�����ɹ�����ǰ����Ա��Ϣ��\n";
					workers[workers.size()-1]->print_info(cout);
					system("pause"); 
				}
				
			}
		}	
	}
	if(index==3)
	{
		cout<<"�����뱻�Ƴ��߱��:\n";
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
			cout<<"�ö��󲻴���!\n";
		}
		else
		{
			workers[i]->print_info(cout);
			cout<<"ȷ��ɾ������Ա��Ϣ������1����������0\n";
			int upg;
			cin>>upg;
			if(upg==1)
			{
				delete workers[i];
				workers[i]=NULL;
				cout<<"��ɾ��\n";
				system("pause");
			}
			else
			{
				cout<<"�����˳��ý���\n"; 
			}
		}		
	}
	if(index==4)
	{
		cout<<"�����뱻�޸Ķ�����:\n";
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
			cout<<"�ö��󲻴���!\n";
		}
		else
		{
			workers[i]->print_info(cout);
			cout<<"�������޸ĺ��������";
			string rename;
			cin>> rename;
			cout<<"�������޸ĺ���Ա��Ӧ��ţ�0��ʾ���ԣ�1��ʾŮ�ԣ���";
			string tmp;
			cin>>tmp;
			Sex resex;
			while(tmp.length()!=1||(tmp[0]!='0'&&tmp[0]!='1'))
			{
				cout<<"�������ݸ�ʽ��Χ�������������룡\n";
				cin>>tmp; 
			}
			if(tmp[0]=='0')resex=Male;
			else resex=Female; 
			workers[i]->revise(rename,resex);
			cout<<"�޸ĳɹ���\n";
			system("pause"); 
		}
	}
	Sleep(3000);
}

void data_copy()throw(myException)
{
	system("cls");
	string filename1;
	cout<<"�������½��ı��ļ��ļ�����������.txt)\n";
	cin>>filename1;
	string filename=filename1+string(".txt");
	ofstream outfile(filename,ios::out);
	if(!outfile)throw(myException("�޷��½��ļ���ת����Ӧ��Ϣ"));
	vector<employee*> tmp;
	for(int i=0;i<workers.size();i++)
	{
		if(workers[i]==NULL)continue;
		else tmp.push_back(workers[i]);
	}
	cout<<"��ѡ����Ϣ����ʽ��1��ʾ����ְҵ�͵ȼ�����2��ʾ���ձ�����򣩣�";
	string ttmp;
	cin>>ttmp;
	while(ttmp.length()!=1||ttmp[0]<='0'||ttmp[0]>'2')
	{
		cout<<"�������ݸ�ʽ��Χ�������������룡\n";
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
	cout<<"ת��ɹ�����������������";
	Sleep(3000);
	return; 
}

void quit()throw(myException)
{
	system("cls");
	cout<<"�ļ��ѱ��棡\n"; 
	cout<<"Goodbye and Good Luck!";
	Sleep(3000); 
	ofstream outfile("employee_info.txt",ios::out);
	if(!outfile)throw(myException("��Ϣ����ʱ��������!"));
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
					std::cout<<"�����ʽ����������\n";
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
