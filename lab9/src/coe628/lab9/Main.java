package coe628.lab9;

public class Main {

    private static final int num_Philos = 5;

    public static void main(String[] args) {

        Semaphore[] forks = new Semaphore[num_Philos];
	Philosopher[] philosophers = new Philosopher[num_Philos];	
	for(int i = 0; i < num_Philos; i++)
	{
		forks[i] = new Semaphore(0);
	}	
	for(int i = 0; i < num_Philos; i++)
	{
		philosophers[i] = new Philosopher(i, forks[i], forks[(i+1) % num_Philos]);
		new Thread(philosophers[i]).start();
        }
    }
}
