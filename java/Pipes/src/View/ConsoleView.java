package View;

import Controller.Controller;
import Other.Messages.ViewEndMessage;
import Other.Messages.ViewReadyMessage;
import Other.Status;

import java.util.Map;
import java.util.Scanner;
import java.util.Set;
import java.util.TreeMap;

public class ConsoleView implements View
{
    private Map<Integer, Status> stateMap = new TreeMap<Integer, Status>();
    private Controller controller;

    public ConsoleView(Controller controller)
    {
        this.controller = controller;
        controller.sendMessage(new ViewReadyMessage());
    }

    @Override
    public void setState(Map<Integer, Status> stateMap)
    {
        this.stateMap = new TreeMap<Integer, Status>(stateMap);
    }

    @Override
    public int howManySenders()
    {
        System.out.println("How many senders?");
        return new Scanner(System.in).nextInt();
    }

    @Override
    public void end()
    {
        for(int i = 0; stateMap.containsKey(i); i++)
            System.out.println("Sender " + (i + 1) + " send " + stateMap.get(i).getPrimes() + " primes");
        controller.sendMessage(new ViewEndMessage());
    }


}
