#include<iostream>
#include<map>
#include<random>
#include<math.h>
#include<set>   
#include<algorithm>

using namespace std;

class vec
{
	vector<int> date;

public:

	vec ()
	{
		date.push_back(0);
	}

	vec (int length)
	{
		if ( length == 0 ) date.push_back(0);
		for( int i=0 ; i<length ; i++ )
			date.push_back(0);
	}

	vec (vector<int> date)
	{
		this->date=date;
	}

	int size ()
	{
		return date.size();
	}

	vector<int> dates()
	{
		vector<int> ret;
		for (int i=0; i < date.size() ; i++)
		{
			ret.push_back(date[i]);
		}
		return ret;
	}

	int& operator [](int adress)
	{
		return date[adress];
	}

	vec operator +(vec other)// оператор сложения, если векторы разной длины, то автоматически удлиняет 'короткий' до 'длинного'
	{
		vector<int> date2 = other.dates();

		if ( other.size() < this->size() )
		{
			for (int i = other.size() ; i < this->size() ; i++ )
			{
				date2.push_back(0);
			}
		}

		for (int i = 0 ; i < this->size() ; i++ )
		{
			date2[i]=date2[i]+date[i];
		}


		vec ret(date2);
		return ret;
	}

	vec operator +(int other)//считает, что int одномерный вектор и складывает его с первой компонентой
	{
		vector<int> date2 = this->dates();

		date2[0]=date2[0]+other;

		vec ret(date2);
		return ret;
	}

	int operator *(vec other)// скалярное произведение
	{
		vector<int> date2 = other.dates();

		if ( other.size() < this->size() )
		{
			for (int i = other.size() ; i < this->size() ; i++ )
			{
				date2.push_back(0);
			}
		}

		int result=0;

		for (int i = 0 ; i < this->size() ; i++ )
		{
			date2[i]=date2[i]*date[i];
			result=result+date2[i];
		}

		return result;
	}

	vec operator *(int other)// умножение вектора на число
	{
		vector<int> result;
		for (int i=0; i < date.size() ; i++)
		{
			result.push_back(date[i]);
			result[i]=result[i]*other;
		}

		vec ret(result);

		return ret;
	}

	vec operator %(int other)
		/*операция остатка от деления. Я постарался сделать, чтобы она сохраняла 'модульность' числа, то есть количество различных векторов не превышало
		число other. Для этого я беру из него корень степени длинны вектора и после этог делаю операцию % ко всем компонентам.
		*/
	{
		vector<int> date2=this->dates();

		int n = pow( (double)other, 1 / (this->size()) );

		for (int k=0 ; k < this->size() ; k++)
		{
			date2[k]=date2[k]%n;
		}

		vec ret(date2);
		return ret;
	}

	bool operator ==(vec other)
	{
		vector<int> temp1=this->dates();
		vector<int> temp2=other.dates();

		if (temp1==temp2) return true;
		else return false;
	}

	bool operator !=(vec other)
	{
		if ( this->operator==(other) ) return false;
		else return true;
	}

	bool operator < (const vec other) const// для работы map
	{
		if ( (int)(vec)*this < (int)(vec)other ) return true;
		else return false;
	}

	operator int()// приведение вектора к числу с помощью взятия модуля от вектора
	{
		int result=0;
		for (int k=0 ; k < this->size() ; k++)
		{
			result= result + date[k]*date[k];
		}

		result = sqrt((double)result);

		return result;
	}

	void print()
	{
		cout<<"( ";
		for ( int k=0; k < this->size() - 1 ; k++)
		{
			cout<<date[k]<<", ";
		}
		cout<<date[this->size() - 1]<<" ) ";
	}
};


template<class T> class funct{// функтор
 public:
	T function(T x, int n)//сама функция
	{
		return (x*2)%n;
	}
};

template<template<class N> class C,class T> int findPreCycleTest(T in ,int n)// функция вычисления длинны прецикла в лоб, через массив.
{
	if (n==NULL) return NULL;
	T x=in;
	map<T,int> mass;
	int i=0;
	C<T> a;
	mass[x]=0;

	while (true) 
	{
		x=a.function(x,n);
		i++;
		if ( mass.find(x)!=mass.end() ) break;
		else 
		{
			mass[x]=i;
		}
	}
	
	return mass[x];
}

template<template<class N> class C,class T> int findPreCycle(T in ,int n)
{
	if ( n==NULL ) return NULL;
	int i=0,k=0;
	T x=in,y,z;
	
	C<T> a;
	
	y=a.function(x,n);

	for( i=0 ; i<=n ; i++ )// "пролистываем" функцию до некоторой точки, которая 100% будет внутри цикла
	{
		y=a.function(y,n);
		//cout<<y<<endl;
	}

	z=a.function(y,n);// переходим на 1 дальше
	i=1;

	while( z!=y )// ищем длинну цикла
	{
		z=a.function(z,n);
		i++;
	}

	k=i;

	y=a.function(x,n);

	for( i=1 ; i<k ; i++ )// "пролистываем" функцию на длинну цикла
	{
		y=a.function(y,n);
	}
	i=0;

	while( x!=y )// теперь начинаем двойное пролистывание функции, пока точка начала и точка, находящаяся на расстояние цикла от первой не встретяться
	{
		y=a.function(y,n);
		x=a.function(x,n);
		i++;
	}//точка встречи и будет длинной прецикла

	return i;
}

void main()
{
	for (int i=0;i<100;i++)//тестирование на целочисленных значениях
	{
		int x=0,n=0;//входные параметры

		int original=0,test=0;//память для самой программы и программы тестера

		x=rand()%100;
		n=rand()%100;

		cout<<endl<<x<<" ; "<<n<<endl;

		original = findPreCycle <funct,int> (x,n);// вычисление оригинальной программы
		cout<<"original................... ok"<<endl;

		test = findPreCycleTest <funct,int> (x,n);// вычисление тестовой программы
		cout<<"test................... ok"<<endl;

		if (original==test) cout<<original<<" = "<<test<<" ................... test complet"<<endl;// сравнение ответа оригинальной программы и теста, если совпадают, то ок
		else 
		{
			cout<<original<<" != "<<test<<" ................... test fail"<<endl;// иначе останавливаемся и пишем fail
			break;
		}
	}

	cout<<endl<<"testing in int variable is complet"<<endl;
	system("pause");
	cout<<endl<<endl;

	for (int i=0;i<100;i++)//тестирование на векторах
	{
		int len = rand() % 10;

		vec x(len);//входные параметры
		int n=0;

		int original=0,test=0;//память для самой программы и программы тестера

		for (int k=0 ; k < len ; k++)// заполняем вектор
		{
			x[k] = rand() % 1000;
		}
		n = rand() % 1000;

		cout<<endl;
		x.print();
		cout<<" ; "<<n<<endl;

		original = findPreCycle <funct,vec> (x,n);
		cout<<"original................... ok"<<endl;

		test = findPreCycleTest <funct,vec> (x,n);
		cout<<"test................... ok"<<endl;

		if (original==test) cout<<original<<" = "<<test<<" ................... test complet"<<endl;
		else 
		{
			cout<<original<<" != "<<test<<" ................... test fail"<<endl;
			break;
		}
	}

	cout<<endl<<"testing in vec variable is complet"<<endl;
	system("pause");
}