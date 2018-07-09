package Storage;

import com.sun.corba.se.spi.activation.Server;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Calendar;
import java.util.Collection;
import java.util.GregorianCalendar;
import java.util.logging.ConsoleHandler;
import java.util.logging.Level;
import java.util.logging.Logger;

public class ServerProvider implements Provider
{
    private final String SERVER = "localhost";
    private final String SERVER_PORT = "7070";
    private final String ADDRESS = String.format("http://%s:%s", SERVER, SERVER_PORT);

    private static Logger logger = Logger.getAnonymousLogger();

    public static void setLogger(Logger logger)
    {
        ServerProvider.logger = logger;
    }

    private static byte[] extract(InputStream inputStream) throws IOException
    {
        ByteArrayOutputStream byteStream = new ByteArrayOutputStream();
        byte[] buffer = new byte[1024];
        int read = 0;
        while((read = inputStream.read(buffer, 0, buffer.length)) != -1)
            byteStream.write(buffer, 0, read);
        byteStream.flush();
        return byteStream.toByteArray();
    }

    private static String getDateURIFragment(GregorianCalendar date)
    {
        return String.format("%d:%d:%d:%d:%d:%d", date.get(Calendar.YEAR), date.get(Calendar.MONTH),
                             date.get(Calendar.DAY_OF_MONTH), date.get(Calendar.HOUR_OF_DAY),
                             date.get(Calendar.MINUTE), date.get(Calendar.SECOND));
    }

    @Override
    public Collection<FileInfo> getFileNames() throws Exception
    {
        InputStream stream = new URL(ADDRESS + "/list").openStream();
        Object o = new ObjectInputStream(stream).readObject();
        if(o instanceof Exception)
            throw (Exception) o;
        return (Collection) o;
    }

    private void checkResponse(HttpURLConnection connection) throws Exception
    {
        byte responseBody[] = extract(connection.getInputStream());
        if(responseBody.length == 0)
            return;
        Object o = new ObjectInputStream(new ByteArrayInputStream(responseBody)).readObject();
        if(o instanceof Exception)
            throw (Exception) o;
        else
            logger.info("Object with unknown type was received");
    }

    @Override
    public void uploadFile(String filename, byte[] file) throws Exception
    {
        HttpURLConnection httpConnection = (HttpURLConnection) new URL(
                String.format("%s/upload/%s", ADDRESS, filename)).openConnection();
        httpConnection.setDoOutput(true);
        httpConnection.setRequestMethod("POST");
        try(OutputStream stream = httpConnection.getOutputStream())
        {
            stream.write(file);
        }
        checkResponse(httpConnection);
    }

    @Override
    public byte[] downloadFile(FileInfo info) throws Exception
    {
        String filename = info.getFilename();
        String date = getDateURIFragment(info.getDate());
        HttpURLConnection connection = (HttpURLConnection) new URL(
                String.format("%s/download/%s;%s", ADDRESS, filename, date)).openConnection();
        if(connection.getResponseCode() == 51)
            checkResponse(connection);
        return extract(connection.getInputStream());
    }
}
