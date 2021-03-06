#
# Autogenerated by Thrift
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#  @generated
#
from libcpp.memory cimport shared_ptr, make_shared, unique_ptr, make_unique
from libcpp.string cimport string
from libcpp cimport bool as cbool
from cpython cimport bool as pbool
from libc.stdint cimport int8_t, int16_t, int32_t, int64_t
from libcpp.vector cimport vector as vector
from libcpp.set cimport set as cset
from libcpp.map cimport map as cmap
from cython.operator cimport dereference as deref, typeid
from cpython.ref cimport PyObject
from thrift.py3.client cimport cRequestChannel_ptr, makeClientWrapper, cClientWrapper
from thrift.py3.exceptions cimport try_make_shared_exception, create_py_exception
from folly cimport cFollyTry, cFollyUnit, c_unit
from folly.cast cimport down_cast_ptr
from libcpp.typeinfo cimport type_info
import thrift.py3.types
cimport thrift.py3.types
from thrift.py3.types cimport move
from thrift.py3.types import NumberType as __NumberType
import thrift.py3.client
cimport thrift.py3.client
from thrift.py3.common cimport RpcOptions as __RpcOptions
from thrift.py3.common import (
  RpcOptions as __RpcOptions,
  InterfaceSpec as __InterfaceSpec,
  MethodSpec as __MethodSpec,
  ArgumentSpec as __ArgumentSpec,
)

from folly.futures cimport bridgeFutureWith
from folly.executor cimport get_executor
cimport folly.iobuf as __iobuf
import folly.iobuf as __iobuf
from folly.iobuf cimport move as move_iobuf
cimport cython

import sys
import types as _py_types
from asyncio import get_event_loop as asyncio_get_event_loop, shield as asyncio_shield, InvalidStateError as asyncio_InvalidStateError

cimport module.types as _module_types
import module.types as _module_types

from module.clients_wrapper cimport cSinkServiceAsyncClient, cSinkServiceClientWrapper



cdef object _SinkService_annotations = _py_types.MappingProxyType({
})


@cython.auto_pickle(False)
cdef class SinkService(thrift.py3.client.Client):
    annotations = _SinkService_annotations

    cdef const type_info* _typeid(SinkService self):
        return &typeid(cSinkServiceAsyncClient)

    cdef bind_client(SinkService self, cRequestChannel_ptr&& channel):
        self._client = makeClientWrapper[cSinkServiceAsyncClient, cSinkServiceClientWrapper](
            thrift.py3.client.move(channel)
        )


    @classmethod
    def __get_reflection__(cls):
        return __InterfaceSpec(
            name="SinkService",
            methods=[
                cls.__get_reflection_for_method(),
                cls.__get_reflection_for_methodAndReponse(),
                cls.__get_reflection_for_methodThrow(),
                cls.__get_reflection_for_methodSinkThrow(),
                cls.__get_reflection_for_methodFinalThrow(),
                cls.__get_reflection_for_methodBothThrow(),
            ],
            annotations=_py_types.MappingProxyType({
            }),
        )

