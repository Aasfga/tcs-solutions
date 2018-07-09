package Other.IO;

import java.io.IOException;
import java.io.PipedReader;

public class PipedInput
{
    PipedReader reader = new PipedReader();

    public PipedInput()
    {
        reader = new PipedReader();
    }

    public PipedInput(PipedOutput output)
    {
        connect(output);
    }

    public void connect(PipedOutput output)
    {
        try
        {
            reader.connect(output.writer);
        } catch(IOException ignored)
        { }
    }

    public int nextInt()
    {
        int x = 0;
        for(int i = 0; i < 4; i++)
        {
            try
            {
                int s = reader.read();
                x += s << (i * 8);
            }
            catch(IOException ignored)
            { }
        }
        return x;
    }

    public void close()
    {
        try
        {
            reader.close();
        } catch(IOException ignored)
        { }
    }
}
