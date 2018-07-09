package App;

import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.scene.input.MouseEvent;
import javafx.stage.Stage;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class EditController
{
    public Button saveBtn;
    public Button revertBtn;
    public TextArea text;
    String filename;

    public void setFilename(String filename)
    {
        this.filename = filename;
    }

    public void refresh() throws IOException
    {
        String data = new String(Files.readAllBytes(Paths.get("files/" + filename)));
        text.setText(data);
    }

    public void revertChanges(MouseEvent mouseEvent) throws IOException
    {
        refresh();
    }

    public void saveFile(MouseEvent mouseEvent) throws IOException
    {
        String data = text.getText();
        Files.write(Paths.get("files/" + filename), data.getBytes());
    }
}
