#include <stdio.h>
#pragma warning(disable:4996)

int alloc[5][5], claim[5][5], avl[5]; // �Ҵ����, �䱸���, ���� ���� ����
int all_alloc[5] = { 0, 0, 0, 0, 0 }; // �Ҵ�� �� �ڿ��� �� ����
int resource[5], running[5], safe = 0; // �ڿ� ����, ���࿩�� �Ǵ� �迭, ���������Ǵܿ���Ȯ�� ���� ����
int count = 0, i, j, exec; // ���μ��� ���� ����, �ݺ��� ���� i,j, �����Ǵܿ���Ȯ�� ����, 
int process_number, resource_number; // ���μ��� ����, �ڿ� ���� �Է� ����

									 // ���μ��� ����, �ڿ��� ����, �ڿ� ����, �Ҵ����, �䱸����� �Է��Ѵ�. ���μ��� ����, �ڿ� ������ �ִ� 5��
									 // �׸��� �Է��� �ڿ� ����, �Ҵ����, �䱸����� �ٽ� ����Ѵ�.
									 // count�� 0�� �ƴϸ�, �� �Է¹��� ���μ����� ������ 0�� �ƴϸ� ������ �������� �Ǵ��Ѵ�.
int main()
{
	printf("���μ����� ������ �Է��ϼ���(�ִ� 5��) : ");
	scanf("%d", &process_number);
	if (process_number > 5)
	{
		printf("5���� �ʰ��Ͽ����ϴ�\n");
		return 0;
	}
	printf("\n");

	for (i = 0; i < process_number; i++) {
		running[i] = 1; // �Է¹��� ���μ��� ������ŭ running�� �ε��� ���� 1�� �ٲپ��ش�.
						// �̶� 1�� �Էµ� �ε��� ����ŭ ���Ǿ��ٴ� ���̴�.
		count++; // ���μ����� ������ŭ count�� �����Ѵ�. ���� count�� process�� �����̴�.
	}
	printf("�ڿ��� ������ �Է��ϼ���(�ִ� 5��): ");
	scanf("%d", &resource_number);
	if (resource_number > 5)
	{
		printf("5���� �ʰ��Ͽ����ϴ�\n");
		return 0;
	}
	printf("\n");

	printf("�ڿ� ���͸� �Է��ϼ���:");
	for (i = 0; i < resource_number; i++) {
		scanf("%d", &resource[i]);
	}
	printf("\n");

	printf("�Ҵ� ����� �Է��ϼ���.\n");
	for (i = 0; i < process_number; i++) {
		for (j = 0; j < resource_number; j++) {
			scanf("%d", &alloc[i][j]);
		}
	}
	printf("\n");

	printf("�䱸����� �Է��ϼ���.\n");
	for (i = 0; i < process_number; i++) {
		for (j = 0; j < resource_number; j++) {
			scanf("%d", &claim[i][j]);
		}
	}
	printf("\n");

	printf("�Է��� �ڿ����� : ");
	for (i = 0; i < resource_number; i++) {
		printf("\t%d", resource[i]);
	}
	printf("\n");

	printf("�Է��� �Ҵ���� :\n");
	for (i = 0; i < process_number; i++) {
		for (j = 0; j < resource_number; j++) {
			printf("\t%d", alloc[i][j]);
		}

		printf("\n");
	}
	printf("\n");

	printf("�Է��� �䱸���: \n");
	for (i = 0; i < process_number; i++) {
		for (j = 0; j < resource_number; j++) {
			printf("\t%d", claim[i][j]);
		}

		printf("\n");
	}
	printf("\n");

	for (i = 0; i < process_number; i++) {
		for (j = 0; j < resource_number; j++) {
			all_alloc[j] += alloc[i][j];
		}
	}

	printf("�Ҵ�� �ڿ� :");
	for (i = 0; i < resource_number; i++) {
		printf("\t%d", all_alloc[i]);
	}
	printf("\n");

	for (i = 0; i < resource_number; i++) {
		avl[i] = resource[i] - all_alloc[i];
	}

	printf("���� �ڿ� :");
	for (i = 0; i < resource_number; i++) {
		printf("\t%d", avl[i]);
	}
	printf("\n");


	while (count != 0) {
		safe = 0;
		for (i = 0; i < process_number; i++) {
			if (running[i]) { // �Է¹��� ���μ����� ������ 0�� �ƴϸ� ��
				exec = 1; // �����Ǵ� ������ ���� ������ �����Ѵ�.
				for (j = 0; j < resource_number; j++) {
					if (claim[i][j] - alloc[i][j] > avl[j]) { // �䱸��� - �Ҵ������ ���� �ڿ����� ũ�� �����.
						exec = 0;
						break;
					}
				}
				if (exec) { // �����Ǵ� ������ ���� ���̸� ��, ���� ���� ���ǿ��� �������� ���ϰ�, ���μ����� ������ 0 �� �ƴϸ�
					printf("\n");
					running[i] = 0; // ���� �������� ���μ����� ���� 0���� �ٲ۴�. �� ���μ����� �����Ѵ�.
					count--; // �׸��� ���μ����� ���ڸ� �ϳ� ���δ�.
					safe = 1; // ���� ���ǿ��� ������ �ʾұ� ������ ������ �����̹Ƿ� ������ ���¸� �����.

					for (j = 0; j < resource_number; j++) {
						avl[j] += alloc[i][j]; // �׸��� ����� ���μ����� �ڿ��� ��ȯ�Ѵ�.
					}

					break;
				}
			}
		}
		if (!safe) { // ������ ���°� �ƴϸ� �ݺ����� ����������.
			printf("\n");
			printf("�Ҿ����� �����Դϴ�.\n");
			break;
		}
		else { // ������ �����̸� ����
			printf("������ �����Դϴ�.\n");
			printf("Process%d�� �ʿ��� �ڿ��� �����Ͽ� �����Ͽ����ϴ�.\n", i + 1);
			printf("Process%d�� ���� �� �� Process%d�� �ڿ��� �ݳ��Ͽ����ϴ�.\n", i + 1, i + 1);
			printf("���� ���� �ڿ� :");

			for (i = 0; i < resource_number; i++) {
				printf("\t%d", avl[i]);
			}

			printf("\n");
			printf("\n");
		}
	}

	return 0;
}
