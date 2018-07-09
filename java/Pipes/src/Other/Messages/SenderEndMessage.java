package Other.Messages;

public class SenderEndMessage implements Message
{
    private int id;

    public SenderEndMessage(int id)
    {
        this.id = id;
    }

    public int getId()
    {
        return id;
    }
}
