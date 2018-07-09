package Crawler;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import javax.print.Doc;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.net.UnknownHostException;
import java.util.Scanner;
import java.util.Set;
import java.util.TreeSet;

public class WebCrawler
{
    String website;
    String body;
    Set<String> links;
    Set<String> images;

    public WebCrawler(String website) throws IOException
    {
        String url[] = website.split("://");
        if(url.length == 2)
            this.website = website;
        else
            this.website = "http://" + website;
        Document doc = Jsoup.connect(this.website).get();
        this.website = doc.baseUri();
        body = doc.body().toString();
    }

    private Set<String> getAttributes(String element, String attribute)
    {
        Document doc = Jsoup.parse(body, website);
        Elements elements = doc.select(String.format("%s[%s]", element, attribute));
        TreeSet<String> set = new TreeSet<>();
        for(Element e : elements)
            set.add(e.attr("abs:" + attribute));
        return set;
    }

    public Set<String> getImages()
    {
        if(images == null)
            images = getAttributes("img", "src");
        return images;
    }

    public Set<String> getLinks()
    {
        if(links == null)
            links = getAttributes("a", "href");
        return links;
    }

    public String getWebsite()
    {
        return website;
    }
}
