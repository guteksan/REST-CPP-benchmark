# REST-CPP-benchmark

C = 28 (hardware_concurrency), P = 10, N = 200, VPN

| Framework | 0 ms |  200 ms |
|----------|:-------------:|------:|
| CPP-REST-SDK | 22.127 | 1:01.523 |
| Crow     | 21.787 | 1:01.479 |
| HttpLib  | 42.821 | 1:22.168 |
| Pistache | 22.971 | 1:01.558 |
| Restbed | 22.703  | 1:02.839 |
| Restinio | 24.401 | 1:01.485 |

C = 28 (hardware_concurrency), P = 10, N = 2000, Local

| Framework | 0 ms |  |
|----------|:-------------:|------:|
| CPP-REST-SDK | 6.852 |  |
| Crow     | 3.032 |  |
| HttpLib  | | |
| Pistache | | |
| Restbed | | |
| Restinio | | |
