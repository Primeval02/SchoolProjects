import java.util.Scanner; 

public class AccountMain {

    Account a1 = new Account(50);
    Account a2 = new Account(50);

    public void print() {
        System.out.println("Current Balances:");
        System.out.println("*****************");
        System.out.println(a1.balance);
        System.out.println(a2.balance);


    }

    public static void main(String[] args) {
        System.out.println("")
        Scanner in = new Scanner(System.in);
        System.out.println("Enter deposit amount for account 1: ");
    }
}
