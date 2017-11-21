#include <stdio.h>
#include <stdlib.h>

struct listNode{
	int data;
	struct listNode *next;
};
typedef struct listNode ListNode; 
typedef struct listNode * ListNodePtr; 

void insert(ListNodePtr *, int); // insert �Լ� ����
int delete_(ListNodePtr *, int); // delete �Լ� ����
int isEmpty(ListNodePtr); // isEmpty �Լ� ����
void printList(ListNodePtr); // printList �Լ� ����
void instructions(void); // instructions �Լ� ����

int main()
{
	ListNode * head = 0; // head ��� ���� �� �ʱ�ȭ
	
	int choice = 0; // case ���� �� �ʱ�ȭ
	int input, delete_key; // ������ ������ �Է� ��, ������ ������ �Է� �� ����
	int a, b; // isEmpty �Լ� ����� ��ȯ, delete_ �Լ� ����� ��ȯ ����
	
	instructions(); // instructions �Լ� ȣ��
	
	// choice ���� �ʱ�ȭ�Ͽ��� ������, while 1���� ����.
	while(choice !=3){ // choice�� 3�̸� while�� ����
		if(choice > 3) // choice ���� 3�̻� �Էµ� ��� ��µ� ����.
		{
			printf("1:�Է� \n");
			printf("2:���� \n");
			printf("3:���� \n");
		}
		else // choice�� 3������ ��� ����
			printf("���ϴ� ����� �����ϼ��� <1:�Է�, 2:���� 3:����>");
		scanf("%d", &choice); // choice �� �Է�
		switch(choice)
		{
		case 1 : // �޴����� 1�� ������ ��� ����
			printf("�����͸� �Է��ϼ��� : ");
			scanf("%d", &input); // �Է��� ������ �� �Է�
			insert(&head, input); // insert�Լ� ����
			printf("����Ʈ : \n");
			printList(head); // printList�Լ� ����
			break;

		case 2 : // �޴����� 2�� �Է��� ��� ����
			a = isEmpty(head); // ����Ʈ�� ����ִٸ� 1�� ����, ��������ʴٸ� 0�� ����
			if(a == 0 ) // ����Ʈ�� ������� �ʴٸ�
			{
				printf("������ ���ϴ� �����͸� �Է��ϼ��� : ");
				scanf("%d", &delete_key); // ������ ������ �� �Է�
				b = delete_(&head, delete_key); // delete_ �Լ� ���� �� ��ȯ �� b�� ����
				if(b == 0) // ������ ������ ���� ã���� ��� b�� ���� 0
					printList(head); // ������ ���� �� ����Ʈ ���
				else // ������ ������ ���� ã�� ������ ���
					printf("\n");
			}
			else
				printf("\n");
			break;

		case 3 : // �޴����� 3�� �Է��� ��� ����
			break;

		default : // �޴����� 3�̻� �� �Է½� ����
			printf("�߸��� ������ �ٽ� �����ϼ���\n");
			
			break;
		}
	}
	printf("����Ǿ���.\n"); // ���� �˸� ����
	
}
/*insert �Լ��� ������ ���� �Է¹޾�, ��忡 �߰��ϴ� �Լ�
  �Է½� �������� ������ �ϰ� �Ǿ�������, ���� �� �Է½� ���� ���� ���� ��� �ڿ� �߰�
  ex) 1 -> 1 -> 2 -> 3 -> NULL */
void insert(ListNodePtr *head, int input) // *head�� ���۳���� �ּ� ��, input�� �Է��� ������ ��
{
	ListNode * newNode; // �����͸� �Է¹��� ���ο� ��� ����
	ListNode * temp = 0; // �����͸� �ӽ� ������ ��� ����(���� �� ���� ��� �� ���� �뵵)
	ListNode * prev = 0; // �����͸� �ӽ� ������ ��� ����(���� �� ���� ��� �� ���� �뵵)
	newNode = (ListNode *)malloc(sizeof(ListNode)); // ���ο� ��� �޸� �����Ҵ�
	newNode->data = input; // �Է��� ������ �� ���ο� ��忡 ����
	newNode->next = 0; // ���ο� ��尡 ����Ű�� �ּҸ� 0���� �ʱ�ȭ
	
	if(*head == 0) // *head�� �ּҹ����� 0�̸�, �� ó�� ����� ��
	{
		*head = newNode; // ���ο� ����� ������ ���۳���� �ּҹ����� ����
	}
	else // ��ó�� ��尡 �ƴϸ� ����
	{
		temp = *head; // ���۳���� �ּҸ� �ӽ� ���� ��忡 ����
		
		/* ���� ����� ������ ���� ���ο� ����� �Է� ������ ������ ũ��,
		   ���ο� ���� ���� ��� �տ� ��ġ�ؾ��Ѵ�.*/
		if(temp->data > newNode->data) 
		{
			*head = newNode; // ���ο� ����� �ּҸ� ���� ����� �ּҿ� ����
			newNode->next = temp; // �ӽ� ���� ����� �ּҸ� ���ο� ����� next�� ����.
		}	
		else // ��������� ������ ���� ���ο� ����� �Է� �����ͺ��� �۰ų� ������ ����
		{
			/* while���� ���� 2���� ��� �������� ���ϸ� while�� ����*/
			while(temp != 0 && temp->data <= newNode->data) //�ߺ��� ���� �ԷµǴ� ��� ����
			{
				prev = temp; // �ӽ����� ����� ������ prev �ӽ����� ��忡 ����
				temp = temp->next; // ���� �ӽ� ���� ��尡 ����Ű�� ���� ����� �ּҸ� �ӽ� ���� ��忡 ����
			}
			if(temp == 0) /* while���� ù ��° ���� ���� ���� ���
							 ��, ���Ӱ� ���� ��尡 ���� �������� ���Ե� ���*/
			{
				prev->next = newNode; // ���ο� ����� �ּҸ� �ӽ� ���� ����� next�� ����
			}
			else /* while���� �� ��° ���� ���� ���� ���
					��, ���Ӱ� ���� ��尡 �߰��� ���Ե� ���*/
			{
				newNode->next = prev->next; // ���� ��尡 ����Ű�� ���� ����� �ּҸ� ���ο� ����� next�� ����
				prev->next = newNode; // ���ο� ����� �ּҸ� ���� ����� next�� ����
			}
		}
	}
		
}
// delete_ �Լ��� �Է¹��� ������ ���� ã�� �����ϴ� �Լ�
int delete_(ListNodePtr *del, int delete_key) // *del�� ���� ��� �ּ�, delete_key�� ������ ������ ��
{
	ListNode * temp = *del; // �����͸� �ӽ� ������ ��� ����(���� ���� ���� ��� �� ���� �뵵 �� ���� ��� �ּ� ����)
	ListNode * prev = 0; // �����͸� �ӽ� ������ ��� ����(���� ���� ���� ��� �� ���� �뵵)
	
	/* while���� ���� 2���� ��� �������� ���ϸ� while�� ����*/
	while(temp != 0 && temp->data != delete_key)
		{
			prev = temp; // ���� ���� �� �Ӽ� ���� ����� ���� ���� �ӽ� ���� ��忡 ���� 
			temp = temp->next; // ���� �ӽ� �����尡 ����Ű�� ���� ����� �ּҸ� �ӽ� ���� ��忡 ����
		}
	// ������ ������ �˻��Ͽ�����, ����Ʈ�� ������ �����Ͱ� �������� ������ ����
		if(temp == 0) // while���� ù ��° ���� ���� ���� �� ���� 
		{
			printf("%d�� ����Ʈ�� ����\n",delete_key);
			return 1; // ������ �����Ͱ� �������� ������ �����ϱ� ���� 1�� ����
		}
		// ������ ������ ���� �˻��ϴ� ����, ������ ������ ���� ��ġ�ϴ� ��� ����
		else // while���� �� ��° ���� ���� ���� �� ����
		{
			// while���� �� ��° ������ �ѹ��� ���� ���Ͽ� while���� �� ���� �������� ���� ��� ����
			if(prev == 0) // ������ �����Ͱ� ó�� ��忡 ����ִ� ���
			{
				*del = temp->next; // ���� �ӽ� ���� ��尡 ����Ű�� ���� ����� �ּҸ� ���� ����� �ּҿ� ����
			} 
			// while���� �� ��° ������ �������� ���� ���
			else // ������ �����Ͱ� ó�� ��忡 ������� ���� ���
			{
				prev->next = temp->next; // ���� �ӽ� ���� ��尡 ����Ű�� ���� ����� �ּҸ� ���� �ӽ� ���� ����� next�� ����
				free(temp); // ���� �ӽ� ���� ��� ����
			}
		return 0; // �ش� �����͸� ���������� �����ϱ� ���� 0�� ����
		}
	
}
int isEmpty(ListNodePtr Empty) // ����Ʈ�� ����ִ��� Ȯ���ϴ� �Լ�
{
	if(Empty == 0) // ���޹��� ���� ��尡 NULL�̸� ����Ʈ�� ����ִ�.
	{
		printf("����Ʈ�� �������.\n");
		return 1; // ��������� Ȯ���ϱ� ���� 1�� ����
	}
	else
		return 0; // ������������� Ȯ���ϱ� ���� 0�� ����
}
void printList(ListNodePtr Print) // �Էµ� ������ ������ ����ϴ� �Լ�
{
	ListNode * temp; // �ӽ����� ��� ����
	temp = Print; // ���޹��� ���� ��� ����
	while (temp != NULL) // �ӽ� ���� ����� �ּҰ� 0�̸� while ����
	{
		printf("%d -> ", temp->data) ; // �ӽ����� ����� ������ �� ���
		temp = temp->next; // ���� ����� �ּҸ� �ӽ����� ��忡 ����
	}

	printf("NULL \n");
	printf("\n");
}
void instructions() // ���α׷� ���� �Լ�
{
	printf("�ܼ� ���Ḯ��Ʈ ����\n");
	printf("1�� �Է½� �������Է�\n");
	printf("2�� �Է½� ������ ������ �Է� �� �ش� ������ ���� �� ������ ����\n");
	printf("3�� �Է½� ���α׷� ����\n");

	printf("\n\n");
}