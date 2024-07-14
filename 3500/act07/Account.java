//filename: Account.java
// Accout class
public class Account {
  private double balance;
  public Account(double initialBalance) {
    if (initialBalance > 0.0) balance = initialBalance;
  }

 public void credit(double amount){
     this.balance = balance += amount;
 }

 public void debit(double amount){
     if (amount > balance) {
         System.out.println("Debit amount exceeded balance.")
     }
     else {
         this.balance = balance -= amount;
     }

 }

 public double getBalance(){
     return balance;
 }

}
