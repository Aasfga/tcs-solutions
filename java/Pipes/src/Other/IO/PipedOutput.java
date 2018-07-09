package Other.IO;

import java.io.IOException;
import java.io.PipedWriter;

public class PipedOutput
{
    PipedWriter writer = new PipedWriter();

    public PipedOutput()
    { }

    public PipedOutput(PipedInput input)
    {
        try
        {
            writer.connect(input.reader);
        } catch(IOException ignored)
        { }
    }

    public void connect(PipedInput input)
    {
        try
        {
            writer.connect(input.reader);
        } catch(IOException ignored)
        { }
    }

    public void write(int x)
    {
        try
        {
            for(int i = 0; i < 4; i++)
            {
                char s = (char) (x & 0xff);
                writer.write(s);
                x = x >> 8;
            }
        } catch(IOException ignored)
        { }
    }

    public void close()
    {
        try
        {
            writer.close();
        } catch(IOException e)
        { }
    }
}
