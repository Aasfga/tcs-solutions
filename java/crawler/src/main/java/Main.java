import URLHistory.SQLiteHistory.DatabaseHistory;
import URLHistory.SetHistory;
import App.Controller;
import javafx.application.Application;
import javafx.stage.Stage;

import java.io.IOException;

public class Main extends Application
{
    public static void main(String[] args) throws IOException
    {
        launch();
    }

    @Override
    public void start(Stage primaryStage) throws Exception
    {
        new Controller(new DatabaseHistory()).start(primaryStage);
    }
}
