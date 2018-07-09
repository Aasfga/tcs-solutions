package BoardView;

import MenuView.MenuController;
import Sudoku.Map;
import Sudoku.MapBuilder;
import Sudoku.MapInfo;
import javafx.beans.value.ChangeListener;
import javafx.event.EventHandler;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.stage.Stage;

import java.beans.PropertyVetoException;
import java.io.FileNotFoundException;
import java.util.HashSet;
import java.util.Set;
import java.util.logging.Logger;

public class BoardController
{
    static final Logger LOGGER = MenuController.LOGGER;
    private MenuController menu;
    private BoardWindow window;
    private Map map;
    private Set<Map.Tile> crr_errors = new HashSet<>();

    private void addErrors()
    {
        for(Map.Tile tile : crr_errors)
            window.setTileColor(tile.getCordY(), tile.getCordX(), tile.getColor());
        crr_errors = new HashSet<>(map.getErrors());
        for(Map.Tile tile : crr_errors)
            window.setTileColor(tile.getCordY(), tile.getCordX(), "#ff4d4d");
    }

    private void blink(String cords)
    {
        int y = Integer.valueOf(cords.split(" ")[0]);
        int x = Integer.valueOf(cords.split(" ")[1]);
        window.blinkError(y, x);
    }

    private void checkTile(int y, int x)
    {
        try
        {
            String text = window.getText(y, x);
            Integer value = text.equals("") ? null : Integer.valueOf(text);
            map.setTileValue(y, x, value);
        } catch(PropertyVetoException e)
        {
            LOGGER.warning("Can't set value");
            blink(e.getMessage());
            window.setTileValue(y, x, map.getTile(y, x).getValue());
        }
        addErrors();
    }

    private void addDigit(int y, int x, String d)
    {
        Integer number = Integer.valueOf(window.getText(y, x) + d);
        if(number <= map.getSize())
            window.setTileValue(y, x, number);

    }

    private EventHandler<KeyEvent> generateKeyTypeHandler(int y, int x)
    {
        return event -> {
            if(event.getCode().equals(KeyCode.ENTER))
            {
                if(map.getSize() > 9)
                    checkTile(y, x);
            }
            else if(event.getCode().equals(KeyCode.BACK_SPACE))
                window.backspace(y, x);
            else if(event.getCode().isDigitKey())
            {
                if(map.getSize() <= 9)
                    window.backspace(y, x);
                addDigit(y, x, event.getCode().getName());
                if(map.getSize() <= 9)
                    checkTile(y, x);
            }
        };
    }

    private ChangeListener<Boolean> generateOnFocusLostHandler(int y, int x)
    {
        return (o, old_v, new_v) -> {
            if(!new_v)
            {
                window.setTileColor(y, x, map.getTile(y, x).getColor());
                checkTile(y, x);
            } else
                window.setTileColor(y, x, "#ad8bc6");
        };
    }

    public BoardController(MenuController menu, MapInfo mapInfo) throws FileNotFoundException
    {
        this.menu = menu;
        this.map = MapBuilder.readMap(mapInfo);
    }

    private void setBoardProperties()
    {
        for(int y = 0; y < map.getSize(); y++)
        {
            for(int x = 0; x < map.getSize(); x++)
            {
                Map.Tile tile = map.getTile(y, x);
                window.setTileValue(y, x, tile.getValue());
                window.setTileColor(y, x, tile.getColor());
                if(!tile.isFinal())
                {
                    window.setHandler(y, x, generateKeyTypeHandler(y, x));
                    window.setHandler(y, x, generateOnFocusLostHandler(y, x));
                }
                else
                    window.setTileFont(y, x, Font.font("Arial", FontWeight.EXTRA_BOLD, 14));
            }
        }
    }

    public void start(Stage stage)
    {
        LOGGER.entering("BoardController", "start(stage)");
        window = new BoardWindow(stage, map.getSize());
        window.setOnCloseRequest(e -> exit());
        setBoardProperties();
        window.show();
    }

    void exit()
    {
        LOGGER.entering("BoardController", "exit()");
        window.stop();
        menu.start();
    }
}
