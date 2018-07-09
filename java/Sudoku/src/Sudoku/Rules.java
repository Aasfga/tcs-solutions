package Sudoku;

import MenuView.MenuController;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.function.Predicate;
import java.util.logging.Logger;

class Rules
{
    final static Logger LOGGER = MenuController.LOGGER;

    static Map.Tile error;

    static Iterable<Map.Tile> getColumn(Map.Tile tile)
    {
        Map map = tile.getMap();
        ArrayList<Map.Tile> list = new ArrayList<>();

        for(int i = 0; i < map.getSize(); i++)
        {
            list.add(map.tiles[i][tile.getCordX()]);
        }
        return list;
    }

    static Iterable<Map.Tile> getRow(Map.Tile tile)
    {
        Map map = tile.getMap();
        ArrayList<Map.Tile> list = new ArrayList<>();

        for(int i = 0; i < map.getSize(); i++)
        {
            list.add(map.tiles[tile.getCordY()][i]);
        }
        return list;
    }

    static Iterable<Map.Tile> getGroup(Map.Tile tile)
    {
        return tile.getMap().groups.get(tile.groupId);
    }

    static boolean check(Map.Tile tile, Predicate<Map.Tile> p)
    {
        for(Map.Tile other : getColumn(tile))
        {
            if(p.test(other))
            {
                error = other;
                return false;
            }
        }
        for(Map.Tile other : getRow(tile))
        {
            if(p.test(other))
            {
                error = other;
                return false;
            }
        }
        for(Map.Tile other : getGroup(tile))
        {
            if(p.test(other))
            {
                error = other;
                return false;
            }
        }

        return true;
    }

    static Map.Tile canBePlaced(Map.Tile tile, Integer new_value)
    {
        if(new_value == null)
            return null;
        Integer old_value = tile.value;
        tile.value = new_value;
        Predicate<Map.Tile> p = other -> !other.equals(tile) && other.isFinal() && tile.value != null && tile.value.equals(other.value);
        boolean result = check(tile, p);
        tile.value = old_value;
        return result ? null : error;
    }


    static boolean isCorrect(Map.Tile tile)
    {
        if(tile.value == null)
            return true;
        Predicate<Map.Tile> p = other -> !other.equals(tile) && other.value != null && tile.value.equals(other.value);
        return check(tile, p);
    }

    static void refreshErrors(Map.Tile tile)
    {
        Set<Map.Tile> errors = tile.getMap().errors;
        Set<Map.Tile> to_remove = new HashSet<>();

        for(Map.Tile error : errors)
        {
            if(isCorrect(error))
                to_remove.add(error);
        }
        for(Map.Tile correct : to_remove)
            errors.remove(correct);
        for(Map.Tile other : getColumn(tile))
        {
            if(!isCorrect(other))
                errors.add(other);
        }
        for(Map.Tile other : getRow(tile))
        {
            if(!isCorrect(other))
                errors.add(other);
        }
        for(Map.Tile other : getGroup(tile))
        {
            if(!isCorrect(other))
                errors.add(other);
        }
    }
}
