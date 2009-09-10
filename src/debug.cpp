#include <e32svr.h>
#include <flogger.h>

#include "debug.h"
#include <e32std.h>


const TInt LDebugLineLength = 256;
//_LIT( KFileLogFolder,"C:\\logs\\gps60p");
_LIT( KFileLogFolder,"gpsp4symbian");
_LIT( KFileLogName,"gpsp4symbian.log");

class TLogWriter
    {
    public:
        static void Write( TRefByValue<const TDesC> aText, ... );
        static void Write(TRefByValue<const TDesC8> aText, ... );
    };

void WriteLOG( char* x )
    {
    TPtrC8 ptr( (TText8*)x ) ;
    TLogWriter::Write( ptr );
    }

void WriteLOG1( char* x, char* v )
    {
    TPtrC8 ptr( (TText8*)x );
    TPtrC8 ptrv( (TText8*)v );
    TLogWriter::Write( ptr, ptrv );
    }

void WriteLOG2( char* x, char* v, char* v1 )
    {
    TPtrC8 ptr( (TText8*)x );
    TPtrC8 ptrv( (TText8*)v );
    TPtrC8 ptrv1( (TText8*)v1);
    TLogWriter::Write( ptr, ptrv, ptrv1 );
    }

void WriteLOG3( char* x, char* v, char* v1, char* v2 )
    {
    TPtrC8 ptr( (TText8*)x );
    TPtrC8 ptrv( (TText8*)v );
    TPtrC8 ptrv1( (TText8*)v1);
    TPtrC8 ptrv2( (TText8*)v2);
    TLogWriter::Write( ptr, ptrv, ptrv1,ptrv2 );
    }

void WriteLOG4( char* x, char* v, char* v1, char* v2, char* v3 )
    {
    TPtrC8 ptr( (TText8*)x );
    TPtrC8 ptrv( (TText8*)v );
    TPtrC8 ptrv1( (TText8*)v1);
    TPtrC8 ptrv2( (TText8*)v2);
    TPtrC8 ptrv3( (TText8*)v3);
    TLogWriter::Write( ptr, ptrv, ptrv1,ptrv2, ptrv3 );
    }


struct TIgnoreOverFlow : public TDes16Overflow, public TDes8Overflow
    {
    void Overflow( TDes16& )
        {
        }
    void Overflow( TDes8& )
        {
        }
    };

void TLogWriter::Write( TRefByValue<const TDesC> aText, ... )
    { 
    VA_LIST args;
    VA_START( args, aText );
    
    TIgnoreOverFlow of;
    TBuf<LDebugLineLength> buf;
    buf.AppendFormatList( aText, args, &of );
        
    VA_END( args );
#ifdef __DEBUG_TO_FILE__        
    RFileLogger logger;
    TInt ret = logger.Connect();
    if (ret==KErrNone)
        {
        logger.CreateLog( KFileLogFolder, KFileLogName, EFileLoggingModeAppend );
        logger.Write(buf);
        }

    logger.Close();
#else
    _LIT( KRDebugFmt, "%S" );
    RDebug::Print( KRDebugFmt, &buf );
#endif
    }

void TLogWriter::Write(TRefByValue<const TDesC8> aText, ... )
    {
    VA_LIST args;
    VA_START( args, aText );

    TIgnoreOverFlow of;
    TBuf8<LDebugLineLength> buf;
    buf.AppendFormatList( aText, args, &of );
    VA_END( args );     
#ifdef __DEBUG_TO_FILE__        
    RFileLogger logger;
    TInt ret = logger.Connect();
    if (ret==KErrNone)
        {
        logger.CreateLog( KFileLogFolder, KFileLogName, EFileLoggingModeAppend );
        logger.Write(buf);
        }

    logger.Close();
#else
    _LIT( KRDebugFmt, "%S" );
    RDebug::Print( KRDebugFmt, &buf );
#endif
    }

void MyExeptionHandler(TExcType e)
{
DEBUG("MyExeptionHandler");
	switch( e)
		{
		case EExcGeneral:
			{
			DEBUG("EExcGeneral");
			break;
			}
		case EExcIntegerDivideByZero:
			{
			DEBUG("EExcIntegerDivideByZero");
			break;
			}
		case EExcSingleStep:
			{
			DEBUG("EExcSingleStep");
			break;
			}
		case EExcBreakPoint:
			{
			DEBUG("EExcBreakPoint");
			break;
			}
		case EExcIntegerOverflow:
			{
			DEBUG("EExcIntegerOverflow");
			break;
			}
		case EExcBoundsCheck:
			{
			DEBUG("EExcBoundsCheck");
			break;
			}
		case EExcInvalidOpCode:
			{
			DEBUG("EExcInvalidOpCode");
			break;
			}
		case EExcDoubleFault:
			{
			DEBUG("EExcDoubleFault");
			break;
			}
		case EExcStackFault:
			{
			DEBUG("EExcStackFault");
			break;
			}
		case EExcAccessViolation:
			{
			DEBUG("EExcAccessViolation");
			break;
			}
		case EExcPrivInstruction:
			{
			DEBUG("EExcPrivInstruction");
			break;
			}
		case EExcAlignment:
			{
			DEBUG("EExcAlignment");
			break;
			}
		case EExcPageFault:
			{
			DEBUG("EExcPageFault");
			break;
			}
		case EExcFloatDenormal:
			{
			DEBUG("EExcFloatDenormal");
			break;
			}
		case EExcFloatDivideByZero:
			{
			DEBUG("EExcFloatDivideByZero");
			break;
			}
		case EExcFloatInexactResult:
			{
			DEBUG("EExcFloatInexactResult");
			break;
			}
		case EExcFloatInvalidOperation:
			{
			DEBUG("EExcFloatInvalidOperation");
			break;
			}
		case EExcFloatOverflow:
			{
			DEBUG("EExcFloatOverflow");
			break;
			}
		case EExcFloatStackCheck:
			{
			DEBUG("EExcFloatStackCheck");
			break;
			}
		case EExcFloatUnderflow:
			{
			DEBUG("EExcFloatUnderflow");
			break;
			}
		case EExcAbort:
			{
			DEBUG("EExcAbort");
			break;
			}
		case EExcKill:
			{
			DEBUG("EExcKill");
			break;
			}
		case EExcUserInterrupt:
			{
			DEBUG("EExcUserInterrupt");
			break;
			}
		case EExcDataAbort:
			{
			DEBUG("EExcDataAbort");
			break;
			}
		case EExcCodeAbort:
			{
			DEBUG("EExcCodeAbort");
			break;
			}
		case EExcMaxNumber:
			{
			DEBUG("EExcMaxNumber");
			break;
			}
		case EExcInvalidVector:
			{
			DEBUG("EExcInvalidVector");
			break;
			}
		}
	//TODO: We can also write the contents of each register into a file in here
}

void InitDEBUG()
	{
	TInt err = User::SetExceptionHandler( MyExeptionHandler, 0xFFF);
	if( err )
		DEBUG("SetExeceptionHandler failed");
	else
		DEBUG("Exeptionhander set!");
	}
