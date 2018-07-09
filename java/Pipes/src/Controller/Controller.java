package Controller;


import Other.Messages.*;
import Other.Status;
import View.View;
import View.ConsoleView;

import java.io.IOException;
import java.security.KeyException;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingDeque;

public class Controller extends Thread
{
    private BlockingQueue<Message> messages = new LinkedBlockingDeque<Message>();
    private Map<Integer, Status> state = new TreeMap<Integer, Status>();
    private View view = new ConsoleView(this);



    private void handle(ViewReadyMessage message) throws KeyException, IOException
    {
        init();
    }

    private void handle(SenderEndMessage message)
    {
        state.get(message.getId()).notWorking();
        view.setState(state);
    }

    private void handle(ReceiverEndMessage message)
    {
        state.get(-1).notWorking();
        view.setState(state);
        view.end();
    }
    private void handle(PrimeMessage message)
    {
        state.get(message.id).increment();
        view.setState(state);
    }

    private void init() throws KeyException, IOException
    {
        int senders = view.howManySenders();
        for(int i = -1; i < senders; i++)
            state.put(i, new Status());
        MessageReceiver receiver = new MessageReceiver(this);
        receiver.start();
        for(int i = 0; i < senders; i++)
            new MessageSender(receiver, i).start();
    }

    private void main_loop() throws InterruptedException, KeyException, IOException
    {
        while(true)
        {
            Message message = messages.take();

            if(message instanceof SenderEndMessage)
                handle((SenderEndMessage) message);
            else if(message instanceof PrimeMessage)
                handle((PrimeMessage) message);
            else if(message instanceof ViewEndMessage)
                break;
            else if(message instanceof ReceiverEndMessage)
                handle((ReceiverEndMessage) message);
            else if(message instanceof ViewReadyMessage)
                handle((ViewReadyMessage) message);
            else
                System.err.println("No handler for " + message.getClass());

        }
    }

    public void sendMessage(Message message)
    {
        messages.offer(message);
    }

    @Override
    public void run()
    {
        try
        {
            main_loop();
        } catch(Exception ignored)
        {
        }
    }

    public static void main(String[] args) throws IOException, KeyException
    {
        Controller controller = new Controller();
        controller.start();
    }
}
