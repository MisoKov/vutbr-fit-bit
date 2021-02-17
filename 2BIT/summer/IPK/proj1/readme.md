# Projekt 1 - HTTP resolver of domain names
---
Student project for Computer Communications and Networks course at FIT VUTBR. The aim of project is to create simple HTTP resolver of domain namees and IPv4 addresses.

### Design
---
The application is written in Python. For server for used `socket` module. Program firsly checks the arguments for PORT number validity. Then server starts running and listenning. After HTTP request is recieved it is checked for validity and if all tests pass the server tries to resolve the domain name. Only supported requests are `GET` and `POST`. The program is shut down when `CTRL+C` is pressed in terminal.
##### Client return codes
- **400 Bad Request** - Returned when incorrect format of input requests was detected
- **404 Not Found** - Returned when domain was not found 
- **405 Method Not Allowed** - Returned when unsupported request was recieved by server, other than GET or POST
- **200 OK** - Returned when request was handled successfully - at leat one domain was resolved
##### Input requests
Examples using `curl` program:
**POST:**
```
curl --data-binary @queries.txt -X POST http://localhost:5353/dns-query
```
 **queries.txt** - individual queries are on one line each, lines can be empty in the file and file can begin and end with empty lines, both of next are valid
```
www.fit.vutbr.cz:A
www.google.com:A
www.seznam.cz:A
147.229.14.131:PTR
ihned.cz:A
```
```

www.fit.vutbr.cz:A
www.google.com:A
www.seznam.cz:A


147.229.14.131:PTR
ihned.cz:A

```
**GET:** Paramater name must be address when parameter type is PTR, and when type is A name must be domain name.
```
curl localhost:5353/resolve?name=www.name.of.domain.cz\&type=A
curl localhost:5353/resolve?name=123.345.65.02\&type=PTR
```
##### Libraries
- ```socket``` - To create socket server and send and recieve data
- ```sys``` - For program arguments
- ```signal``` - For SIGINT catching and handlig
- ```errno``` - For exception handlig 
##### Shutting down
```CTRL+C``` entered at terminal with running server
##### Functions
```process_args()``` - checks the number and validity of arguments passed to the program, exits the program with return code 1 if more than 1 arguments were passed or if the argument passed as port is not integer
```handler(signum, frame)``` - handles the SIGINT signal to close the server and exits the program with code 0
```port_check(port)``` - checks validity of entered port number, port must be uint16
```is_valid_type(name,n_type)``` - checks the validity of arguments from request, returns False when incorrect combination of parameters was used in request or if the values of parameters are not valid (type = 'A'/'PTR', name = domain name/address), otherwise returns True
```process_get(request)``` - checks GET request and prepares parameters for resolving, returns tuple containing (return code, name, type), in case of invalid request return code is 400 other elemets None, in case of valid request return code is 200 and the name and type parsed from request
```process_post(req)``` - checks POST request and prepares parameters for resolving, return false if request is invalid or none of the queries is valid otherwise returns list valid queries formed as tuples (name, type)
### Prerequisites
---
Python3.6 or newer version. Instalation on ubuntu based linux systems:
```
sudo apt install python3
```
GNU Make to run the application. Instalation: 
```
sudo apt install make
```
### Building
---
No building is required.
```
$ make run PORT=port_num
```
 - when ```PORT``` is not entered, deafault port is 19002
### Author
- *Michal Koval* (<xkoval17@stud.fit.vutbr.cz>)
