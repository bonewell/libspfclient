[![Build Status](https://travis-ci.com/bonewell/libspfclient.svg?branch=master)](https://travis-ci.com/bonewell/libspfclient)

# libspfclient
The library to write client application to use [SPF microservice](https://github.com/bonewell/spfservice).

## Build
```Shell
$ mkdir build
$ cd build
$ conan install ..
$ cmake ..
$ cmake --build .
```

## Run tests
```Shell
$ ./bin/spfclient_unittest
```

## Run command line sample
```Shell
$ ./bin/spfclient_cmd
```

