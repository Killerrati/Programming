#include<iostream>
#include<vector>
#include<string>
#include<set>
#include<deque>
#include<map>

using namespace std;

class vertex;// Объявляем заранее, чтобы можно было использовать в классе edge

class edge
{
	vertex* begin;//Рёбра ориентированы, поэтому у них есть начало
	vertex* end;// и конец

public:
	edge()
	{
		this->begin = 0;
		this->end = 0;
	}

	edge(vertex* Begin, vertex* End)
	{
		this->begin = Begin;
		this->end = End;
	}

	vertex* Begin()
	{
		return this->begin;
	}
	vertex* End()
	{
		return this->end;
	}

	void setBegin (vertex* Begin)// настройки для использования в классе vertex
	{
		this->begin = Begin;
	}
	void setEnd (vertex* End)
	{
		this->end = End;
	}

};

class vertex//вершина
{
	string name;// хранит в себе свой имя 
	vector<edge*> date;// рёбра, на конце которых лежат "дочернии" вершины
	edge* parent;//и ребро, начало которого хранит "родительскую" вершину
public:
	
	vertex (string Name)
	{
		name = Name;
		parent = 0;
	}

	~vertex()// деструктор, работает рекурсивно, уничтожаю вершину за вершиной в поддереве
	{
		name.~basic_string();
		free (parent);
		for ( int i=0 ; i < date.size() ; i++)
		{
			date[i]->End()->~vertex();
		}
		date.~vector();
	}

	void addEdge (vertex* End)
	{
		edge* temp = new edge(this,End);
		date.push_back(temp);
		End->parent = temp;
	}

	string Name()
	{
		return name;
	}

	int rang()// возвращает количество "исходящих" рёбер
	{
		return date.size();
	}

	edge* operator[](int number)// оператор доступа к рёбрам, для удобства
	{
		return date[number];
	}

	edge* Parent ()
	{
		return parent;
	}

	void Print()// распечатывание дерева , работает рекурсивно, направление рёбер помечает "->"
	{
		for ( int i = 0; i < date.size() ; i++)
		{
			cout<<name<<" -> "<<date[i]->End()->Name()<<endl;
			date[i]->End()->Print();
		}
	}

	bool compare (vertex *other)// функция сравнения поддеревьев, работает рекурсивно
	{
		if ( (this->name != other->name) || (this->date.size() != other->date.size() ) ) return false;// сначала проверяет, совпадают ли имена вершин и их ранги 
		//так-же позволяет не запускать в пустую множество лишних вызовов в цикле ниже.
		for (int i=0 ; i < this->date.size() ; i++)// двойной цикл, в котором смотрим, совпадают ли поддеревья одной вершины с поддеревьями другой. Так как порядок 
			//поддеревьев может быть произвольный, цикл двойной (требуется хотя-бы одно совпадение).
		{
			bool flag = 0;//флаг для хранения, было ли совпадение
			for (int j=0 ; j < other->date.size() ; j++)
			{
				if ( this->date[i]->End()->compare( other->date[j]->End() ) )  flag = 1;
			}
			if (!flag) return false;//если совпадений не было, то сразу ворзащаем ложь
		}
		return true;//если все проверки пройдены, то ворзвращаем правду
	}
};

class DFSiterator// итератор обхода в глубину
{
	vertex* begin;//хранит вершину начала дерева
	edge* last;// последнее ребро, по которому он пришёл
	vertex* now;// и вершину, которую сейчас рассматривает

public:

	DFSiterator(vertex* begin)
	{
		this->begin = begin;
		now = begin;
		last = 0; 
	}

	DFSiterator operator ++(int) 
		/*
		оператор ++, схема работы: вершины храняться в некотором определённом порядке, с помощью которого, зная где ты был до этого шага (для этого служит указатель last)
		можно узнать куда идти дальше... точнее сначала проверяем, можно ли спуститься вниз, учитывая, что мы могли только что от туда прити, затем уже решаем куда нам идти.
		*/
	{
		bool flag = true;// индикатор, который в последствие и будет говорить, куда нам надо идти, вниз или вверх.
		while (flag)// если индикатор правда, то мы идём вверх и затем заново рассматриваем следующий шаг
		{
			edge* nextEdge = 0;// будет хранить ребро, по которому пойдём в следующем шагу
			for (int i = 0; i < now->rang() ; i++)
			{
				if ( ( (*now)[i] == last) && (i != now->rang() -1 ) ) //если на предыдущем шаге мы поднялись наверх, то последнее ребро, по которому мы прошли
					// будет лежать среди исходящих рёбер текущей вершины
				{// и если это так, а так-же это ребро не последнее в списке, то ставим флаг на 0, что означает идём вниз и запоминаем по какому ребру
					nextEdge = (*now)[ i + 1];
					flag = false;
					break;
				}

				if ( ( (*now)[i] == last) && (i == now->rang() -1 ) )// если же ребро было последним в списке, то запоминаем ребро родителя
				{
					nextEdge = now->Parent();
				}

			}

			if ( (now == begin) && (now->rang() == 0 ) ) // специально для корректной обработки одно-вершинных графов
			{
				break;
			}
			
			if (nextEdge == 0 && now == begin && last != 0)// если мы добрались до начальной вершины и алгоритм сверху говорит, что следующее ребро 0 
				//(можно сказать, что пусто, но на самом деле это родитель начальной вершины графа) то значит мы обошли весь граф и можно дальше просто стоять на месте
			{
				flag = 0;
				break;
			}
			
			if ( nextEdge == 0  &&  now->rang() != 0)// если следующее ребро 0 (то-есть пусто) и степень текущей вершины не 0, то мы в ситуации, когда мы впервые вошли
				// в данную вершину и можно начать обход с "первого" ребра
			{
				nextEdge = (*now)[0];
				flag = 0;
			}

			if ( nextEdge == 0  &&  now->rang() == 0)//если следующее ребро 0 (то-есть пусто) и степень вершины так-же 0, то мы в листике дерева, значит идём наверх
			{
				nextEdge = now->Parent();
			}
	
			if (flag == 0)// напоминаю, если флаг 0, значит идём вниз
			{
				now = nextEdge->End();
				last = nextEdge;
			}
			else// иначе наверх
			{
				now = nextEdge->Begin();
				last = nextEdge;
			}
		}

		return *this;
	}

	vertex* operator ->()
	{
		return now;
	}

	bool end()// определяет, завершил ли обход итератор ( одновершинный граф учитывается)
	{
		if (begin->rang() != 0) return ( (now == begin) && (last != 0) );
		else return true;
	}

	void reuse()// позволяет переиспользовать итератор (перености в начало "DFS списка") вне зависимости от нынешнего состояния итератора
	{
		now = begin;
		last = 0;
	}
};

void CreateTree (vertex* begin, int* number, int max, set<int> *name_space)
	/*
	Генератор случайного дерева, началом которого является begin, number служебный счётчик, max - количество вершин в дереве, name_space - служебная куча для хранения
	уже использованных имён (для исключения совпадения). Работает рекурсивным методом
	*/
{
	int rang = rand() % 4;// степень вершины
	for (int i=0 ; i < rang ; i++)// заполнение вершины дочерними
	{
		if ( (*number) == max) break;// если мы достигли лимита, то отсанавливаемся
		char* temp = new char();//для string
		int new_name = ( rand() % (max-1) ) + 1;// создаём имя
		while ( name_space->find(new_name) != name_space->end() ) new_name = ( rand() % (max-1) ) + 1;// и пересоздаём пока не появится новое (которого нет в name_space)
		(*name_space).insert(new_name);// записываем имя в name_space для помечания, что использовано

		sprintf(temp,"%d", new_name );//превращаем его в строку
		(*number)++;
		string temp_String(temp);// и записываем

		vertex* temp_vertex = new vertex(temp_String);// создаём вершину
		begin->addEdge(temp_vertex);// проводим до него ребро
		CreateTree (temp_vertex , number , max, name_space);// и спускаемся рекурсивно
	}
}

vector<string> BFS(vertex* begin)// обход в глубину с последовательный выписыванием в вектор, алгоритм стандартный
{
	deque<vertex*> que;
	vector<string> result;

	que.push_back(begin);
	result.push_back( begin->Name() );

	while(!que.empty())
	{
		vertex* temp = que.front();
		que.pop_front();

		for ( int i=0 ; i < temp->rang() ; i++)
		{
			que.push_back( (*temp)[i]->End() );
			result.push_back( (*temp)[i]->End()->Name() );
		}
	}

	return result;
}

vector< vector<string> > Level_BFS(vertex *begin) // обход в глубину с запоминанием уровней, уровни будут запоминаться прямо в очереди
	// в принципе алгоритм так-же стандартный, единственно что каждая вершина пишет в очередь своих "детей" с увеличенным на 1-цу уровнем
{
	deque<pair<vertex*,int>> que;
	vector<vector<string>> result;

	que.push_back( pair<vertex*,int>( begin, 1) );
	result.push_back ( vector<string> (1) );
	result[0][0] = begin->Name();

	while(!que.empty() )
	{
		vertex* temp = que.front().first;
		int level = que.front().second;
		que.pop_front();
		if ( (result.size() == level) && (temp->rang()!=0) ) result.push_back(vector<string> () );

		for ( int i=0 ; i < temp->rang() ; i++)
		{
			que.push_back( pair <vertex*,int> ( (*temp)[i]->End() , level+1 ) );
			result[level].push_back( (*temp)[i]->End()->Name() );
		}
	}

	return result;
}

vector< vector<string> > Recuva_Level_BFS( vector<string> bfs , DFSiterator iterator)
/*
создаёт из обычных обходов дерева в глубину и ширину уровневый обход в глубину
Костяк алгоритма, который восстанавливает дерево неоднозначно, но с идентичным DFS и BFS
*/
{
	iterator.reuse();
	vector< vector<string> > result(1);
	result[0].push_back( iterator->Name() );// ложем в итоговый массив начальную вершину
	iterator++;// переходим к следующией
	int level = 0;// хранит уровень, который сечас обрабатывается
	bool flag = 0;// индикатор, хранит информацию о совпадениях бфс и дфс потоков, подробнее в цикле...
	set<string> data;// сдесь мы храним вершины, которые мы уже обработали
	data.insert( result[0][0] );
	
	for ( int i=1 ; i < bfs.size() ; i++ )// в цикле обойдём весь бфс
	{
		while ( data.find( iterator->Name() ) != data.end() || iterator.end() ) iterator++;// пролистываем итератор до вершины, в которой мы не были
		if ( iterator->Name() == bfs[i] )// пока значения DFS совпадают со значениями BFS заполняем result, одновременно пролистывая и итератор и BFS
		{
			if ( !flag ) // если потоки до этого не совпадали, а сейчас совпали, мы попали на новый уровень
			{
				level++;
				result.push_back(vector<string>() );
				flag = 1;
			}

			iterator++;
		}
		else// если значения потоков не совпадают (не совпали), значит DFS ушла либо наверх, либо вниз. Вариант, когда она ушла вверх обрабатывается в while вверху
			//(благодаря массиву вершин, которые мы уже обработали, мы можем определить, ушёл ли итератор на вверх или вниз)
			//то бишь он пролистывается назад до нужного уровня (на котором мы сейчас сидим)... ну или до конца массива, тогда далее произойдёт просто дозаполнение
			// уровня с помощью оставшегося куска BFS. Если DFS ушёл вниз, значит весь кусок BFS, который перед нами лежит вплоть до момента совпадения вновь - это
			// вся оставшаяся часть уровня, которую мы записываем и... всё :)
		{
			flag = 0;
		}

		result[level].push_back( bfs[i] );
		data.insert( bfs[i] );
	}

	return result;
}

vertex* Recuva(vector< vector<string> > Level_bfs , DFSiterator iterator)
/*
Собственно ядро обоих алгоритмов, реализованных в программе. Восстанавливает дерево однозначно из уровневой BFS и DFS
*/
{
	iterator.reuse();
	vertex* result = new vertex( iterator->Name() );// начало нового дерева

	map<string,int> Levels;// ассоциирует вершины с уровнями
	map<string,vertex*> memory;// связь имён и самих вершин, оба массива для ускорения поиска и удобства

	for (int i=0 ; i < Level_bfs.size() ; i++)
	for (int j=0 ; j < Level_bfs[i].size() ; j++)// заполняем уровневый массив
	{
		Levels[ Level_bfs[i][j] ] = i;
	}
	vector<int> position( Level_bfs.size() , -1);//массив, хранящий позиции, на которых мы сейчас находимся в BFS на разных уровнях, сделан для памяти от кого "удочеряется" текущая вершина 
	position[0] = 0;
	memory[ iterator->Name() ] = result;
	iterator++;

	while ( !iterator.end() )// цикл по DFS (хотя на самом деле он паралелен по DFS и BFS с шагом 1)
	{
		vertex* temp = new vertex( iterator->Name() );// создаём вершину
		memory [ Level_bfs [ Levels [ iterator->Name() ] - 1 ] [ position [ Levels [ iterator->Name() ] - 1 ] ] ]  -> addEdge(temp);
		//С помощью memory вытягиваем вершину, которая находится "над" текущей. Вытягиваем с помощью position, которая помнит какая это была... ну а уровень выбираем с Level
		memory [temp->Name() ] = temp;//ассоциируем новую вершину с именем её
		position [ Levels [ iterator->Name() ] ]++;
		iterator++;
	}

	return result;
}

void main()
{
	for (int i=1 ; i <= 100 ; i++)// 100 тестов
	{
		int number = (rand() % 50) + 1;// выбираем уоличество вершин(пустое дерево - некорректный ввод)
		int* num = new int(1);
		vertex* Tree = new vertex("0");
		set<int>* name_space = new set<int>;
		
		CreateTree (Tree , num , number, name_space);// создаём
		DFSiterator it(Tree);

		vector<string> bfs = BFS(Tree);//создаём дерево для тестирования неоднозначного восстановления с последующим сравниванием BFS и DFS
		vector< vector<string> >Level_bfs = Recuva_Level_BFS(bfs,it);
		vertex* Test1 = Recuva(Level_bfs,it);
		vector<string> bfs_Test = BFS(Test1);
		DFSiterator it_Test(Test1);
		
		vector< vector<string> > Level_bfs_original = Level_BFS(Tree);// теперь для однозначного
		vertex* Test2 = Recuva(Level_bfs_original,it);

		cout<<endl<<"Test "<<i<<endl<<"compare Original Tree and Tree from BFS,DFS ";// и начинаем тестить, ниже идёт сравнение BFS и DFS соответственно
		bool flag = 1;

		if (bfs.size() == bfs_Test.size() ) 
			for (int j=0 ; j < bfs.size() ; j++)
			{
				if ( bfs[j] != bfs_Test[j] ) flag = 0;
				if (j % 5 == 0) cout<<".";
			}
		else flag = 0;

		it.reuse();
		while ( !it.end() )
		{
			if (it->Name() != it_Test->Name() ) flag = 0;
			it++;
			it_Test++;
		}

		cout<<".....  ";
		if (flag) cout<<"ok"<<endl;
		else 
		{
			cout<<"false"<<endl;
			break;
		}
		cout<<"compare Original Tree and Tree from Level_BFS,DFS .............. ";// тестим однозначное восстановление
		if (Tree->compare(Test2) ) cout<<"ok"<<endl<<endl;
		else 
		{
			cout<<"false"<<endl<<endl;
			break;
		}
	}
	system("pause");// и всё.
}