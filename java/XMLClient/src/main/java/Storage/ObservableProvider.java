package Storage;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

import javax.xml.bind.JAXBException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;

public class ObservableProvider implements Provider
{

    private Provider provider;
    private ObservableList<FileInfo> list = FXCollections.observableList(new ArrayList<>());

    public ObservableProvider(Provider provider)
    {
        this.provider = provider;
    }

    @Override
    public Collection<FileInfo> getFileNames() throws Exception
    {
        Collection<FileInfo> files = provider.getFileNames();
        list.setAll(files);
        return list;
    }

    @Override
    public void uploadFile(String filename, byte[] file) throws Exception
    {
        provider.uploadFile(filename, file);
        refresh();
    }

    @Override
    public byte[] downloadFile(FileInfo info) throws Exception
    {
        return provider.downloadFile(info);
    }

    public ObservableList<FileInfo> getList()
    {
        return list.sorted();
    }

    public void refresh() throws Exception
    {
        list.setAll(provider.getFileNames());
    }
}
