#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void  ClientProcess(volatile int []);

int  main(int  argc, char *argv[])
{
     int    ShmID;
     volatile int    *ShmPTR;
     pid_t  pid;
     int    status;


     ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     printf("Server has received a shared memory of two integers...\n");

     ShmPTR = (volatile int *) shmat(ShmID, NULL, 0);
     if (ShmPTR ==(void *) -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     printf("Server has attached the shared memory...\n");

     ShmPTR[0] = 0;  // BankAccount
     ShmPTR[1] = 0;  // Turn

     printf("BankAccount = %d, Turn = %d\n",
       ShmPTR[0], ShmPTR[1]);


     printf("Server is about to fork a child process...\n");
     pid = fork();
     if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     }
     else if (pid == 0) {
          ClientProcess(ShmPTR);
          shmdt((const void *) ShmPTR);
          exit(0);
     }

     else {
        int account;
        int balance;

        srand(time(NULL) ^ getpid());

        for (int i = 0; i < 25; i++) {

            // Sleep between 0 and 5 seconds
            sleep(rand() % 6);

            // Wait for Dad's turn
            while (ShmPTR[1] != 0) {
                // Do nothing
            }

            // Copy shared balance to local variable
            account = ShmPTR[0];

            if (account <= 100) {

                // Generate amount between 0 and 100
                balance = rand() % 101;

                if (balance % 2 == 0) {
                    account += balance;

                    printf("Dear old Dad: Deposits $%d / Balance = $%d\n",
                           balance, account);
                }
                else {
                    printf("Dear old Dad: Doesn't have any money to give\n");
                }
            }
            else {
                printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n",
                       account);
            }

            // Update shared BankAccount
            ShmPTR[0] = account;

            // Give turn to Student
            ShmPTR[1] = 1;
        }
    }

     wait(&status);
     printf("Server has detected the completion of its child...\n");
     shmdt((void *) ShmPTR);
     printf("Server has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("Server has removed its shared memory...\n");
     printf("Server exits...\n");
     exit(0);
}

void  ClientProcess(volatile int  SharedMem[])
{
     int account;
     int balance;

     srand(time(NULL) ^ getpid());
     printf("   Client process started\n");

     for (int i = 0; i < 25; i++) {

        // Sleep between 0 and 5 seconds
        sleep(rand() % 6);

        // Wait for Student's turn
        while (SharedMem[1] != 1) {
            // Do nothing
        }

        // Copy shared balance to local variable
        account = SharedMem[0];

        // Generate amount needed between 0 and 50
        balance = rand() % 51;

        printf("Poor Student needs $%d\n", balance);

        if (balance <= account) {
            account -= balance;

            printf("Poor Student: Withdraws $%d / Balance = $%d\n",
                   balance, account);
        }
        else {
            printf("Poor Student: Not Enough Cash ($%d)\n",
                   account);
        }

        // Update shared BankAccount
        SharedMem[0] = account;

        // Give turn back to Dad
        SharedMem[1] = 0;
    }
           
     printf("   Client is about to exit\n");
}