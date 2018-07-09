package Storage;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;

public class FileProvider implements Provider
{

    @Override
    public Collection<FileInfo> getFileNames() throws IOException
    {
        File directory = new File("files");
        File files[] = directory.listFiles();
        Set<FileInfo> names = new TreeSet<>();

        if(files == null)
            throw new IOException("Not able to list files. FATAL ERROR");


        for(File f : files)
        {
            GregorianCalendar date = new GregorianCalendar();
            date.setTimeInMillis(f.lastModified());
            names.add(new FileInfo(f.getName(), date));
        }

        return names;
    }

    @Override
    public void uploadFile(String filename, byte[] file) throws IOException
    {
            Files.write(Paths.get("files/" + filename), file);
    }

    @Override
    public byte[] downloadFile(FileInfo info) throws IOException
    {
            return Files.readAllBytes(Paths.get("files/" + info.getFilename()));
    }
}
