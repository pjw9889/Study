#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct // ���� ����ü ����
{
	int stack[100];
	int top; 
} StackType;

void init(StackType *s); // ���� �ʱ�ȭ �Լ� ����
int is_empty(StackType *s); // ���� ���� ���� Ȯ�� �Լ� ����
int is_full(StackType *s); // ���� ��ȭ ���� Ȯ�� �Լ� ����
void push(StackType *s, int item); // ���� �����Լ� ����
int pop(StackType *s); // ���� �����Լ� ����
int peek(StackType *s); // top�� ����Ű�� �ε����� ������ ���� �Լ� ����

enum state {FALSE, TRUE}; // ���� ������ ����

struct _exp {		// ������ �� ��� ���� ����ü
	int number;		// ���� ����
	enum state is_op;	// ������ Ȯ�� ���� �������� ��� TRUE, �ǿ������� ��� FALSE
};

typedef struct exp {	//�������� ���̿� �� ���� ����ü
	struct _exp newExp[250];	//������ ���� ����ü �迭
	int length;		//�������� ���� ����
} Exp;

int prec(char op); // ������ �켱���� ��ȯ �Լ� ����
int infix_to_postfix(char*, Exp*); // ����ǥ��->����ǥ�� ���� �Լ� ����
int calculrate(Exp*); // ������ ��� �Լ� ����

void main()
{
	char str[1000]; // �Է¹��� �迭 ����
	Exp newExp[250];	// ������ ���� ����ü
	int true_function; // �Լ��� Error ���� ������ ����
	int result; // �Է¹��� ���� ���� �� ������ ����

	printf("���� ������ �Է����ּ��� : ");
	gets(str);   // str�迭�� �������� �Է�
	printf("����ǥ��� : ");
	true_function = infix_to_postfix(str, newExp); // ���� ���� -> ���� ���� ����

	if (true_function) // �Է¹��� �����Ŀ� ���ݴ� ��ȣ Matching Error�߻� �����ʾҴٸ� ����
	{
		result = calculrate(newExp); //	�Է¹��� ���Ŀ� ���� �� ����ϴ� �Լ� ������ �Լ� ����
		printf("���� ǥ����� ���� ���� : %d\n", result);
	}
	else // �Է¹��� �����Ŀ� ���ݴ� ��ȣ Matching Error�߻���
		fprintf(stderr, "��ȣ ������ �߸��Ǿ� ������ �� �����ϴ�.\n");

}

void init(StackType *s) // ���� �ʱ�ȭ �Լ�
{
	s->top = -1; // ������ top ���� -1�� �ʱ�ȭ
}

int is_empty(StackType *s) // ���� ���� ���� Ȯ�� �Լ�
{
	if (s->top == -1) // ������ top ���� ���� -1�̸� ��������̹Ƿ� true �� return
		return 1;
	else // ������ top ���� ���� -1�� �ƴϸ� ������°� �ƴϹǷ� false �� return
		return 0;
}

int is_full(StackType *s) // ���� ��ȭ ���� Ȯ�� �Լ�
{
	// ���� ����ü�� �迭 ũ�Ⱑ 100�̹Ƿ� �ִ� �ε����� 99�̴�.
	if (s->top == 99) // ������ top ���� ���� 99�̸� ��ȭ�����̹Ƿ� true �� return
		return 1;
	else // ������ top ���� ���� 99�� �ƴϸ� ��ȭ���°� �ƴϹǷ� false �� return
		return 0;
}

void push(StackType *s, int item) // ���ÿ� ������ push�ϴ� �Լ�
{
	if (is_full(s)) // ������ ����á����
	{
		fprintf(stderr, "������ ���� á���ϴ�. \n"); // ������ ����á�ٴ� Error return
		exit(-1);
	}
	else // ������ �������� �ʾ�����
	{
		s->top += 1; // ������ top ���� ���� 1 ������Ų��.
		s->stack[(s->top)] = item; // ���� �迭�� top ���� ���� �ε����� item ���� push

	}
}

int pop(StackType *s) // ���ÿ� ������ pop�ϴ� �Լ�
{
	if (is_empty(s)) { // ������ ����ִٸ�
		fprintf(stderr, "������ ����ֽ��ϴ�.\n"); // ������ ����ִٴ� Error return
		exit(-1);
	}
	else // ������ ������� �ʴٸ�
	{
		int rindex; // pop�� �����Ͱ� ����� �ε��� �� ������ ����
		rindex = s->top; // pop�� �����Ͱ� ����� �ε��� ���� rindex�� ����
		s->top -= 1; // ������ top ���� ���� 1 ���ҽ�Ų��.
		return s->stack[rindex]; // rindex�� ���� return
	}
}

int peek(StackType *s) // ������ top ���� ���� ����Ű�� �迭 �ε��� ���� ���� �Լ�
{
	if (is_empty(s)) // ������ ����ִٸ�
	{
		fprintf(stderr, "������ ����ֽ��ϴ�.\n"); //������ ����ִٴ� Error return
		exit(-1);
	}
	else return s->stack[s->top]; // ������ ��������ʴٸ� top ���� ���� ����Ű�� �ε����� ������ return
}


int prec(char op) // ������ �켱���� ���� �Լ�
{
	switch (op) {
	case '[': case ']': return 0; // ���ȣ�� 0
	case '{': case '}': return 1; // �߰�ȣ�� 1
	case '(': case')': return 2; // �Ұ�ȣ�� 2
	case '+': case '-': return 3; // ����, ������ 3
	case '*': case '/': return 4; // ����, �������� 4
	}
	return -1;
}

int infix_to_postfix(char exp[], Exp newexp[]) {	// ������ -> ������ ��ȯ �Լ�
	StackType s; // ���ñ���ü ����
	int i; // �ݺ��� ��� ����
	int newIdx = 0; // ���������� �迭�� �ε���
	int temp; // atoi�� ����� �����ϴ� ����
	char tok, top_op;	//tok:���� �� ��Ҹ� ����, top_op:���ÿ��� ���� ��Ҹ� ����
	int length = strlen(exp);	//����ڷκ��� ���� ���� ���� ����
	newexp->length = 0;	//������ �迭�� ���� �ʱ�ȭ
	
	init(&s);	//���� �ʱ�ȭ

	for (i = 0; i < length; i++) {
		tok = exp[i];

		if (isdigit(tok)) {		//tok�� ���ڹ����� ���
			temp = atoi(&exp[i]);	//���� ���ڸ� ���� ���ڷ� ��ȯ

			while (isdigit(exp[i]))	//���� �����ڸ� ���������� i�� ����
				i++;
			i--;

			//������ �迭�� atoi ��� ����, ���� ����
			newexp->newExp[newIdx].number = temp;
			newexp->newExp[newIdx++].is_op = FALSE;
			newexp->length++;
			continue;
		}
		else if (tok == '-' && i==0) {		//�� �տ� ������ ���� ���

			temp = atoi(&exp[i]);	//���� ���ڸ� ���� ���ڷ� ��ȯ

			while (isdigit(exp[i + 1]))	//���� �����ڸ� ���������� i�� ����
				i++;

			//������ �迭�� atoi ��� ����, ���� ����
			newexp->newExp[newIdx].number = temp;
			newexp->newExp[newIdx++].is_op = FALSE;
			newexp->length++;
			continue;
		}
		else if (tok == '-' && exp[i-1]!=')' && !isdigit(exp[i - 1])) {	//�߰��� ������ ���� ���

			temp = atoi(&exp[i]);	//���� ���ڸ� ���� ���ڷ� ��ȯ

			while (isdigit(exp[i+1]))	//���� �����ڸ� ���������� i�� ����
				i++;

			//������ �迭�� atoi ��� ����, ���� ����
			newexp->newExp[newIdx].number = temp;
			newexp->newExp[newIdx++].is_op = FALSE;
			newexp->length++;
			continue;
		}

		switch (tok) {
		case '+': case '-': case '*': case '/': // ch ���� +, -, *, /�� ���
			while (!is_empty(&s) && (prec(tok) <= prec(peek(&s)))) {// ������ ��������ʰ�, ch�� �켱���� < ������ top ���� ���� ����Ű�� �������� �켱�����̸�
				newexp->newExp[newIdx].number = pop(&s); // ���� pop ����
				newexp->newExp[newIdx++].is_op = TRUE;	// �������̹Ƿ� is_op�� TRUE�� ����
				newexp->length++;	// �������̹Ƿ� is_op�� TRUE�� ����
			}
			push(&s, tok); // �� while���� ������ �������� �ʴ� ���, ch�� ���� ���ÿ� push
			break;
		case '(': case '{': case '[': // ch���� ���� ��ȣ(��,��,��)�� ���
			push(&s, tok); // ���ÿ� push ����
			break;

		case ')': // ch���� �ݴ� ��ȣ(��)�� ���
			while (1) {
				if(s.top == -1) // �� �̻� �ҷ��� ���� �Ұ�ȣ�� ������ Error ���� ��ȯ
					return 0; 
				else
				{
					top_op = pop(&s); // ���ÿ��� pop�Ͽ� top_op�� ����
					if (top_op == '(') // top_op�� ���� ���� �Ұ�ȣ�̸� �����.
						break;
					else if (top_op == '{' || top_op == '[') // top_op�� ���� �Ұ�ȣ�� �ƴ� �ٸ� ��ȣ�̸�
						return 0;
					else {	//���ÿ��� ���� ������ �����Ŀ� ����
						newexp->newExp[newIdx].number = top_op;
						newexp->newExp[newIdx++].is_op = TRUE;
						newexp->length++;
					}
				}
			}
			break;

		case '}': // ch���� �ݴ� ��ȣ(��)�� ���
			while (1) {
				if(s.top == -1) // �� �̻� �ҷ��� ���� �߰�ȣ�� ������ Error ���� ��ȯ
					return 0; 
				else
				{
					top_op = pop(&s); //���ÿ��� pop�Ͽ� top_op�� ����
					if (top_op == '{') // top_op�� ���� ���� �߰�ȣ�̸� �����.
						break;
					else if (top_op == '(' || top_op == '[') // top_op�� ���� �߰�ȣ�� �ƴ� �ٸ� ��ȣ�̸�
						return 0;
					else {	//���ÿ��� ���� ������ �����Ŀ� ����
						newexp->newExp[newIdx].number = top_op;
						newexp->newExp[newIdx++].is_op = TRUE;
						newexp->length++;
					}
				}
			}
			break;
		case ']': // ch���� �ݴ� ��ȣ(��)�� ���
			while (1) {
				if(s.top == -1) // �� �̻� �ҷ��� ���� ���ȣ�� ������ Error ���� ��ȯ
					return 0; 
				else
				{
					top_op = pop(&s); //���ÿ��� pop�Ͽ� top_op�� ����
					if (top_op == '[') // top_op�� ���� ���� ���ȣ�̸� �����.
						break;
					else if (top_op == '(' || top_op == '{')// top_op�� ���� ���ȣ�� �ƴ� �ٸ� ��ȣ�̸�
						return 0;
					else {	//���ÿ��� ���� ������ �����Ŀ� ����
						newexp->newExp[newIdx].number = top_op;
						newexp->newExp[newIdx++].is_op = TRUE;
						newexp->length++;
					}
				}
			}
			break;
		}
	}
	while (!is_empty(&s)) // ������ ��������ʴٸ�
	{
		top_op = pop(&s); // ���ÿ��� pop�Ͽ� top_op ������ ���� �����Ѵ�.
		if (top_op == '(' || top_op == '{' || top_op == '[') // top_op ���� ���� ��ȣ(��,��,��)�̸�
			return 0;
		else {		//���ÿ��� ���� ������ �����Ŀ� ����
			newexp->newExp[newIdx].number = top_op;
			newexp->newExp[newIdx++].is_op = TRUE;
			newexp->length++;
		}
	}

	//������ ���
	for (i = 0; i < newexp->length; i++) {
		if (newexp->newExp[i].is_op)	//�������� ���
			printf("%c", newexp->newExp[i].number);		//���ڷ� ���
		else	//�ǿ������ϰ��
			printf("%d", newexp->newExp[i].number);		//���ڷ� ���
	}
	puts("");

	return 1;
}


int calculrate(Exp *Postfix)
{
	StackType cal;		//���� ����
	int tok;		//�������� �� ��Ҹ� ������ ����
	int op1, op2, cal_result;	//���ÿ��� ���� 2���� �ǿ����ڸ� ������ ������ op1�� op2�� ��갪�� ������ ����
	int i = 0, length;	//�������� ���̿� �ݺ����� ���� ����
	int sum;	//������� ������ ����

	init(&cal);	//���� �ʱ�ȭ
	length = Postfix->length;	//�������� ���� ����

	for (i = 0; i < length; i++)
	{

		tok = Postfix->newExp[i].number;	//�������� ��Ҹ� ����

		if ((tok == '+' || tok == '-' || tok == '*' || tok == '/') && Postfix->newExp[i].is_op) {	//������ ��Ұ� �������� ���
			op2 = pop(&cal);	//���ÿ� �ִ� ��Ҹ� ����
			op1 = pop(&cal);	//���ÿ� �ִ� ��Ҹ� ����

			if (tok == '+')		//tok�� ��Ģ������ ��� ������ ����Ͽ� value�� ����
				cal_result = op1 + op2;
			else if (tok == '-')
				cal_result = op1 - op2;
			else if (tok == '*')
				cal_result = op1 * op2;
			else if (tok == '/')
				cal_result = op1 / op2;
			else
				return 0;

			push(&cal, cal_result);	//��� ����� ���ÿ� �ִ´�
		}
		else
			push(&cal, tok); // ������ ��Ұ� �ǿ������� ��� ���ÿ� �ִ´�
	}

	sum = pop(&cal);		//������� ���ÿ��� ������

	if (is_empty(&cal))	//������ ������� Ȯ�� �� ����� ��ȯ
		return sum;
	else
	{
		fprintf(stderr, "Calculrate Error\n");
		exit(1);
	}
}

