package CORBAServer;

/**
* CORBAServer/CorbaIOExceptionHolder.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from /home/starcraftman/programming/423DistributedProject/a2/423-a2/src/corba/base/interface.idl
* Sunday, November 4, 2012 6:35:54 o'clock AM EST
*/

public final class CorbaIOExceptionHolder implements org.omg.CORBA.portable.Streamable
{
  public CORBAServer.CorbaIOException value = null;

  public CorbaIOExceptionHolder ()
  {
  }

  public CorbaIOExceptionHolder (CORBAServer.CorbaIOException initialValue)
  {
    value = initialValue;
  }

  public void _read (org.omg.CORBA.portable.InputStream i)
  {
    value = CORBAServer.CorbaIOExceptionHelper.read (i);
  }

  public void _write (org.omg.CORBA.portable.OutputStream o)
  {
    CORBAServer.CorbaIOExceptionHelper.write (o, value);
  }

  public org.omg.CORBA.TypeCode _type ()
  {
    return CORBAServer.CorbaIOExceptionHelper.type ();
  }

}
