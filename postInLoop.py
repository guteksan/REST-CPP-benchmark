import http.client
import argparse
from multiprocessing import Process, Lock

def worker(lock, num, args):
    headers = {'Content-type': 'application/json'}

    dummyRequest = "{ param1: 42, param2: \"test\"}"

    for r in range(int(args.requests)):
        with lock:
            print("#",num,": Sending request: ", dummyRequest)
        conn = http.client.HTTPConnection(args.server, args.port)
        conn.request("POST", args.endpoint, dummyRequest, headers)
        response = conn.getresponse()
        with lock:
            print("#", num, ": Reading response:", response.status," ,",response.read())
        conn.close()

if __name__ == "__main__":
    argp = argparse.ArgumentParser()

    group = argp.add_mutually_exclusive_group()

    argp.add_argument('-s', '--server',
                      help='Address of the composer service', required=True)
    argp.add_argument('-p', '--port',
                      help='Port of the composer service', default=80)
    argp.add_argument('-e', '--endpoint',
                      help='Endpoint to send data', default='')
    group.add_argument('-f', '--filename',
                      help='File of format <filename>_N.txt with input requests in JSON format')
    group.add_argument('-r', '--requests',
                      help ='Number of requests to run if no are specified in the file')
    argp.add_argument('-n', '--number',
                      help='Number of processes to run in parallel', default=1)
    args = argp.parse_args()

    lock = Lock()

    #worker(lock,1,args)
    for n in range(int(args.number)):
        Process(target=worker, args=(lock,n,args)).start()
