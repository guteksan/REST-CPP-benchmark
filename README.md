# REST-CPP-benchmark

## Introduction
There are many C++ RESTful frameworks available. This document aims to provide a multidimensional comparison of the most popular ones. The chosen frameworks are:

* C++ REST SDK (https://github.com/microsoft/cpprestsdk) v2.10.15
* Crow (https://github.com/ipkn/crow) v0.1
* HttpLib (https://github.com/yhirose/cpp-httplib) v0.5.8
* Pistache (http://pistache.io/) - v0.0.002
* Restbed (https://github.com/Corvusoft/restbed) v4.6
* Restinio (https://stiffstream.com/en/products/restinio.html) v0.6.2

## Evaluation method
Each framework has been used to create a basic REST services. All services are equivalent and contain 3 endpoints:

* ```/test``` - handles POST message (JSON) and sends it back with additional text and status code 200.
* ```/test/status``` - handles GET message and prints "OK".
* ```/test/labels/<string>``` - handles GET message with parameter. 

The ```/test``` endpoint was used for testing performance. There were 5 testing parameters used to test various aspects:

* A number of producer threads P. They represented unique clients attempting to send message to the service. For all tescases P=10.
* A number of consumer threads C. This was the number of threads used by the service. Two values were tested:
    * C=28 (corresponding to the value of ```hardware_concurrency()``` on the machine used to run tests.
    * C=4 - arbitrarily chosen value smaller than P, to test service queueing.
* A number of messages N sent by a producer. Values: 200 and 1000.
* Processing time t necessary to handle the request. Values: 0ms and 200ms.
* Environment. Two values were used:
    * "Local" - clients and the service were located on the same machine
    * "VPN" - clients and the service were located on different machines connected by a VPN. 

A testcase is any combination of above parameters. There could be in total 16 testcases, but testcases for N=200 and t=0ms. were intentionally excluded as not providing any new information.

Services were deployed on Ubuntu 16.04. To simulate clients, a python script ```postInLoop.py``` was used.

## Test results

All services logged the reception of message and response with millisecond precision. The tables contain time (min:sec.ms) from the reception of the first request to the last reply. The best times are marked in green, the worst in red.

![Image](images/rest_all.PNG)

Red ```x``` means that the service run into deadlock and test could not be finished.

## Analysis
1. **Cpp-REST-SDK** apparently has problems to handle situation when C < P.
2. In the *sprint* testcases (Local environment, 0ms delay) the best performance was achieved by **Restinio**. The worst was **Cpp-REST-SDK** followed by **Httplib**. It is worth to notice that for some frameworks the best results were achieved using the limited threadpool. It can be explained that if requests require very little processing, handling of threads themselves becomes noticable.
