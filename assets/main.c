#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

int input(int min, int max, int prompt_i); // 범위의 값이 입력될 때 까지 계속 입력받는 함수
void print_train(int M_pos, int Z_pos, int C_pos, int train_length); // 열차 정보 출력

int Stage1(int len, int stamina, int p);
int Stage2(int len, int stamina, int p);
//int Stage3(int len, int stamina, int p);
//int Stage4(int len, int stamina, int p);

#define LEN_MIN 15 // 기차 길이
#define LEN_MAX 50
#define STM_MIN 0  // 마동석 체력
#define STM_MAX 5
#define PROB_MIN 10 // 확률
#define PROB_MAX 90 
#define AGGRO_MIN 0 // 어그로 범위
#define AGGRO_MAX 5

// 마동석 이동 방향
#define MOVE_LEFT  1
#define MOVE_STAY  0

// 좀비의공격대상
#define ATK_NONE  0
#define ATK_CITIZEN  1
#define ATK_DONGSEOK  2

// 마동석행동
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
	// 인트로
	// 현재 시간을 토대로 랜덤 시드 생성
	srand((unsigned int)time(NULL));
	int len, stamina, p;
	// 열차 길이 입력
	len = input(LEN_MIN, LEN_MAX, 0);
	// 마동석 스테미나 입력
	stamina = input(STM_MIN, STM_MAX, 1);
	// 확률 입력
	p = input(PROB_MIN, PROB_MAX, 2);

	int result;
	
	result = Stage1(len, stamina, p);
	if (result == 1) {
		printf("\n\n-----------------\nStage1 Clear!!!\n-----------------\n\nNext Stage...\n\n");
		result = Stage2(len, stamina, p);
	}

	// 시민 승리 시
	if (result == 1) printf("SUCCESS!\ncitizen(s) escaped to the next train");
	// 좀비 승리 시 (시민 사망)
	else if (result == 2) printf("GAME OVER! citizen dead...");
	// 좀비 승리 시 (마동석 사망)
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
	// 열차 출력
	for (int i = 1; i <= 3; i++) {
		// 홀 수 행일 때는 #으로 만 표현 
		if (i % 2 == 1) {
			for (int j = 0; j < train_length; j++) {
				printf("#");
			}
			printf("\n");
		}
		// 짝 수 행일 때 본격적인 정보 출력
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
	// 열차 출력
	for (int i = 1; i <= 3; i++) {
		// 홀 수 행일 때는 #으로 만 표현 
		if (i % 2 == 1) {
			for (int j = 0; j < train_length; j++) {
				printf("#");
			}
			printf("\n");
		}
		// 짝 수 행일 때 본격적인 정보 출력
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
	// 열차 초기 상태 출력
	int M = len - 2; // 마동석 위치
	int Z = len - 3; // 좀비 위치
	int C = len - 6; // 시민 위치

	int C_aggro = 1; // 시민 어그로
	int M_aggro = 1; // 마동석 어그로

	int M_move; // 마동석 움직임

	int Z_pulled = 0;
	int C_prev;
	int Z_prev;

	int Win = 0; // 승리 여부, 1 : 시민 승리, 2 : 좀비 승리 (시민 사망), 3: 좀비 승리 (마동석 사망)

	// 열차 출력
	print_train(M, Z, C, len);

	int random;
	int turn = 1;
	while (1) {
		// 이동 페이즈
			// 시민 이동
		random = rand() % 100 + 1; // 1~100 숫자 랜덤 생성
		if (random > p) { // 확률 성공 시 왼쪽으로 한 칸 이동
			C_prev = C;
			C--;
		}
		else C_prev = 0;

		// 좀비 이동
			// 좀비는 턴이 홀수 일때만 움직일 수 있음
		if (turn % 2 == 1 && !Z_pulled) {
			Z_prev = Z;
			if (M_aggro > C_aggro) Z++;
			else Z--;
		}

		// 열차 상태 출력
		print_train(M, Z, C, len);

		// 시민 출력
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
		// 좀비 출력
		if (Z_prev) printf("zombie: %d -> %d\n\n\n", Z_prev, Z);
		else printf("zombie: stay %d (cannot move)\n\n\n", Z);

		// 마동석 이동
		if (Z + 1 == M) M_move = input(0, 0, 4);
		else M_move = input(0, 1, 3);

		if (M_move == 1) {
			M--;
			if (M_aggro + 1 <= AGGRO_MAX) M_aggro++;
		}
		else {
			if (M_aggro - 1 >= AGGRO_MIN) M_aggro--;
		}


		// 열차 상태 출력
		print_train(M, Z, C, len);

		// 행동 페이즈

			// 시민 행동
		if (C == 1) { // 시민이 왼쪽 끝(1) 에 도달 했다면 시민 승리
			Win = 1;
			break;
		}
		else printf("citizen does nothing.\n");


		// 좀비 행동
			// 양 옆에 모두 있을 경우
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
		// 옆에 시민이 있는 경우 
		else if (Z - 1 == C) {
			Win = 2;
			break;
		}
		// 옆에 마동석이 있는 경우
		else if (Z + 1 == M) {
			if (stamina - 1 == STM_MIN) {
				Win = 3;
				break;
			}
			printf("Zomibe attacked madongseok (madongseok stamina: %d -> %d)\n",
				stamina--, stamina);
		}
		else printf("zombie attacked nobody.\n");

		// 마동석 행동
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
			random = rand() % 100 + 1; // 1~100 숫자 랜덤 생성
			if (random > p) { // 확률 성공
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

		turn++; // 턴 증가
	}
	return Win;
}

int Stage2(int len, int stamina, int p) {
	// 열차 초기 상태 출력
	int M = len - 2; // 마동석 위치
	int Z = len - 3; // 좀비 위치
	int C = len - 6; // 시민 위치
	int V = len - 5; // 빌런 위치

	int C_aggro = 1; // 시민 어그로
	int M_aggro = 1; // 마동석 어그로
	int V_aggro = 1; // 빌런 어그로

	int M_move; // 마동석 움직임

	int Z_pulled = 0;
	int C_prev;
	int Z_prev;
	int V_prev;

	int Win = 0; // 승리 여부, 1 : 시민 승리, 2 : 좀비 승리 (시민 사망), 3: 좀비 승리 (마동석 사망)

	// 열차 출력
	print_train2(M, Z, C, V, len);

	int random;
	int turn = 1;
	while (1) {
		// 이동 페이즈
			// 시민 이동
		random = rand() % 100 + 1; // 1~100 숫자 랜덤 생성
		if (random > p) { // 확률 성공 시 왼쪽으로 한 칸 이동
			C_prev = C;
			C--;
			V_prev = V;
			V--;
		}
		else {
			C_prev = 0;
			V_prev = 0;
		}

		// 좀비 이동
			// 좀비는 턴이 홀수 일때만 움직일 수 있음
		if (turn % 2 == 1 && !Z_pulled) {
			Z_prev = Z;
			if (M_aggro > C_aggro && M_aggro > V_aggro) Z++;
			else Z--;
		}

		// 열차 상태 출력
		print_train2(M, Z, C, V, len);

		// 시민 출력
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
		// 빌런 출력
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

		// 좀비 출력
		if (Z_prev) printf("zombie: %d -> %d\n\n\n", Z_prev, Z);
		else printf("zombie: stay %d (cannot move)\n\n\n", Z);

		// 마동석 이동
		if (Z + 1 == M) M_move = input(0, 0, 4);
		else M_move = input(0, 1, 3);

		if (M_move == 1) {
			M--;
			if (M_aggro + 1 <= AGGRO_MAX) M_aggro++;
		}
		else {
			if (M_aggro - 1 >= AGGRO_MIN) M_aggro--;
		}


		// 열차 상태 출력
		print_train2(M, Z, C, V, len);

	// 행동 페이즈

		// 시민 행동
		if (C == 1) { // 시민이 왼쪽 끝(1) 에 도달 했다면 시민 승리
			Win = 1;
			break;
		}
		else printf("citizen does nothing.\n");

		// 빌런 행동
		if (C + 1 == V || C - 1 == V) {
			random = rand() % 100 + 1; // 1~100 숫자 랜덤 생성
			if (random > 30) { // 확률 성공
				printf("\nvillain's 발악 성공!!!\n");
				int tmp = C;
				C = V;
				V = tmp;
			}
			else printf("\nvillain's 발악 실패...\n");
		}
		else printf("villain does nothing.\n");

		// 마동석 행동
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
			random = rand() % 100 + 1; // 1~100 숫자 랜덤 생성
			if (random > p) { // 확률 성공
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

		// 좀비 행동
			// 양 옆에 모두 있을 경우
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
		// 옆에 시민이 있는 경우 
		else if (Z - 1 == C) {
			Win = 2;
			break;
		}
		// 옆에 마동석이 있는 경우
		else if (Z + 1 == M) {
			if (stamina - 1 == STM_MIN) {
				Win = 3;
				break;
			}
			printf("Zomibe attacked madongseok (madongseok stamina: %d -> %d)\n",
				stamina--, stamina);
		}
		else printf("zombie attacked nobody.\n");

		turn++; // 턴 증가
	}
	return Win;
}