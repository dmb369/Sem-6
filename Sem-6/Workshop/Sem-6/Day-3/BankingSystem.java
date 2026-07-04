import java.util.Scanner;

abstract class BankAccount
{
    protected String holder;
    protected double balance;
    protected double interest;

    double min_balance = 5000; // Let Rs.5000 be the minimun balance required to hold an account

    public BankAccount(String holder, double balance, double interest)
    {
        this.holder = holder;
        this.balance = balance;
        this.interest = interest;
    }

    public void deposit(double amount)
    {
        if (amount>=0 && amount%200 == 0)   // Can deposit in only multiples of Rs.200
        {
            balance+=amount;
            System.out.printf("The balance after the amount deposit is: Rs. %.2f\n", balance);
        }

        else
        {
            System.out.println("Amount to be deposited in multiples of Rs.200 only.");
        }
    }

    public void withdraw(double amount)
    {
        if ((balance-amount) >= min_balance)
        {
            balance-=amount;
            System.out.printf("The balance after the amount withdrawal is: Rs. %.2f.\n", balance);
        }

        else
        {
            System.out.println("The amount to withdraw is more than the balance in the account.\n");
        }
    }

    public abstract void calc_Interest();
    
    public double get_balance() {return balance;}
    public String get_holder() {return holder;}
}

class SavingsAccount extends BankAccount
{
    public SavingsAccount(String holder, double balance, double interest) { super(holder, balance, interest); }

    public void calc_Interest()
    {
        balance = balance*(1+(interest/100));
        System.out.printf("The balance amount after interest calculation for 1 year is: %.2f.\n", balance);
    }
}

public class BankingSystem 
{
    public static void main(String[] args)
    {
        String holder;
        double balance;
        double amount;
        double interest;

        Scanner sc = new Scanner(System.in);

        System.out.println("Enter the name of the holder:");
        holder = sc.nextLine();

        System.out.println("Enter the balance of the holder account:");
        balance = sc.nextDouble();

        System.out.println("Enter the amount to be deposited/withdrawn:");
        amount = sc.nextDouble();

        System.out.println("Enter the interest rate:");
        interest = sc.nextDouble();

        sc.close();

        SavingsAccount sa = new SavingsAccount(holder, balance, interest);
        
        System.out.printf("The holder of the account is %s.\n", sa.get_holder());
        System.out.printf("The balance of the holder account is Rs. %.2f\n", sa.get_balance());
        sa.deposit(amount);
        sa.withdraw(amount);
        sa.calc_Interest();
    }
}
