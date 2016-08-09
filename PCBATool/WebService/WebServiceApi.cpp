#include "WebServiceApi.h"
#include "BlakeTestServiceHttpBinding.nsmap"

CWebServiceApi::CWebServiceApi()
{
    m_soap_endpoint = "";
    soap_init(&clientSOAP);
    setlocale(LC_ALL,"chs");
    soap_set_mode(&clientSOAP,SOAP_C_MBSTRING);
}

CWebServiceApi::CWebServiceApi(std::string endpoint)
:m_soap_endpoint(endpoint)
{
    soap_init(&clientSOAP);
    setlocale(LC_ALL,"chs");
    soap_set_mode(&clientSOAP,SOAP_C_MBSTRING);
}


CWebServiceApi::~CWebServiceApi()
{
    // 关闭SOAP
    soap_destroy(&clientSOAP);
    soap_end(&clientSOAP);
    soap_done(&clientSOAP);
}

std::string CWebServiceApi::checkingSNNextRoute(std::string SN)
{
    std::string strResult = "";
    int nResult;
    checkingSNNextRouteObject.in0 = &SN;

    nResult=soap_call___ns1__checkingSNNextRoute(&clientSOAP,m_soap_endpoint.c_str(),NULL,&checkingSNNextRouteObject,checkingSNNextRouteResponseObject);
    //if(SOAP_OK == nResult)
    //{
    //    /// 输出返回结果
    //    strResult.append(*checkingSNNextRouteResponseObject.out);
    //}
    //else
    //{
    //    return strResult;
    //}
    if (clientSOAP.error)
    {
        //soap_print_fault(&clientSOAP, stderr);
        return strResult;
    }
    else
    {
        /// 输出返回结果
        strResult.append(*checkingSNNextRouteResponseObject.out);
    }
    return strResult;
}

std::string CWebServiceApi::SnSaveTechProcess_A(std::string SN,std::string Station,std::string FixtureID,std::string LineID,std::string result,std::string ErrorCode,std::string OperatorID,std::string DiagVersion,std::string LogfilePath)
{
    std::string strResult = "";
    SnSaveTechProcessObject.in0 = &SN;
    SnSaveTechProcessObject.in1 = &Station;
    SnSaveTechProcessObject.in2 = &FixtureID;
    SnSaveTechProcessObject.in3 = &LineID;
    SnSaveTechProcessObject.in4 = &result;
    SnSaveTechProcessObject.in5 = &ErrorCode;
    SnSaveTechProcessObject.in6 = &OperatorID;
    SnSaveTechProcessObject.in7 = &DiagVersion;
    SnSaveTechProcessObject.in8 = &LogfilePath;	

    soap_call___ns1__SnSaveTechProcess_USCOREA(&clientSOAP,m_soap_endpoint.c_str(),NULL,&SnSaveTechProcessObject,SnSaveTechProcessResponseObject);

    if (clientSOAP.error)
    {
        soap_print_fault(&clientSOAP, stderr);
        return strResult;
    }
    else
    {
        // 输出返回结果
        strResult.append(*SnSaveTechProcessResponseObject.out);
    }
    return strResult;
}

std::string CWebServiceApi::bindAssembly(std::string SN,std::string materialName,std::string value)
{
    std::string strResult = "";
    bindAssemblyObject.in0 = &SN;
    bindAssemblyObject.in1 = &materialName;
    bindAssemblyObject.in2 = &value;

    soap_call___ns1__bindAssembly(&clientSOAP,m_soap_endpoint.c_str(),NULL,&bindAssemblyObject,bindAssemblyResponseObject);
    if (clientSOAP.error)
    {
        //soap_print_fault(&clientSOAP, stderr);
        return strResult;
    }
    else
    {
        /// 输出返回结果
        strResult.append(*bindAssemblyResponseObject.out);
    }
    return strResult;
}

std::string CWebServiceApi::getAssemblyData(std::string SN)
{
    std::string strResult = "";
    getAssemblyDataObject.in0 = &SN;

    soap_call___ns1__getAssemblyData(&clientSOAP,m_soap_endpoint.c_str(),NULL,&getAssemblyDataObject,getAssemblyDataResponseObject);
    if (clientSOAP.error)
    {
        //soap_print_fault(&clientSOAP, stderr);
        return strResult;
    }
    else
    {
        // 输出返回结果
        if (!getAssemblyDataResponseObject.out->empty())
        {
            strResult.append(*getAssemblyDataResponseObject.out);
        }
    }
    return strResult;
}

std::string CWebServiceApi::getMaintanceParm(std::string SN,std::string type,std::string qty)
{
    std::string strResult = "";
    getMaintanceParmObject.in0 = &SN;
    getMaintanceParmObject.in1 = &type;
    getMaintanceParmObject.in2 = &qty;

    soap_call___ns1__getMaintanceParm(&clientSOAP,m_soap_endpoint.c_str(),NULL,&getMaintanceParmObject,getMaintanceParmResponseObject);
    if (clientSOAP.error)
    {
        //soap_print_fault(&clientSOAP, stderr);
        return strResult;
    }
    else
    {
        // 输出返回结果
        if (!getMaintanceParmResponseObject.out->empty())
        {
            strResult.append(*getMaintanceParmResponseObject.out);
        }
        //strResult.append(*getAssemblyDataResponseObject.out);
    }

    return strResult;
}

void CWebServiceApi::SetURL(std::string& endpoint)
{
    m_soap_endpoint = endpoint;
}
std::string CWebServiceApi::ParseResult(std::string& result,const std::string& resultItem,int nMode)
{
    std::string::size_type  posl1,posl2,posl3,posr;
    std::string strResult = "";
    if (result.empty()||resultItem.empty())
    {
        return strResult;
    }
    posr = result.find(resultItem);
    if(std::string::npos != posr)
    {
        posl1 = result.find_first_of(':',posr);
        posl2 = result.find_first_of('\"',posl1);
        posl3 = result.find_first_of('\"',posl2+1);

        strResult = result.substr(posl2+1,posl3-posl2-1);//结果字符串结构："sn":"0425115006699","result":"OKOK"
    }
    //switch(nMode)
    //{
    //	case CHECK_SN_NEXT_ROUTE:
    //	{

    //	}
    //	break;
    //	case SN_SAVE_TECHPRO:
    //	{
    //	
    //	}
    //	break;
    //	case BIND_ASSEMBLY:
    //	{
    //	
    //	}
    //	break;
    //	case GET_MAINTANCE_PARM:
    //	{
    //	
    //	}
    //	break;

    //}
    return strResult;
}
