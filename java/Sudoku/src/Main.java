import MenuView.MenuController;
import javafx.application.Application;
import javafx.stage.Stage;

public class Main extends Application
{
    public static void main(String[] args)
    {
        Main.launch();
    }

    @Override
    public void start(Stage primaryStage) throws Exception
    {
        MenuController controller = MenuController.getMenuController();
        controller.start(primaryStage);
    }
}
