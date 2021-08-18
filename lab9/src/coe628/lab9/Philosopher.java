package coe628.lab9;
import java.util.Random;


public class Philosopher implements Runnable {
    	
	private Random ran = new Random();
	
	private int sema_ID;
	
	private Semaphore leftFork;
	private Semaphore rightFork;
	
	public Philosopher(int id, Semaphore left, Semaphore right)
	{
		sema_ID = id;
		leftFork = left;
		rightFork = right;
	}
	
    @Override
	public void run() 
	{
		try 
		{
			while(true)
			{
				think();
				takeLeftFork();
				takeRightFork();
				eat();
				releaseRightFork();
				releaseLeftFork();
			}
		} 
		catch (InterruptedException ex) 
		{
			System.out.println("Philosopher " + sema_ID + " was interrupted.\n");
		}
	}
	
	private void think() throws InterruptedException
	{
		System.out.println("Philosopher " + sema_ID + " is thinking.\n");
		System.out.flush();
		Thread.sleep(ran.nextInt(10));
	}
	
	private void takeLeftFork()
	{
		leftFork.up();
		System.out.println("Philosopher " + sema_ID + " took the left fork but needs 1 more fork.\n");
		System.out.flush();
	}
	
	private void takeRightFork()
	{
		rightFork.up();
		System.out.println("Philosopher " + sema_ID + " took the right fork has both forks.\n");
	}
	
	private void eat() throws InterruptedException
	{
		System.out.println("Philosopher " + sema_ID + " is eating.\n");
		System.out.flush();
		Thread.sleep(ran.nextInt(10));
	}
	
	private void releaseLeftFork() throws InterruptedException
	{
		leftFork.down();
	}
	
	private void releaseRightFork() throws InterruptedException
	{
		rightFork.down();
	}
}