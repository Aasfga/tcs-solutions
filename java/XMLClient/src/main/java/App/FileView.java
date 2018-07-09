package App;


import Storage.FileInfo;
import javafx.collections.ObservableList;
import javafx.event.EventHandler;
import javafx.scene.control.Button;
import javafx.scene.control.ListCell;
import javafx.scene.control.ListView;
import javafx.scene.input.MouseEvent;
import javafx.scene.text.Text;


public class FileView
{
    private Button refreshBtn;
    private ListView<FileInfo> list;

    public FileView(Button refreshBtn, ListView<FileInfo> list, CellFactory factory)
    {
        this.refreshBtn = refreshBtn;
        this.list = list;
        this.list.setCellFactory(param -> factory.createCell());
    }

    public FileInfo getSelectedFile()
    {
        return list.getSelectionModel().getSelectedItem();
    }

    public void setRefreshButtonHandler(EventHandler<? super MouseEvent> handler)
    {
        refreshBtn.setOnMouseClicked(handler);
    }

    public void setObservableList(ObservableList<FileInfo> list)
    {
        this.list.setItems(list);
    }

    public interface CellFactory
    {
        ListCell<FileInfo> createCell();
    }
}
