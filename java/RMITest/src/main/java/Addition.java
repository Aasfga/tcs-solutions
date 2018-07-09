import java.rmi.RemoteException;

public class Addition implements AdditionInterface
{
    @Override
    public int add(int a, int b) throws RemoteException
    {
        int result = a + b;
        return result;
    }
}
