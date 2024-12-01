#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SENTENCES 5   // 연습할 문장의 개수
#define MAX_LENGTH 256    // 각 문장의 최대 길이

// 예제 문장들
const char *sentences[MAX_SENTENCES] = {
    "죽는 날까지 하늘을 우러러 한 점 부끄럼이 없기를",
    "잎새에 이는 바람에도 나는 괴로워 했다.",
    "별을 노래하는 마음으로 모든 죽어가는 것을 사랑해야지",
    "그리고 나한테 주어진 길을 걸어가야겠다.",
    "오늘 밤에도 별이 바람에 스치운다."
};

void typing_practice();
int calculate_errors(const char *correct, const char *input);

int main() {
    printf("타자 연습에 오신 것을 환영합니다!\n");
    printf("----------------------------------------------\n\n");

    typing_practice();

    printf("\n-----------------------------------------------\n");
    printf("타자 연습을 이용해주셔서 감사합니다. 안녕히 가세요!\n");
    return 0;
}

void typing_practice() {
    char user_input[MAX_LENGTH];
    int total_errors = 0;
    double total_time = 0;

    for (int i = 0; i < MAX_SENTENCES; i++) {
        printf("문장 : %s\n", sentences[i]);

        // 입력 시작 시간 기록 (time()을 사용하여 초 단위 시간 측정)
        time_t start_time = time(NULL);

        // 사용자 입력 받기
        fgets(user_input, sizeof(user_input), stdin);
        user_input[strcspn(user_input, "\n")] = '\0'; // 개행 문자 제거

        // 입력 종료 시간 기록
        time_t end_time = time(NULL);

        // 타이핑 시간 계산 (초 단위)
        double elapsed_time = difftime(end_time, start_time);
        total_time += (elapsed_time > 0.01 ? elapsed_time : 0.01); // 최소 0.01초 보장

        // 오류 횟수 계산
        int errors = calculate_errors(sentences[i], user_input);
        total_errors += errors;

        printf("오류 수: %d, 소요 시간: %.2f초\n\n", errors, elapsed_time);
    }

    // WPM 계산
    double total_words = 0;
    for (int i = 0; i < MAX_SENTENCES; i++) {
        total_words += strlen(sentences[i]) / 5.0; // 평균 1단어는 5글자
    }

    double wpm = (total_words / total_time) * 60;

    printf("타자 연습이 완료되었습니다!\n");
    printf("총 오류 수: %d\n", total_errors);
    printf("평균 분당 타자수(WPM): %.2f\n", wpm);
}

// 오류 수 계산 함수
int calculate_errors(const char *correct, const char *input) {
    size_t len_correct = strlen(correct);
    size_t len_input = strlen(input);
    int errors = 0;

    // 최소 길이로 비교
    size_t max_len = len_correct > len_input ? len_correct : len_input;

    for (size_t i = 0; i < max_len; i++) {
        if (i >= len_input || i >= len_correct || correct[i] != input[i]) {
            errors++;
        }
    }

    return errors;
}
