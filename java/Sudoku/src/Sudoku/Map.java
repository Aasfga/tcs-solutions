package Sudoku;

import java.beans.*;
import java.io.Serializable;
import java.util.*;

public class Map
{

    private static final String COLORS[] = {
            "#FFCCF9",
            "#B5B9FF",
            "#97A2FF",
            "#AFF8DB",
            "#FFFFD1",
            "#FFABAB",
            "#d0bc95",
            "#b2b3b3",
            "#b7aeae",
            "#CD919E",
            "#CDB5CD",
            "#00FFFF",
            "#DC143C"
    };

    public abstract class Tile
    {
        int cordY;
        int cordX;
        int groupId;
        Integer value;

        public Tile(int cordY, int cordX, int groupId)
        {
            this.cordY = cordY;
            this.cordX = cordX;
            this.groupId = groupId;
        }

        public abstract void setValue(Integer value) throws PropertyVetoException;

        public abstract boolean isFinal();

        public int getCordX()
        {
            return cordX;
        }

        public int getCordY()
        {
            return cordY;
        }

        public String getColor()
        {
            return COLORS[groupId];
        }

        public int getGroupId()
        {
            return groupId;
        }

        public Integer getValue()
        {
            return value;
        }

        public Map getMap()
        {
            return Map.this;
        }

        @Override
        public boolean equals(Object o)
        {
            if(this == o) return true;
            if(o == null || getClass() != o.getClass()) return false;
            Tile tile = (Tile) o;
            return cordY == tile.cordY && cordX == tile.cordX;
        }

        @Override
        public int hashCode()
        {

            return Objects.hash(cordY, cordX);
        }
    }

    public class FinalTile extends Tile
    {
        public FinalTile(int cordY, int cordX, int groupId, int value)
        {
            super(cordY, cordX, groupId);
            this.value = value;
        }

        @Override
        public void setValue(Integer value)
        {
            throw new UnsupportedOperationException();
        }

        @Override
        public boolean isFinal()
        {
            return true;
        }
    }

    public class MutableTile extends Tile implements Serializable
    {
        private PropertyChangeSupport propertySupport = new PropertyChangeSupport(this);
        private VetoableChangeSupport vetoableSupport = new VetoableChangeSupport(this);

        public MutableTile(int cordY, int cordX, int groupId)
        {
            super(cordY, cordX, groupId);
        }

        @Override
        public void setValue(Integer value) throws PropertyVetoException
        {
            Integer old_v = this.value;
            vetoableSupport.fireVetoableChange("value", old_v, value);
            this.value = value;
            propertySupport.firePropertyChange("value", old_v, value);
        }

        public void addPropertyChangeListener(PropertyChangeListener listener) {
            propertySupport.addPropertyChangeListener(listener);
        }

        public void removePropertyChangeListener(PropertyChangeListener listener) {
            propertySupport.removePropertyChangeListener(listener);
        }

        public void addVetoableChangeListener(VetoableChangeListener listener) {
            vetoableSupport.addVetoableChangeListener(listener);
        }

        public void removeVetoableChangeListener(VetoableChangeListener listener) {
            vetoableSupport.removeVetoableChangeListener(listener);
        }
        @Override
        public boolean isFinal()
        {
            return false;
        }
    }

    Tile tiles[][];
    java.util.Map<Integer, ArrayList<Tile>> groups = new HashMap<>();
    Set<Tile> errors = new HashSet<>();

    public Map(int size)
    {
        tiles = new Tile[size][];
        for(int i = 0; i < size; i++)
            tiles[i] = new Tile[size];
    }

    public void setTileValue(int y, int x, Integer value) throws PropertyVetoException
    {
        tiles[y][x].setValue(value);
    }

    public int getSize()
    {
        return tiles.length;
    }

    @Override
    public String toString()
    {
        StringBuilder builder = new StringBuilder();

        for(int y = 0; y < getSize(); y++)
        {
            for(int x = 0; x < getSize(); x++)
            {
                Integer value = tiles[y][x].getValue();
                if(value == null)
                    builder.append("_ ");
                else
                    builder.append(value).append(" ");
            }
            builder.append("\n");
        }
        return builder.toString();
    }

    public Tile getTile(int y, int x)
    {
        if(y < getSize() && x < getSize())
            return tiles[y][x];
        return null;
    }

    public Set<Tile> getErrors()
    {
        return errors;
    }
}
