```
  Compute and apply signature-based file differences using a rolling hash algorithm
```
## Rolling hash algorithm diff lib
The library rsync (is called rsync because I'm bad at choosing names) permit to apply patch to files calculating a delta file to apply.
This library could be used to send only the diff file instead of the complete file to update a remote storage or use less space in a backup system.

The tipical application is a backup machine B that has stored a file "foo.bin" vers. 1.0, a remote machine A have modified file "foo.bin" that now is vers. 2.0
B generate signature of file "foo.bin"(1.0) and send to machine A, A compute the difference with vers. 2.0 and send the delta file to B.
B apply the delto to recreate a "foo.bin" vers 2.0


I used IStream, OStream and IOStream to permit to users to reimplement amd provide different IO accesss, for example you could receive 
the signature and delta file from a socket.

## Note

Please look at the test test_rsync.cpp and test_delta.cpp for the assignement

## What is missing
- Use of Cmake
- block_size has to be set in IStream, better an argument for rsync functions
- remove IStream/OStream (mock and file derived class also) and use STL istream/ostream
- Create a file from Delta class
- complete the RollingHashDiff application parsing argument and usign rsync library
- a better rolling hash algorithm 
- use a better container for byte read from stream (a circular buffer ?)

## Requirements

- Visual Studio 2022

