#include<iostream>
#include<vector>
#include<string>
#include<set>
#include<deque>
#include<map>

using namespace std;

class vertex;// ��������� �������, ����� ����� ���� ������������ � ������ edge

class edge
{
	vertex* begin;//и��� �������������, ������� � ��� ���� ������
	vertex* end;// � �����

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

	void setBegin (vertex* Begin)// ��������� ��� ������������� � ������ vertex
	{
		this->begin = Begin;
	}
	void setEnd (vertex* End)
	{
		this->end = End;
	}

};

class vertex//�������
{
	string name;// ������ � ���� ���� ��� 
	vector<edge*> date;// ����, �� ����� ������� ����� "��������" �������
	edge* parent;//� �����, ������ �������� ������ "������������" �������
public:
	
	vertex (string Name)
	{
		name = Name;
		parent = 0;
	}

	~vertex()// ����������, �������� ����������, ��������� ������� �� �������� � ���������
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

	int rang()// ���������� ���������� "���������" ����
	{
		return date.size();
	}

	edge* operator[](int number)// �������� ������� � �����, ��� ��������
	{
		return date[number];
	}

	edge* Parent ()
	{
		return parent;
	}

	void Print()// �������������� ������ , �������� ����������, ����������� ���� �������� "->"
	{
		for ( int i = 0; i < date.size() ; i++)
		{
			cout<<name<<" -> "<<date[i]->End()->Name()<<endl;
			date[i]->End()->Print();
		}
	}

	bool compare (vertex *other)// ������� ��������� �����������, �������� ����������
	{
		if ( (this->name != other->name) || (this->date.size() != other->date.size() ) ) return false;// ������� ���������, ��������� �� ����� ������ � �� ����� 
		//���-�� ��������� �� ��������� � ������ ��������� ������ ������� � ����� ����.
		for (int i=0 ; i < this->date.size() ; i++)// ������� ����, � ������� �������, ��������� �� ���������� ����� ������� � ������������ ������. ��� ��� ������� 
			//����������� ����� ���� ������������, ���� ������� (��������� ����-�� ���� ����������).
		{
			bool flag = 0;//���� ��� ��������, ���� �� ����������
			for (int j=0 ; j < other->date.size() ; j++)
			{
				if ( this->date[i]->End()->compare( other->date[j]->End() ) )  flag = 1;
			}
			if (!flag) return false;//���� ���������� �� ����, �� ����� ��������� ����
		}
		return true;//���� ��� �������� ��������, �� ����������� ������
	}
};

class DFSiterator// �������� ������ � �������
{
	vertex* begin;//������ ������� ������ ������
	edge* last;// ��������� �����, �� �������� �� ������
	vertex* now;// � �������, ������� ������ �������������

public:

	DFSiterator(vertex* begin)
	{
		this->begin = begin;
		now = begin;
		last = 0; 
	}

	DFSiterator operator ++(int) 
		/*
		�������� ++, ����� ������: ������� ��������� � ��������� ����������� �������, � ������� ��������, ���� ��� �� ��� �� ����� ���� (��� ����� ������ ��������� last)
		����� ������ ���� ���� ������... ������ ������� ���������, ����� �� ���������� ����, ��������, ��� �� ����� ������ ��� �� ���� �����, ����� ��� ������ ���� ��� ����.
		*/
	{
		bool flag = true;// ���������, ������� � ����������� � ����� ��������, ���� ��� ���� ����, ���� ��� �����.
		while (flag)// ���� ��������� ������, �� �� ��� ����� � ����� ������ ������������� ��������� ���
		{
			edge* nextEdge = 0;// ����� ������� �����, �� �������� ����� � ��������� ����
			for (int i = 0; i < now->rang() ; i++)
			{
				if ( ( (*now)[i] == last) && (i != now->rang() -1 ) ) //���� �� ���������� ���� �� ��������� ������, �� ��������� �����, �� �������� �� ������
					// ����� ������ ����� ��������� ���� ������� �������
				{// � ���� ��� ���, � ���-�� ��� ����� �� ��������� � ������, �� ������ ���� �� 0, ��� �������� ��� ���� � ���������� �� ������ �����
					nextEdge = (*now)[ i + 1];
					flag = false;
					break;
				}

				if ( ( (*now)[i] == last) && (i == now->rang() -1 ) )// ���� �� ����� ���� ��������� � ������, �� ���������� ����� ��������
				{
					nextEdge = now->Parent();
				}

			}

			if ( (now == begin) && (now->rang() == 0 ) ) // ���������� ��� ���������� ��������� ����-��������� ������
			{
				break;
			}
			
			if (nextEdge == 0 && now == begin && last != 0)// ���� �� ��������� �� ��������� ������� � �������� ������ �������, ��� ��������� ����� 0 
				//(����� �������, ��� �����, �� �� ����� ���� ��� �������� ��������� ������� �����) �� ������ �� ������ ���� ���� � ����� ������ ������ ������ �� �����
			{
				flag = 0;
				break;
			}
			
			if ( nextEdge == 0  &&  now->rang() != 0)// ���� ��������� ����� 0 (��-���� �����) � ������� ������� ������� �� 0, �� �� � ��������, ����� �� ������� �����
				// � ������ ������� � ����� ������ ����� � "�������" �����
			{
				nextEdge = (*now)[0];
				flag = 0;
			}

			if ( nextEdge == 0  &&  now->rang() == 0)//���� ��������� ����� 0 (��-���� �����) � ������� ������� ���-�� 0, �� �� � ������� ������, ������ ��� ������
			{
				nextEdge = now->Parent();
			}
	
			if (flag == 0)// ���������, ���� ���� 0, ������ ��� ����
			{
				now = nextEdge->End();
				last = nextEdge;
			}
			else// ����� ������
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

	bool end()// ����������, �������� �� ����� �������� ( ������������� ���� �����������)
	{
		if (begin->rang() != 0) return ( (now == begin) && (last != 0) );
		else return true;
	}

	void reuse()// ��������� ���������������� �������� (��������� � ������ "DFS ������") ��� ����������� �� ��������� ��������� ���������
	{
		now = begin;
		last = 0;
	}
};

void CreateTree (vertex* begin, int* number, int max, set<int> *name_space)
	/*
	��������� ���������� ������, ������� �������� �������� begin, number ��������� �������, max - ���������� ������ � ������, name_space - ��������� ���� ��� ��������
	��� �������������� ��� (��� ���������� ����������). �������� ����������� �������
	*/
{
	int rang = rand() % 4;// ������� �������
	for (int i=0 ; i < rang ; i++)// ���������� ������� ���������
	{
		if ( (*number) == max) break;// ���� �� �������� ������, �� ���������������
		char* temp = new char();//��� string
		int new_name = ( rand() % (max-1) ) + 1;// ������ ���
		while ( name_space->find(new_name) != name_space->end() ) new_name = ( rand() % (max-1) ) + 1;// � ���������� ���� �� �������� ����� (�������� ��� � name_space)
		(*name_space).insert(new_name);// ���������� ��� � name_space ��� ���������, ��� ������������

		sprintf(temp,"%d", new_name );//���������� ��� � ������
		(*number)++;
		string temp_String(temp);// � ����������

		vertex* temp_vertex = new vertex(temp_String);// ������ �������
		begin->addEdge(temp_vertex);// �������� �� ���� �����
		CreateTree (temp_vertex , number , max, name_space);// � ���������� ����������
	}
}

vector<string> BFS(vertex* begin)// ����� � ������� � ���������������� ������������ � ������, �������� �����������
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

vector< vector<string> > Level_BFS(vertex *begin) // ����� � ������� � ������������ �������, ������ ����� ������������ ����� � �������
	// � �������� �������� ���-�� �����������, ����������� ��� ������ ������� ����� � ������� ����� "�����" � ����������� �� 1-�� �������
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
������ �� ������� ������� ������ � ������� � ������ ��������� ����� � �������
������ ���������, ������� ��������������� ������ ������������, �� � ���������� DFS � BFS
*/
{
	iterator.reuse();
	vector< vector<string> > result(1);
	result[0].push_back( iterator->Name() );// ����� � �������� ������ ��������� �������
	iterator++;// ��������� � ����������
	int level = 0;// ������ �������, ������� ����� ��������������
	bool flag = 0;// ���������, ������ ���������� � ����������� ��� � ��� �������, ��������� � �����...
	set<string> data;// ����� �� ������ �������, ������� �� ��� ����������
	data.insert( result[0][0] );
	
	for ( int i=1 ; i < bfs.size() ; i++ )// � ����� ������ ���� ���
	{
		while ( data.find( iterator->Name() ) != data.end() || iterator.end() ) iterator++;// ������������ �������� �� �������, � ������� �� �� ����
		if ( iterator->Name() == bfs[i] )// ���� �������� DFS ��������� �� ���������� BFS ��������� result, ������������ ����������� � �������� � BFS
		{
			if ( !flag ) // ���� ������ �� ����� �� ���������, � ������ �������, �� ������ �� ����� �������
			{
				level++;
				result.push_back(vector<string>() );
				flag = 1;
			}

			iterator++;
		}
		else// ���� �������� ������� �� ��������� (�� �������), ������ DFS ���� ���� ������, ���� ����. �������, ����� ��� ���� ����� �������������� � while ������
			//(��������� ������� ������, ������� �� ��� ����������, �� ����� ����������, ���� �� �������� �� ����� ��� ����)
			//�� ���� �� �������������� ����� �� ������� ������ (�� ������� �� ������ �����)... �� ��� �� ����� �������, ����� ����� ��������� ������ ������������
			// ������ � ������� ����������� ����� BFS. ���� DFS ���� ����, ������ ���� ����� BFS, ������� ����� ���� ����� ������ �� ������� ���������� ����� - ���
			// ��� ���������� ����� ������, ������� �� ���������� �... �� :)
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
���������� ���� ����� ����������, ������������� � ���������. ��������������� ������ ���������� �� ��������� BFS � DFS
*/
{
	iterator.reuse();
	vertex* result = new vertex( iterator->Name() );// ������ ������ ������

	map<string,int> Levels;// ����������� ������� � ��������
	map<string,vertex*> memory;// ����� ��� � ����� ������, ��� ������� ��� ��������� ������ � ��������

	for (int i=0 ; i < Level_bfs.size() ; i++)
	for (int j=0 ; j < Level_bfs[i].size() ; j++)// ��������� ��������� ������
	{
		Levels[ Level_bfs[i][j] ] = i;
	}
	vector<int> position( Level_bfs.size() , -1);//������, �������� �������, �� ������� �� ������ ��������� � BFS �� ������ �������, ������ ��� ������ �� ���� "�����������" ������� ������� 
	position[0] = 0;
	memory[ iterator->Name() ] = result;
	iterator++;

	while ( !iterator.end() )// ���� �� DFS (���� �� ����� ���� �� ��������� �� DFS � BFS � ����� 1)
	{
		vertex* temp = new vertex( iterator->Name() );// ������ �������
		memory [ Level_bfs [ Levels [ iterator->Name() ] - 1 ] [ position [ Levels [ iterator->Name() ] - 1 ] ] ]  -> addEdge(temp);
		//� ������� memory ���������� �������, ������� ��������� "���" �������. ���������� � ������� position, ������� ������ ����� ��� ����... �� � ������� �������� � Level
		memory [temp->Name() ] = temp;//����������� ����� ������� � ������ �
		position [ Levels [ iterator->Name() ] ]++;
		iterator++;
	}

	return result;
}

void main()
{
	for (int i=1 ; i <= 100 ; i++)// 100 ������
	{
		int number = (rand() % 50) + 1;// �������� ���������� ������(������ ������ - ������������ ����)
		int* num = new int(1);
		vertex* Tree = new vertex("0");
		set<int>* name_space = new set<int>;
		
		CreateTree (Tree , num , number, name_space);// ������
		DFSiterator it(Tree);

		vector<string> bfs = BFS(Tree);//������ ������ ��� ������������ �������������� �������������� � ����������� ������������ BFS � DFS
		vector< vector<string> >Level_bfs = Recuva_Level_BFS(bfs,it);
		vertex* Test1 = Recuva(Level_bfs,it);
		vector<string> bfs_Test = BFS(Test1);
		DFSiterator it_Test(Test1);
		
		vector< vector<string> > Level_bfs_original = Level_BFS(Tree);// ������ ��� ������������
		vertex* Test2 = Recuva(Level_bfs_original,it);

		cout<<endl<<"Test "<<i<<endl<<"compare Original Tree and Tree from BFS,DFS ";// � �������� �������, ���� ��� ��������� BFS � DFS ��������������
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
		cout<<"compare Original Tree and Tree from Level_BFS,DFS .............. ";// ������ ����������� ��������������
		if (Tree->compare(Test2) ) cout<<"ok"<<endl<<endl;
		else 
		{
			cout<<"false"<<endl<<endl;
			break;
		}
	}
	system("pause");// � ��.
}