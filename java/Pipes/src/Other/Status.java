package Other;


public class Status
{
    private int primes = 0;
    private Boolean isWorking = true;


    public void increment()
    {
        primes++;
    }

    public void notWorking()
    {
        isWorking = false;
    }

    public int getPrimes()
    {
        return primes;
    }

    public Boolean getWorking()
    {
        return isWorking;
    }
}
