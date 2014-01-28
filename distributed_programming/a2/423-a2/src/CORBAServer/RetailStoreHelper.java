package CORBAServer;


/**
* CORBAServer/RetailStoreHelper.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from /home/starcraftman/programming/423DistributedProject/a2/423-a2/src/corba/base/interface.idl
* Sunday, November 4, 2012 6:35:54 o'clock AM EST
*/


/* Attempting to stay somewhat close to my original design.	*/
abstract public class RetailStoreHelper
{
  private static String  _id = "IDL:CORBAServer/RetailStore:1.0";

  public static void insert (org.omg.CORBA.Any a, CORBAServer.RetailStore that)
  {
    org.omg.CORBA.portable.OutputStream out = a.create_output_stream ();
    a.type (type ());
    write (out, that);
    a.read_value (out.create_input_stream (), type ());
  }

  public static CORBAServer.RetailStore extract (org.omg.CORBA.Any a)
  {
    return read (a.create_input_stream ());
  }

  private static org.omg.CORBA.TypeCode __typeCode = null;
  synchronized public static org.omg.CORBA.TypeCode type ()
  {
    if (__typeCode == null)
    {
      __typeCode = org.omg.CORBA.ORB.init ().create_interface_tc (CORBAServer.RetailStoreHelper.id (), "RetailStore");
    }
    return __typeCode;
  }

  public static String id ()
  {
    return _id;
  }

  public static CORBAServer.RetailStore read (org.omg.CORBA.portable.InputStream istream)
  {
    return narrow (istream.read_Object (_RetailStoreStub.class));
  }

  public static void write (org.omg.CORBA.portable.OutputStream ostream, CORBAServer.RetailStore value)
  {
    ostream.write_Object ((org.omg.CORBA.Object) value);
  }

  public static CORBAServer.RetailStore narrow (org.omg.CORBA.Object obj)
  {
    if (obj == null)
      return null;
    else if (obj instanceof CORBAServer.RetailStore)
      return (CORBAServer.RetailStore)obj;
    else if (!obj._is_a (id ()))
      throw new org.omg.CORBA.BAD_PARAM ();
    else
    {
      org.omg.CORBA.portable.Delegate delegate = ((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate ();
      CORBAServer._RetailStoreStub stub = new CORBAServer._RetailStoreStub ();
      stub._set_delegate(delegate);
      return stub;
    }
  }

  public static CORBAServer.RetailStore unchecked_narrow (org.omg.CORBA.Object obj)
  {
    if (obj == null)
      return null;
    else if (obj instanceof CORBAServer.RetailStore)
      return (CORBAServer.RetailStore)obj;
    else
    {
      org.omg.CORBA.portable.Delegate delegate = ((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate ();
      CORBAServer._RetailStoreStub stub = new CORBAServer._RetailStoreStub ();
      stub._set_delegate(delegate);
      return stub;
    }
  }

}
