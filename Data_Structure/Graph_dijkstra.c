/*

���� indata.txt�� �ҷ����δ�.
indata.txt�� ù ��° �ٿ��� ���� �ϳ�(n)�� �о�鿩 n*n�� 2���� �迭�� �����Ѵ�.
Indata.txt�� ���� �� INF�� -1�� �����Ͽ�����, -1�� �о��� �� INT_MAX ��� ������ ��ȯ�Ѵ�.
�׸��� indata.txt�� ���� �����͵��� �о� 2���� �迭�� �����Ѵ�.
��� �����Ͱ� ����� 2���� �迭�� �׷����� ��Ÿ����.
2���� �迭�� Prim, shortest_path �Լ��� ���ڷ� �ش�.
Prim, shortest_path �Լ� ���� �� Node �迭�� �����Ͽ�, �׷����� Ʈ��ȭ �Ѵ�.
Prim, shortest_path �Լ��� ���� �Ǳ� �� Node �迭�� ����Ѵ�.
shortest_path �Լ��� distance �迭�� ��Ʈ ��忡�� �� ��� �� ����ġ�� �����Ѵ�.
shortest_path �Լ��� ���� �� ��Ʈ ��忡�� �� ��� ���� ����ġ�� ����Ѵ�.

get_min_vertex, prim, choose, shortest_path�� �ҽ� ������ å�� �����Ͽ�����,
�Լ��� ���� ���� �ణ �����Ͽ����ϴ�. 

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_MAX 214748

int get_min_vertex(int * selected, int * dist, int n); // Prim�� ���� ����ġ�� �ּ��� ��带 ã�� �Լ�
void prim(int ** arr, int s, int n); // Prim �Լ�
int choose(int * distance, int n, int * found); // shortest_path �Լ����� �ִܰŸ��� �����ϰ� ���ִ� �Լ�
void shortest_path(int ** arr, int start, int n); // Dijkstra �ִܰŸ� �Լ�
void main()
{
	int input; // ��Ʈ�� ������ ��ȣ �Է�
	int inputnumber; // �Է� ���� ����
	int index; // ���簢 ����� size ����
	int i,j; // �ݺ��� ����
	int **arr; // �Է¹��� �׷����� ������ ������ ����
	
	FILE * inputfile; // input ���� �����͸� ������ ����
	printf(" ��Ʈ�� ������ ��ȣ�� �Է����ּ��� : ");
	scanf("%d", &input);

	inputfile = fopen("indata.txt", "rt"); // indata.txt�� ���� ���������͸� input ������ ����

	fscanf(inputfile, "%d\n", &inputnumber); // input ���������Ϳ��� �ϳ��� ������ �о�´�.
	index = inputnumber; // ó�� �о�� ������ ���簢 ����� size�̴�.

	if(index <input) // indata.txt�� ������ ù ��(n�������� ���) ���뺸�� �Էµ� ��� ���ڰ� ũ��
		fprintf(stderr, "���Ͽ��� �Էµ� ��� ���ں��� �Է��� ��� ���ڰ� Ů�ϴ�.");
	
	else
	{
		// 1���� �迭 ���� �� ������ �迭 ��ҿ� �� �ٽ� �迭�� �����ϰԵǸ�, 2�����迭�� ��������.
		arr = (int **) malloc (sizeof(int*) * index); // index size��ŭ�� 1���� �迭 ����
		for(i = 0; i<index; i++) // 1���� �迭�� ������ ��ҿ� 1���� �迭 ����
			arr[i] = (int *) malloc (sizeof(int) * index); 

		// n*n ������ ��Ŀ� indata.txt�� �����͸� �о���� �ݺ���
		for(i = 0; i<index; i++)
			for(j = 0; j<index; j++)
			{
				fscanf(inputfile, "%d\t", &inputnumber);
				if(inputnumber < 0) // indata.txt���� �о� �� ���� -1�̸�
					inputnumber = INT_MAX; // INF �� ����
				arr[i][j] = inputnumber;
			}

		prim(arr, input, index); // 2���� �迭, ���� ��� ��, indata.txt���� �о�� �迭�� ũ��
		shortest_path(arr, input, index); // 2���� �迭, ���� ��� ��, indata.txt���� �о�� �迭�� ũ��
	}

}

int get_min_vertex(int * selected, int * dist, int n)
{
	int v, i;
	for(i = 0; i<n; i++)
		if(!selected[i])
		{
			v = i;
			break;
		}
	for(i=0; i<n; i++)
	{
		if(!selected[i] && (dist[i] < dist[v]))
			v = i;
	}
	
	return (v);
}

void prim(int **arr, int s, int n)
{
	int i, u, v;
	int * selected;
	int * dist;
	int * Prim_node;
	selected = (int *) malloc(sizeof(int) * n); // ����� ���ÿ��� �Ǵ�, select �迭 n�����ŭ �����Ҵ�
	dist = (int *) malloc(sizeof(int) * n); // ��尣 �Ÿ� �� ���� �迭, dist �迭 n �����ŭ �����Ҵ�
	Prim_node = (int *) malloc(sizeof(int) * n); // Prim �Լ� ���� �� Prim ����� ��Ÿ���� Ʈ�� ������ �迭, Prim_node �迭 n �����ŭ �����Ҵ�

	// dist, selected, Prim_node �迭 �ʱ�ȭ�ϴ� �ݺ���
	for(u = 0; u<n; u++)
	{
		dist[u] = INT_MAX;
		selected[u] = 0;
		Prim_node[u] = 0;
	}

	dist[s] = 0; // ���� ����� �Ÿ��� 0���� �ʱ�ȭ(s node���� s node�� �Ÿ��� 0)
	Prim_node[s] = -1; // ���� ���� ��Ʈ�̹Ƿ� �θ��尡 ����. ���� -1�� ��Ʈ���� ǥ��

	for(i = 0; i<n; i++)
	{
		u = get_min_vertex(selected, dist, n); // ��尣 �Ÿ� �ּ��� ��� u�� ����
		selected[u] = 1; // u ��� ���õ�
		
		if(dist[u] == INT_MAX)
		{
			return ;
		}
		// arr�迭�� ���� INF�� �ƴϰ�, v��° ��尡 ���õ����ʾҰ�, arr[u][v]�� ���� dist[v]���� ������ ����
		else
		{
			for(v = 0; v<n; v++)
				if(arr[u][v] != INT_MAX)
					if(!selected[v] && arr[u][v] < dist[v])
					{
						dist[v] = arr[u][v]; // arr[u][v] ���� �ּ� �Ÿ��̹Ƿ� ����
						Prim_node[v] = u; // u ��带 v��° Prim_node�� ���������ν� Prim_node �迭�� Prim�� ����Ǿ��� ��, Ʈ���� ����ȴ�.
					}
		}
	}
	printf("    Prim set node : ");
	for(v = 0; v<n; v++) // 
		printf("%d\t", Prim_node[v]); // Prim_node�� ����� �迭 ������ ��� -> Ʈ���� ���
	printf("\n"); 
	
}

int choose(int * distance, int n, int * found)
{
	int i, min, minpos;
	min = INT_MAX;
	minpos = -1;
	for(i = 0; i<n; i++)
		if(distance[i] < min && ! found[i])
		{
			min = distance[i];
			minpos = i;
		}
	return minpos;
}

void shortest_path(int **arr, int start, int n)
{
	int i, u, w;
	int * distance; // ����ġ ������ ������ ����, distance ������ ���� ����
	int * found; // ���õ� ��� �ĺ��� ������ ����, found ������ ���� ����
	int * Dijkstra_node; // Dijkstra �Լ��� ����� �� ����� Ʈ���� ������ ������ ����, Dijkstra_node ������ ���� ����

	distance = (int *) malloc(sizeof(int) * n); // n�����ŭ distacne �迭 �����Ҵ�
	found = (int *) malloc(sizeof(int) * n); // n�����ŭ found �迭 �����Ҵ�
	Dijkstra_node = (int *) malloc(sizeof(int) * n); // n�����ŭ Dijkstra_node �迭 �����Ҵ�
	memset(Dijkstra_node, start, sizeof(int) * n); // Dijkstra_node�� n�����ŭ ���� ��� �̸����� �ʱ�ȭ

	// distance �迭�� found �迭�� �ʱ�ȭ
	for(i = 0; i<n; i++)
	{
		distance[i] = arr[start][i];
		found[i] = 0; // 0�� ���� ���õǾ��������� ����� ��
	}

	found[start] = 1; // ��Ʈ ��忡�� �����ϹǷ� ���õǾ��� ����. ���� 1�� ����
	distance[start] = 0; // ��Ʈ ���� ����ġ�� ������ �����Ƿ� 0���� ����
	Dijkstra_node[start] = -1; // ��Ʈ ���� �θ� ��尡 �����Ƿ� 
	for(i = 0; i<n-1; i++)
	{
		u = choose(distance, n , found); // ��Ʈ ��忡�� �ּ� ����ġ�� �� �� �ִ� ��� ��ȣ ����
		found[u] = 1; // u ���� ���õǾ����⿡ 1�� ����
		for(w = 0; w<n; w++)
			if(!found[w])
				if(distance[u] + arr[u][w] < distance[w]) // w��尡 ���õǾ����� �ʾҰ�, u ����� ����ġ + arr[u][w] ���� w����� ����ġ���� ũ��
				{
					distance[w] = distance[u] + arr[u][w]; // u�� ����ġ�� arr[u][w]�� ����ġ�� �� �����Ƿ� w�� ����ġ�� ����
					Dijkstra_node[w] = u; // u�� w�� ���� ��尡 �ȴ�. ����, Dijkstra_node �迭�� w �ε����� u�� �����Ѵ�.
					// ���⼭ ���� ���� �ʴ� ���� �θ� ��尡 ��Ʈ�� ���� ���Ե��� �ʴ´�.
				}
	}
	
	for(w = 0; w<n; w++)
		if(Dijkstra_node[w] > n || Dijkstra_node[w] < -1) // �θ� ��尡 ��Ʈ�� ��쿡 ����
			Dijkstra_node[w] = start; // ��Ʈ ����� ��ȣ�� Dijkstra_node �迭�� w �ε����� �߰��Ѵ�.

	printf("Dijkstra Pre Node : ");
	for(w = 0; w<n; w++)
		printf("%d\t", Dijkstra_node[w]); // Dijkstra_node �迭 ���
	printf("\n");

	printf("  Dijkstra Weight : ");
	for(w = 0; w<n; w++)
		printf("%d\t", distance[w]); // ����ġ �迭 ���
	printf("\n");
}

