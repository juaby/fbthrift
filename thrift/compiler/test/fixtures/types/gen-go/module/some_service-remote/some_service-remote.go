// Autogenerated by Thrift Compiler (facebook)
// DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
// @generated

package main

import (
        "flag"
        "fmt"
        "math"
        "net"
        "net/url"
        "os"
        "strconv"
        "strings"
        thrift "github.com/facebook/fbthrift/thrift/lib/go/thrift"
        "../../module"
)

func Usage() {
  fmt.Fprintln(os.Stderr, "Usage of ", os.Args[0], " [-h host:port] [-u url] [-f[ramed]] function [arg1 [arg2...]]:")
  flag.PrintDefaults()
  fmt.Fprintln(os.Stderr, "\nFunctions:")
  fmt.Fprintln(os.Stderr, "  SomeMap bounce_map(SomeMap m)")
  fmt.Fprintln(os.Stderr, "   binary_keyed_map( r)")
  fmt.Fprintln(os.Stderr)
  os.Exit(0)
}

func main() {
  flag.Usage = Usage
  var host string
  var port int
  var protocol string
  var urlString string
  var framed bool
  var useHttp bool
  var parsedUrl url.URL
  var trans thrift.Transport
  _ = strconv.Atoi
  _ = math.Abs
  flag.Usage = Usage
  flag.StringVar(&host, "h", "localhost", "Specify host")
  flag.IntVar(&port, "p", 9090, "Specify port")
  flag.StringVar(&protocol, "P", "binary", "Specify the protocol (binary, compact, simplejson, json)")
  flag.StringVar(&urlString, "u", "", "Specify the url")
  flag.BoolVar(&framed, "framed", false, "Use framed transport")
  flag.BoolVar(&useHttp, "http", false, "Use http")
  flag.Parse()
  
  if len(urlString) > 0 {
    parsedUrl, err := url.Parse(urlString)
    if err != nil {
      fmt.Fprintln(os.Stderr, "Error parsing URL: ", err)
      flag.Usage()
    }
    host = parsedUrl.Host
    useHttp = len(parsedUrl.Scheme) <= 0 || parsedUrl.Scheme == "http"
  } else if useHttp {
    _, err := url.Parse(fmt.Sprint("http://", host, ":", port))
    if err != nil {
      fmt.Fprintln(os.Stderr, "Error parsing URL: ", err)
      flag.Usage()
    }
  }
  
  cmd := flag.Arg(0)
  var err error
  if useHttp {
    trans, err = thrift.NewHTTPPostClient(parsedUrl.String())
  } else {
    portStr := fmt.Sprint(port)
    if strings.Contains(host, ":") {
           host, portStr, err = net.SplitHostPort(host)
           if err != nil {
                   fmt.Fprintln(os.Stderr, "error with host:", err)
                   os.Exit(1)
           }
    }
    trans, err = thrift.NewSocket(thrift.SocketAddr(net.JoinHostPort(host, portStr)))
    if err != nil {
      fmt.Fprintln(os.Stderr, "error resolving address:", err)
      os.Exit(1)
    }
    if framed {
      trans = thrift.NewFramedTransport(trans)
    }
  }
  if err != nil {
    fmt.Fprintln(os.Stderr, "Error creating transport", err)
    os.Exit(1)
  }
  defer trans.Close()
  var protocolFactory thrift.ProtocolFactory
  switch protocol {
  case "compact":
    protocolFactory = thrift.NewCompactProtocolFactory()
    break
  case "simplejson":
    protocolFactory = thrift.NewSimpleJSONProtocolFactory()
    break
  case "json":
    protocolFactory = thrift.NewJSONProtocolFactory()
    break
  case "binary", "":
    protocolFactory = thrift.NewBinaryProtocolFactoryDefault()
    break
  default:
    fmt.Fprintln(os.Stderr, "Invalid protocol specified: ", protocol)
    Usage()
    os.Exit(1)
  }
  client := module.NewSomeServiceClientFactory(trans, protocolFactory)
  if err := trans.Open(); err != nil {
    fmt.Fprintln(os.Stderr, "Error opening socket to ", host, ":", port, " ", err)
    os.Exit(1)
  }
  
  switch cmd {
  case "bounce_map":
    if flag.NArg() - 1 != 1 {
      fmt.Fprintln(os.Stderr, "BounceMap requires 1 args")
      flag.Usage()
    }
    arg29 := flag.Arg(1)
    mbTrans30 := thrift.NewMemoryBufferLen(len(arg29))
    defer mbTrans30.Close()
    _, err31 := mbTrans30.WriteString(arg29)
    if err31 != nil { 
      Usage()
      return
    }
    factory32 := thrift.NewSimpleJSONProtocolFactory()
    jsProt33 := factory32.GetProtocol(mbTrans30)
    containerStruct0 := module.NewSomeServiceBounceMapArgs()
    err34 := containerStruct0.ReadField1(jsProt33)
    if err34 != nil {
      Usage()
      return
    }
    argvalue0 := containerStruct0.M
    value0 := module.SomeMap(argvalue0)
    fmt.Print(client.BounceMap(value0))
    fmt.Print("\n")
    break
  case "binary_keyed_map":
    if flag.NArg() - 1 != 1 {
      fmt.Fprintln(os.Stderr, "BinaryKeyedMap requires 1 args")
      flag.Usage()
    }
    arg35 := flag.Arg(1)
    mbTrans36 := thrift.NewMemoryBufferLen(len(arg35))
    defer mbTrans36.Close()
    _, err37 := mbTrans36.WriteString(arg35)
    if err37 != nil { 
      Usage()
      return
    }
    factory38 := thrift.NewSimpleJSONProtocolFactory()
    jsProt39 := factory38.GetProtocol(mbTrans36)
    containerStruct0 := module.NewSomeServiceBinaryKeyedMapArgs()
    err40 := containerStruct0.ReadField1(jsProt39)
    if err40 != nil {
      Usage()
      return
    }
    argvalue0 := containerStruct0.R
    value0 := argvalue0
    fmt.Print(client.BinaryKeyedMap(value0))
    fmt.Print("\n")
    break
  case "":
    Usage()
    break
  default:
    fmt.Fprintln(os.Stderr, "Invalid function ", cmd)
  }
}
