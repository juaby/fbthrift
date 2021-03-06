#
# Autogenerated by Thrift
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#  @generated
#

cimport cython
from cpython.version cimport PY_VERSION_HEX
from libcpp.memory cimport shared_ptr, make_shared, unique_ptr, make_unique
from libcpp.string cimport string
from libcpp cimport bool as cbool
from cpython cimport bool as pbool
from libc.stdint cimport int8_t, int16_t, int32_t, int64_t
from libcpp.vector cimport vector
from libcpp.set cimport set as cset
from libcpp.map cimport map as cmap
from cython.operator cimport dereference as deref
from cpython.ref cimport PyObject
from thrift.py3.common import (
  InterfaceSpec as __InterfaceSpec,
  MethodSpec as __MethodSpec,
  ArgumentSpec as __ArgumentSpec,
)
from thrift.py3.exceptions cimport (
    cTApplicationException,
    ApplicationError as __ApplicationError,
    cTApplicationExceptionType__UNKNOWN)
from thrift.py3.server cimport ServiceInterface, RequestContext, Cpp2RequestContext
from thrift.py3.server import RequestContext, pass_context
from folly cimport (
  cFollyPromise,
  cFollyUnit,
  c_unit
)
from thrift.py3.types cimport move
from thrift.py3.types import NumberType as __NumberType

if PY_VERSION_HEX >= 0x030702F0:  # 3.7.2 Final
    from thrift.py3.server cimport THRIFT_REQUEST_CONTEXT as __THRIFT_REQUEST_CONTEXT

cimport folly.futures
from folly.executor cimport get_executor
cimport folly.iobuf as __iobuf
import folly.iobuf as __iobuf
from folly.iobuf cimport move as move_iobuf

cimport service.types as _service_types
import service.types as _service_types
import includes.types as _includes_types
cimport includes.types as _includes_types
import module.types as _module_types
cimport module.types as _module_types
import transitive.types as _transitive_types
cimport transitive.types as _transitive_types

import asyncio
import functools
import sys
import traceback
import types as _py_types

from service.services_wrapper cimport cMyServiceInterface


cdef extern from "<utility>" namespace "std":
    cdef cFollyPromise[cFollyUnit] move_promise_cFollyUnit "std::move"(
        cFollyPromise[cFollyUnit])

@cython.auto_pickle(False)
cdef class Promise_cFollyUnit:
    cdef cFollyPromise[cFollyUnit] cPromise

    @staticmethod
    cdef create(cFollyPromise[cFollyUnit] cPromise):
        inst = <Promise_cFollyUnit>Promise_cFollyUnit.__new__(Promise_cFollyUnit)
        inst.cPromise = move_promise_cFollyUnit(cPromise)
        return inst

cdef object _MyService_annotations = _py_types.MappingProxyType({
})


@cython.auto_pickle(False)
cdef class MyServiceInterface(
    ServiceInterface
):
    annotations = _MyService_annotations

    def __cinit__(self):
        self._cpp_obj = cMyServiceInterface(
            <PyObject *> self,
            get_executor()
        )

    @staticmethod
    def pass_context_query(fn):
        return pass_context(fn)

    async def query(
            self,
            s,
            i):
        raise NotImplementedError("async def query is not implemented")

    @staticmethod
    def pass_context_has_arg_docs(fn):
        return pass_context(fn)

    async def has_arg_docs(
            self,
            s,
            i):
        raise NotImplementedError("async def has_arg_docs is not implemented")

    @staticmethod
    def __get_reflection_for_query():
        return __MethodSpec(
            name="query",
            arguments=[
                __ArgumentSpec(
                    name="s",
                    type=_module_types.MyStruct,
                    kind=None,
                    annotations=_py_types.MappingProxyType({
                    }),
                ),
                __ArgumentSpec(
                    name="i",
                    type=_includes_types.Included,
                    kind=None,
                    annotations=_py_types.MappingProxyType({
                    }),
                ),
            ],
            result=None,
            result_kind=None,
            exceptions=[
            ],
            annotations=_py_types.MappingProxyType({
            }),
        )

    @staticmethod
    def __get_reflection_for_has_arg_docs():
        return __MethodSpec(
            name="has_arg_docs",
            arguments=[
                __ArgumentSpec(
                    name="s",
                    type=_module_types.MyStruct,
                    kind=None,
                    annotations=_py_types.MappingProxyType({
                    }),
                ),
                __ArgumentSpec(
                    name="i",
                    type=_includes_types.Included,
                    kind=None,
                    annotations=_py_types.MappingProxyType({
                    }),
                ),
            ],
            result=None,
            result_kind=None,
            exceptions=[
            ],
            annotations=_py_types.MappingProxyType({
            }),
        )

    @classmethod
    def __get_reflection__(cls):
        return __InterfaceSpec(
            name="MyService",
            methods=[
                cls.__get_reflection_for_query(),
                cls.__get_reflection_for_has_arg_docs(),
            ],
            annotations=_py_types.MappingProxyType({
            }),
        )



cdef api void call_cy_MyService_query(
    object self,
    Cpp2RequestContext* ctx,
    cFollyPromise[cFollyUnit] cPromise,
    unique_ptr[_module_types.cMyStruct] s,
    unique_ptr[_includes_types.cIncluded] i
):
    cdef MyServiceInterface __iface
    __iface = self
    __promise = Promise_cFollyUnit.create(move_promise_cFollyUnit(cPromise))
    arg_s = _module_types.MyStruct.create(shared_ptr[_module_types.cMyStruct](s.release()))
    arg_i = _includes_types.Included.create(shared_ptr[_includes_types.cIncluded](i.release()))
    __context_obj = RequestContext.create(ctx)
    __context = None
    if __iface._pass_context_query:
        __context = __context_obj
    if PY_VERSION_HEX >= 0x030702F0:  # 3.7.2 Final
        __context_token = __THRIFT_REQUEST_CONTEXT.set(__context_obj)
    asyncio.get_event_loop().create_task(
        MyService_query_coro(
            self,
            __context,
            __promise,
            arg_s,
            arg_i
        )
    )
    if PY_VERSION_HEX >= 0x030702F0:  # 3.7.2 Final
        __THRIFT_REQUEST_CONTEXT.reset(__context_token)

async def MyService_query_coro(
    object self,
    object ctx,
    Promise_cFollyUnit promise,
    s,
    i
):
    try:
        if ctx is not None:
            result = await self.query(ctx,
                      s,
                      i)
        else:
            result = await self.query(
                      s,
                      i)
    except __ApplicationError as ex:
        # If the handler raised an ApplicationError convert it to a C++ one
        promise.cPromise.setException(cTApplicationException(
            ex.type.value, ex.message.encode('UTF-8')
        ))
    except Exception as ex:
        print(
            "Unexpected error in service handler query:",
            file=sys.stderr)
        traceback.print_exc()
        promise.cPromise.setException(cTApplicationException(
            cTApplicationExceptionType__UNKNOWN, repr(ex).encode('UTF-8')
        ))
    else:
        promise.cPromise.setValue(c_unit)

cdef api void call_cy_MyService_has_arg_docs(
    object self,
    Cpp2RequestContext* ctx,
    cFollyPromise[cFollyUnit] cPromise,
    unique_ptr[_module_types.cMyStruct] s,
    unique_ptr[_includes_types.cIncluded] i
):
    cdef MyServiceInterface __iface
    __iface = self
    __promise = Promise_cFollyUnit.create(move_promise_cFollyUnit(cPromise))
    arg_s = _module_types.MyStruct.create(shared_ptr[_module_types.cMyStruct](s.release()))
    arg_i = _includes_types.Included.create(shared_ptr[_includes_types.cIncluded](i.release()))
    __context_obj = RequestContext.create(ctx)
    __context = None
    if __iface._pass_context_has_arg_docs:
        __context = __context_obj
    if PY_VERSION_HEX >= 0x030702F0:  # 3.7.2 Final
        __context_token = __THRIFT_REQUEST_CONTEXT.set(__context_obj)
    asyncio.get_event_loop().create_task(
        MyService_has_arg_docs_coro(
            self,
            __context,
            __promise,
            arg_s,
            arg_i
        )
    )
    if PY_VERSION_HEX >= 0x030702F0:  # 3.7.2 Final
        __THRIFT_REQUEST_CONTEXT.reset(__context_token)

async def MyService_has_arg_docs_coro(
    object self,
    object ctx,
    Promise_cFollyUnit promise,
    s,
    i
):
    try:
        if ctx is not None:
            result = await self.has_arg_docs(ctx,
                      s,
                      i)
        else:
            result = await self.has_arg_docs(
                      s,
                      i)
    except __ApplicationError as ex:
        # If the handler raised an ApplicationError convert it to a C++ one
        promise.cPromise.setException(cTApplicationException(
            ex.type.value, ex.message.encode('UTF-8')
        ))
    except Exception as ex:
        print(
            "Unexpected error in service handler has_arg_docs:",
            file=sys.stderr)
        traceback.print_exc()
        promise.cPromise.setException(cTApplicationException(
            cTApplicationExceptionType__UNKNOWN, repr(ex).encode('UTF-8')
        ))
    else:
        promise.cPromise.setValue(c_unit)

