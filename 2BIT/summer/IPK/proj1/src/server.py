# Project: IPK Project 1 - HTTP resolver of domain names
# Creates socket server and accepts the GET and POST HTTP requests to resolve domain names, sends the names back to client
# Michal Koval <xkoval17@stud.fit.vutbr.cz>

import socket
import signal
import errno
import sys

HOST = '127.0.0.1'
PORT = 19002 #default value
MSG400 = "HTTP/1.1 400 Bad Request\r\n\r\n"
MSG405 = "HTTP/1.1 405 Method Not Allowed\r\n\r\n"
MSG404 = "HTTP/1.1 404 Not Found\r\n\r\n"
MSG200 = "HTTP/1.1 200 OK\r\n\r\n"

# checks the number and validity of arguments passed to the program
def process_args():
    argc = len(sys.argv)
    if argc > 2:
        print("Too many arguments passed to the program")
        exit(1)
    elif argc == 2:
        try:
            port = int(sys.argv[1])
        except ValueError:
            print("Wrong format of parameter, should be unsigned integer")
            exit(1)
        port_check(port)
        global PORT # editing global port outside of the function scope
        PORT = port

# handles the SIGINT signal to close the server
def handler(signum, frame):
    print("Server closing")
    global s
    s.close() # closing to socket server
    exit(0)

# checks validity of entered port number
def port_check(port):
    if not isinstance(port, int) and port.bit_length() > 16 and port < 0:
        print("Incorrect format of port entered")
        exit(1)

# checks the validity of arguments from request
def is_valid_type(name,n_type):
    # checking addres
    if n_type == "PTR":
        ptr = name.split(".")
        if len(ptr) != 4:
            return False
        for i in range(4):
            ptr_len = len(ptr)
            if ptr_len > 3 and ptr_len < 1:
                return False
            for n in range(ptr_len):
                if not ptr[n].isdigit():
                    return False
    # checking hostname
    elif n_type == "A":
        a = name.split(".")
        if len(a) <= 1:
            return False
    # this state shouldnt occur
    else:
        return False
    return True
        
# checks GET request and prepares parameters for resolving
def process_get(request):
    if request[5:13] != "resolve?":
        return (400,None,None)
    elif request[13:18] != "name=":
        return (400,None,None)
    i = 18
    name = ""
    req_len = len(request)
    while i < req_len and request[i] != "&":
        name += request[i]
        i += 1
    if request[i+1:i+6] != "type=":
        return (400,None,None)
    name_type = ""
    i = i+6
    while request[i] != " ":
        name_type += request[i]
        i += 1
    if not is_valid_type(name,name_type):
        return (400,None,None)
    return(200,name,name_type)
    
# checks POST request and prepares parameters for resolving
def process_post(req):
    if req[0][0:16] != "POST /dns-query ":
        return False
    req_len = len(req)
    ret = []
    valid_requests = 0
    for i in range (6,req_len):
        split = req[i].split(":")
        if len(split) <= 1 or not is_valid_type(split[0], split[1]):
            continue
        else:
            valid_requests += 1
            ret.append((split[0], split[1]))
    if valid_requests == 0:
        return False
    else:
        return ret

# -- MAIN --
process_args()
signal.signal(signal.SIGINT, handler)
# opening the socket server
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    try:
        s.bind((HOST, PORT))
    except socket.error as e:
        # port is already occupied
        if e.errno == errno.EADDRINUSE:
            print("Port is already in use")
            exit(1)
        else:
            print(e)
            exit(1)
    s.listen()
    # looping until CTRL - C is pressed
    while True:
        client, addr = s.accept()
        with client:
            data = client.recv(1024)
            if not data:
                break
            data = data.decode("utf-8") 
            split = data.splitlines()
            # removing empty lines for robustness of inputs
            while("" in split): 
                split.remove("") 
            #   POST
            if split[0][0:4] == "POST":
                ret = process_post(split)
                if ret == False:
                    client.sendall(MSG400.encode("utf-8"))
                else:
                    ret_len = (len(ret))
                    not_found_count = 0
                    response = MSG200
                    for i in range(ret_len):
                        if ret[i][1] == "A":
                            try:
                                trans = socket.gethostbyname(ret[i][0])
                            except Exception as e:
                                not_found_count += 1
                                continue
                        else:
                            try:
                                trans = socket.gethostbyaddr(ret[i][0])[0]
                            except Exception as e:
                                not_found_count += 1
                                continue
                        response += str(ret[i][0])+":"+str(ret[i][1])+"="+trans+"\r\n"
                    if not_found_count == ret_len:
                        client.sendall(MSG404.encode("utf-8"))
                    else:
                        client.sendall(response.encode("utf-8"))
            #   GET
            elif split[0][0:3] == "GET":
                ret = process_get(split[0])
                if ret[0] != 200: # 200 means its OK
                    client.sendall(MSG400.encode("utf-8"))
                else :
                    if ret[2] == "A":
                        try:
                            name = socket.gethostbyname(ret[1])
                        except Exception as e:
                            client.sendall(MSG404.encode("utf-8"))
                        else:
                            response = MSG200+ret[1]+":"+ret[2]+"="+name+"\r\n"
                            client.sendall(response.encode("utf-8"))
                    else:
                        try:
                            adr = socket.gethostbyaddr(ret[1])[0]
                        except Exception as e:
                            client.sendall(MSG404.encode("utf-8"))
                        else:
                            response = MSG200+ret[1]+":"+ret[2]+"="+adr+"\r\n"
                            client.sendall(response.encode("utf-8"))
            #   Unsupported methods
            else:
                client.sendall(MSG405.encode("utf-8"))


