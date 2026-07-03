// Project 3
#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"
#include "../kernel/fcntl.h"
#include "../kernel/memlayout.h"
#include "../kernel/param.h"
#include "../kernel/spinlock.h"
#include "../kernel/sleeplock.h"
#include "../kernel/fs.h"
#include "../kernel/syscall.h"

// param.h에 있는 상수들을 유저 공간에서 쓰기 위해 재정의
#define PROT_READ 0x1
#define PROT_WRITE 0x2
#define MAP_ANONYMOUS 0x1
#define MAP_POPULATE 0x2

int main(int argc, char *argv[]) {
  int initial_free, after_pop, after_lazy, after_munmap;
  char *test1, *test2, *test3;
  int fd;
  char ch;

  printf("\n=== Project 3 Virtual Memory Test Start ===\n");
  
  initial_free = freemem();
  printf("[1] Initial free pages: %d\n", initial_free);

  // Test 1: Anonymous + MAP_POPULATE (즉시 할당 테스트)
  test1 = (char*)mmap(0, 8192, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_POPULATE, -1, 0);
  after_pop = freemem();
  printf("[2] After Anon+Populate (2 pages): %d (Should be %d)\n", after_pop, initial_free - 2);

  // Test 2: Anonymous + No Populate (Lazy Allocation 테스트)
  test2 = (char*)mmap(8192, 4096, PROT_READ|PROT_WRITE, MAP_ANONYMOUS, -1, 0);
  after_lazy = freemem();
  printf("[3] After Anon+Lazy (1 page, NO fault yet): %d (Should be same as above)\n", after_lazy);

  // 강제로 Page Fault 발생시키기
  test2[0] = 'A'; 
  printf("[4] After triggering Page Fault (Write): %d (Should decrease by 1)\n", freemem());

  // Test 3: README File Mapping + No Populate 
  fd = open("README", O_RDWR);
  test3 = (char*)mmap(8192 * 2, 4096, PROT_READ|PROT_WRITE, 0, fd, 0);
  
  printf("\n[5] File Mapped Contents Check (triggers Page Fault):\n");
  printf("fd data: ");
  ch = test3[0]; write(1, &ch, 1);
  ch = test3[1]; write(1, &ch, 1);
  ch = test3[2]; write(1, &ch, 1);
  printf("\n\n");

  // Test 4: Fork Test (자식 프로세스 상속 확인) [cite: 576-580]
  int pid = fork();
  if (pid == 0) {
    // Child Process
    printf("[Child] Inherited free pages: %d\n", freemem());
    printf("[Child] Reading inherited fd data: ");
    ch = test3[0]; write(1, &ch, 1);
    ch = test3[1]; write(1, &ch, 1);
    ch = test3[2]; write(1, &ch, 1);
    printf("\n");
    exit(0);
  } else {
    // Parent Process
    wait(0); 
  }

  // Test 5: munmap Test (메모리 해제 및 반환 확인)
  munmap((uint64)test1);
  munmap((uint64)test2);
  munmap((uint64)test3);
  
  after_munmap = freemem();
  printf("\n[6] After all munmaps: %d (Should recover memory)\n", after_munmap);
  
  printf("=== Test Finished ===\n\n");
  exit(0);
}


//#include "kernel/types.h"
//#include "kernel/stat.h"
//#include "user/user.h"
//// Project 2
//int main(int argc, char *argv[]) {
//  // 스크린샷과 똑같이 깔끔한 헤더만 출력
//  printf("=== TEST START ===\n");
//
//  // 1. 부모 프로세스 (PID 4 예정)의 우선순위를 0으로 설정 (최고 가중치: 88761)
//  setnice(getpid(), 0);
//
//  int pid = fork();
//  
//  if (pid == 0) {
//    // 2. 자식 프로세스 (PID 5 예정)의 우선순위를 10으로 설정 (가중치: 9548)
//    setnice(getpid(), 10);
//    
//    // 자식은 무한 루프를 돌며 CPU를 맹렬하게 소모합니다.
//    while(1) {
//      volatile int i;
//      for(i = 0; i < 1000; i++); // 컴파일러 최적화 방지
//    }
//  }
//
//  // 3. 부모 프로세스 역시 CPU를 집중적으로 소모하여 서로 경쟁하게 만듭니다.
//  volatile int j, k;
//  // 루프 횟수를 아주 크게 주어 runtime이 수백만이 되도록 합니다.
//  for(j = 0; j < 4000; j++) {
//    for(k = 0; k < 1000000; k++) {
//      // 아무것도 하지 않음 (CPU 태우기)
//    }
//  }
//
//  // 4. 부모의 긴 루프가 끝나면 스냅샷을 찍습니다.
//  // 부모가 직접 호출하므로 부모는 'run', 루프를 돌고 있는 자식은 'runble'로 찍힙니다.
//  ps(0);
//
//  // 5. 테스트가 끝나면 무한 루프를 도는 자식을 조용히 암살(?)하고 종료합니다.
//  // 화면에 출력되지 않게 처리하여 스크린샷처럼 프롬프트($)만 딱 떨어지게 만듭니다.
//  kill(pid);
//  wait(0);
//
//  exit(0);
//}


// Project 1
// 시간 지연을 위한 함수
//void delay(int count) {
//    int i, dummy = 0;
//    for(i = 0; i < count * 2000000; i++) {
//        dummy += i;
//    }
//}
//
//int main(int argc, char *argv[]) {
//    printf("Student ID: 2021312773\n"); 
//    printf("Name: Jaegeun Lee\n\n");
//
//    // [테스트 1] meminfo: 현재 남은 메모리 확인
//    printf("[1] Testing meminfo...\n");
//    printf("Available memory: %d bytes\n\n", (int)meminfo());
//
//    // [테스트 2] getnice & setnice: 부모 프로세스의 우선순위 변경
//    printf("[2] Testing getnice & setnice...\n");
//    int mypid = getpid();
//    printf("PID %d initial nice: %d\n", mypid, getnice(mypid));
//    
//    if (setnice(mypid, 10) == 0) {
//        printf("PID %d new nice: %d\n\n", mypid, getnice(mypid));
//    } else {
//        printf("Failed to set nice value!\n\n");
//    }
//
//    // [테스트 3] waitpid & ps: 자식 프로세스 생성 후 상태 및 대기 확인
//    printf("[3] Testing waitpid & ps...\n");
//    
//    int child1 = fork();
//    if (child1 == 0) {
//        setnice(getpid(), 5); // 1번 자식은 nice를 5로 설정
//        delay(10);            // 부모가 표를 출력할 때까지 대기
//        exit(0);
//    }
//
//    int child2 = fork();
//    if (child2 == 0) {
//        setnice(getpid(), 15); // 2번 자식은 nice를 15로 설정
//        delay(20);             // 1번 자식보다 조금 더 늦게 종료되도록 대기
//        exit(0);
//    }
//
//    // 자식들이 nice 값을 설정할 수 있도록 부모가 아주 잠시 양보
//    delay(2);
//
//    // 자식들이 살아있는 동안 전체 프로세스 상태 출력
//    printf("\n--- Calling ps(0) ---\n");
//    ps(0);
//    printf("---------------------\n\n");
//
//    // 특정 자식 프로세스가 끝날 때까지 순서대로 대기
//    printf("Waiting for child1 (PID %d)...\n", child1);
//    if (waitpid(child1) == 0) {
//        printf("child1 (PID %d) terminated successfully.\n", child1);
//    }
//
//    printf("Waiting for child2 (PID %d)...\n", child2);
//    if (waitpid(child2) == 0) {
//        printf("child2 (PID %d) terminated successfully.\n", child2);
//    }
//
//    printf("\n=== All Tests Finished Successfully! ===\n");
//    exit(0);
//}

