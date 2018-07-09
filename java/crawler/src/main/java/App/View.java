package App;

import javafx.scene.control.Button;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.text.Text;

import java.util.Collection;


public class View
{

    public ListView<String> links;
    public ListView<String> history;
    public TextField website;
    public Button sendBtn;
    public Text errorMsg;
    public Text imgNumber;
    public Text linkNumber;
    public Text imgSize;
    public Button prevSite;

    public void initialize()
    {
        prevSite.setDisable(true);
        prevSite.setVisible(false);
        errorMsg.setVisible(false);
    }

    public View()
    { }

    public void setImageNumber(int number)
    {
        imgNumber.setText(String.format("Found %d images", number));
    }

    public void setLinkNumber(int number)
    {
        linkNumber.setText(String.format("Found %d links", number));
    }

    public void setImageSize(long size)
    {
        imgSize.setText(String.format("All images take %.2fMB", size/(1024D * 1024D)));
    }

    public void showPrevButton()
    {
        prevSite.setDisable(false);
        prevSite.setVisible(true);
    }

    public void hidePrevButton()
    {
        prevSite.setDisable(true);
        prevSite.setVisible(false);
    }

    public String getWebsite()
    {
        return website.getText();
    }

    public void setWebsite(String website)
    {
        this.website.setText(website);
    }

    public void setErrorMessage(String msg)
    {
        errorMsg.setText(msg);
    }

    public ListView<String> getLinks()
    {
        return links;
    }

    public ListView<String> getHistory()
    {
        return history;
    }

    public Button getSendBtn()
    {
        return sendBtn;
    }

    public Button getPrevSite()
    {
        return prevSite;
    }

    public void setHistoryElements(Collection<String> elements)
    {
        history.getItems().setAll(elements);
    }

    public void addHistoryElements(Collection<String> elements)
    {
        history.getItems().addAll(elements);
    }

    public void addHistoryElements(String... elements)
    {
        history.getItems().addAll(elements);
    }

    public void setLinksElements(Collection<String> elements)
    {
        links.getItems().setAll(elements);
    }

    public void addLinksElements(Collection<String> elements)
    {
        links.getItems().addAll(elements);
    }

    public void addLinksElements(String... elements)
    {
        links.getItems().addAll(elements);
    }

    public void showErrorMessage()
    {
        errorMsg.setVisible(true);
    }

    public void hideErrorMessage()
    {
        errorMsg.setVisible(false);
    }
}
