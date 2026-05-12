# Mbed CE ST87M01 EVKIT Example App

Reference application that drives the ST87M01-1 NB-IoT modem (EVKITST87M01)
through the
[`cellular_test`](https://github.com/61ca52/mbed-ce-cellular-test) library to
exercise UDP, TCP, TLS, DTLS and HTTP(S) end-to-end against a configurable
echo server.

- **MCU target:** `NUCLEO_L4A6ZG` (default), also supported: `NUCLEO_L496ZG`
- **Modem driver:** [`st87m01/`](https://github.com/61ca52/mbed-ce-st87m01) (Mbed CE cellular framework)
- **Mode:** AT sockets, module-IP (PPP disabled)

## Build

```powershell
cmake -S . -B build/NUCLEO_L4A6ZG-Debug -G Ninja `
      -DMBED_BOARD=NUCLEO_L4A6ZG -DCMAKE_BUILD_TYPE=Debug
cmake --build build/NUCLEO_L4A6ZG-Debug --target MbedCE_ST87M01_EVAL_KIT_APP
```

In VS Code: run task **Build MbedCE_ST87M01_EVAL_KIT_APP**, debug with
**Debug MbedCE_ST87M01_EVAL_KIT_APP NUCLEO_L4A6ZG Debug** (uses
`ST-LINK_gdbserver` on `127.0.0.1:23331`, hex auto-loaded).

## Operator configuration

Patch `mbed_app.json5` under `target_overrides."*"` for the SIM in use.

### 1NCE (default)

```json5
"nsapi.default-cellular-plmn": "\"26201\"",
"nsapi.default-cellular-apn": "\"iot.1nce.net\"",
"st87m01.provide-default": true,
"st87m01.bands": "\"8\"",
```

### Vodafone

```json5
"nsapi.default-cellular-plmn": "\"26202\"",
"nsapi.default-cellular-apn": "\"lpwa.vodafone.iot\"",
"st87m01.provide-default": true,
```

### Proximus (BE)

```json5
"nsapi.default-cellular-apn": "\"m2minternet.proximus.be\"",
"st87m01.provide-default": true,
```

## Selecting a test procedure

`cellular-test.test-procedure` selects exactly one path; only that one
compiles in to keep flash low.

| value | path         |
|------:|--------------|
| 0     | UDP echo     |
| 1     | TCP echo     |
| 2     | TLS echo     |
| 3     | fw-TLS echo  |
| 4     | DTLS echo    |
| 5     | fw-DTLS echo |
| 6     | HTTP GET     |
| 7     | HTTPS GET    |

Echo-server endpoints, ports, certificate-verify mode, ping-pong cycles
and timeouts live under the `cellular-test.*` block in `mbed_app.json5`.
See [`cellular_test`](https://github.com/61ca52/mbed-ce-cellular-test) for
the full parameter list.

## Layout

```text
.
├── main.cpp                 // app entry; calls cellular_test
├── mbed_app.json5           // per-target / per-operator config
├── cellular_test/           // protocol echo-test library
├── st87m01/                 // ST ST87M01-1 NB-IoT modem driver
└── mbed-os/                 // Mbed OS sources (read-only)
```
