#include "soapH.h"
#include "soapAPI.nsmap"
int main(int argc, char**argv)
{
	int nPort = 8080;
	struct soap fun_soap;
	soap_init(&fun_soap);
	int nMaster = soap_bind(&fun_soap, NULL, nPort, 100);
	if (nMaster < 0)
	{
		soap_print_fault(&fun_soap, stderr);
		exit(-1);
	}
	fprintf(stderr, "Socket connection successful : master socket = %d\n", nMaster);
	while (true)
	{
		int nSlave = (int)soap_accept(&fun_soap);
		if (nSlave < 0)
		{
			soap_print_fault(&fun_soap, stderr);
			exit(-1);
		}


		fprintf(stderr, "Socket connection successful : slave socket = %d\n", nSlave);


		soap_serve(&fun_soap);
		soap_end(&fun_soap);
	}
	return 0;
}
/*加法的具体实现*/
int ns__add(struct soap *soap, int num1, int num2, int* result)
{
	if (NULL == result)
	{
		printf("Error:The third argument should not be NULL!\n");
		return SOAP_ERR;
	}
	else
	{
		(*result) = num1 + num2;
		return SOAP_OK;
	}
	return SOAP_OK;
}


/*减法的具体实现*/
int ns__sub(struct soap *soap, int num1, int num2, int* result)
{
	if (NULL == result)
	{
		printf("Error:The third argument should not be NULL!\n");
		return SOAP_ERR;
	}
	else
	{
		(*result) = num1 - num2;
		return SOAP_OK;
	}
	return SOAP_OK;
}


/*乘法的具体实现*/
int ns__mult(struct soap *soap, int num1, int num2, int *result)
{
	if (NULL == result)
	{
		printf("Error:The third argument should not be NULL!\n");
		return SOAP_ERR;
	}
	else
	{
		(*result) = num1 * num2;
		return SOAP_OK;
	}
	return SOAP_OK;
}

/*除法的具体实现*/
int ns__divid(struct soap *soap, int num1, int num2, int *result)
{
	if (NULL == result || 0 == num2)
	{
		printf("Error:The second argument is 0 or The third argument is NULL!\n");
		return SOAP_ERR;
	}
	else
	{
		(*result) = num1 / num2;
		return SOAP_OK;
	}
	return SOAP_OK;
}
