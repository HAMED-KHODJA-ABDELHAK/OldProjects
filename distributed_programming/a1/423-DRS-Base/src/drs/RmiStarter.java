
package drs;

/**
 * Base class simply does a set of common tasks, sets the security policy
 * and also adds needed classes to the codebase.
 */
public abstract class RmiStarter {
    public RmiStarter(Class<?> clazzToAddToServerCodebase) {

        System.setProperty("java.rmi.server.codebase", clazzToAddToServerCodebase
            .getProtectionDomain().getCodeSource().getLocation().toString());

        System.setProperty("java.security.policy", PolicyFileLocator.getLocationOfPolicyFile());

        if(System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }

        startupProcedure();
    }

    public abstract void startupProcedure();
}

