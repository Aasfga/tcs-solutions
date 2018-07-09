package Controller;

import Other.IO.PipedInput;
import Other.Messages.PrimeMessage;
import Other.Messages.ReceiverEndMessage;
import Other.Messages.SenderEndMessage;

import java.io.IOException;
import java.security.KeyException;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingDeque;

public class MessageReceiver extends Thread
{
    private BlockingQueue<Integer> primes = new LinkedBlockingDeque<Integer>();
    private Controller controller;
    private Map<Integer, PipedInput> pipes = new TreeMap<Integer, PipedInput>();

    PipedInput getInput(int id)
    {
        return pipes.get(id);
    }

    BlockingQueue<Integer> getPrimesQueue()
    {
        return primes;
    }

    MessageReceiver(Controller controller)
    {
        this.controller = controller;
    }

    void addPipe(PipedInput reader, int id) throws KeyException
    {
        if(pipes.containsKey(id))
            throw new KeyException();
        pipes.put(id, reader);
    }

    private int nextSender() throws InterruptedException
    {
        int x;
        int id;
        do
        {
            id = primes.take();
            PipedInput input = pipes.get(id);
            x = input.nextInt();
            if(x == 0)
            {
                pipes.get(id).close();
                pipes.remove(id);
                controller.sendMessage(new SenderEndMessage(id));
            }
        } while(x == 0 && pipes.size() != 0);

        return x == 0 ? -1 : id;
    }

    private void readPrimes() throws IOException, InterruptedException
    {
        do
        {
            int id = nextSender();
            if(id != -1)
                controller.sendMessage(new PrimeMessage(id));
        } while(pipes.size() != 0);
        controller.sendMessage(new ReceiverEndMessage());

    }

    @Override
    public void run()
    {
        try
        {
            readPrimes();
        }
        catch(Exception ignored)
        { }
    }

}
