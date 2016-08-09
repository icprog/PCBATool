#ifndef WEBSERVICEAPI_H
#define WEBSERVICEAPI_H
#include <string>
#include "stdsoap2.h"
#include "soaph.h"
#include "soapstub.h"

class CWebServiceApi
{
public:
    enum Enum_WebServiceApiMode
    {
        CHECK_SN_NEXT_ROUTE  = 0,
        SN_SAVE_TECHPRO      = 1,
        BIND_ASSEMBLY        = 2,
        GET_MAINTANCE_PARM   = 3
    };

public:
    CWebServiceApi();
    CWebServiceApi(std::string endpoint);
    std::string checkingSNNextRoute (std::string SN);
    std::string SnSaveTechProcess_A(std::string SN,std::string Station,std::string FixtureID,	std::string LineID,std::string result,std::string ErrorCode,std::string OperatorID,std::string DiagVersion,std::string LogfilePath);
    std::string bindAssembly(std::string SN,std::string materialName,std::string value);
    std::string getAssemblyData(std::string SN);
    std::string getMaintanceParm(std::string SN,std::string type,std::string qty);

    void SetURL(std::string& endpoint);
    std::string ParseResult(std::string& result,const std::string& resultItem,int nMode = 0);
    virtual ~CWebServiceApi();


private:
    struct soap clientSOAP;
    // WebService调用对象
    _ns1__checkingSNNextRoute checkingSNNextRouteObject;
    _ns1__SnSaveTechProcess_USCOREA SnSaveTechProcessObject;
    _ns1__bindAssembly bindAssemblyObject;
    _ns1__getAssemblyData getAssemblyDataObject;
    _ns1__getMaintanceParm getMaintanceParmObject;
    /// WebService返回对象
    _ns1__checkingSNNextRouteResponse checkingSNNextRouteResponseObject;
    _ns1__SnSaveTechProcess_USCOREAResponse SnSaveTechProcessResponseObject;
    _ns1__bindAssemblyResponse bindAssemblyResponseObject; 
    _ns1__getAssemblyDataResponse getAssemblyDataResponseObject;
    _ns1__getMaintanceParmResponse getMaintanceParmResponseObject;

public:
    std::string m_soap_endpoint;
};



#endif