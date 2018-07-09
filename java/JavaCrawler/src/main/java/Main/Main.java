package Main;

import jdk.incubator.http.HttpClient;
import jdk.incubator.http.HttpRequest;
import jdk.incubator.http.HttpResponse;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

public class Main
{
    public static void main(String[] args) throws URISyntaxException, IOException, InterruptedException
    {
        HttpClient client = HttpClient.newHttpClient();

        // GET
        HttpResponse<String> response = client.send(
                HttpRequest
                        .newBuilder(new URI("http://www.foo.com/"))
                        .headers("Foo", "foovalue", "Bar", "barvalue")
                        .GET()
                        .build(),
                HttpResponse.BodyHandler.asString()
        );
        int statusCode = response.statusCode();
        String body = response.body();
        System.out.println(body);
    }
}
