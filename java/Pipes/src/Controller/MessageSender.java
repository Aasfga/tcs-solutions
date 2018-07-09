package Controller;

import Other.IO.PipedInput;
import Other.IO.PipedOutput;

import java.io.*;
import java.security.KeyException;
import java.util.Random;
import java.util.concurrent.BlockingQueue;

public class MessageSender extends Thread
{
    private PipedOutput output = new PipedOutput();
    private int id ;
    private BlockingQueue<Integer> primes;


    private boolean isPrime(int x)
    {
        if(x <= 1)
            return false;
        if(x == 2)
            return true;
        if(x % 2 == 0)
            return false;
        for(int i = 3; i * i < x; i += 2)
        {
            if(x % i == 0)
                return false;
        }
        return true;
    }

    private void sendPrimes() throws IOException
    {
        int x;
        Random generator = new Random();
        do
        {
            x = generator.nextInt(10000);
            if(isPrime(x))
            {
                output.write(x);
                primes.offer(id);
            }
        } while(x != 0);
        output.write(0);
        primes.offer(id);
    }

    MessageSender(MessageReceiver receiver, int id) throws KeyException, IOException
    {
        this.id = id;
        PipedInput input = new PipedInput(output);
        receiver.addPipe(input, id);
        primes = receiver.getPrimesQueue();
    }

    @Override
    public void run()
    {
        try
        {
            sendPrimes();
        } catch(Exception ignored)
        { }
    }
}
