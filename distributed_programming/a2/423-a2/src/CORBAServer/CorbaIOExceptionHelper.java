package CORBAServer;


/**
* CORBAServer/CorbaIOExceptionHelper.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from /home/starcraftman/programming/423DistributedProject/a2/423-a2/src/corba/base/interface.idl
* Sunday, November 4, 2012 6:35:54 o'clock AM EST
*/

abstract public class CorbaIOExceptionHelper
{
  private static String  _id = "IDL:CORBAServer/CorbaIOException:1.0";

  public static void insert (org.omg.CORBA.Any a, CORBAServer.CorbaIOException that)
  {
    org.omg.CORBA.portable.OutputStream out = a.create_output_stream ();
    a.type (type ());
    write (out, that);
    a.read_value (out.create_input_stream (), type ());
  }

  public static CORBAServer.CorbaIOException extract (org.omg.CORBA.Any a)
  {
    return read (a.create_input_stream ());
  }

  private static org.omg.CORBA.TypeCode __typeCode = null;
  private static boolean __active = false;
  synchronized public static org.omg.CORBA.TypeCode type ()
  {
    if (__typeCode == null)
    {
      synchronized (org.omg.CORBA.TypeCode.class)
      {
        if (__typeCode == null)
        {
          if (__active)
          {
            return org.omg.CORBA.ORB.init().create_recursive_tc ( _id );
          }
          __active = true;
          org.omg.CORBA.StructMember[] _members0 = new org.omg.CORBA.StructMember [1];
          org.omg.CORBA.TypeCode _tcOf_members0 = null;
          _tcOf_members0 = org.omg.CORBA.ORB.init ().create_string_tc (0);
          _members0[0] = new org.omg.CORBA.StructMember (
            "message",
            _tcOf_members0,
            null);
          __typeCode = org.omg.CORBA.ORB.init ().create_exception_tc (CORBAServer.CorbaIOExceptionHelper.id (), "CorbaIOException", _members0);
          __active = false;
        }
      }
    }
    return __typeCode;
  }

  public static String id ()
  {
    return _id;
  }

  public static CORBAServer.CorbaIOException read (org.omg.CORBA.portable.InputStream istream)
  {
    CORBAServer.CorbaIOException value = new CORBAServer.CorbaIOException ();
    // read and discard the repository ID
    istream.read_string ();
    value.message = istream.read_string ();
    return value;
  }

  public static void write (org.omg.CORBA.portable.OutputStream ostream, CORBAServer.CorbaIOException value)
  {
    // write the repository ID
    ostream.write_string (id ());
    ostream.write_string (value.message);
  }

}
