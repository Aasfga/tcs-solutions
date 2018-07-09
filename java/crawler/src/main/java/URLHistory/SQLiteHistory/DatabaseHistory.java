package URLHistory.SQLiteHistory;

import URLHistory.History;

import java.sql.*;
import java.util.Collection;
import java.util.Set;
import java.util.TreeSet;

public class DatabaseHistory implements History
{

    final String databaseURL = "jdbc:sqlite:history.db";

    Connection connection = DriverManager.getConnection(databaseURL);

    public DatabaseHistory() throws SQLException
    {
        Statement statement = connection.createStatement();
        statement.execute("CREATE TABLE IF NOT EXISTS history (website PPRIMARY KEY)");
    }

    @Override
    public void add(String name) throws SQLException
    {
        String sql = "INSERT INTO history(website) VALUES(?)";
        try(PreparedStatement statement = connection.prepareStatement(sql))
        {
            statement.setString(1, name);
            statement.executeUpdate();
        }
    }

    @Override
    public Collection<String> getHistory() throws SQLException
    {
        String sql = "SELECT website FROM history";
        Set<String> set = new TreeSet<>();


        try(Statement stmt = connection.createStatement();
            ResultSet rs = stmt.executeQuery(sql))
        {
            while(rs.next())
                set.add(rs.getString("website"));
        }
        return set;
    }

    @Override
    protected void finalize() throws Throwable
    {
        super.finalize();
        connection.close();
    }
}
