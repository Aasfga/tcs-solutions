package View;

import Other.Status;

import java.util.Map;

public interface View
{
    void setState(Map<Integer, Status> stateMap);

    int howManySenders();

    void end();
}
