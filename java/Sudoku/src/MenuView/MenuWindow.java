package MenuView;

import Sudoku.MapInfo;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ListCell;
import javafx.scene.control.ListView;
import javafx.scene.input.MouseEvent;
import javafx.stage.Stage;

import java.io.IOException;

public class MenuWindow
{
    private Stage stage;
    private Parent root;
    private Button closeButton;
    private ListView<MapInfo> easyList;
    private ListView<MapInfo> mediumList;
    private ListView<MapInfo> hardList;
    static private ButtonEventGenerator buttonEventGenerator;

    public interface ButtonEventGenerator
    {
        EventHandler<? super MouseEvent> generateHandler(MapInfo mapInfo);
    }

    private static class MenuCell extends ListCell<MapInfo>
    {
        @Override
        protected void updateItem(MapInfo item, boolean empty)
        {
            super.updateItem(item, empty);
            setStyle("-fx-background: WHITE;");
            if(empty || item == null)
            {
                setGraphic(null);
                setText(null);
            }
            else
            {
                String name = item.getName().substring(0, item.getName().length() - 4);
                Button button = new Button(name);
                button.setPrefSize(50, 50);
                button.setOnMouseClicked(buttonEventGenerator.generateHandler(item));
                setGraphic(button);
            }
        }
    }

    private void loadFXMLFields() throws IOException
    {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("menu_window.fxml"));
        root = loader.load();
        closeButton = (Button) loader.getNamespace().get("close_button");
        easyList = (ListView) loader.getNamespace().get("easy_list");
        mediumList = (ListView) loader.getNamespace().get("medium_list");
        hardList = (ListView) loader.getNamespace().get("hard_list");
    }

    public MenuWindow(Stage stage) throws IOException
    {
        loadFXMLFields();
        stage.setScene(new Scene(root, 500, 350));
        this.stage = stage;
        easyList.setCellFactory(param -> new MenuCell());
        mediumList.setCellFactory(param -> new MenuCell());
        hardList.setCellFactory(param -> new MenuCell());
    }

    public void show(MapInfo mapInfo)
    {
        MenuController.LOGGER.entering("MenuWindow", "show(MapInfo)");
        switch(mapInfo.getDifficulty())
        {
            case "Easy":
                easyList.getItems().add(mapInfo);
                break;
            case "Medium":
                mediumList.getItems().add(mapInfo);
                break;
            case "Hard":
                hardList.getItems().add(mapInfo);
                break;
        }
    }

    public Button getCloseButton()
    {
        return closeButton;
    }

    public ListView<MapInfo> getEasyList()
    {
        return easyList;
    }

    public ListView<MapInfo> getMediumList()
    {
        return mediumList;
    }

    public ListView<MapInfo> getHardList()
    {
        return hardList;
    }

    public static void setButtonEventGenerator(ButtonEventGenerator buttonEventGenerator)
    {
        MenuWindow.buttonEventGenerator = buttonEventGenerator;
    }

    public void show()
    {
        stage.show();
    }

    public void stop()
    {
        stage.hide();
    }

}
