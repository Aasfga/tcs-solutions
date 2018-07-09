package Sudoku;

public class MapInfo implements Comparable<MapInfo>
{
    private String difficulty;
    private String name;

    MapInfo(String difficulty, String name)
    {
        this.difficulty = difficulty;
        this.name = name;
    }

    public String getDifficulty()
    {
        return difficulty;
    }

    public String getName()
    {
        return name;
    }

    @Override
    public int compareTo(MapInfo o)
    {
        if(name.equals(o.name))
            return difficulty.compareTo(o.difficulty);
        else
            return name.compareTo(o.name);
    }
}
