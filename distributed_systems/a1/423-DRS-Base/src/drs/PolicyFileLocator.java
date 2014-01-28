
package drs;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;

/**
 * Abstract away the policy file, shared between client and server.
 */
public class PolicyFileLocator {

    public static final String POLICY_FILE_NAME = File.separator + "allow_all.policy";

    public static String getLocationOfPolicyFile() {
        try {
            File tempFile = File.createTempFile("423-DRS-Base", ".policy");
            InputStream is = PolicyFileLocator.class.getResourceAsStream(POLICY_FILE_NAME);
            BufferedWriter writer = new BufferedWriter(new FileWriter(tempFile));
            int read = 0;

            while((read = is.read()) != -1) {
                writer.write(read);
            }

            writer.close();
            tempFile.deleteOnExit();
            return tempFile.getAbsolutePath();
        }
        catch(IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static void main(String[] args) {
        System.out.println(new File(getLocationOfPolicyFile()).exists());
    }

}
