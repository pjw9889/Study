#include <stdio.h>
#include <stdlib.h>

#define MAX_ELEMENT 200

typedef struct {
	int heap[MAX_ELEMENT];
	int heap_size;
} HeapType;

void init(HeapType *h); // Heap �ʱ�ȭ �Լ� ����
void insert_max_heap(HeapType *, int element); // �ִ� ���� ������ ���� �Լ� ����
int delete_max_heap(HeapType *h); // �ִ� �� ������ ���� �Լ� ����

void main()
{
	HeapType heap1;
	int e,i,j;
		init(&heap1);
		for(i = 0; i<10; i++)
		{
			printf("���ڸ� �Է��ϼ��� : ");
			scanf("%d", &e);
			insert_max_heap(&heap1, e);
		}
		printf("Insert �Լ� 10�� ��� �� Heap state : ");
		for(i = 1; i<=10; i++)
			printf("%d\t", heap1.heap[i]);
		printf("\n");

		printf("delete �Լ� ��� �� ���� �� : ");
		for(i = 1; i<=10; i++)
		{
			j = delete_max_heap(&heap1);
			printf("%d\t", j);
		}
}

void insert_max_heap(HeapType *h, int element)
{
	int j;
    j = ++(h->heap_size);
	while( (j!=1) && (element > h->heap[j/2]) )
	{
		h->heap[j] = h->heap[j/2];
		j/=2;
    }
    h->heap[j]=element;
}
int delete_max_heap(HeapType *h)
{
	int parent=1, child=2;
    int element, temp;
       
    element=h->heap[1];
	temp=h->heap[(h->heap_size)--];
    while( child <= h->heap_size )
    {
	if((child < h->heap_size) && (h->heap[child] < h->heap[child+1]))
	    child++;
        if( temp >= h->heap[child] ) 
			break;
	    h->heap[parent]=h->heap[child];
        parent=child;
        child*=2;
    }
    h->heap[parent]=temp;
    return element;
}
void init(HeapType *h)
{
	h->heap_size=0;
}