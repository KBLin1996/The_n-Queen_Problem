#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
using namespace std;

// Hill Climbing

void mapping(int **m, int row, int *position, int q)
{
		int i=0;
		// Randonly add the queen into row
		i = rand() % q;

		// Add a queen at m[row][i] again
		m[row][i] = -1;
		position[row] = i;

		return;
}

void attack(int **a, int *position, int q)
{
		int i=0, j=0, row=0, col=0, temp=0;
		int pos[q] = {0};
		// From a[0][0] to a[q][q]
		for(row=0;row<q;row++)
		{
				for(col=0;col<q;col++)
				{
						if(col == position[row])
						{
								continue;
						}
						else
						{
								a[row][col] = 0;
						}
				}
		}

		for(row=0;row<q;row++)
		{
				for(i=0;i<q;i++)
				{
						pos[i] = position[i];
				}
				for(col=0;col<q;col++)
				{
						if(col == position[row])
						{
								continue;
						}
						pos[row] = col;

						// From row=0 to row=q
						for(j=0;j<q;j++)
						{
								i = j + 1;
								temp = pos[j]; // Other queens (Other rows)

								while(i < q)
								{
										// Accumulate the queens on the same row
										if(temp == pos[i])
										{
												a[row][col]++;
										}
										i++;
								}
						}

						// From row=0 to row=q
						for(j=0;j<q;j++)
						{
								i = j;
								temp = pos[j]; // Control variable

								while((i + 1) < q && (temp - 1) >= 0)
								{
										temp--;
										i++;

										// Accumulate the queens on the same row
										if(temp == pos[i])
										{
												a[row][col]++;
										}
								}
						}

						// From row=0 to row=q
						for(j=0;j<q;j++)
						{
								i = j;
								temp = pos[j]; // Control variable

								while((i + 1) < q && (temp + 1) >= 0)
								{
										//	cout << "a" << endl;
										temp++;
										i++;

										// Accumulate the queens on the same row
										if(temp == pos[i])
										{
												a[row][col]++;
										}
								}
						}
				}
		}
		return;
}

void Hill(int **a, int *position, int q, int &min)
{
		int row=0, col=0, temprow=0, tempcol=0;
		for(row=0;row<q;row++)
		{
				for(col=0;col<q;col++)
				{
						if(col == position[row])
						{
								continue;
						}

						else if(min >= a[row][col])
						{
								min = a[row][col];
								temprow = row;
								tempcol = col;
						}
				}
		}

		position[temprow] = tempcol;
		a[temprow][tempcol] = -1;
}


// Genetic Algorithm

struct genetic
{
		int data[60];
		int attack;
		bool operator < (const genetic & a) const
		{
				return (attack < a.attack);
		}
		bool operator > (const genetic & a) const
		{
				return (attack > a.attack);
		}
};

/* Randomly produce population */
void produce(int mapping[], int q)
{
		int i=0, j=0, temp=0;
		for(i=0;i<q;i++)
		{
				mapping[i] = i;
		}

		for(i=0;i<q;i++)
		{
				j = (rand() % q);
				temp = mapping[j];
				mapping[j] = mapping[i];
				mapping[i] = temp;
		}

		return;
}

int attack2(int *data, int q)
{
		int i=0, j=0, temp=0, count=0;
		int pos[q] = {0};

		for(i=0;i<q;i++)
		{
				pos[i] = data[i];
		}

		// From row=0 to row=q
		for(j=0;j<q;j++)
		{
				i = j + 1;
				temp = pos[j]; // Other queens (Other rows)

				while(i < q)
				{
						// Accumulate the queens on the same row
						if(temp == pos[i])
						{
								count++;
						}
						i++;
				}
		}

		// From row=0 to row=q
		for(j=0;j<q;j++)
		{
				i = j;
				temp = pos[j]; // Control variable

				while((i + 1) < q && (temp - 1) >= 0)
				{
						temp--;
						i++;

						// Accumulate the queens on the same row
						if(temp == pos[i])
						{
								count++;
						}
				}
		}

		// From row=0 to row=q
		for(j=0;j<q;j++)
		{
				i = j;
				temp = pos[j]; // Control variable

				while((i + 1) < q && (temp + 1) >= 0)
				{
						temp++;
						i++;

						// Accumulate the queens on the same row
						if(temp == pos[i])
						{
								count++;
						}
				}
		}
		return count;
}

vector <genetic> child;

/* Order Crossover */
void OrderCross(genetic parent1, genetic parent2, int q)
{
		genetic child1, child2;
		int temp1[60] = {0}, temp2[60] = {0};
		int start = (rand() % q), end = (rand() % q);
		int i=0, j=0, temp=0, k=0;
		int x=0, y=0;

		if(start > end)
		{
				temp = start;
				start = end;
				end = temp;
		}

		for(i=0;i<q;i++)
		{
				child1.data[i] = parent1.data[i];
				child2.data[i] = parent2.data[i];
		}

		k = end+1;
		for(i=0;i<q;i++)
		{
				if(k == q)
				{
						k = 0;
				}

				temp1[i] = child2.data[k];
				temp2[i] = child1.data[k];
				k++;
		}

		j=0;
		for(i=start;i<end+1;i++)
		{
				while(j<q)
				{
						if(parent1.data[i] == temp1[j])
						{
								temp1[j] = -1;
						}

						if(parent2.data[i] == temp2[j])
						{
								temp2[j] = -1;
						}
						j++;
				}
		}

		for(i=end+1;(i != start);i++)
		{
				if(i == q)
				{
						i = 0;
				}

				if(i == start)
				{
						break;
				}

				while(temp1[x] == -1)
				{
						x++;
				}

				child1.data[i] = temp1[x];
				x++;
				while(temp2[y] == -1)
				{
						y++;
				}

				child2.data[i] = temp2[y];
				y++;
		}

		child1.attack = attack2(child1.data, q);
		child2.attack = attack2(child2.data, q);

		child.push_back(child1);
		child.push_back(child2);

		return;
}

int main()
{
		int k=0, GA=0, q=0;
		float count=0, success=0;

		cout << "Please enter the algorithm that you want to display (0 for HC, 1 for GA): ";
		cin >> GA;

		if(GA != 0 && GA != 1)	
		{
				cout << "Didn't import the right number, please try it again." << endl;
				return 0;
		}

		cout << "Please enter the number of queens: ";
		cin >> q;
		srand(time(NULL));

		if(GA == 0)
		{
				cout << endl << "Welcome to Hill Climbing ! (Add " << q << "-Queen Problem)" << endl;

				for(k=0;k<30;k++)
				{
						int i=0;
						int min=1000;
						int **map = {0}; // Record the attacks of each position
						int *position = {0}; // Record where the queen is

						map = new int *[q];
						position = new int[q];
						for(i=0;i<q;i++)
						{
								map[i] = new int[q];
						}


						for(i=0;i<q;i++)
						{
								mapping(map,i,position,q);
						}

						for(i=0;i<44;i++)
						{
								attack(map,position,q);
								Hill(map,position,q,min);

								if(min == 0)
								{
										success++;
										break;
								}
						}

						count = count + min;
				}
		}

		else if(GA == 1)
		{
				int iteration = 60000;
				int size = 60;
				cout << endl << "Welcome to Genetic Algorithm ! (Add " << q << "-Queen Problem)" << endl;
				cout << "Iteration = " << iteration << ", Population Size = " << size << endl << endl;

				for(k=0;k<30;k++)
				{
						int i=0;
						iteration = 60000;

						vector <genetic> population, parent;
						genetic best;

						best.attack = 1000;
						population.clear();
						parent.clear();
						child.clear();

						/* Produce the next spring */
						for(i=0;i<size;i++)
						{
								genetic a;
								produce(a.data, q);
								a.attack = attack2(a.data, q);
								population.push_back(a);
						}

						sort(population.begin(),population.end());

						for(i=0;i<size;i++)
						{
								parent.push_back(population[i]);
						}

						/* Do 40000 times Iteration */
						while(iteration--)
						{
								child.clear();
								for(i=0;i<parent.size();i++)
								{
										child.push_back(parent[i]);
								}

								/* Order Crossover */
								for(i=0;i < (parent.size()/2);i++)
								{
										OrderCross(parent[i], parent[parent.size()-i-1], q);
								}

								/* Mutation */
								int mutation = (rand() % q);

								for(i=0;i<child.size();i++)
								{
										if(mutation == 0)
										{
												int start=0, end=0, temp=0;

												start= (rand() % q);
												end = (rand() % q);
												temp = child[i].data[start];

												child[i].data[start] = child[i].data[end];
												child[i].data[end] = temp;
												child[i].attack = attack2(child[i].data, q);
										}
								}

								sort(child.begin(), child.end());
								parent.clear();

								/* Survivor */
								for(i=0;i<(child.size()/2); i++)
								{
										parent.push_back(child[i]);
								}

								if(parent[0].attack < best.attack)
								{
										for(i=0;i<q;i++)
										{
												best.data[i] = parent[0].data[i];
										}
										best.attack = parent[0].attack;
								}

								if(best.attack == 0)
								{
										break;
								}
						}

						if(best.attack == 0)
						{
								success++;
						}
						count = best.attack + count;
				}
		}

		cout << "Success Rate =  " << (success/30) << endl;
		cout << "Average Attack = " << (count / 30) << endl;
		return 0;
}
