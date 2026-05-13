// ======================================================
// BANK MANAGEMENT SYSTEM
// ======================================================
// FEATURES
// 1. Store accounts in text file
// 2. Update account
// 3. Add new account
// 4. Delete account
// 5. Search account
// 6. Deposit money
// 7. Withdraw money
// 8. Display all accounts
// 9. Transfer money
// 10. Check total bank balance
// 11. Show richest account
// 12. Count total accounts   <-- NEW FEATURE
// 13. Exit
// ======================================================

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

void displayAllAccounts(FILE *fPtr);
void transferMoney(FILE *fPtr);

void totalBankBalance(FILE *fPtr);
void richestAccount(FILE *fPtr);

void countAccounts(FILE *fPtr); // NEW FEATURE

// ======================================================
// MAIN FUNCTION
// ======================================================
int main()
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
    while ((choice = enterChoice()) != 13)
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

        case 8:
            displayAllAccounts(cfPtr);
            break;

        case 9:
            transferMoney(cfPtr);
            break;

        case 10:
            totalBankBalance(cfPtr);
            break;

        case 11:
            richestAccount(cfPtr);
            break;

        case 12:
            countAccounts(cfPtr); // NEW FEATURE
            break;

        default:
            printf("Invalid choice.\n");
            break;
        }
    }

    fclose(cfPtr);

    return 0;
}

// ======================================================
// STORE TEXT FILE
// ======================================================
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
                        "%-6u%-16s%-11s%10.2f\n",
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

// ======================================================
// UPDATE ACCOUNT
// ======================================================
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number to update: ");
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
        printf("Current Balance: %.2f\n",
               client.balance);

        printf("Enter amount (+ deposit / - withdraw): ");
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

// ======================================================
// ADD NEW ACCOUNT
// ======================================================
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter new account number: ");
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

// ======================================================
// DELETE ACCOUNT
// ======================================================
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete: ");
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

// ======================================================
// SEARCH ACCOUNT
// ======================================================
void searchRecord(FILE *fPtr)
{
    unsigned int accountNum;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number to search: ");
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

// ======================================================
// DEPOSIT MONEY
// ======================================================
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
    }
}

// ======================================================
// WITHDRAW MONEY
// ======================================================
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
        }
    }
}

// ======================================================
// DISPLAY ALL ACCOUNTS
// ======================================================
void displayAllAccounts(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    rewind(fPtr);

    printf("\n=================================================\n");

    printf("%-10s%-15s%-15s%-10s\n",
           "Account",
           "Last Name",
           "First Name",
           "Balance");

    printf("=================================================\n");

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-10u%-15s%-15s%-10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

// ======================================================
// TRANSFER MONEY
// ======================================================
void transferMoney(FILE *fPtr)
{
    unsigned int senderAcc;
    unsigned int receiverAcc;

    double amount;

    struct clientData sender = {0, "", "", 0.0};
    struct clientData receiver = {0, "", "", 0.0};

    printf("Enter sender account number: ");
    scanf("%u", &senderAcc);

    printf("Enter receiver account number: ");
    scanf("%u", &receiverAcc);

    printf("Enter transfer amount: ");
    scanf("%lf", &amount);

    fseek(fPtr,
          (senderAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&sender,
          sizeof(struct clientData),
          1,
          fPtr);

    fseek(fPtr,
          (receiverAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&receiver,
          sizeof(struct clientData),
          1,
          fPtr);

    if (sender.acctNum == 0)
    {
        printf("Sender account not found.\n");
    }
    else if (receiver.acctNum == 0)
    {
        printf("Receiver account not found.\n");
    }
    else if (amount > sender.balance)
    {
        printf("Insufficient balance.\n");
    }
    else
    {
        sender.balance -= amount;

        fseek(fPtr,
              (senderAcc - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&sender,
               sizeof(struct clientData),
               1,
               fPtr);

        receiver.balance += amount;

        fseek(fPtr,
              (receiverAcc - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&receiver,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Money transferred successfully.\n");
    }
}

// ======================================================
// TOTAL BANK BALANCE
// ======================================================
void totalBankBalance(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    double total = 0;

    rewind(fPtr);

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 fPtr))
    {
        if (client.acctNum != 0)
        {
            total += client.balance;
        }
    }

    printf("\nTOTAL BANK BALANCE : %.2f\n", total);
}

// ======================================================
// SHOW RICHEST ACCOUNT
// ======================================================
void richestAccount(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    struct clientData richest = {0, "", "", 0.0};

    rewind(fPtr);

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 fPtr))
    {
        if (client.acctNum != 0)
        {
            if (client.balance > richest.balance)
            {
                richest = client;
            }
        }
    }

    if (richest.acctNum == 0)
    {
        printf("No accounts available.\n");
    }
    else
    {
        printf("\n===== RICHEST ACCOUNT =====\n");

        printf("Account Number : %u\n",
               richest.acctNum);

        printf("Last Name      : %s\n",
               richest.lastName);

        printf("First Name     : %s\n",
               richest.firstName);

        printf("Balance        : %.2f\n",
               richest.balance);
    }
}

// ======================================================
// NEW FEATURE
// COUNT TOTAL ACCOUNTS
// ======================================================
void countAccounts(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    int count = 0;

    rewind(fPtr);

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 fPtr))
    {
        if (client.acctNum != 0)
        {
            count++;
        }
    }

    printf("\n=================================\n");
    printf("TOTAL ACTIVE ACCOUNTS : %d\n", count);
    printf("=================================\n");
}

// ======================================================
// MENU FUNCTION
// ======================================================
unsigned int enterChoice(void)
{
    unsigned int menuChoice;

    printf("\n========== BANK MENU ==========\n");

    printf("1  - Store accounts in text file\n");
    printf("2  - Update an account\n");
    printf("3  - Add new account\n");
    printf("4  - Delete an account\n");
    printf("5  - Search account\n");
    printf("6  - Deposit money\n");
    printf("7  - Withdraw money\n");
    printf("8  - Display all accounts\n");
    printf("9  - Transfer money\n");
    printf("10 - Check total bank balance\n");
    printf("11 - Show richest account\n");
    printf("12 - Count total accounts\n");
    printf("13 - Exit\n");

    printf("Enter your choice: ");

    scanf("%u", &menuChoice);

    return menuChoice;
}