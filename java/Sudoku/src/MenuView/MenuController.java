package MenuView;


import BoardView.BoardController;
import Sudoku.Map;
import Sudoku.MapBuilder;
import Sudoku.MapInfo;
import javafx.application.Platform;
import javafx.stage.Stage;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.NoSuchElementException;
import java.util.logging.ConsoleHandler;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.Logger;

public class MenuController
{
    public static final Logger LOGGER = Logger.getLogger("appLogger");

    static
    {
        LOGGER.setUseParentHandlers(false);
        Handler consoleHandler = new ConsoleHandler();
        LOGGER.addHandler(consoleHandler);
        Level level = Level.INFO;
        LOGGER.setLevel(level);
        consoleHandler.setLevel(level);
    }

    private static MenuController controller = new MenuController();

    public static MenuController getMenuController()
    {
        return controller;
    }

    private MenuWindow window;

    private MenuController()
    { }

    private void startGame(MapInfo mapInfo)
    {
        if(mapInfo == null)
            throw new NullPointerException();
        try
        {
            LOGGER.fine("Game will start with mapInfo: " + mapInfo.getDifficulty() + " " + mapInfo.getName());
            BoardController boardController = new BoardController(this, mapInfo);
            stop();
            boardController.start(new Stage());
        }
        catch(FileNotFoundException e)
        {
            LOGGER.warning("Can't find file Levels/" + mapInfo.getDifficulty() + "/" + mapInfo.getName());
        }
        catch(NoSuchElementException e)
        {
            LOGGER.warning("Can't load Levels/" + mapInfo.getDifficulty() + "/" + mapInfo.getName());
        }
    }

    public void start(Stage stage)
    {
        LOGGER.fine("First show of the menu window");
        try
        {
            window = new MenuWindow(stage);
            MenuWindow.setButtonEventGenerator(mapInfo -> (e) -> startGame(mapInfo));
            for(MapInfo l : MapBuilder.getAllLevels())
                window.show(l);

            window.getCloseButton().setOnMouseClicked(e -> exit());
            window.show();
        } catch(IOException e)
        {
            LOGGER.warning("Not able to create a window, closing application");
            e.printStackTrace();
            Platform.exit();
        }

    }

    public void start()
    {
        LOGGER.fine("Show menu");
        window.show();
    }

    public void stop()
    {
        LOGGER.fine("Hide menu");
        window.stop();
    }

    public void exit()
    {
        LOGGER.fine("Exit from application");
        Platform.exit();
    }
}
