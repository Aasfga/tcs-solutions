package URLHistory;

import java.util.*;

public class SetHistory implements History
{
    Set<String> history = new TreeSet<>();

    {
        history.add("www.onet.pl");
        history.add("www.satori.tcs.uj.edu.pl");
        history.add("www.pewnie_ten_link_nie_dziala.yolo");
    }
    public SetHistory()
    {}

    public SetHistory(String ...links)
    {
        history.addAll(Arrays.asList(links));
    }

    @Override
    public void add(String name)
    {
        history.add(name);
    }

    @Override
    public Collection<String> getHistory()
    {
        return history;
    }
}
