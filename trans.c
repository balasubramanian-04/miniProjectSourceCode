// BANK MANAGEMENT SYSTEM
// Features:
// 1. Store accounts in text file
// 2. Update account
// 3. Add new account
// 4. Delete account
// 5. Search account
// 6. Deposit money
// 7. Withdraw money
// 8. Exit

#include <stdio.h>
#include <stdlib.h>

// structure definition
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// function prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void searchRecord(FILE *fPtr);
void depositMoney(FILE *fPtr);
void withdrawMoney(FILE *fPtr);

// main function
int main(int argc, char *argv[])
{
    FILE *cfPtr;
    unsigned int choice;

    // open file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("File could not be opened.\n");
        exit(-1);
    }

    // menu loop
    while ((choice = enterChoice()) != 8)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            newRecord(cfPtr);
            break;

        case 4:
            deleteRecord(cfPtr);
            break;

        case 5:
            searchRecord(cfPtr);
            break;

        case 6:
            depositMoney(cfPtr);
            break;

        case 7:
            withdrawMoney(cfPtr);
            break;

        default:
            printf("Incorrect choice.\n");
            break;
        }
    }

    fclose(cfPtr);

    return 0;
}

// create formatted text file
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    int result;

    struct clientData client = {0, "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("File could not be opened.\n");
    }
    else
    {
        rewind(readPtr);

        fprintf(writePtr,
                "%-6s%-16s%-11s%10s\n",
                "Acct",
                "Last Name",
                "First Name",
                "Balance");

        while (!feof(readPtr))
        {
            result = fread(&client,
                           sizeof(struct clientData),
                           1,
                           readPtr);

            if (result != 0 && client.acctNum != 0)
            {
                fprintf(writePtr,
                        "%-6d%-16s%-11s%10.2f\n",
                        client.acctNum,
                        client.lastName,
                        client.firstName,
                        client.balance);
            }
        }

        fclose(writePtr);

        printf("accounts.txt created successfully.\n");
    }
}

// update account balance
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number to update (1 - 100): ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\nCurrent Details\n");

        printf("%-6d%-16s%-11s%10.2f\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.balance);

        printf("\nEnter amount (+ deposit / - withdraw): ");
        scanf("%lf", &transaction);

        client.balance += transaction;

        fseek(fPtr,
              -(long)sizeof(struct clientData),
              SEEK_CUR);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account updated successfully.\n");
    }
}

// add new account
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter new account number (1 - 100): ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
    }
    else
    {
        printf("Enter lastname firstname balance\n");
        printf("? ");

        scanf("%14s%9s%lf",
              client.lastName,
              client.firstName,
              &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr,
              (client.acctNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("New account added successfully.\n");
    }
}

// delete account
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete (1 - 100): ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account deleted successfully.\n");
    }
}

// search account
void searchRecord(FILE *fPtr)
{
    unsigned int accountNum;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number to search (1 - 100): ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\n===== ACCOUNT DETAILS =====\n");

        printf("Account Number : %u\n",
               client.acctNum);

        printf("Last Name      : %s\n",
               client.lastName);

        printf("First Name     : %s\n",
               client.firstName);

        printf("Balance        : %.2f\n",
               client.balance);
    }
}

// deposit money
void depositMoney(FILE *fPtr)
{
    unsigned int accountNum;
    double amount;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number: ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("Enter deposit amount: ");
        scanf("%lf", &amount);

        client.balance += amount;

        fseek(fPtr,
              -(long)sizeof(struct clientData),
              SEEK_CUR);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Money deposited successfully.\n");
        printf("Current Balance: %.2f\n",
               client.balance);
    }
}

// withdraw money
void withdrawMoney(FILE *fPtr)
{
    unsigned int accountNum;
    double amount;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number: ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("Enter withdraw amount: ");
        scanf("%lf", &amount);

        if (amount > client.balance)
        {
            printf("Insufficient balance.\n");
        }
        else
        {
            client.balance -= amount;

            fseek(fPtr,
                  -(long)sizeof(struct clientData),
                  SEEK_CUR);

            fwrite(&client,
                   sizeof(struct clientData),
                   1,
                   fPtr);

            printf("Money withdrawn successfully.\n");
            printf("Remaining Balance: %.2f\n",
                   client.balance);
        }
    }
}

// menu function
unsigned int enterChoice(void)
{
    unsigned int menuChoice;

    printf("\n========== BANK MENU ==========\n");

    printf("1 - Store accounts in text file\n");
    printf("2 - Update an account\n");
    printf("3 - Add new account\n");
    printf("4 - Delete an account\n");
    printf("5 - Search account\n");
    printf("6 - Deposit money\n");
    printf("7 - Withdraw money\n");
    printf("8 - Exit\n");

    printf("Enter your choice: ");

    scanf("%u", &menuChoice);

    return menuChoice;
}