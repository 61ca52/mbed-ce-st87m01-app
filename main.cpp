#include "mbed.h"
#include "cellular_test.h"
#include "cellular_demo_tracing.h"

#if CELLULAR_TEST_NEEDS_CERTIFICATES
static const char ROOT_CA_CERT[] =
"-----BEGIN CERTIFICATE-----\n"
"<add ca here>\n"
"-----END CERTIFICATE-----\n";
#endif

DigitalOut led(LED1);

int main()
{
    printf("\n");
    printf("================================================================================\n");
    printf("              Cellular Test Application (Mbed CE)\n");
    printf("================================================================================\n");
    printf("\n");

    printf("[CONFIGURATION]\n");
    printf("  Test Procedure:   %s\n", CELLULAR_TEST_PROCEDURE_NAME);
    printf("  Ping-Pong Cycles: %d\n", MBED_CONF_CELLULAR_TEST_PING_PONG_CYCLES);
    printf("  Test Payload:     \"%s\"\n", MBED_CONF_CELLULAR_TEST_TEST_PAYLOAD);
    printf("\n");

    trace_open();

    NetworkInterface *net = CellularInterface::get_default_instance();
    if (!net) {
        printf("FATAL: Failed to get default cellular interface\n");
        trace_close();
        return -1;
    }

    cellular_test::ITest *test = nullptr;

#if CELLULAR_TEST_IS_UDP || CELLULAR_TEST_IS_TCP
    test = new cellular_test::EchoTest(*net);

#elif CELLULAR_TEST_IS_TLS || CELLULAR_TEST_IS_FWTLS
    cellular_test::TlsServerConfig tls_config;
    tls_config.root_ca_cert = ROOT_CA_CERT;
    test = cellular_test::create_tls_test(*net, tls_config);

#elif CELLULAR_TEST_IS_DTLS || CELLULAR_TEST_IS_FWDTLS
    cellular_test::DtlsServerConfig dtls_config;
    dtls_config.root_ca_cert = ROOT_CA_CERT;
    test = cellular_test::create_dtls_test(*net, dtls_config);

#elif CELLULAR_TEST_IS_HTTP
    test = new cellular_test::HttpTest(*net, cellular_test::HttpServerConfig());

#elif CELLULAR_TEST_IS_HTTPS
    cellular_test::HttpServerConfig https_config;
    https_config.root_ca_cert = ROOT_CA_CERT;
    test = cellular_test::create_http_test(*net, https_config);
#endif

    if (!test) {
        printf("FATAL: Failed to create test for procedure %d\n", CELLULAR_TEST_PROCEDURE);
        trace_close();
        return -1;
    }

    printf("Running: %s\n", test->name());
    bool success = test->run();
    test->print_results();

    delete test;
    trace_close();

    printf("\n");
    if (success) {
        printf("=== TEST PASSED ===\n");
        return 0;
    } else {
        printf("=== TEST FAILED ===\n");
        return 1;
    }
}
