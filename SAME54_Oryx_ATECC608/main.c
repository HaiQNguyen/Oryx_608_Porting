#include <atmel_start.h>

#include "same54.h"
#include "same54_xplained_pro.h"
#include "core/net.h"
#include "drivers/mac/same54_eth_driver.h"
#include "drivers/phy/ksz8091_driver.h"
#include "dhcp/dhcp_client.h"
#include "ipv6/slaac.h"
#include "mqtt/mqtt_client.h"
#include "hardware/same54_crypto.h"
#include "rng/yarrow.h"
#include "debug.h"

#include "cryptoauthlib.h"
#include "atca_host.h"
#include "tng_atca.h"
#include "tng_atcacert_client.h"
#include "atcacert_pem.h"
#include "atcacert\atcacert_host_hw.h"
#include "tngtn_cert_def_1_signer.h"
#include "tngtn_cert_def_2_device.h"
#include "lib\tls\atcatls.h"
#include "lib\tls\atcatls_cfg.h"


/* Define section ---------------------------------------------------*/


//Ethernet interface configuration
#define APP_IF_NAME "eth0"
#define APP_HOST_NAME "mqtt-client-demo"
#define APP_MAC_ADDR "00-AB-CD-EF-54-20"

#define APP_USE_DHCP_CLIENT ENABLED
#define APP_IPV4_HOST_ADDR "192.168.0.20"
#define APP_IPV4_SUBNET_MASK "255.255.255.0"
#define APP_IPV4_DEFAULT_GATEWAY "192.168.0.254"
#define APP_IPV4_PRIMARY_DNS "8.8.8.8"
#define APP_IPV4_SECONDARY_DNS "8.8.4.4"

#define APP_USE_SLAAC ENABLED
#define APP_IPV6_LINK_LOCAL_ADDR "fe80::5420"
#define APP_IPV6_PREFIX "2001:db8::"
#define APP_IPV6_PREFIX_LENGTH 64
#define APP_IPV6_GLOBAL_ADDR "2001:db8::5420"
#define APP_IPV6_ROUTER "fe80::1"
#define APP_IPV6_PRIMARY_DNS "2001:4860:4860::8888"
#define APP_IPV6_SECONDARY_DNS "2001:4860:4860::8844"


/* Define section ---------------------------------------------------*/
#define CHECK_STATUS(s)										\
if(s != ATCA_SUCCESS) {										\
	printf("status code: 0x%x\r\n", s);						\
	printf("Error: Line %d in %s\r\n", __LINE__, __FILE__); \
	while(1);												\
}



//MQTT server name
//#define APP_SERVER_NAME "iot.eclipse.org"
//#define APP_SERVER_NAME "test.mosquitto.org"
#define APP_SERVER_NAME "a3h7mvn22mrdzs-ats.iot.eu-central-1.amazonaws.com"

//MQTT server port
//#define APP_SERVER_PORT 1883   //MQTT over TCP
#define APP_SERVER_PORT 8883 //MQTT over TLS
//#define APP_SERVER_PORT 8884 //MQTT over TLS Mosquitto
//#define APP_SERVER_PORT 80   //MQTT over WebSocket
//#define APP_SERVER_PORT 443  //MQTT over secure WebSocket

//URI (for MQTT over WebSocket only)
#define APP_SERVER_URI "a3h7mvn22mrdzs-ats.iot.eu-central-1.amazonaws.com"


/* Local variable section --------------------------------------------*/
ATCAIfaceCfg cfg_ateccx08a_i2c_device = {
	.iface_type				= ATCA_I2C_IFACE,
	.devtype				= ATECC608A,
	.atcai2c.slave_address	= 0x6A,
	.atcai2c.bus			= 1,
	.atcai2c.baud			= 400000,
	.wake_delay				= 800,
	.rx_retries				= 20,
	.cfg_data              = &I2C_0
};

volatile ATCA_STATUS status;
tng_type_t t = TNGTYPE_22;
uint8_t serial_number[ATCA_SERIAL_NUM_SIZE];
char_t certChain[2500];


//List of trusted CA certificates
char_t trustedCaList[] =
"-----BEGIN CERTIFICATE-----"
"MIIBtjCCAVugAwIBAgITBmyf1XSXNmY/Owua2eiedgPySjAKBggqhkjOPQQDAjA5"
"MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6b24g"
"Um9vdCBDQSAzMB4XDTE1MDUyNjAwMDAwMFoXDTQwMDUyNjAwMDAwMFowOTELMAkG"
"A1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJvb3Qg"
"Q0EgMzBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABCmXp8ZBf8ANm+gBG1bG8lKl"
"ui2yEujSLtf6ycXYqm0fc4E7O5hrOXwzpcVOho6AF2hiRVd9RFgdszflZwjrZt6j"
"QjBAMA8GA1UdEwEB/wQFMAMBAf8wDgYDVR0PAQH/BAQDAgGGMB0GA1UdDgQWBBSr"
"ttvXBp43rDCGB5Fwx5zEGbF4wDAKBggqhkjOPQQDAgNJADBGAiEA4IWSoxe3jfkr"
"BqWTrBqYaGFy+uGh0PsceGCmQ5nFuMQCIQCcAu/xlJyzlvnrxir4tiz+OpAUFteM"
"YyRIHN8wfdVoOw=="
"-----END CERTIFICATE-----"
"-----BEGIN CERTIFICATE-----"
"MIIB8TCCAZegAwIBAgIQd9NtlW7IrmIF5Y46y5hagTAKBggqhkjOPQQDAjBPMSEw"
"HwYDVQQKDBhNaWNyb2NoaXAgVGVjaG5vbG9neSBJbmMxKjAoBgNVBAMMIUNyeXB0"
"byBBdXRoZW50aWNhdGlvbiBSb290IENBIDAwMjAgFw0xODExMDgxOTEyMTlaGA8y"
"MDU4MTEwODE5MTIxOVowTzEhMB8GA1UECgwYTWljcm9jaGlwIFRlY2hub2xvZ3kg"
"SW5jMSowKAYDVQQDDCFDcnlwdG8gQXV0aGVudGljYXRpb24gUm9vdCBDQSAwMDIw"
"WTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAS9VOZt44dUhABrU64VgNUKoGnnit9V"
"eNhc4tVN1bgwKWv/3W5vclb72Z7xoRaxHTOtSRA6oYWHOdz65DfhnWNOo1MwUTAd"
"BgNVHQ4EFgQUeu19bca3eJ2yOAGl6EqMsKQOKowwHwYDVR0jBBgwFoAUeu19bca3"
"eJ2yOAGl6EqMsKQOKowwDwYDVR0TAQH/BAUwAwEB/zAKBggqhkjOPQQDAgNIADBF"
"AiEAodxjRZDsgZ7h3luBEmVRrdTCxPjllSgu4EvnaOx8AnMCID5rp06eTArWjCSw"
"+y7nk9LmvpRlyhXQ6lvIf1V5mVyt"
"-----END CERTIFICATE-----";

#if 1
char_t aws_pri_key[] = 
"-----BEGIN RSA PRIVATE KEY-----"
"MIIEogIBAAKCAQEA1j/l5rSZfWuF1zh3srCjdm6cTS1ZorcoIIXph57bVjUN7uRZ"
"X6m+3LeUeQ10r3FUENamXty6OK6vspny2xElumINopLclQ69AnAsTSgVZJxASffi"
"ITMxX6l1kpmmmIeYudmg7jyBqRdkQRToggjdd+LsoK6H6fSzacxfY9asEZkeMBn5"
"vTDFtnYKfOSPvNAuAq/UjZRfc2hEX3FNtzvH4i5YWjTXs5ePZ7eh1QrTllwcLc9V"
"aXdfAWB9x5Vyh5qnoSDLT7G4C1R2JpXCPdCiYnrFheXMx5lEFD5ufiRtcNB7WXsn"
"DHa+yT98k+qTndz/YJjp0uT2dFA/LrafDVglMwIDAQABAoIBAGTJnm9PWj1kDYxX"
"ZgfLjLo0ApdT1Cz1mIzkMh24n1oIj0toZJraEY1nVxMzP0chOvjI4W8Syw/LLaAJ"
"R/PCN0tcwSlPiTEdw9CX0F9jkdzInH2vfNM9b+aeo06ZtYNflnsnY8tu71gKRwFO"
"wqoZXYX+XG6ibBGlKZmFqQIghMWpszaWWarAX6YtsJqR3upOIvudqPKUhJoNlXSJ"
"1vQspZ1tlAgJhCua+hmzcJmVRNhUK4Cd95RS2RVbqtpbnX8mHpgmmaqESP3P/Ixo"
"KJ1Q1r/SqLHGY4YcAku/yGwESPTRbiws0PE+hMNppD3uKxQx5nF5VBtkbdOLZNm/"
"CvaLI4ECgYEA/fCO/vkFhJZYxMJVRpkUhxZVU2N4i15FabDIzvOjRaHE4xgyN4iT"
"mGo/KQIrBoC6yNaVgCARJM+BH4w48J9nZ/8wWs8Yh/+yzR00FtCNhoSpfNgioZTi"
"3lvaCB5fv2633ni5XqQXQ0gk1X7eVYUV4jh5zDLa1cOoUibJydf7aSECgYEA1/zm"
"3XZH5gOFsSRX+K0dm3Mh2ksYVMRbcGUpGwVlk4XA8wLB1tiiXeH5DxxDNVCLSgi+"
"o6l36z4SlTwWk2NYhGrX3MFiLA8T4bJ4XPqPjR9BOo8c8/3bC8tXIRtsfMZ5G+ji"
"42kXcLoQPilWomr5rXH+BWOmvXZWg52XET81n9MCgYBJnc5mnlx6JnfPeqLsF366"
"9r9/sCuHA6pNzIE6dakUi6QfTalpLf/TxKFQyx8cAH+lr4ehoCo4KKu/MJBlOWDp"
"5jHncXgkHP2BtwCdyJHuuFDGL4ZCn33xhmE7z0pknb1SEl9yz1ErISQEfIYJ9s30"
"SfkNOIT2SYVnY3VGPYXV4QKBgB8k3n1Mku190IdMgErhW4WQwm7hqS5/6dd5HsZA"
"rvbosHvaB+1c63yyGuGs55wGcy9Ht4WN6DbJipBuS6sRBjCB34J7eBZ2Th2bSHPP"
"xWdJ/HPfMwOTtUQsG/IwLCKJ0+jMWfsEqlac3b644z8kQcBa0ZAhYGetaqgrzx6z"
"iJ31AoGAfppHsi+slF43MPxi+sAnTu8xzWNLrhMamFsKlHJRRvl4cy6jHFi2lKen"
"6CQh9JSmgoSS89zVNZW2SizVx45M5Jkqgllj8m9rKrnaZ/xSn3dT5FEvePVp5e4Q"
"JxKtuOSdd+xZFjkUkz1O0LY5ej7Dn5Lmpk/FRlpCvMWURVHjm90="
"-----END RSA PRIVATE KEY-----";

char_t aws_cert[] =
"-----BEGIN CERTIFICATE-----"
"MIIBtjCCAVugAwIBAgITBmyf1XSXNmY/Owua2eiedgPySjAKBggqhkjOPQQDAjA5"
"MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6b24g"
"Um9vdCBDQSAzMB4XDTE1MDUyNjAwMDAwMFoXDTQwMDUyNjAwMDAwMFowOTELMAkG"
"A1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJvb3Qg"
"Q0EgMzBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABCmXp8ZBf8ANm+gBG1bG8lKl"
"ui2yEujSLtf6ycXYqm0fc4E7O5hrOXwzpcVOho6AF2hiRVd9RFgdszflZwjrZt6j"
"QjBAMA8GA1UdEwEB/wQFMAMBAf8wDgYDVR0PAQH/BAQDAgGGMB0GA1UdDgQWBBSr"
"ttvXBp43rDCGB5Fwx5zEGbF4wDAKBggqhkjOPQQDAgNJADBGAiEA4IWSoxe3jfkr"
"BqWTrBqYaGFy+uGh0PsceGCmQ5nFuMQCIQCcAu/xlJyzlvnrxir4tiz+OpAUFteM"
"YyRIHN8wfdVoOw=="
"-----END CERTIFICATE-----"
"-----BEGIN CERTIFICATE-----"
"MIIDWjCCAkKgAwIBAgIVAJwyjCGkn+UJ77erQNSIDUXZq4sHMA0GCSqGSIb3DQEB"
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t"
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xOTA4MDEyMjMx"
"MzdaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh"
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDWP+XmtJl9a4XXOHey"
"sKN2bpxNLVmitygghemHnttWNQ3u5Flfqb7ct5R5DXSvcVQQ1qZe3Lo4rq+ymfLb"
"ESW6Yg2iktyVDr0CcCxNKBVknEBJ9+IhMzFfqXWSmaaYh5i52aDuPIGpF2RBFOiC"
"CN134uygrofp9LNpzF9j1qwRmR4wGfm9MMW2dgp85I+80C4Cr9SNlF9zaERfcU23"
"O8fiLlhaNNezl49nt6HVCtOWXBwtz1Vpd18BYH3HlXKHmqehIMtPsbgLVHYmlcI9"
"0KJiesWF5czHmUQUPm5+JG1w0HtZeycMdr7JP3yT6pOd3P9gmOnS5PZ0UD8utp8N"
"WCUzAgMBAAGjYDBeMB8GA1UdIwQYMBaAFBSrVZWWsOtAmgy0ojFNIg2jTKX/MB0G"
"A1UdDgQWBBRI3licNixsYjasbu8Gzsbt51UjfjAMBgNVHRMBAf8EAjAAMA4GA1Ud"
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAfsPc6DcN0rcPgRwfNY4sdjYE"
"i6HTRaARtdE+D3NWBBfPiE+p80aghFIp/ltx/AddxQTGTls2NGU+YtgCpR2WgcVY"
"Kt9zyN34aLs/Ctdme/F7vGNroFW8WE38S3JX7CBO1Mq2v5F5WuSoBgFIkCKmQDI4"
"V1UuTTvudJdr9XjvykQb+X0Gp0GfAjt/v0qLV39lMFa8ZtHInDKtdt97X8cv67bY"
"eoiZbS8XpGEKYYLAjotWFL6ctcZkyg8Lpieu2uwSqFf+Lt3Ayj27GomfEqjx1ngv"
"rA7cum2zY7ai0ooXvVVVnxYsZu84F8Zm2y8gZmS/imcPewqQIfqPRUdv7+otuw=="
"-----END CERTIFICATE-----";
#endif



//Global variables
DhcpClientSettings dhcpClientSettings;
DhcpClientContext dhcpClientContext;
SlaacSettings slaacSettings;
SlaacContext slaacContext;
MqttClientContext mqttClientContext;
YarrowContext yarrowContext;
MdnsResponderSettings mdnsResponderSettings;
MdnsResponderContext mdnsResponderContext;

uint8_t seed[32];



/* Local function prototype section ----------------------------------*/
void print_bytes(uint8_t * ptr, uint16_t length);
error_t ecc508aEcdhCallback(TlsContext *context);
error_t ecc508aEcdsaSignCallback(TlsContext *context, const uint8_t *digest, 
									size_t digestLength, EcdsaSignature *signature);
error_t ecc508aEcdsaVerifyCallback(TlsContext *context, const uint8_t *digest, 
									size_t digestLength, EcdsaSignature *signature);

/**
 * @brief I/O initialization
 **/
void ioInit(void)
{
   //Enable PORT bus clock (CLK_PORT_APB)
   MCLK->APBBMASK.bit.PORT_ = 1;

   //Configure LED0
   PORT->Group[LED0_GROUP].DIRSET.reg = LED0_MASK;
   PORT->Group[LED0_GROUP].OUTSET.reg = LED0_MASK;

   //Configure SW0 button
   PORT->Group[SW0_GROUP].DIRCLR.reg = SW0_MASK;
   PORT->Group[SW0_GROUP].PINCFG[SW0_PIN].bit.INEN = 1;
}

/**
 * @brief Certificate Store init
 **/
void certStoreInit(void)
{
	volatile ATCA_STATUS status;
	tng_type_t t = TNGTYPE_22;
	
	uint8_t devicePubkey[64];
	uint8_t signerPubkey[64]; 
	uint8_t rootPubkey[64];
	uint8_t cert[800]; 
	uint8_t certsigner[800];
	uint8_t	certdevice[768];
	char cert_pem[1024]; 
	size_t cert_size_pem = sizeof(cert_pem);
	
	memset(cert, 0x00, sizeof(cert) );
	
	bool is_verified;
	
	uint8_t dig[32];
	uint8_t rootSignature[64];
	uint16_t dig_size;
	uint8_t offset = 432+2; //location of R
	
	char device_pem[1024]; 
	size_t device_size_pem = sizeof(device_pem);
	
	status = atcab_read_serial_number((uint8_t*)&serial_number);
	CHECK_STATUS(status);
	
	printf("Serial Number of host\r\n");
	print_bytes((uint8_t*)&serial_number, 9);
	printf("\r\n");
	
	printf("CryptoAuthLib Basics Trust'N'GO MAH22 Certificate Auth\n\r");
	printf("--------Root Certificate Section---------\n");
	size_t cert_size;
	
	status = tng_atcacert_root_cert_size(&cert_size); 
	CHECK_STATUS(status);
	
	status = tng_atcacert_root_cert(cert, &cert_size); 
	CHECK_STATUS(status);
	
	status = tng_atcacert_root_public_key(rootPubkey); 
	CHECK_STATUS(status);
	
	printf("Root cert [%d]\n", cert_size);
	print_bytes(cert, cert_size);
	
	
	status = atcacert_encode_pem_cert(cert, cert_size, cert_pem, &cert_size_pem); 
	CHECK_STATUS(status)
	
	printf("Root cert PEM [%d]\n%s\n", strlen(cert_pem), cert_pem);
	
	//char rootPubkey_pem[176]; size_t rootPubkey_size_pem = sizeof(rootPubkey_pem);
	//status = atcacert_encode_pem_pubkey(rootPubkey, sizeof(rootPubkey), rootPubkey_pem, &rootPubkey_size_pem);
	//printf("Public key [%d]\n%s\n", strlen(rootPubkey_pem), rootPubkey_pem);
	//printf("Public key [%d]\n", sizeof(rootPubkey));
	//print_bytes(rootPubkey, sizeof(rootPubkey));
	
	//printf("TBS Digest\n");
	
	memcpy(rootSignature, cert+offset, 32);//size of R
	offset = 467+2; //location of S
	memcpy(rootSignature+32, cert+offset, 32);//size of S
	status = atcab_sha(411, cert+4, NULL); 
	CHECK_STATUS(status);
	status = atcab_verify_extern(dig, rootSignature, rootPubkey, &is_verified);
	//status = atcab_verify(VERIFY_MODE_EXTERNAL | VERIFY_MODE_SOURCE_TEMPKEY, NULL, rootSignature,rootPubkey, NULL, NULL);
	printf("--------Root validated: %s--------\n\n", (is_verified == TRUE)? "OK" : "FAILED");
	
	printf("--------Signer Certificate Section---------\n");
	size_t signer_size;
	status = tng_atcacert_max_signer_cert_size(&signer_size); 
	CHECK_STATUS(status);
	
	status = tng_atcacert_read_signer_cert(certsigner, &signer_size); 
	CHECK_STATUS(status);
	
	status = tng_atcacert_signer_public_key(signerPubkey, certsigner); 
	CHECK_STATUS(status);
	
	char signer_pem[1024]; size_t signer_size_pem = sizeof(signer_pem);
	status = atcacert_encode_pem_cert(certsigner, signer_size, signer_pem, &signer_size_pem); CHECK_STATUS(status)
	printf("Signer cert PEM [%d]\n%s\n", strlen(signer_pem), signer_pem);
	
	//printf("Public key [%d]\n", sizeof(signerPubkey));
	//print_bytes(signerPubkey, sizeof(signerPubkey));
	status = atcacert_verify_cert_hw(&g_tngtn_cert_def_1_signer, certsigner, signer_size, rootPubkey); CHECK_STATUS(status);
	printf("--------Signer validated: %s--------\n\n", (status == ATCA_SUCCESS)? "OK" : "FAILED");
	
	printf("--------Device Certificate Section---------\n");
	size_t device_size;
	status = tng_atcacert_max_device_cert_size(&device_size); 
	CHECK_STATUS(status);
	status = tng_atcacert_read_device_cert(certdevice, &device_size, certsigner); 
	CHECK_STATUS(status);
	status = tng_atcacert_device_public_key(devicePubkey, certdevice); 
	CHECK_STATUS(status);
	status = atcacert_encode_pem_cert(certdevice, device_size, device_pem, &device_size_pem);
	 CHECK_STATUS(status)
	printf("Device cert PEM [%d]\n%s\n", strlen(device_pem), device_pem);
	//printf("Public key [%d]\n", sizeof(devicePubkey));
	//print_bytes(devicePubkey, sizeof(devicePubkey));
	status = atcacert_verify_cert_hw(&g_tngtn_cert_def_2_device, certdevice, device_size, signerPubkey); 
	CHECK_STATUS(status);
	printf("--------Device validated: %s--------\n\n", (status == ATCA_SUCCESS)? "OK" : "FAILED");   
	
	//TODO
	//memcpy(certChain, certsigner, signer_size);
	//memcpy(certChain+signer_size, certdevice, device_size);
	strcpy(certChain, device_pem);
	strcat(certChain, signer_pem);
	strcat(certChain, cert_pem);

	printf("Cert Chain PEM [%d]\n%s\n", strlen(certChain), certChain);
}
/**
 * @brief Random data generation callback function
 * @param[out] data Buffer where to store the random data
 * @param[in] length Number of bytes that are required
 * @return Error code
 **/

error_t webSocketRngCallback(uint8_t *data, size_t length)
{
   //Generate some random data
   return yarrowRead(&yarrowContext, data, length);
}

/**
 * @brief TLS initialization callback
 * @param[in] context Pointer to the MQTT client context
 * @param[in] tlsContext Pointer to the TLS context
 * @return Error code
 **/

error_t mqttTestTlsInitCallback(MqttClientContext *context,
   TlsContext *tlsContext)
{
	
   error_t error;
	//Debug message
	TRACE_INFO("MQTT: TLS initialization callback\r\n");

	//Set the PRNG algorithm to be used
	error = tlsSetPrng(tlsContext, YARROW_PRNG_ALGO, &yarrowContext);
	//Any error to report?
	if(error)
	return error;
	  
#if (TLS_ECC_CALLBACK_SUPPORT == 1)
	//Register ECDH key agreement callback function
	error = tlsSetEcdhCallback(tlsContext, ecc508aEcdhCallback);
	//Any error to report?
	if(error)
		return error;

	//Register ECDSA signature generation callback function
	error = tlsSetEcdsaSignCallback(tlsContext, ecc508aEcdsaSignCallback);
	//Any error to report?
	if(error)
		return error;

	//Register ECDSA signature verification callback function
	error = tlsSetEcdsaVerifyCallback(tlsContext, ecc508aEcdsaVerifyCallback);
	//Any error to report?
	if(error)
		return error;

	//Set the fully qualified domain name of the server
	error = tlsSetServerName(tlsContext, APP_SERVER_NAME);
	//Any error to report?
	if(error)
		return error;

	//Import the list of trusted CA certificates
	error = tlsSetTrustedCaList(tlsContext, trustedCaList, strlen(trustedCaList));
	//Any error to report?
	if(error)
		return error;

	//TODO
	//Import the client's certificate
	error = tlsAddCertificate(tlsContext, aws_cert, strlen(aws_cert), aws_pri_key, strlen(aws_pri_key));
	//Any error to report?
	if(error){
		TRACE_INFO("Err: add certchain [%d]\n", error);
		return error;
	}
#else
	//Set the fully qualified domain name of the server
	error = tlsSetServerName(tlsContext, APP_SERVER_NAME);
	//Any error to report?
	if(error)
		return error;

	//Import the list of trusted CA certificates
	error = tlsSetTrustedCaList(tlsContext, trustedCaList, strlen(trustedCaList));
	//Any error to report?
	if(error)
		return error;
#endif

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Publish callback function
 * @param[in] context Pointer to the MQTT client context
 * @param[in] topic Topic name
 * @param[in] message Message payload
 * @param[in] length Length of the message payload
 * @param[in] dup Duplicate delivery of the PUBLISH packet
 * @param[in] qos QoS level used to publish the message
 * @param[in] retain This flag specifies if the message is to be retained
 * @param[in] packetId Packet identifier
 **/

void mqttTestPublishCallback(MqttClientContext *context,
   const char_t *topic, const uint8_t *message, size_t length,
   bool_t dup, MqttQosLevel qos, bool_t retain, uint16_t packetId)
{
   //Debug message
   TRACE_INFO("PUBLISH packet received...\r\n");
   TRACE_INFO("  Dup: %u\r\n", dup);
   TRACE_INFO("  QoS: %u\r\n", qos);
   TRACE_INFO("  Retain: %u\r\n", retain);
   TRACE_INFO("  Packet Identifier: %u\r\n", packetId);
   TRACE_INFO("  Topic: %s\r\n", topic);
   TRACE_INFO("  Message (%" PRIuSIZE " bytes):\r\n", length);
   TRACE_INFO_ARRAY("    ", message, length);

   //Check topic name
   if(!strcmp(topic, "board/leds/1"))
   {
      if(length == 6 && !strncasecmp((char_t *) message, "toggle", 6))
      {
         //Toggle user LED
         PORT->Group[LED0_GROUP].OUTTGL.reg = LED0_MASK;
      }
      else if(length == 2 && !strncasecmp((char_t *) message, "on", 2))
      {
         //Set user LED
         PORT->Group[LED0_GROUP].OUTCLR.reg = LED0_MASK;
      }
      else
      {
         //Clear user LED
         PORT->Group[LED0_GROUP].OUTSET.reg = LED0_MASK;
      }
   }
}

/**
 * @brief Establish MQTT connection
 **/

error_t mqttTestConnect(void)
{
   error_t error;
   IpAddr ipAddr;
   MqttClientCallbacks mqttClientCallbacks;

   //Debug message
   TRACE_INFO("\r\n\r\nResolving server name...\r\n");

   //Resolve MQTT server name
   error = getHostByName(NULL, APP_SERVER_NAME, &ipAddr, 0);
   //Any error to report?
   if(error)
      return error;

#if (APP_SERVER_PORT == 80 || APP_SERVER_PORT == 443)
   //Register RNG callback
   webSocketRegisterRandCallback(webSocketRngCallback);
#endif

   //Initialize MQTT client callbacks
   mqttClientInitCallbacks(&mqttClientCallbacks);

   //Attach application-specific callback functions
   mqttClientCallbacks.publishCallback = mqttTestPublishCallback;
#if (APP_SERVER_PORT == 8883 || APP_SERVER_PORT == 8884 || APP_SERVER_PORT == 443)
   mqttClientCallbacks.tlsInitCallback = mqttTestTlsInitCallback;
#endif

   //Register MQTT client callbacks
   mqttClientRegisterCallbacks(&mqttClientContext, &mqttClientCallbacks);

   //Set the MQTT version to be used
   mqttClientSetVersion(&mqttClientContext, MQTT_VERSION_3_1);

#if (APP_SERVER_PORT == 1883)
   //MQTT over TCP
   mqttClientSetTransportProtocol(&mqttClientContext, MQTT_TRANSPORT_PROTOCOL_TCP);
#elif (APP_SERVER_PORT == 8883 || APP_SERVER_PORT == 8884)
   //MQTT over TLS
   mqttClientSetTransportProtocol(&mqttClientContext, MQTT_TRANSPORT_PROTOCOL_TLS);
#elif (APP_SERVER_PORT == 80)
   //MQTT over WebSocket
   mqttClientSetTransportProtocol(&mqttClientContext, MQTT_TRANSPORT_PROTOCOL_WS);
#elif (APP_SERVER_PORT == 443)
   //MQTT over secure WebSocket
   mqttClientSetTransportProtocol(&mqttClientContext, MQTT_TRANSPORT_PROTOCOL_WSS);
#endif

   //Set communication timeout
   mqttClientSetTimeout(&mqttClientContext, 20000);
   //Set keep-alive value
   mqttClientSetKeepAlive(&mqttClientContext, 3600);

#if (APP_SERVER_PORT == 80 || APP_SERVER_PORT == 443)
   //Set the hostname of the resource being requested
   mqttClientSetHost(&mqttClientContext, APP_SERVER_NAME);
   //Set the name of the resource being requested
   mqttClientSetUri(&mqttClientContext, APP_SERVER_URI);
#endif

   //Set client identifier
   //mqttClientSetIdentifier(&mqttClientContext, "client12345678");

   //Set user name and password
   //mqttClientSetAuthInfo(&mqttClientContext, "username", "password");

   //Set Will message
   mqttClientSetWillMessage(&mqttClientContext, "board/status",
      "offline", 7, MQTT_QOS_LEVEL_0, FALSE);

   //Debug message
   TRACE_INFO("Connecting to MQTT server %s...\r\n", ipAddrToString(&ipAddr, NULL));
	uint8_t retry = 3;
   //Start of exception handling block
   do
   {

	//Establish connection with the MQTT server
	error = mqttClientConnect(&mqttClientContext,
	&ipAddr, APP_SERVER_PORT, TRUE);
	//Any error to report?
	
	if(error){
		TRACE_INFO("MQTT connect error: [%d]\r\n", error);
		break;
	}
		

	   
     

      //Subscribe to the desired topics
      error = mqttClientSubscribe(&mqttClientContext,
         "board/leds/+", MQTT_QOS_LEVEL_1, NULL);
      //Any error to report?
      if(error)
         break;

      //Send PUBLISH packet
      error = mqttClientPublish(&mqttClientContext, "board/status",
         "online", 6, MQTT_QOS_LEVEL_1, TRUE, NULL);
      //Any error to report?
      if(error)
         break;

      //End of exception handling block
   } while(0);

   //Check status code
   if(error)
   {
      //Close connection
      mqttClientClose(&mqttClientContext);
   }

   //Return status code
   return error;
}

/**
 * @brief MQTT test task
 **/

void mqttTestTask(void *param)
{
   error_t error;
   bool_t connectionState;
   uint_t buttonState;
   uint_t prevButtonState;
   char_t buffer[16];

   //Initialize variables
   connectionState = FALSE;
   prevButtonState = 0;

   //Initialize MQTT client context
   mqttClientInit(&mqttClientContext);

   //Endless loop
   while(1)
   {
      //Check connection state
      if(!connectionState)
      {
         //Make sure the link is up
         if(netGetLinkState(&netInterface[0]))
         {
            //Try to connect to the MQTT server
            error = mqttTestConnect();

            //Successful connection?
            if(!error)
            {
               //The MQTT client is connected to the server
               connectionState = TRUE;
            }
            else
            {
               //Delay between subsequent connection attempts
               osDelayTask(2000);
            }
         }
      }
      else
      {
         //Initialize status code
         error = NO_ERROR;

         //Get SW0 button state
         buttonState = !(PORT->Group[SW0_GROUP].IN.reg & SW0_MASK);

         //Any change detected?
         if(buttonState != prevButtonState)
         {
            if(buttonState)
               strcpy(buffer, "pressed");
            else
               strcpy(buffer, "released");

            //Send PUBLISH packet
            error = mqttClientPublish(&mqttClientContext, "board/buttons/1",
               buffer, strlen(buffer), MQTT_QOS_LEVEL_1, TRUE, NULL);

            //Save current state
            prevButtonState = buttonState;
         }

         //Check status code
         if(!error)
         {
            //Process events
            error = mqttClientTask(&mqttClientContext, 100);
         }

         //Connection to MQTT server lost?
         if(error)
         {
            //Close connection
            mqttClientClose(&mqttClientContext);
            //Update connection state
            connectionState = FALSE;
            //Recovery delay
            osDelayTask(2000);
         }
      }
   }
}


/**
 * @brief LED task
 * @param[in] param Unused parameter
 **/

void ledTask(void *param)
{
   //Endless loop
   while(1)
   {
      PORT->Group[LED0_GROUP].OUTCLR.reg = LED0_MASK;
      osDelayTask(100);
      PORT->Group[LED0_GROUP].OUTSET.reg = LED0_MASK;
      osDelayTask(900);
   }
}



int main(void)
{
	error_t error;
	uint_t i;
	uint32_t value;
	OsTask *task;
	NetInterface *interface;
	MacAddr macAddr;
	#if (APP_USE_DHCP_CLIENT == DISABLED)
	Ipv4Addr ipv4Addr;
	#endif
	#if (APP_USE_SLAAC == DISABLED)
	Ipv6Addr ipv6Addr;
	#endif
	
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	//Initialize kernel
	osInitKernel();
	//Configure debug UART
	debugInit(115200); //TODO I expect the UART Debug is doubled initialize here

	//Start-up message
	TRACE_INFO("\r\n");
	TRACE_INFO("***********************************\r\n");
	TRACE_INFO("*** CycloneTCP MQTT Client Demo ***\r\n");
	TRACE_INFO("***********************************\r\n");
	TRACE_INFO("Copyright: 2010-2019 Oryx Embedded SARL\r\n");
	TRACE_INFO("Compiled: %s %s\r\n", __DATE__, __TIME__);
	TRACE_INFO("Target: SAME54\r\n");
	TRACE_INFO("\r\n");
	
	printf("Authentication in progress\n\r");
	
	status = atcab_init( &cfg_ateccx08a_i2c_device );
	CHECK_STATUS(status);
	
	status = tng_get_type(&t);
	CHECK_STATUS(status);
	
	switch (t){
		case TNGTYPE_22:
		printf("type: 22\r\n");
		break;
		case TNGTYPE_TN:
		printf("type: TN\r\n");
		break;
		case TNGTYPE_UNKNOWN:
		printf("unknown\r\n");
		break;
	}
	printf("ATECC608 init complete\n\r");

	certStoreInit();
	
	//Configure I/Os
	ioInit();
	
	//Initialize hardware cryptography accelerator
	same54CryptoInit();
	

	//Enable TRNG peripheral clock
	MCLK->APBCMASK.reg |= MCLK_APBCMASK_TRNG;
	//Enable TRNG
	TRNG->CTRLA.reg |= TRNG_CTRLA_ENABLE;

	//Generate a random seed
	for(i = 0; i < 32; i += 4)
	{
		//Wait for the TRNG to contain a valid data
		while(!(TRNG->INTFLAG.reg & TRNG_INTFLAG_DATARDY));

		//Get 32-bit random value
		value = TRNG->DATA.reg;

		//Copy random value
		seed[i] = value & 0xFF;
		seed[i + 1] = (value >> 8) & 0xFF;
		seed[i + 2] = (value >> 16) & 0xFF;
		seed[i + 3] = (value >> 24) & 0xFF;
	}

	//PRNG initialization
	error = yarrowInit(&yarrowContext);
	//Any error to report?
	if(error)
	{
		//Debug message
		TRACE_ERROR("Failed to initialize PRNG!\r\n");
	}

	//Properly seed the PRNG
	error = yarrowSeed(&yarrowContext, seed, sizeof(seed));
	//Any error to report?
	if(error)
	{
		//Debug message
		TRACE_ERROR("Failed to seed PRNG!\r\n");
	}

	//TCP/IP stack initialization
	error = netInit();
	//Any error to report?
	if(error)
	{
		//Debug message
		TRACE_ERROR("Failed to initialize TCP/IP stack!\r\n");
	}
	//Configure the first Ethernet interface
	interface = &netInterface[0];

	//Set interface name
	netSetInterfaceName(interface, APP_IF_NAME);
	//Set host name
	netSetHostname(interface, APP_HOST_NAME);
	//Set host MAC address
	macStringToAddr(APP_MAC_ADDR, &macAddr);
	netSetMacAddr(interface, &macAddr);
	//Select the relevant network adapter
	netSetDriver(interface, &same54EthDriver);
	netSetPhyDriver(interface, &ksz8091PhyDriver);

	//Initialize network interface
	error = netConfigInterface(interface);
	//Any error to report?
	if(error)
	{
		//Debug message
		TRACE_ERROR("Failed to configure interface %s!\r\n", interface->name);
	}

#if (IPV4_SUPPORT == ENABLED)
#if (APP_USE_DHCP_CLIENT == ENABLED)
	//Get default settings
	dhcpClientGetDefaultSettings(&dhcpClientSettings);
	//Set the network interface to be configured by DHCP
	dhcpClientSettings.interface = interface;
	//Disable rapid commit option
	dhcpClientSettings.rapidCommit = FALSE;

	//DHCP client initialization
	error = dhcpClientInit(&dhcpClientContext, &dhcpClientSettings);
	//Failed to initialize DHCP client?
	if(error)
	{
		//Debug message
		TRACE_ERROR("Failed to initialize DHCP client!\r\n");
	}

	//Start DHCP client
	error = dhcpClientStart(&dhcpClientContext);
	//Failed to start DHCP client?
	if(error)
	{
		//Debug message
		TRACE_ERROR("Failed to start DHCP client!\r\n");
	}
#else
	//Set IPv4 host address
	ipv4StringToAddr(APP_IPV4_HOST_ADDR, &ipv4Addr);
	ipv4SetHostAddr(interface, ipv4Addr);

	//Set subnet mask
	ipv4StringToAddr(APP_IPV4_SUBNET_MASK, &ipv4Addr);
	ipv4SetSubnetMask(interface, ipv4Addr);

	//Set default gateway
	ipv4StringToAddr(APP_IPV4_DEFAULT_GATEWAY, &ipv4Addr);
	ipv4SetDefaultGateway(interface, ipv4Addr);

	//Set primary and secondary DNS servers
	ipv4StringToAddr(APP_IPV4_PRIMARY_DNS, &ipv4Addr);
	ipv4SetDnsServer(interface, 0, ipv4Addr);
	ipv4StringToAddr(APP_IPV4_SECONDARY_DNS, &ipv4Addr);
	ipv4SetDnsServer(interface, 1, ipv4Addr);
#endif
#endif

#if (MDNS_CLIENT_SUPPORT == ENABLED)
#if (MDNS_RESPONDER_SUPPORT == ENABLED)

	//Get default settings
	mdnsResponderGetDefaultSettings(&mdnsResponderSettings);
	//Underlying network interface
	mdnsResponderSettings.interface = &netInterface[0];

	//mDNS responder initialization
	error = mdnsResponderInit(&mdnsResponderContext, &mdnsResponderSettings);
	//Failed to initialize mDNS responder?
	if(error)
	{
		//Debug message
		TRACE_ERROR("Failed to initialize mDNS responder!\r\n");
	}

	//Set mDNS hostname
	error = mdnsResponderSetHostname(&mdnsResponderContext, "ArrowHostName");
	//Any error to report?
	if(error)
	{
		//Debug message
		TRACE_ERROR("Failed to set hostname!\r\n");
	}

	//Start mDNS responder
	error = mdnsResponderStart(&mdnsResponderContext);
	//Failed to start mDNS responder?
	if(error)
	{
		//Debug message
		TRACE_ERROR("Failed to start mDNS responder!\r\n");
	}
#endif
#endif

#if (IPV6_SUPPORT == ENABLED)
#if (APP_USE_SLAAC == ENABLED)
	//Get default settings
	slaacGetDefaultSettings(&slaacSettings);
	//Set the network interface to be configured
	slaacSettings.interface = interface;

	//SLAAC initialization
	error = slaacInit(&slaacContext, &slaacSettings);
	//Failed to initialize SLAAC?
	if(error)
	{
		//Debug message
		TRACE_ERROR("Failed to initialize SLAAC!\r\n");
	}

	//Start IPv6 address autoconfiguration process
	error = slaacStart(&slaacContext);
	//Failed to start SLAAC process?
	if(error)
	{
		//Debug message
		TRACE_ERROR("Failed to start SLAAC!\r\n");
	}
	#else
	//Set link-local address
	ipv6StringToAddr(APP_IPV6_LINK_LOCAL_ADDR, &ipv6Addr);
	ipv6SetLinkLocalAddr(interface, &ipv6Addr);

	//Set IPv6 prefix
	ipv6StringToAddr(APP_IPV6_PREFIX, &ipv6Addr);
	ipv6SetPrefix(interface, 0, &ipv6Addr, APP_IPV6_PREFIX_LENGTH);

	//Set global address
	ipv6StringToAddr(APP_IPV6_GLOBAL_ADDR, &ipv6Addr);
	ipv6SetGlobalAddr(interface, 0, &ipv6Addr);

	//Set default router
	ipv6StringToAddr(APP_IPV6_ROUTER, &ipv6Addr);
	ipv6SetDefaultRouter(interface, 0, &ipv6Addr);

	//Set primary and secondary DNS servers
	ipv6StringToAddr(APP_IPV6_PRIMARY_DNS, &ipv6Addr);
	ipv6SetDnsServer(interface, 0, &ipv6Addr);
	ipv6StringToAddr(APP_IPV6_SECONDARY_DNS, &ipv6Addr);
	ipv6SetDnsServer(interface, 1, &ipv6Addr);
#endif
#endif


	//Create MQTT test task
	task = osCreateTask("MQTT", mqttTestTask, NULL, 750, OS_TASK_PRIORITY_NORMAL);
	//Failed to create the task?
	if(task == OS_INVALID_HANDLE)
	{
		//Debug message
		TRACE_ERROR("Failed to create task!\r\n");
	}

	//Create a task to blink the LED
	task = osCreateTask("LED", ledTask, NULL, 200, OS_TASK_PRIORITY_NORMAL);
	//Failed to create the task?
	if(task == OS_INVALID_HANDLE)
	{
		//Debug message
		TRACE_ERROR("Failed to create task!\r\n");
	}

	//Start the execution of tasks
	TRACE_INFO("before OSKernelStart\r\n");
	osStartKernel();
	TRACE_INFO("after OSKernelStart\r\n");
	while(1){
	error = 0;
	}
	//This function should never return
	return 0;
}

void print_bytes(uint8_t * ptr, uint16_t length)
{
	
	uint16_t i = 0;
	uint8_t line_count = 0;
	for(;i < length; i++) {
		printf("0x%02x, ",ptr[i]);
		line_count++;
		if(line_count == 8) {
			printf("\r\n");
			line_count = 0;
		}
	}
	
	printf("\r\n");
}


error_t ecc508aEcdhCallback(TlsContext *context)
{
   error_t error;
   uint8_t qa[ATCA_PUB_KEY_SIZE];
   uint8_t qb[ATCA_PUB_KEY_SIZE];
   uint8_t s[ATCA_KEY_SIZE];
   ATCA_STATUS status;

   //Debug message
   TRACE_INFO("ECC608A ECDH callback\r\n");

   //NIST-P256 elliptic curve?
   if(context->ecdhContext.params.type == EC_CURVE_TYPE_SECP_R1 &&
      mpiGetBitLength(&context->ecdhContext.params.p) == 256)
   {
      //Convert the x-coordinate of the peer's public key to an octet string
      error = mpiWriteRaw(&context->ecdhContext.qb.x, qb, 32);

      //Check status code
      if(!error)
      {
         //Convert the y-coordinate of the peer's public key to an octet string
         error = mpiWriteRaw(&context->ecdhContext.qb.y, qb + 32, 32);
      }

      //Check status code
      if(!error)
      {
         //Debug message
         TRACE_DEBUG_ARRAY("  qb: ", qb, ATCA_PUB_KEY_SIZE);

         //Perform ECDH key exchange
         status = atcatls_ecdhe(TLS_SLOT_ECDHE_PRIV, qb, qa, s);

         //Debug message
         TRACE_INFO("atcatls_ecdhe = %u\r\n", status);
         TRACE_DEBUG_ARRAY("  qa: ", qa, ATCA_PUB_KEY_SIZE);
         TRACE_DEBUG_ARRAY("  s: ", s, ATCA_KEY_SIZE);

         //Successful operation?
         if(status == ATCA_SUCCESS)
         {
            //Save the shared secret
            memcpy(context->premasterSecret, s, 32);
            context->premasterSecretLen = 32;

            //Convert the x-coordinate of our public key to an octet string
            error = mpiReadRaw(&context->ecdhContext.qa.x, qa, 32);
            
            //Check status code
            if(!error)
            {
               //Convert the y-coordinate of our public key to an octet string
               error = mpiReadRaw(&context->ecdhContext.qa.y, qa + 32, 32);
            }
         }
         else
         {
            //Failed to perform ECDH key exchange
            error = ERROR_FAILURE;
         }
      }
   }
   else
   {
      //The specified elliptic curve is not supported
      error = ERROR_UNSUPPORTED_ELLIPTIC_CURVE;
   }

   //Return status code
   return error;
}


/**
 * @brief ECDSA signature generation callback function
 **/

error_t ecc508aEcdsaSignCallback(TlsContext *context,
   const uint8_t *digest, size_t digestLength, EcdsaSignature *signature)
{
   error_t error;
   ATCA_STATUS status;
   uint8_t sig[ATCA_SIG_SIZE];

   //Debug message
   TRACE_INFO("ECC608A ECDSA signature generation callback\r\n");
   TRACE_DEBUG("  digest:\r\n");
   TRACE_DEBUG_ARRAY("    ", digest, digestLength);

   //SHA-256 hash algorithm?
   if(context->signHashAlgo == TLS_HASH_ALGO_SHA256 &&
      digestLength == SHA256_DIGEST_SIZE)
   {
      //Generate an ECDSA signature using the client's private key
      status = atcatls_sign(TLS_SLOT_AUTH_PRIV, digest, sig);

      //Debug message
      TRACE_INFO("atcatls_sign = %u\r\n", status);
      TRACE_INFO_ARRAY("  ", sig, ATCA_SIG_SIZE);

      //Successful operation?
      if(status == ATCA_SUCCESS)
      {
         //Convert R to a multiple precision integer
         error = mpiReadRaw(&signature->r, sig, ATCA_SIG_SIZE / 2);

         //Check status code
         if(!error)
         {
            //Convert S to a multiple precision integer
            error = mpiReadRaw(&signature->s,
               sig + ATCA_SIG_SIZE / 2, ATCA_SIG_SIZE / 2);
         }

         //Check status code
         if(!error)
         {
            //Debug message
            TRACE_DEBUG_MPI("r:  ", &signature->r);
            TRACE_DEBUG_MPI("s:  ", &signature->s);
         }
      }
      else
      {
         //Failed to generate ECDSA signature
         error = ERROR_FAILURE;
      }
   }
   else
   {
      //The specified hash algorithm is not supported
      error = ERROR_UNSUPPORTED_HASH_ALGO;
   }

   //Return status code
   return error;
}


/**
 * @brief ECDSA signature verification callback function
 **/

error_t ecc508aEcdsaVerifyCallback(TlsContext *context,
   const uint8_t *digest, size_t digestLength, EcdsaSignature *signature)
{
   error_t error;
   ATCA_STATUS status;
   uint8_t sig[ATCA_SIG_SIZE];
   uint8_t pubKey[ATCA_PUB_KEY_SIZE];
   bool verified;

   //Debug message
   TRACE_INFO("ECC608A ECDSA signature verification callback\r\n");
   TRACE_DEBUG("  digest:\r\n");
   TRACE_DEBUG_ARRAY("    ", digest, digestLength);
   
   //NIST-P256 elliptic curve?
   if(context->peerEcParams.type == EC_CURVE_TYPE_SECP_R1 &&
      mpiGetBitLength(&context->peerEcParams.p) == 256)
   {
      if(digestLength == SHA256_DIGEST_SIZE)
      {
         //Convert R to an octet string
         error = mpiWriteRaw(&signature->r, sig, ATCA_SIG_SIZE / 2);

         //Check status code
         if(!error)
         {
            //Convert S to an octet string
            error = mpiWriteRaw(&signature->s,
               sig + ATCA_SIG_SIZE / 2, ATCA_SIG_SIZE / 2);
         }
         
         //Check status code
         if(!error)
         {
            //Convert the x-coordinate of the public key to an octet string
            error = mpiWriteRaw(&context->peerEcPublicKey.x, pubKey,
               ATCA_PUB_KEY_SIZE / 2);
         }

         //Check status code
         if(!error)
         {
            //Convert the y-coordinate of the public key to an octet string
            error = mpiWriteRaw(&context->peerEcPublicKey.y,
               pubKey + ATCA_PUB_KEY_SIZE / 2, ATCA_PUB_KEY_SIZE / 2);
         }
         
         //Check status code
         if(!error)
         {
            //Verify the ECDSA signature
            status = atcatls_verify(digest, sig, pubKey, &verified);  

            //Debug message
            TRACE_INFO("atcatls_verify = %u,%u\r\n", status, verified);
            TRACE_INFO_ARRAY("  ", sig, ATCA_SIG_SIZE);

            //Failed to verify ECDSA signature?
            if(status != ATCA_SUCCESS || !verified)
            {
               //Report an error
               error = ERROR_FAILURE;
            }
         }
      }
      else
      {
         //The specified hash algorithm is not supported
         error = ERROR_UNSUPPORTED_HASH_ALGO;
      }
   }
   else
   {
      //The specified elliptic curve is not supported
      error = ERROR_UNSUPPORTED_ELLIPTIC_CURVE;
   }

   //Return status code
   return error;
}