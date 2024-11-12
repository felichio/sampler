
# Reservoir sampling simulation

Build running
```
make
```

In case of Debug build

```
make -DEBUG=1
```

Build artifacts will be located on root directory and inside build folders respectively

Firstly run the consumer
```
./consume -p 9090
```

where
```
-p <port_number> | denotes the listening port number of the tcp server
```

Aftewards inititiate the middleware (sampler)
```
./sample -p 8080 -r 20 -z 0.1 -dip x.x.x.x -dp 9090
```

where
```
-p <port_number> | denotes the listening port number of the tcp server
-r <size>        | denotes the reservoir buffer size used
-z <threshold>   | denotes the upper bound based on which Epoch are segmented
-dip <ipv4>      | denotes the destination ip address of the final consumer
-dp <port_number>| denotes the destination port number of the listening service
```


In order to stream values to sampler run the producer
```
./produce -ip 127.0.0.1 -p 8080 -e LE -f input.csv
```
where
```
-ip <ip_address>   |  use a valid ipv4 address where the consumer instance is running. You can use the localhost ip for the same machine

-p  <port_number>  | the port number of the listening (consume) instance

-e  <le|LE|be|BE>  | denotes the endianess used for transmitting the values. Consumer will adapt accordingly

-f  <file.csv>     | input file containing the comma delimeted values to stream. The csv should be headed 
                   | (first line denotes heading names). Dimensions are taken from the number of columns used.  
```

Example for creating a dimensional csv file from the excel.

Firstly convert the excel file to a csv

Output: some.csv

Then you can use
```
cut -d, -f1 < some.csv > first_dimension.csv
```
to extract the first dimension

For more than one dimensions specify the columns
```
cut -d, -f1,3,5,6 < some.csv > 4dimensional.csv

or

cut -d, -f1-5 < some.csv > 5dimensional.csv

```
You can then feed the exported first_dimension.csv (or 4dimensional.csv, 5dimensional.csv etc) to a sampler instance via the producer

```
./produce -ip 127.0.0.1 -p 8080 -e LE -f first_dimension.csv
```

The sampler will generate an output file  (name <n>.txt, n denotes a static counter based on the number of streams the consumer served.) describing the sampling process. The digested stream is transformed and buffer outputs are
directed to the consumer instance.



