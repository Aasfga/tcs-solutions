import App.Controller;
import Storage.ExampleProvider;
import Storage.FileProvider;
import Storage.ServerProvider;
import javafx.application.Application;
import javafx.stage.Stage;

import java.io.File;
import java.util.logging.ConsoleHandler;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Main extends Application
{

    private static Logger appLogger = Logger.getLogger("AppLogger");

    static
    {
        Level level = Level.ALL;
        appLogger.setUseParentHandlers(false);
        ConsoleHandler handler = new ConsoleHandler();
        handler.setLevel(level);
        appLogger.setLevel(level);
        appLogger.addHandler(handler);
    }

    @Override
    public void start(Stage primaryStage) throws Exception
    {
        new Controller(new FileProvider(), new ServerProvider()).start(primaryStage);
    }

    public static void main(String[] args)
    {
        Controller.setLogger(appLogger);
        ServerProvider.setLogger(appLogger);
        new File("files").mkdir();
        launch();
    }
}
