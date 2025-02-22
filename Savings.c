/*
 * Name: Noah Patterson
 * Username: nspatter
 * Description: This is a program that computes the interest on an initial
 * amount of money invested in a saving account. The user calls this program
 * with four required commands line arguments: account name, initial amount,
 * annual percentage interest rate, and an integer number of months
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv) {
  if (argc < 5) {
    printf("Savings <account> <amount> <rate> <months>");
    return 0;
  }

  double amount = atof(argv[2]);
  double rate = atof(argv[3])/100;
  int months = atoi(argv[4]);
  double total = amount;
  char* account = argv[1];
  
  if (amount <= 0){
    printf("ERROR: amount must be positive!\n");
    return 0;
  } else {
    printf("Deposited $%.2f into %s\n", amount, account);

    //increment through months calculating the change for each month
    for (int i = 1; i <= months; i++){
      double change = (total*(rate/12));
      total = total + change;
      printf("Month %d, $%.2f change, new total $%.2f \n", i, change, total);
    }
    // calculate the relative change
    double relative = ((total - amount)/amount) * 100;
    printf("Relative Change: %.2f%%\n", relative);
  }
  return 0;
}
