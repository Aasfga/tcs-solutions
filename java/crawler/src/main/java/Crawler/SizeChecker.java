package Crawler;

import sun.rmi.runtime.Log;

import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.concurrent.CountDownLatch;
import java.util.logging.Logger;

public class SizeChecker
{
    private Set<String> sources;
    private Long size = null;
    private TreeMap<String, Integer> sizes = new TreeMap<>();

    Logger LOGGER = Logger.getLogger("SizeChecker");

    synchronized void addSize(String source, int size)
    {
        sizes.put(source, size);
    }

    public SizeChecker(Set<String> sources)
    {
        this.sources = sources;
    }

    private static int getFileSize(String source) throws IOException
    {
        URLConnection connection = new URL(source).openConnection();
        connection.setRequestProperty("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.95 Safari/537.11");
        if(connection.getContentLength() == -1)
        {
            InputStream stream = connection.getInputStream();
            int size = 0;
            byte array[] = new byte[2048];
            int br = 0;
            do
            {
                size += br;
                br = stream.read(array);
            }while(br > 0);
            return size;
        }
        else
            return connection.getContentLength();
    }

    private void countSize() throws InterruptedException
    {
        CountDownLatch latch = new CountDownLatch(sources.size());
        for(String s : sources)
            new Thread(() -> {
                try
                {
                    addSize(s, getFileSize(s));
                } catch(IOException e)
                {
                   LOGGER.fine("Not able to load image size. Message: " + e.getMessage());
                }
                finally
                {
                    latch.countDown();
                }
            }).start();
        latch.await();
        size = 0L;
        for(Integer i : sizes.values())
            size += i;
    }

    public long getSize() throws InterruptedException
    {
        if(size == null)
            countSize();
        return size;
    }
}
