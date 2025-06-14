#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

int input(int min, int max, int prompt_i); // ������ ���� �Էµ� �� ���� ��� �Է¹޴� �Լ�
void print_train(int M_pos, int Z_pos, int C_pos, int train_length); // ���� ���� ���

int Stage1(int len, int stamina, int p);
int Stage2(int len, int stamina, int p);
//int Stage3(int len, int stamina, int p);
//int Stage4(int len, int stamina, int p);

#define LEN_MIN 15 // ���� ����
#define LEN_MAX 50
#define STM_MIN 0  // ������ ü��
#define STM_MAX 5
#define PROB_MIN 10 // Ȯ��
#define PROB_MAX 90 
#define AGGRO_MIN 0 // ��׷� ����
#define AGGRO_MAX 5

// ������ �̵� ����
#define MOVE_LEFT  1
#define MOVE_STAY  0

// �����ǰ��ݴ��
#define ATK_NONE  0
#define ATK_CITIZEN  1
#define ATK_DONGSEOK  2

// �������ൿ
#define ACTION_REST  0
#define ACTION_PROVOKE  1
#define ACTION_PULL  2

char prompt[7][50] = {
	"train length(15~50)>> ",
	"madongseok stamina(0~5)>> ",
	"percentile probability 'p'(10~90)>> ",
	"madongseok move(0:stay, 1:left)>> ",
	"madongseok move(0:stay)>> ",
	"madongseok move(0:rest, 1:provoke)>> ",
	"madongseok move(0:rest, 1:provoke, 2. pull)>> "
};

int main(void) {
	// ��Ʈ��
	// ���� �ð��� ���� ���� �õ� ����
	srand((unsigned int)time(NULL));
	int len, stamina, p;
	// ���� ���� �Է�
	len = input(LEN_MIN, LEN_MAX, 0);
	// ������ ���׹̳� �Է�
	stamina = input(STM_MIN, STM_MAX, 1);
	// Ȯ�� �Է�
	p = input(PROB_MIN, PROB_MAX, 2);

	int result;
	
	result = Stage1(len, stamina, p);
	if (result == 1) {
		printf("\n\n-----------------\nStage1 Clear!!!\n-----------------\n\nNext Stage...\n\n");
		result = Stage2(len, stamina, p);
	}

	// �ù� �¸� ��
	if (result == 1) printf("SUCCESS!\ncitizen(s) escaped to the next train");
	// ���� �¸� �� (�ù� ���)
	else if (result == 2) printf("GAME OVER! citizen dead...");
	// ���� �¸� �� (������ ���)
	else if (result == 3) printf("GAME OVER! madongseok dead...");

	return 0;
}

int input(int min, int max, int prompt_i) {
	int n;
	while (1) {
		printf("%s", prompt[prompt_i]);
		scanf_s(" %d", &n);
		if (min <= n && n <= max)
			break;
	}
	return n;
}

void print_train(int M_pos, int Z_pos, int C_pos, int train_length) {
	// ���� ���
	for (int i = 1; i <= 3; i++) {
		// Ȧ �� ���� ���� #���� �� ǥ�� 
		if (i % 2 == 1) {
			for (int j = 0; j < train_length; j++) {
				printf("#");
			}
			printf("\n");
		}
		// ¦ �� ���� �� �������� ���� ���
		else {
			for (int j = 0; j < train_length; j++) {
				if (j == 0 || j == train_length - 1) printf("#");
				else if (j == M_pos) printf("M");
				else if (j == Z_pos) printf("Z");
				else if (j == C_pos) printf("C");
				else printf(" ");
			}
			printf("\n");
		}
	}
	printf("\n\n");
}
void print_train2(int M_pos, int Z_pos, int C_pos, int V_pos, int train_length) {
	// ���� ���
	for (int i = 1; i <= 3; i++) {
		// Ȧ �� ���� ���� #���� �� ǥ�� 
		if (i % 2 == 1) {
			for (int j = 0; j < train_length; j++) {
				printf("#");
			}
			printf("\n");
		}
		// ¦ �� ���� �� �������� ���� ���
		else {
			for (int j = 0; j < train_length; j++) {
				if (j == 0 || j == train_length - 1) printf("#");
				else if (j == M_pos) printf("M");
				else if (j == Z_pos) printf("Z");
				else if (j == C_pos) printf("C");
				else if (j == V_pos) printf("V");
				else printf(" ");
			}
			printf("\n");
		}
	}
	printf("\n\n");
}

int Stage1(int len, int stamina, int p) {
	// ���� �ʱ� ���� ���
	int M = len - 2; // ������ ��ġ
	int Z = len - 3; // ���� ��ġ
	int C = len - 6; // �ù� ��ġ

	int C_aggro = 1; // �ù� ��׷�
	int M_aggro = 1; // ������ ��׷�

	int M_move; // ������ ������

	int Z_pulled = 0;
	int C_prev;
	int Z_prev;

	int Win = 0; // �¸� ����, 1 : �ù� �¸�, 2 : ���� �¸� (�ù� ���), 3: ���� �¸� (������ ���)

	// ���� ���
	print_train(M, Z, C, len);

	int random;
	int turn = 1;
	while (1) {
		// �̵� ������
			// �ù� �̵�
		random = rand() % 100 + 1; // 1~100 ���� ���� ����
		if (random > p) { // Ȯ�� ���� �� �������� �� ĭ �̵�
			C_prev = C;
			C--;
		}
		else C_prev = 0;

		// ���� �̵�
			// ����� ���� Ȧ�� �϶��� ������ �� ����
		if (turn % 2 == 1 && !Z_pulled) {
			Z_prev = Z;
			if (M_aggro > C_aggro) Z++;
			else Z--;
		}

		// ���� ���� ���
		print_train(M, Z, C, len);

		// �ù� ���
		if (C_prev) {
			printf("citizen: %d -> %d ", C_prev, C);
			if (C_aggro + 1 <= AGGRO_MAX) printf("(aggro: %d -> %d)\n", C_aggro++, C_aggro);
			else printf("\n");
		}
		else {
			printf("citizen: stay %d", C);
			if (C_aggro - 1 >= AGGRO_MIN) printf("(aggro: %d -> %d)\n", C_aggro--, C_aggro);
			else printf("\n");
		}
		// ���� ���
		if (Z_prev) printf("zombie: %d -> %d\n\n\n", Z_prev, Z);
		else printf("zombie: stay %d (cannot move)\n\n\n", Z);

		// ������ �̵�
		if (Z + 1 == M) M_move = input(0, 0, 4);
		else M_move = input(0, 1, 3);

		if (M_move == 1) {
			M--;
			if (M_aggro + 1 <= AGGRO_MAX) M_aggro++;
		}
		else {
			if (M_aggro - 1 >= AGGRO_MIN) M_aggro--;
		}


		// ���� ���� ���
		print_train(M, Z, C, len);

		// �ൿ ������

			// �ù� �ൿ
		if (C == 1) { // �ù��� ���� ��(1) �� ���� �ߴٸ� �ù� �¸�
			Win = 1;
			break;
		}
		else printf("citizen does nothing.\n");


		// ���� �ൿ
			// �� ���� ��� ���� ���
		if (Z - 1 == C && Z + 1 == M) {
			if (M_aggro > C_aggro) {
				if (--stamina == STM_MIN) {
					Win = 3;
					break;
				}
				printf("Zomibe attacked madongseok (aggro: %d vs. %d, madongseok stamina: %d -> %d)\n",
					M_aggro, C_aggro, stamina--, stamina);
			}
			else {
				Win = 2;
				break;
			}
		}
		// ���� �ù��� �ִ� ��� 
		else if (Z - 1 == C) {
			Win = 2;
			break;
		}
		// ���� �������� �ִ� ���
		else if (Z + 1 == M) {
			if (stamina - 1 == STM_MIN) {
				Win = 3;
				break;
			}
			printf("Zomibe attacked madongseok (madongseok stamina: %d -> %d)\n",
				stamina--, stamina);
		}
		else printf("zombie attacked nobody.\n");

		// ������ �ൿ
		Z_pulled = 0;
		if (Z + 1 == M) M_move = input(0, 2, 6);
		else M_move = input(0, 1, 5);

		if (M_move == 1) {
			printf("\nmadongseok provoked zombie...\n");
			printf("madongseok: %d ", M);
			if (M_aggro == AGGRO_MAX)
				printf("(aggro: %d, stamina: %d)\n", M_aggro, stamina);
			else {
				printf("(aggro: %d -> %d, stamina: %d)\n", M_aggro, AGGRO_MAX, stamina);
				M_aggro = AGGRO_MAX;
			}
		}
		else if (M_move == 2) {
			random = rand() % 100 + 1; // 1~100 ���� ���� ����
			if (random > p) { // Ȯ�� ����
				printf("\nmadonseok pulled zombie... Next turn, it can't move\n");
				Z_pulled = 1;
			}
			else {
				printf("\nmadonseok failed to pull zombie\n");
			}
			printf("madongseok: %d ", M);
			if (M_aggro != AGGRO_MAX && stamina - 1 > STM_MIN) {
				if (M_aggro + 2 > AGGRO_MAX) {
					printf("(aggro: %d -> %d, stamina: %d -> %d)\n", M_aggro, AGGRO_MAX, stamina--, stamina);
					M_aggro = AGGRO_MAX;
				}
				else {
					printf("(aggro: %d -> %d, stamina: %d -> %d)\n", M_aggro, M_aggro + 2, stamina--, stamina);
					M_aggro += 2;
				}
			}
			else if (M_aggro == AGGRO_MAX && stamina - 1 > STM_MIN)
				printf("(aggro: %d, stamina: %d -> %d)\n", M_aggro, stamina--, stamina);
			else if (stamina - 1 == STM_MIN) {
				printf("\n");
				Win = 3;
				break;
			}
		}
		else {
			printf("\nmadongseok rests...\n");
			printf("madongseok: %d ", M);
			if (M_aggro - 1 >= AGGRO_MIN && stamina + 1 <= STM_MAX)
				printf("(aggro: %d -> %d, stamina: %d -> %d)\n", M_aggro--, M_aggro, stamina++, stamina);
			else if (M_aggro - 1 > AGGRO_MIN && stamina == STM_MAX)
				printf("(aggro: %d -> %d, stamina: %d)\n", M_aggro--, M_aggro, stamina);
			else if (M_aggro == AGGRO_MIN && stamina + 1 <= STM_MAX)
				printf("(aggro: %d, stamina: %d -> %d)\n", M_aggro, stamina++, stamina);
			else printf("(aggro: %d, stamina: %d)\n", M_aggro, stamina);
		}

		turn++; // �� ����
	}
	return Win;
}

int Stage2(int len, int stamina, int p) {
	// ���� �ʱ� ���� ���
	int M = len - 2; // ������ ��ġ
	int Z = len - 3; // ���� ��ġ
	int C = len - 6; // �ù� ��ġ
	int V = len - 5; // ���� ��ġ

	int C_aggro = 1; // �ù� ��׷�
	int M_aggro = 1; // ������ ��׷�
	int V_aggro = 1; // ���� ��׷�

	int M_move; // ������ ������

	int Z_pulled = 0;
	int C_prev;
	int Z_prev;
	int V_prev;

	int Win = 0; // �¸� ����, 1 : �ù� �¸�, 2 : ���� �¸� (�ù� ���), 3: ���� �¸� (������ ���)

	// ���� ���
	print_train2(M, Z, C, V, len);

	int random;
	int turn = 1;
	while (1) {
		// �̵� ������
			// �ù� �̵�
		random = rand() % 100 + 1; // 1~100 ���� ���� ����
		if (random > p) { // Ȯ�� ���� �� �������� �� ĭ �̵�
			C_prev = C;
			C--;
			V_prev = V;
			V--;
		}
		else {
			C_prev = 0;
			V_prev = 0;
		}

		// ���� �̵�
			// ����� ���� Ȧ�� �϶��� ������ �� ����
		if (turn % 2 == 1 && !Z_pulled) {
			Z_prev = Z;
			if (M_aggro > C_aggro && M_aggro > V_aggro) Z++;
			else Z--;
		}

		// ���� ���� ���
		print_train2(M, Z, C, V, len);

		// �ù� ���
		if (C_prev) {
			printf("citizen: %d -> %d ", C_prev, C);
			if (C_aggro + 1 <= AGGRO_MAX) printf("(aggro: %d -> %d)\n", C_aggro++, C_aggro);
			else printf("\n");
		}
		else {
			printf("citizen: stay %d", C);
			if (C_aggro - 1 >= AGGRO_MIN) printf("(aggro: %d -> %d)\n", C_aggro--, C_aggro);
			else printf("\n");
		}
		// ���� ���
		if (V_prev) {
			printf("villain: %d -> %d ", V_prev, V);
			if (V_aggro + 1 <= AGGRO_MAX) printf("(aggro: %d -> %d)\n", V_aggro++, V_aggro);
			else printf("\n");
		}
		else {
			printf("villain: stay %d", V);
			if (V_aggro - 1 >= AGGRO_MIN) printf("(aggro: %d -> %d)\n", V_aggro--, V_aggro);
			else printf("\n");
		}

		// ���� ���
		if (Z_prev) printf("zombie: %d -> %d\n\n\n", Z_prev, Z);
		else printf("zombie: stay %d (cannot move)\n\n\n", Z);

		// ������ �̵�
		if (Z + 1 == M) M_move = input(0, 0, 4);
		else M_move = input(0, 1, 3);

		if (M_move == 1) {
			M--;
			if (M_aggro + 1 <= AGGRO_MAX) M_aggro++;
		}
		else {
			if (M_aggro - 1 >= AGGRO_MIN) M_aggro--;
		}


		// ���� ���� ���
		print_train2(M, Z, C, V, len);

	// �ൿ ������

		// �ù� �ൿ
		if (C == 1) { // �ù��� ���� ��(1) �� ���� �ߴٸ� �ù� �¸�
			Win = 1;
			break;
		}
		else printf("citizen does nothing.\n");

		// ���� �ൿ
		if (C + 1 == V || C - 1 == V) {
			random = rand() % 100 + 1; // 1~100 ���� ���� ����
			if (random > 30) { // Ȯ�� ����
				printf("\nvillain's �߾� ����!!!\n");
				int tmp = C;
				C = V;
				V = tmp;
			}
			else printf("\nvillain's �߾� ����...\n");
		}
		else printf("villain does nothing.\n");

		// ������ �ൿ
		Z_pulled = 0;
		if (Z + 1 == M) M_move = input(0, 2, 6);
		else M_move = input(0, 1, 5);

		if (M_move == 1) {
			printf("\nmadongseok provoked zombie...\n");
			printf("madongseok: %d ", M);
			if (M_aggro == AGGRO_MAX)
				printf("(aggro: %d, stamina: %d)\n", M_aggro, stamina);
			else {
				printf("(aggro: %d -> %d, stamina: %d)\n", M_aggro, AGGRO_MAX, stamina);
				M_aggro = AGGRO_MAX;
			}
		}
		else if (M_move == 2) {
			random = rand() % 100 + 1; // 1~100 ���� ���� ����
			if (random > p) { // Ȯ�� ����
				printf("\nmadonseok pulled zombie... Next turn, it can't move\n");
				Z_pulled = 1;
			}
			else {
				printf("\nmadonseok failed to pull zombie\n");
			}
			printf("madongseok: %d ", M);
			if (M_aggro != AGGRO_MAX && stamina - 1 > STM_MIN) {
				if (M_aggro + 2 > AGGRO_MAX) {
					printf("(aggro: %d -> %d, stamina: %d -> %d)\n", M_aggro, AGGRO_MAX, stamina--, stamina);
					M_aggro = AGGRO_MAX;
				}
				else {
					printf("(aggro: %d -> %d, stamina: %d -> %d)\n", M_aggro, M_aggro + 2, stamina--, stamina);
					M_aggro += 2;
				}
			}
			else if (M_aggro == AGGRO_MAX && stamina - 1 > STM_MIN)
				printf("(aggro: %d, stamina: %d -> %d)\n", M_aggro, stamina--, stamina);
			else if (stamina - 1 == STM_MIN) {
				printf("\n");
				Win = 3;
				break;
			}
		}
		else {
			printf("\nmadongseok rests...\n");
			printf("madongseok: %d ", M);
			if (M_aggro - 1 >= AGGRO_MIN && stamina + 1 <= STM_MAX)
				printf("(aggro: %d -> %d, stamina: %d -> %d)\n", M_aggro--, M_aggro, stamina++, stamina);
			else if (M_aggro - 1 > AGGRO_MIN && stamina == STM_MAX)
				printf("(aggro: %d -> %d, stamina: %d)\n", M_aggro--, M_aggro, stamina);
			else if (M_aggro == AGGRO_MIN && stamina + 1 <= STM_MAX)
				printf("(aggro: %d, stamina: %d -> %d)\n", M_aggro, stamina++, stamina);
			else printf("(aggro: %d, stamina: %d)\n", M_aggro, stamina);
		}

		// ���� �ൿ
			// �� ���� ��� ���� ���
		if (Z - 1 == C && Z + 1 == M) {
			if (M_aggro > C_aggro) {
				if (--stamina == STM_MIN) {
					Win = 3;
					break;
				}
				printf("Zomibe attacked madongseok (aggro: %d vs. %d, madongseok stamina: %d -> %d)\n",
					M_aggro, C_aggro, stamina--, stamina);
			}
			else {
				Win = 2;
				break;
			}
		}
		// ���� �ù��� �ִ� ��� 
		else if (Z - 1 == C) {
			Win = 2;
			break;
		}
		// ���� �������� �ִ� ���
		else if (Z + 1 == M) {
			if (stamina - 1 == STM_MIN) {
				Win = 3;
				break;
			}
			printf("Zomibe attacked madongseok (madongseok stamina: %d -> %d)\n",
				stamina--, stamina);
		}
		else printf("zombie attacked nobody.\n");

		turn++; // �� ����
	}
	return Win;
}