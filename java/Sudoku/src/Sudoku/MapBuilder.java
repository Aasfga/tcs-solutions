package Sudoku;


import java.beans.PropertyVetoException;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

public class MapBuilder
{
    public static Set<MapInfo> getAllLevels(String d)
    {
        File directory = new File("./Levels/" + d);
        File files[] = directory.listFiles();
        Set<MapInfo> set = new TreeSet<>();

        if(files == null)
            return set;
        for(File f : files)
            set.add(new MapInfo(d, f.getName()));
        return set;
    }

    public static Set<MapInfo> getAllLevels()
    {
        Set<MapInfo> set = new TreeSet<>();
        set.addAll(getAllLevels("Easy"));
        set.addAll(getAllLevels("Medium"));
        set.addAll(getAllLevels("Hard"));
        return set;
    }


    private static void readGroups(Map map, Scanner scanner)
    {
        int size = map.getSize();
        for(int y = 0; y < size; y++)
        {
            for(int x = 0; x < size; x++)
            {
                map.tiles[y][x] = map.new MutableTile(y, x, scanner.nextInt());
            }
        }
    }

    private static void readFinalTiles(Map map, Scanner scanner)
    {
        int final_number = scanner.nextInt();
        for(int i = 0; i < final_number; i++)
        {
            int y = scanner.nextInt();
            int x = scanner.nextInt();
            int val = scanner.nextInt();
            int groupId = map.tiles[y][x].groupId;
            map.tiles[y][x] = map.new FinalTile(y, x, groupId, val);
        }
    }

    private static void setGroupField(Map map)
    {
        for(int y = 0; y < map.getSize(); y++)
        {
            for(int x = 0; x < map.getSize(); x++)
            {
                Map.Tile tile = map.tiles[y][x];
                if(!map.groups.containsKey(tile.groupId))
                    map.groups.put(tile.groupId, new ArrayList<>());
                map.groups.get(tile.groupId).add(tile);
            }
        }
    }

    private static void addHandlers(Map map)
    {
        for(int y = 0; y < map.getSize(); y++)
        {
            for(int x = 0; x < map.getSize(); x++)
            {
                if(!map.tiles[y][x].isFinal())
                {
                    Map.MutableTile tile = (Map.MutableTile) map.tiles[y][x];
                    tile.addVetoableChangeListener(evt -> {
                        Map.Tile evt_tile = (Map.Tile) evt.getSource();
                        Map.Tile error = Rules.canBePlaced(evt_tile, (Integer) evt.getNewValue());
                        if(error != null)
                            throw new PropertyVetoException(error.getCordY() + " " + error.getCordX(), evt);
                    });
                    tile.addPropertyChangeListener(evt -> {
                        Map.Tile evt_tile = (Map.Tile) evt.getSource();
                        Rules.refreshErrors(evt_tile);
                    });
                }
            }
        }


    }

    public static Map readMap(MapInfo mapInfo) throws FileNotFoundException
    {
        File file = new File("Levels/" + mapInfo.getDifficulty() + "/" + mapInfo.getName());
        Scanner scanner = new Scanner(file);
        Map map = new Map(scanner.nextInt());
        readGroups(map, scanner);
        readFinalTiles(map, scanner);
        setGroupField(map);
        addHandlers(map);

        return map;
    }
}
