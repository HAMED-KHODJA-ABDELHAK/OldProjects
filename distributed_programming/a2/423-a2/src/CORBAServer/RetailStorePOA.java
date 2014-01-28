package CORBAServer;


/**
* CORBAServer/RetailStorePOA.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from /home/starcraftman/programming/423DistributedProject/a2/423-a2/src/corba/base/interface.idl
* Sunday, November 4, 2012 6:35:54 o'clock AM EST
*/


/* Attempting to stay somewhat close to my original design.	*/
public abstract class RetailStorePOA extends org.omg.PortableServer.Servant
 implements CORBAServer.RetailStoreOperations, org.omg.CORBA.portable.InvokeHandler
{

  // Constructors

  private static java.util.Hashtable _methods = new java.util.Hashtable ();
  static
  {
    _methods.put ("buyItems", new java.lang.Integer (0));
    _methods.put ("returnItems", new java.lang.Integer (1));
    _methods.put ("exchange", new java.lang.Integer (2));
    _methods.put ("checkStock", new java.lang.Integer (3));
  }

  public org.omg.CORBA.portable.OutputStream _invoke (String $method,
                                org.omg.CORBA.portable.InputStream in,
                                org.omg.CORBA.portable.ResponseHandler $rh)
  {
    org.omg.CORBA.portable.OutputStream out = null;
    java.lang.Integer __method = (java.lang.Integer)_methods.get ($method);
    if (__method == null)
      throw new org.omg.CORBA.BAD_OPERATION (0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);

    switch (__method.intValue ())
    {
       case 0:  // CORBAServer/RetailStore/buyItems
       {
         try {
           String cID = in.read_string ();
           int itemID = in.read_long ();
           int numItems = in.read_long ();
           String initStore = in.read_string ();
           CORBAServer.Result_s $result = null;
           $result = this.buyItems (cID, itemID, numItems, initStore);
           out = $rh.createReply();
           CORBAServer.Result_sHelper.write (out, $result);
         } catch (CORBAServer.CorbaNotEnoughStockException $ex) {
           out = $rh.createExceptionReply ();
           CORBAServer.CorbaNotEnoughStockExceptionHelper.write (out, $ex);
         } catch (CORBAServer.CorbaIOException $ex) {
           out = $rh.createExceptionReply ();
           CORBAServer.CorbaIOExceptionHelper.write (out, $ex);
         } catch (CORBAServer.CorbaInterruptedException $ex) {
           out = $rh.createExceptionReply ();
           CORBAServer.CorbaInterruptedExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 1:  // CORBAServer/RetailStore/returnItems
       {
         try {
           String cID = in.read_string ();
           int itemID = in.read_long ();
           int numItems = in.read_long ();
           CORBAServer.Result_s $result = null;
           $result = this.returnItems (cID, itemID, numItems);
           out = $rh.createReply();
           CORBAServer.Result_sHelper.write (out, $result);
         } catch (CORBAServer.CorbaIOException $ex) {
           out = $rh.createExceptionReply ();
           CORBAServer.CorbaIOExceptionHelper.write (out, $ex);
         } catch (CORBAServer.CorbaInterruptedException $ex) {
           out = $rh.createExceptionReply ();
           CORBAServer.CorbaInterruptedExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 2:  // CORBAServer/RetailStore/exchange
       {
         try {
           String cID = in.read_string ();
           int bItemID = in.read_long ();
           int bNumItems = in.read_long ();
           int dItemID = in.read_long ();
           int dNumItems = in.read_long ();
           CORBAServer.Result_s $result = null;
           $result = this.exchange (cID, bItemID, bNumItems, dItemID, dNumItems);
           out = $rh.createReply();
           CORBAServer.Result_sHelper.write (out, $result);
         } catch (CORBAServer.CorbaNotEnoughStockException $ex) {
           out = $rh.createExceptionReply ();
           CORBAServer.CorbaNotEnoughStockExceptionHelper.write (out, $ex);
         } catch (CORBAServer.CorbaIOException $ex) {
           out = $rh.createExceptionReply ();
           CORBAServer.CorbaIOExceptionHelper.write (out, $ex);
         } catch (CORBAServer.CorbaInterruptedException $ex) {
           out = $rh.createExceptionReply ();
           CORBAServer.CorbaInterruptedExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 3:  // CORBAServer/RetailStore/checkStock
       {
         try {
           int itemID = in.read_long ();
           CORBAServer.Stock_s $result[] = null;
           $result = this.checkStock (itemID);
           out = $rh.createReply();
           CORBAServer.StockHelper.write (out, $result);
         } catch (CORBAServer.CorbaIOException $ex) {
           out = $rh.createExceptionReply ();
           CORBAServer.CorbaIOExceptionHelper.write (out, $ex);
         } catch (CORBAServer.CorbaInetAddressException $ex) {
           out = $rh.createExceptionReply ();
           CORBAServer.CorbaInetAddressExceptionHelper.write (out, $ex);
         }
         break;
       }

       default:
         throw new org.omg.CORBA.BAD_OPERATION (0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);
    }

    return out;
  } // _invoke

  // Type-specific CORBA::Object operations
  private static String[] __ids = {
    "IDL:CORBAServer/RetailStore:1.0"};

  public String[] _all_interfaces (org.omg.PortableServer.POA poa, byte[] objectId)
  {
    return (String[])__ids.clone ();
  }

  public RetailStore _this() 
  {
    return RetailStoreHelper.narrow(
    super._this_object());
  }

  public RetailStore _this(org.omg.CORBA.ORB orb) 
  {
    return RetailStoreHelper.narrow(
    super._this_object(orb));
  }


} // class RetailStorePOA
