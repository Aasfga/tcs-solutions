package App;

import Storage.FileInfo;
import javafx.scene.control.Button;
import javafx.scene.control.ListView;
import javafx.scene.text.Text;

public class View
{
    public Button refreshClient;
    public ListView<FileInfo> clientFiles;
    public Button refreshServer;
    public ListView<FileInfo> serverFiles;
    public Button postBtn;
    public Button getBtn;
    public Text errorText;


    public void hideError()
    {
        errorText.setText("");
    }

    public void showError(String error)
    {
        errorText.setText(error);
    }
}
