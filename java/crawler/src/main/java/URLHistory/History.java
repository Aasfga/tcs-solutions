package URLHistory;

import java.sql.SQLException;
import java.util.Collection;

public interface History
{
    void add(String name) throws Exception;

    Collection<String> getHistory() throws Exception;
}
