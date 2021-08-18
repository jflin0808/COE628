/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package coe628.lab8;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Ken Clowes
 */
public class ProducerConsumer {

    static final int BUFFER_SIZE = 1;
    private static ProducerConsumer.Producer Producer;
    static Semaphore full = new Semaphore(0);
    static Semaphore empty = new Semaphore(BUFFER_SIZE);

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        ProducerConsumer.Queue q = new ProducerConsumer.Queue(BUFFER_SIZE);
        ProducerConsumer.Producer p = new ProducerConsumer.Producer(q);
        ProducerConsumer.Consumer c = new ProducerConsumer.Consumer(q);

        p.start();
        c.start();

    }

    public static class Queue {

        private final int[] numbers;
        private int rear;
        private int front;
        private final int size;

        public Queue(int size) {
            numbers = new int[size + 1];
            front = 0;
            rear = 0;
            this.size = size;
        }

        public int get() {
            int value = numbers[front];
            front = (front + 1) % size;
            return value;
        }

        public void put(int value) {
            numbers[rear] = value;
            rear = (rear + 1) % size;
        }
    }

    private static class Producer extends Thread {

        private final ProducerConsumer.Queue q;

        public Producer(ProducerConsumer.Queue q) {
            this.q = q;
        }

        @Override
        public void run() {
            for (int i = 0; i < BUFFER_SIZE * 10; i++) {
                try {
                    System.out.println("Producing: " + i);
                    empty.down();
                    q.put(i);
                    try {
                        Thread.sleep((int) (Math.random() * 10));
                    } catch (InterruptedException ex) {
                        Logger.getLogger(ProducerConsumer.class.getName()).log(Level.SEVERE, null, ex);
                    }
                    empty.up();
                } catch (InterruptedException ex) {
                    Logger.getLogger(ProducerConsumer.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
    }

    private static class Consumer extends Thread {

        private final ProducerConsumer.Queue q;

        public Consumer(ProducerConsumer.Queue q) {
            this.q = q;
        }

        @Override
        public void run() {
            for (int i = 0; i < BUFFER_SIZE * 10; i++) {
                System.out.println("\t\tConsuming: " + q.get());
                q.get();
                try {
                    Thread.sleep((int) (Math.random() * 10));
                } catch (InterruptedException ex) {
                    Logger.getLogger(ProducerConsumer.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
    }

}
