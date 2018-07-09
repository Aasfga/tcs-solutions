package BoardView;

import javafx.animation.Animation;
import javafx.animation.FadeTransition;
import javafx.beans.value.ChangeListener;
import javafx.event.EventHandler;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.TextField;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.Text;
import javafx.scene.text.TextAlignment;
import javafx.scene.text.TextFlow;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;
import javafx.util.Duration;


public class BoardWindow
{
    private final int TILE_SIZE = 40;
    private final int MSG_BOX_H = 25;
    private Stage stage;
    private VBox vbox = new VBox();
    private int size;
    private TextField tiles[][];

    private void addTiles(GridPane grid)
    {
        for(int y = 0 ; y < size; y++)
        {
            for(int x = 0; x < size; x++)
            {
                tiles[y][x] = new TextField("");
                tiles[y][x].setPrefSize(TILE_SIZE, TILE_SIZE);
                tiles[y][x].setStyle("-fx-text-box-border: transparent;");
                tiles[y][x].setAlignment(Pos.CENTER);
                tiles[y][x].setEditable(false);
                tiles[y][x].setBorder(new Border(new BorderStroke(Color.BLACK, BorderStrokeStyle.SOLID,
                        CornerRadii.EMPTY, new BorderWidths(0.5, 0.5, 0.5, 0.5))));
                grid.add(tiles[y][x], x, y);
            }
        }
    }

    private GridPane createGridPane()
    {
        GridPane grid = new GridPane();
        grid.setPrefSize(TILE_SIZE * size, TILE_SIZE * size);
        return grid;
    }


    public BoardWindow(Stage stage, int size)
    {
        this.stage = stage;
        this.size = size;
        this.tiles = new TextField[size][];
        for(int i = 0; i < size; i++)
            tiles[i] = new TextField[size];
        GridPane gridPane = createGridPane();
        addTiles(gridPane);
        vbox.getChildren().addAll(gridPane);
    }

    public void show()
    {
        stage.setScene(new Scene(vbox, size * TILE_SIZE, size * TILE_SIZE));
        stage.setResizable(false);
        stage.show();
    }

    public void stop()
    {
        stage.close();
    }

    public void setOnCloseRequest(EventHandler<WindowEvent> handler)
    {
        stage.setOnCloseRequest(handler);
    }

    public void setTileValue(int y, int x, Integer val)
    {
        String text;
        if(val == null)
            text = "";
        else
            text = val.toString();
        tiles[y][x].setText(text);
    }

    public void setTileColor(int y, int x, String color)
    {
        tiles[y][x].setStyle("-fx-background-color: " + color);
    }

    public void setTileFont(int y, int x, Font font)
    {
        tiles[y][x].setFont(font);
    }

    public void setHandler(int y, int x, EventHandler<? super KeyEvent> handler)
    {
        tiles[y][x].setOnKeyPressed(handler);
    }

    public void setHandler(int y, int x, ChangeListener<? super Boolean> listener)
    {
        tiles[y][x].focusedProperty().addListener(listener);
    }

    public String getText(int y, int x)
    {
        return tiles[y][x].getText();
    }

    public void backspace(int y, int x)
    {
        String text = tiles[y][x].getText();
        if(text.length() > 0)
            tiles[y][x].setText(text.substring(0, text.length() - 1));
    }


    public void blinkError(int y, int x)
    {
        double time = 0.4;
        FadeTransition fade = new FadeTransition(Duration.seconds(time), tiles[y][x]);
        fade.setFromValue(1.0);
        fade.setToValue(0.0);
        fade.play();
        FadeTransition show = new FadeTransition(Duration.seconds(time), tiles[y][x]);
        show.setDelay(Duration.seconds(time));
        show.setFromValue(0.0);
        show.setToValue(1.0);
        show.play();
    }

}
