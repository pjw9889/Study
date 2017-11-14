#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define Input_size 10000 // �ִ� �Է� ũ�� ����

void Selection_sort(int arr[], int n); // �������� �Լ� ����
void Insertion_sort(int arr[], int n); // ���� ���� �Լ� ����
void Bubble_sort(int arr[], int n); // ���� ���� �Լ� ����   
void merge_sort(int arr[], int left, int right); // �պ����� �� Devide �Լ� ����
void merge(int arr[], int left, int mid, int right);    // �պ����� �� Conquer �Լ� ����
void quick_sort(int arr[], int left, int right); // Quick���� �Լ� ����
int partition(int arr[], int left, int right); // Qucik������ Partition ��ġ���� �Լ� ����
void insert_sort(FILE *input, FILE *output, int n); // ��� ���� ���� �Լ� ����
void file_errorchecking(int close); // �Է� �� ���� �ʰ� Ȯ�� �Լ� ����
int merge_sort_cmpcount = 0; // �պ����� �񱳿���Ƚ�� ������ �������� ���� �� �ʱ�ȭ
int quick_sort_cmpcount = -2; // �� ���� �񱳿���Ƚ�� ������ �������� ���� �� �ʱ�ȭ

// �� ���� �񱳿���Ƚ�� ���� ������ �ʱ� ������ -2�� �� ���� �ϳ��� do~while ���ȿ��� �⺻������ 2���� do while���� ������ �Ǳ⶧���Դϴ�.
void main()
{
   int inputnumber; // �Է¹��� ���ڿ� ���� ���� ����
   FILE *input = 0; // FILE ������ ���� �� �ʱ�ȭ
   FILE *output = 0; // FILE ������ ���� �� �ʱ�ȭ

   printf("���ڸ� �Է��ϼ���(���� ������ 1~10000) : ");
   scanf("%d", &inputnumber);
   if (inputnumber > Input_size) // �Էµ� ���� �ִ� �Է� ũ�⺸�� ũ��
      fprintf(stderr, "�Էµ� ���� ������ �� �ִ� ũ�⸦ �Ѿ����ϴ�");
   else
      insert_sort(input, output, inputnumber);
}

void insert_sort(FILE *input, FILE *output, int n) //��� ���� ���� �Լ�
{
   //i�� �ݺ��� ����, Random�� ���� �� �����ϴ� ����, close�� fclose ��ȯ �� ���� ����
	int i, Random, close, load_number; // load_number�� ���Ͽ��� ���� �о�� �� �����ϴ� ����
   int * arr = 0; // ���Ͽ��� �о�� ������ ������ ������ ����
   int * select_arr = 0; // �������� �� ������ ������ ������ ����
   int * insert_arr = 0; // �������� �� ������ ������ ������ ����
   int * bubble_arr = 0; // �������� �� ������ ������ ������ ����
   int * merge_arr = 0; // �պ����ĵ� ������ ������ ������ ����
   int * quick_arr = 0; // �� ���ĵ� ������ ������ ������ ����

   arr = (int *)malloc(sizeof(int)*n); // �Է¹��� ũ�⸸ŭ arr �����Ҵ�
   select_arr = (int *)malloc(sizeof(int)*n); // �Է¹��� ũ�⸸ŭ select_arr �����Ҵ�
   insert_arr = (int *)malloc(sizeof(int)*n); // �Է¹��� ũ�⸸ŭ insert_arr �����Ҵ�
   bubble_arr = (int *)malloc(sizeof(int)*n); // �Է¹��� ũ�⸸ŭ buuble_arr �����Ҵ�
   merge_arr = (int *)malloc(sizeof(int)*n); // �Է¹��� ũ�⸸ŭ merge_arr �����Ҵ�
   quick_arr = (int *)malloc(sizeof(int)*n); // �Է¹��� ũ�⸸ŭ quick_arr �����Ҵ�

   srand((unsigned)time(NULL)); // time seed �� ����
   input = fopen("input1.txt", "wt"); // ���� �������·� ����
   for (i = 1; i <= n; i++)  // �Է¹��� ���ڸ�ŭ �ݺ�
   {
      Random = rand() % 10000 + 1; // 1~10000�� ���� ����
      fprintf(input, "%d\t", Random); // ����� ���Ͽ� ���� �� ����
      if (i % 10 == 0) // 10�� �ԷµǾ��� ������ �� �ٲ�
         fprintf(input, "\n");
   }
   close = fclose(input); // ���� �ݱ�
   file_errorchecking(close); // ���� �������� Ȯ��

   input = fopen("input1.txt", "rt"); // ���� �б����·� ����
   for (i = 0; i<n; i++) // �Է¹��� ���ڸ�ŭ �ݺ�
   {
      fscanf(input, "%d\t", &load_number); // ���� ���� �ϳ� �о����
      arr[i] = load_number; // �о�� ���� arr�� ����
      // ������ �迭���� ���� ������ ���� �� �����Ƿ�, ���� �ϳ��� ���������� ��� �־��ش�.
	  select_arr[i] = arr[i]; 
      insert_arr[i] = arr[i];
      bubble_arr[i] = arr[i];
      merge_arr[i] = arr[i];
      quick_arr[i] = arr[i];
   }
   close = fclose(input); // ���ϴݱ�
   file_errorchecking(close); // ���� �������� Ȯ��

   Selection_sort(select_arr, n); // �������� �Լ� ȣ��, select_arr�� ������ �����Ѵ�.
   output = fopen("output1.txt", "w"); // �������·� ���� ����
   fprintf(output, "��������\n "); // ���� index ���Ͽ� ����
   for (i = 1; i <= n; i++) // �Է¹��� ���ڸ�ŭ �ݺ�
   {
      fprintf(output, "%d\t", select_arr[i - 1]); // select_arr�� ���� ���Ͽ� ����
      if (i % 10 == 0) // 10�� �ԷµǸ� �� �ٲ�
         fprintf(output, "\n");
   }
   fprintf(output, "\n");
   close = fclose(output); // ���� �ݱ�
   file_errorchecking(close); // ���� �������� Ȯ��

   Insertion_sort(insert_arr, n); // �������� �Լ� ȣ��, insert_arr�� ������ �����Ѵ�.
   output = fopen("output1.txt", "a"); // �̾�� ���·� ���� ����
   fprintf(output, "��������\n "); // ���� index ���Ͽ� ����
   for (i = 1; i <= n; i++) // �Է¹��� ���ڸ�ŭ �ݺ�
   {
      fprintf(output, "%d\t", insert_arr[i - 1]); // insert_arr ���� ���Ͽ� ����
      if (i % 10 == 0) // 10�� �ԷµǸ� �� �ٲ�
         fprintf(output, "\n");
   }
   fprintf(output, "\n");
   close = fclose(output); // ���ϴݱ�
   file_errorchecking(close); //���� �������� Ȯ��

   Bubble_sort(bubble_arr, n); // �������� �Լ� ȣ��, buuble_arr�� ������ �����Ѵ�.
   output = fopen("output1.txt", "a"); // �̾�� ���·� ���� ����
   fprintf(output, "��������\n"); // ���� index ���Ͽ� ����
   for (i = 1; i <= n; i++) // �Է¹��� ���ڸ�ŭ �ݺ�
   {
      fprintf(output, "%d\t", bubble_arr[i - 1]); // buuble_arr ���� ���Ͽ� ����
      if (i % 10 == 0) // 10�� �ԷµǸ� �� �ٲ�
         fprintf(output, "\n");
   }
   fprintf(output, "\n");
   close = fclose(output); // ���ϴݱ�
   file_errorchecking(close); // ���� �������� Ȯ��

   merge_sort(merge_arr, 0, n - 1); // �պ����� �Լ� ȣ��, merge_arr�� ������ �����Ѵ�.
   printf("�պ����� �񱳿���Ƚ�� : %d\n", merge_sort_cmpcount); // �պ����� �񱳿���Ƚ�� ���
   output = fopen("output1.txt", "a"); // �̾�� ���·� ���� ����
   fprintf(output, "�պ�����\n"); // ���� index ���Ͽ� ����
   for (i = 1; i <= n; i++) // �Է¹��� ���ڸ�ŭ �ݺ�
   {
      fprintf(output, "%d\t", merge_arr[i - 1]); // merge_arr ���� ���Ͽ� ����
      if (i % 10 == 0) // 10�� �ԷµǸ� �� �ٲ�
         fprintf(output, "\n");
   }
   fprintf(output, "\n"); 
   close = fclose(output); // ���� �ݱ�
   file_errorchecking(close); // ���� �������� Ȯ��

   quick_sort(quick_arr, 0, n - 1); // �� ���� �Լ� ȣ�� quick_arr�� ������ �����Ѵ�.
    printf("������ �񱳿���Ƚ�� : %d\n", quick_sort_cmpcount); // �� ���� �񱳿���Ƚ�� ���
   output = fopen("output1.txt", "a"); // �̾�� ���·� ���� ����
   fprintf(output, "������\n"); // ���� index ���Ͽ� ����
   for (i = 1; i <= n; i++) // �Է¹��� ���ڸ�ŭ �ݺ�
   {
      fprintf(output, "%d\t", quick_arr[i - 1]); // quick_arr�� ������ �����Ѵ�.
      if (i % 10 == 0) // 10�� �ԷµǸ� �� �ٲ�
         fprintf(output, "\n");
   }
   fprintf(output, "\n");
   close = fclose(output); // ���ϴݱ�
   file_errorchecking(close); // ���� �������� Ȯ��
}
void file_errorchecking(int close) // File close ��ȯ ������ ���� ���� ���� ����Ȯ�� �Լ�
{
   if (close != 0)
   {
      printf("������ ������ �ʾҽ��ϴ�.\n");
      exit(1);
   }
}
void Selection_sort(int arr[], int n) // �������� �Լ�
{
   int i, j, least, temp;
   int cmp = 0; // �񱳿��� Ƚ�� ����

   for (i = 0; i<n - 1; i++)
   {
      least = i;
      for (j = i + 1; j<n; j++)
      {
         if (arr[least] > arr[j])
            least = j;
         cmp++; // IF���� �񱳿������� ���Ͽ� �񱳿��� Ƚ�� ����
      }

      temp = arr[i];
      arr[i] = arr[least];
      arr[least] = temp;
   }
   printf("�������� �񱳿���Ƚ�� : %d\n", cmp);
}
void Insertion_sort(int arr[], int n) // �������� �Լ�
{
   int i, j, key;
   int cmp = 0; // �񱳿��� Ƚ�� ����
   for (i = 1; i<n; i++)
   {
      key = arr[i];
      for (j = i - 1; j>-1 && arr[j]>key; j--)
      {
         arr[j + 1] = arr[j];
         cmp++; // for���� arr[j]>key �񱳿����ϹǷ� �񱳿��� Ƚ�� ����
      }
      arr[j + 1] = key;

   }
   printf("�������� �񱳿���Ƚ�� : %d\n", cmp);
}
void Bubble_sort(int arr[], int n) // �������� �Լ� 
{
   int i, j, temp;
   int cmp = 0; // �񱳿��� Ƚ�� ����
   for (i = n - 1; i>0; i--)
   {
      for (j = 0; j<i; j++)
      {
         if (arr[j] > arr[j + 1])
         {
            temp = arr[j];
            arr[j] = arr[j + 1];
            arr[j + 1] = temp;
           
         }
		  cmp++; // IF�� �������� ���� �񱳿��� Ƚ�� ����
      }
   }
   printf("�������� �񱳿���Ƚ�� : %d\n", cmp);
}
void merge_sort(int arr[], int left, int right) // �պ����� �� Devide �Լ� ����
{
   int mid;
   
   if (left<right)
   {
      merge_sort_cmpcount++; // IF�� �������� ���� �񱳿��� ����
      mid = (right + left) / 2;

      merge_sort(arr, left, mid);
      merge_sort(arr, mid + 1, right);
      merge(arr, left, mid, right);
   }  
}

void merge(int arr[], int left, int mid, int right) // �պ����� �� Conquer �Լ� ����
{ 
   int i, j, k, l;
   
   int * temp; // �պ��� ���ο� ������ ����
   i = left;
   j = mid + 1;
   k = left;

   temp = (int *)malloc(sizeof(int)*(right + 1)); // right+1 ũ���� �����Ҵ�

   while (i <= mid && j <= right)
   {
      
      if (arr[i] <= arr[j])
      {
         temp[k++] = arr[i++];
	  }
      else
      {
         temp[k++] = arr[j++];
      }
      	 merge_sort_cmpcount++; // �񱳿��� ���� Ƚ�� ����
   }

   if (i > mid)
   {
      for (l = j; l <= right; l++)
         temp[k++] = arr[l];
   }
   else
   {
      for (l = i; l <= mid; l++)
         temp[k++] = arr[l];
   }
   
   for (l = left; l <= right; l++)
      arr[l] = temp[l];
     
}

void quick_sort(int arr[], int left, int right) // ������ �Լ� 
{
   if (left <= right)
   {
      int mid = partition(arr, left, right);
      quick_sort(arr, left, mid - 1);
      quick_sort(arr, mid + 1, right);
   }
  
}
// �񱳿��� Ƚ�� �����ϴ� ������ ���� -2�̴�.
// do ~ while���� while�� ������ �������� ���ص� 1���� �����Ͽ��� �ϱ⶧����, ���� ���� -2�� �־�����,
// �ʱ� ���� 0�� ����.
int partition(int arr[], int left, int right)// Qucik������ Partition ��ġ���� �Լ�
{
   int pivot = arr[left];
   int low = left, high = right + 1;
   int temp;


   do {
      do
	  {
         low++;
		  quick_sort_cmpcount++; // �񱳿��� Ƚ�� ����
	  }
      while (low <= right && arr[low] < pivot);

      do
	  {
         high--;
		 quick_sort_cmpcount++; // �񱳿��� Ƚ�� ����
	  }
      while (high >= left && arr[high] > pivot);


      if (low < high)
      {
		 quick_sort_cmpcount++; // IF�� �����ϹǷ� �񱳿��� Ƚ�� ����
         temp = arr[low];
         arr[low] = arr[high];
         arr[high] = temp;
      }
   } while (low < high);

   temp = arr[left];
   arr[left] = arr[high];
   arr[high] = temp;

   return high;
}

