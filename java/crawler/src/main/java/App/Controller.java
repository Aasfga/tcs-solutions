package App;

import Crawler.SizeChecker;
import Crawler.WebCrawler;
import URLHistory.History;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.ListCell;
import javafx.scene.control.ListView;
import javafx.stage.Stage;

import java.io.IOException;
import java.sql.SQLException;
import java.util.Stack;
import java.util.logging.ConsoleHandler;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Controller
{
    private static final Logger logger = Logger.getLogger(Controller.class.getName());
    private static final Level level = Level.ALL;
    private History history;
    private Stack<String> localHistory = new Stack<>();
    private View view;


    static
    {
        logger.setUseParentHandlers(false);
        logger.setLevel(level);
        ConsoleHandler handler = new ConsoleHandler();
        handler.setLevel(level);
        logger.addHandler(handler);
    }
    public Controller(History history)
    {
        this.history = history;
    }

    private void addScene(Stage stage, FXMLLoader loader) throws IOException
    {
        Parent root = loader.load();
        Scene scene = new Scene(root, 700, 400);
        stage.setScene(scene);
    }

    private void showData(WebCrawler crawler, SizeChecker checker) throws InterruptedException
    {
        view.setImageNumber(crawler.getImages().size());
        view.setImageSize(checker.getSize());
        view.setLinkNumber(crawler.getLinks().size());
        view.setLinksElements(crawler.getLinks());
        view.setWebsite(crawler.getWebsite());
    }

    private void scan(String website) throws IOException, InterruptedException
    {
        WebCrawler crawler = new WebCrawler(website);
        SizeChecker checker = new SizeChecker(crawler.getImages());
        showData(crawler, checker);
        view.hideErrorMessage();
        try
        {
            history.add(crawler.getWebsite());
            view.setHistoryElements(history.getHistory());
        }
        catch(Exception e)
        {
            logger.warning("History exception " + e.getMessage());
        }
        if(localHistory.size() >= 1)
            view.showPrevButton();
    }

    private void newScan(String website)
    {
        try
        {
            scan(website);
            localHistory.add(website);
        }
        catch(Exception e)
        {
            view.showErrorMessage();
        }
    }

    private void listCellClicked(String item)
    {
        logger.fine("cell list was clicked");
        newScan(item);
    }

    private void sendButtonClicked()
    {
        logger.fine("send button was clicked");
        newScan(view.getWebsite());
    }

    private void prevButtonClicked()
    {
        logger.fine("prev button was clicked");
        String website = localHistory.pop();
        if(localHistory.empty())
            view.hidePrevButton();
        try
        {
            scan(website);
        }
        catch(Exception e)
        {
            view.showErrorMessage();
        }
    }

    private void setListCellFactory(ListView<String> list)
    {
        class MyCell extends ListCell<String>
        {
            @Override
            protected void updateItem(String item, boolean empty)
            {
                super.updateItem(item, empty);
                if(item == null)
                {
                    setOnMouseClicked(event -> {
                    });
                    setText("");
                } else
                {
                    setText(item);
                    setOnMouseClicked(event -> {
                        if(event.getClickCount() == 2)
                            listCellClicked(item);
                    });
                }
            }
        }

        list.setCellFactory(param -> new MyCell());
    }


    private void initializeView()
    {
        try
        {
            view.getPrevSite().setOnMouseClicked(event -> prevButtonClicked());
            view.getSendBtn().setOnMouseClicked(event -> sendButtonClicked());
            setListCellFactory(view.getHistory());
            setListCellFactory(view.getLinks());
            view.addHistoryElements(history.getHistory());
        }
        catch(Exception e)
        {
            logger.warning("History exception " + e.getMessage());
        }
    }

    public void start(Stage stage) throws IOException
    {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("view.fxml"));
        addScene(stage, loader);
        view = loader.getController();
        initializeView();
        stage.show();
    }
}

