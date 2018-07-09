package App;

import Storage.FileInfo;
import Storage.ObservableProvider;
import Storage.Provider;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.ListCell;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.GridPane;
import javafx.scene.text.Text;
import javafx.stage.Stage;

import javax.xml.bind.JAXBException;
import java.io.IOException;
import java.nio.file.FileAlreadyExistsException;
import java.util.Calendar;
import java.util.logging.ConsoleHandler;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Controller
{
    static Logger logger = Logger.getLogger("Controller");


    public static void setLogger(Logger logger)
    {
        Controller.logger = logger;
    }



    private ObservableProvider clientProvider;
    private ObservableProvider serverProvider;
    private FileView clientView;
    private FileView serverView;
    private View view;
    private Stage stage;

    public Controller(Provider clientProvider, Provider serverProvider)
    {
        this.clientProvider = new ObservableProvider(clientProvider);
        this.serverProvider = new ObservableProvider(serverProvider);
    }

    private static EventHandler<MouseEvent> refreshGenerator(ObservableProvider provider, View view)
    {
        return event -> {
            try
            {
                provider.refresh();
                view.hideError();
            }
            catch(Exception e)
            {
                logger.warning("Not able to refresh. Error msg: " + e.getMessage());
                view.showError(String.format("%s: %s", e.getClass(), e.getMessage()));
            }
        };
    }

    private static EventHandler<MouseEvent> buttonEventGenerator(ObservableProvider sender,
                                                                 FileView senderView,
                                                                 ObservableProvider receiver,
                                                                 View view)
    {
        return event -> {
            byte file[];
            FileInfo info = senderView.getSelectedFile();
            if(info == null)
            {
                logger.warning("Null pointer was found");
                return;
            }

            try
            {
                file = sender.downloadFile(info);
                receiver.uploadFile(info.getFilename(), file);
                view.hideError();
            }
            catch(FileAlreadyExistsException e)
            {
                view.showError("Try with another file. The same file has already been on the server");
                logger.info("User tried to send file which had been already on the server");
            }
            catch(JAXBException e)
            {
                view.showError("JAXB error");
                logger.warning("JAXB error: " + e.getMessage());
            }
            catch(IOException e)
            {
                view.showError(e.getMessage());
                logger.warning("IO error: " + e.getMessage());
            }
            catch(Exception e)
            {
                view.showError("Unknown error");
                logger.warning(String.format("%s: %s", e.getClass(), e.getMessage()));
            }
        };
    }

    private void setHandlers()
    {
        clientView.setRefreshButtonHandler(refreshGenerator(clientProvider, view));
        serverView.setRefreshButtonHandler(refreshGenerator(serverProvider, view));
        view.getBtn.setOnMouseClicked(buttonEventGenerator(serverProvider, serverView, clientProvider, view));
        view.postBtn.setOnMouseClicked(buttonEventGenerator(clientProvider, clientView, serverProvider, view));

    }

    private void setVariables() throws Exception
    {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("view.fxml"));
        Parent root = loader.load();
        stage.setScene(new Scene(root, 600, 430));
        view = loader.getController();
        clientView = new FileView(view.refreshClient, view.clientFiles, new FileSystemCellFactory(this));
        clientView.setObservableList(clientProvider.getList());
        serverView = new FileView(view.refreshServer, view.serverFiles, DataCellFactory.getFactory());
        serverView.setObservableList(serverProvider.getList());
    }

    public void start(Stage stage) throws Exception
    {
        this.stage = stage;
        setVariables();
        setHandlers();
        stage.setResizable(false);
        stage.show();
    }


    static class FileSystemCellFactory implements FileView.CellFactory
    {
        Controller app;

        public FileSystemCellFactory(Controller app)
        {
            this.app = app;
        }

        private void createEditView(String filename)
        {
            try
            {
                FXMLLoader loader = new FXMLLoader(Controller.class.getResource("edit_view.fxml"));
                Parent root = loader.load();
                EditController controller = loader.getController();
                controller.setFilename(filename);
                controller.refresh();
                Stage stage = new Stage();
                stage.setResizable(false);
                stage.setScene(new Scene(root, 600, 400));
                stage.setOnCloseRequest(closeEvent -> {
                    stage.close();
                    app.start();
                });
                app.hide();
                stage.show();
            }
            catch(IOException e)
            {
                logger.warning(e.getMessage());
            }
        }

        @Override
        public ListCell<FileInfo> createCell()
        {
            return new ListCell<FileInfo>()
            {
                @Override
                protected void updateItem(FileInfo item, boolean empty)
                {
                    super.updateItem(item, empty);
                    if(empty || item == null)
                    {
                        setGraphic(null);
                        setText(null);
                        setOnMouseClicked(event -> {
                        });
                    }
                    else
                    {
                        setText(item.getFilename());
                        setOnMouseClicked(event -> {
                            if(event.getClickCount() == 2)
                                createEditView(item.getFilename());
                        });
                    }
                }
            };
        }
    }

    static class DataCellFactory implements FileView.CellFactory
    {
        private static DataCellFactory factory = new DataCellFactory();


        static DataCellFactory getFactory()
        {
            return DataCellFactory.factory;
        }

        private DataCellFactory()
        {
        }

        @Override
        public ListCell<FileInfo> createCell()
        {
            return new ListCell<FileInfo>()
            {
                {
                    setWidth(200);
                }

                @Override
                protected void updateItem(FileInfo item, boolean empty)
                {
                    super.updateItem(item, empty);
                    if(empty || item == null)
                    {
                        setText(null);
                        setGraphic(null);
                    }
                    else
                    {
                        GridPane pane = new GridPane();
                        String dateString = String.format("%d/%d/%d: ", item.getDate().get(Calendar.DAY_OF_MONTH), item.getDate().get(Calendar.MONTH) + 1, item.getDate().get(Calendar.YEAR));
                        Text date = new Text(dateString);
                        Text filename = new Text(item.getFilename());
                        pane.add(filename, 1, 0);
                        pane.add(date, 0, 0);
                        setGraphic(pane);
                        setText(null);
                    }
                }
            };
        }
    }

    public void hide()
    {
        stage.hide();
    }

    public void start()
    {
        stage.show();
    }
}
