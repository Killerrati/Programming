#include<iostream>
#include<vector>

using namespace std;

void main()
{
	int n=0,d=0;
	cin>>n>>d;
	d++;
	vector< pair<int,int> > input(n-1);
	for (int i=0 ; i < n-1 ; i++)
	{
		cin>>input[i].first>>input[i].second;
		input[i].first--;
		input[i].second--;
	}

	vector< vector<int> > main_mass(2*n - 1 , vector<int>(d) );
	vector<int> parent(n);

	for (int i=0 ; i < n ; i++)
	{
		main_mass[i][0] = 0;
		for (int j=1 ; j < d ; j++)
		{
			main_mass[i][j] = d;
		}

		parent[i] = i;
	}

	for (int i=n ; i < 2*n-1 ; i++)
	{
		main_mass[i][0] = d;
	}
	
	for (int i=0 ; i < n-1 ; i++)
	{
		pair <int,int> current_pair ( parent [ input[i].first ] , parent[ input[i].second ] );

		for (int x=0 ; x < d-1; x++)
		{
			int f_first = n;
			for (int y=0 ; y < d ; y++)
			{
				if (f_first > max<int> ( main_mass[ current_pair.first ][x] , main_mass[ current_pair.second ][y] +1 ) ) 
				{
					f_first = max<int> ( main_mass[ current_pair.first ][x] , main_mass[ current_pair.second ][y] +1 );
				}
			}

			int f_second = n;
			for (int y=0 ; y < d ; y++)
			{
				if (f_second > max<int> ( main_mass[ current_pair.second ][x] , main_mass[ current_pair.first ][y] +1 ) ) 
				{
					f_second = max<int> ( main_mass[ current_pair.second ][x] , main_mass[ current_pair.first ][y] +1 );
				}
			}

			main_mass [i+n][x+1] = min (f_first , f_second);
		}
		
		for (int j=0 ; j < n ; j++)
		{
			if ( (parent[j] == current_pair.first) || (parent[j] == current_pair.second) ) parent[j] = i+n;
		}

	}

	int result = n;

	for (int i=0 ; i < d ; i++)
	{
		if ( result > main_mass[2*n-2][i]) result = main_mass[2*n-2][i];
	}

	cout<<endl<<result+1<<endl;

	system("pause");
}
