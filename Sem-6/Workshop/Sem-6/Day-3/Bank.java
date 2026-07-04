import java.util.*;

public class Bank
{
    public static void main(String[] args)
    {
        Scanner sc = new Scanner(System.in);
        int n, p;
        n = sc.nextInt();
        p = sc.nextInt();

        double result = 0;

        PriorityQueue<Double> q = new PriorityQueue<Double>();
        for (int i=0; i<n; i++) q.add(sc.nextDouble());
        sc.close();

        while(q.size()>1)
        {
            double a = q.poll();
            double b = q.poll();
            result = a+b;
            result -= (p/100.0)*result;
            q.add(result);
        }

        System.out.printf("%.2f\n", result);

    }
}