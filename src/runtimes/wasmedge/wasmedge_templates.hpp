#pragma once

#include "../../core/ModsRuntime.hpp"
#include "wasmedge/enum_types.h"
#include "wasmedge/wasmedge.h"
#include <cstdint>
#include <string>

namespace webrogue {
namespace runtimes {
namespace wasmedge {

// ------------- UTILITY---------------
// tuple_for_each
namespace detail {
template <class Tp, std::size_t... J, class F>
static inline F
tupleForEachImpl(Tp &&tp, std::integer_sequence<std::size_t, J...>, F &&f) {
    using A = int[sizeof...(J)];
    return (void)A{((void)f(std::get<J>(std::forward<Tp>(tp))), 0)...},
           std::forward<F>(f);
}

template <class Tp, class F>
static inline F tupleForEachImpl(Tp && /*tp*/,
                                 std::integer_sequence<std::size_t>, F &&f) {
    return std::forward<F>(f);
}

} // namespace detail

template <class Tp, class F> F static inline tupleForEach(Tp &&tp, F &&f) {
    using seq = std::make_index_sequence<
        std::tuple_size<typename std::remove_reference<Tp>::type>::value>;
    return detail::tupleForEachImpl(std::forward<Tp>(tp), seq(),
                                    std::forward<F>(f));
}

template <typename Function, typename Tuple, size_t... I>
static inline auto call(Function f, Tuple t, std::index_sequence<I...>) {
    return f(std::get<I>(t)...);
}

template <typename Function, typename Tuple>
static inline auto call(Function f, Tuple t) {
    static constexpr auto size = std::tuple_size<Tuple>::value;
    return call(f, t, std::make_index_sequence<size>{});
}
// ---END---

// signatures

typedef WasmEdge_ValType WasmEdge_ValTypeGen_T(void);

template <WasmEdge_ValTypeGen_T c> struct WasmedgeGlueSig {
    static inline WasmEdge_ValType genType() {
        return c();
    }
};
template <typename T, typename = void> struct wasmedge_glue_type_to_sig;

template <typename T, typename U>
using is_enum_of_t = typename std::enable_if<
    std::is_enum<T>::value &&
    std::is_same<std::underlying_type_t<T>, U>::value>::type;

template <typename T>
struct wasmedge_glue_type_to_sig<T, is_enum_of_t<T, int32_t>>
    : WasmedgeGlueSig<WasmEdge_ValTypeGenI32> {};

template <class T>
struct wasmedge_glue_type_to_sig<T, is_enum_of_t<T, int64_t>>
    : WasmedgeGlueSig<WasmEdge_ValTypeGenI64> {};

template <>
struct wasmedge_glue_type_to_sig<int32_t>
    : WasmedgeGlueSig<WasmEdge_ValTypeGenI32> {};
template <>
struct wasmedge_glue_type_to_sig<uint32_t>
    : WasmedgeGlueSig<WasmEdge_ValTypeGenI32> {};
template <>
struct wasmedge_glue_type_to_sig<int64_t>
    : WasmedgeGlueSig<WasmEdge_ValTypeGenI64> {};
template <>
struct wasmedge_glue_type_to_sig<uint64_t>
    : WasmedgeGlueSig<WasmEdge_ValTypeGenI64> {};
template <>
struct wasmedge_glue_type_to_sig<float>
    : WasmedgeGlueSig<WasmEdge_ValTypeGenF32> {};
template <>
struct wasmedge_glue_type_to_sig<double>
    : WasmedgeGlueSig<WasmEdge_ValTypeGenF64> {};
template <typename T>
struct wasmedge_glue_type_to_sig<T *>
    : WasmedgeGlueSig<WasmEdge_ValTypeGenI32> {};
template <typename T>
struct wasmedge_glue_type_to_sig<const T *>
    : WasmedgeGlueSig<WasmEdge_ValTypeGenI32> {};

// concatenated signature

template <typename... Args> struct ArgsHelper {
    static WasmEdge_ValType *getSignature(size_t &count) {
        static WasmEdge_ValType result[] = {
            wasmedge_glue_type_to_sig<Args>::genType()...};
        count = sizeof(result) / sizeof(WasmEdge_ValType);
        return result;
    }
};

template <> struct ArgsHelper<> {
    static WasmEdge_ValType *getSignature(size_t &count) {
        count = 0;
        return nullptr;
    }
};

template <typename Ret> struct ReturnSignatureHelper {
    static WasmEdge_ValType *getSignature(size_t &count) {
        static WasmEdge_ValType result[] = {
            wasmedge_glue_type_to_sig<Ret>::genType()};
        count = 1;
        return result;
    }
};

template <> struct ReturnSignatureHelper<void> {
    static WasmEdge_ValType *getSignature(size_t &count) {
        count = 0;
        return nullptr;
    }
};

// obj helper

template <typename Obj> struct obj_helper;
template <> struct obj_helper<core::ApiObject> {
    using Obj = core::ApiObject;
    static Obj *get(core::ModsRuntime *runtime) {
        return &(runtime->apiObject);
    }
};
template <> struct obj_helper<core::WASIObject> {
    using Obj = core::WASIObject;
    static Obj *get(core::ModsRuntime *runtime) {
        return &(runtime->wasiObject);
    }
};
template <> struct obj_helper<core::Linker> {
    using Obj = core::Linker;
    static Obj *get(core::ModsRuntime *runtime) {
        return runtime->linker;
    }
};

// func helper

template <typename Func, Func func> struct func_helper;
template <typename Obj, typename Ret, typename... Args,
          Ret (Obj::*method)(Args...)>
struct func_helper<Ret (Obj::*)(Args...), method> {
    static inline Ret fn(void *data, Args... args) {
        return (
            obj_helper<Obj>::get((webrogue::core::ModsRuntime *)data)->*method)(
            args...);
    }
};

// get_args_from_stack

template <typename T> struct arg_stack_helper;

template <> struct arg_stack_helper<int32_t> {
    static inline void
    argFromStack(int32_t &dest, const WasmEdge_Value *in, size_t argI,
                 const WasmEdge_CallingFrameContext *callFrameCxt) {
        dest = WasmEdge_ValueGetI32(in[argI]);
    }
};
template <> struct arg_stack_helper<uint32_t> {
    static inline void
    argFromStack(uint32_t &dest, const WasmEdge_Value *in, size_t argI,
                 const WasmEdge_CallingFrameContext *callFrameCxt) {
        dest = WasmEdge_ValueGetI32(in[argI]);
    }
};
template <> struct arg_stack_helper<int64_t> {
    static inline void
    argFromStack(int64_t &dest, const WasmEdge_Value *in, size_t argI,
                 const WasmEdge_CallingFrameContext *callFrameCxt) {
        dest = WasmEdge_ValueGetI64(in[argI]);
    }
};
template <> struct arg_stack_helper<uint64_t> {
    static inline void
    argFromStack(uint64_t &dest, const WasmEdge_Value *in, size_t argI,
                 const WasmEdge_CallingFrameContext *callFrameCxt) {
        dest = WasmEdge_ValueGetI64(in[argI]);
    }
};
template <> struct arg_stack_helper<float> {
    static inline void
    argFromStack(float &dest, const WasmEdge_Value *in, size_t argI,
                 const WasmEdge_CallingFrameContext *callFrameCxt) {
        dest = WasmEdge_ValueGetF32(in[argI]);
    }
};
template <> struct arg_stack_helper<double> {
    static inline void
    argFromStack(double &dest, const WasmEdge_Value *in, size_t argI,
                 const WasmEdge_CallingFrameContext *callFrameCxt) {
        dest = WasmEdge_ValueGetF64(in[argI]);
    }
};
template <> struct arg_stack_helper<void *> {
    static inline void
    argFromStack(void *&dest, const WasmEdge_Value *in, size_t argI,
                 const WasmEdge_CallingFrameContext *callFrameCxt) {
        WasmEdge_MemoryInstanceContext *memCxt =
            WasmEdge_CallingFrameGetMemoryInstance(callFrameCxt, 0);
        size_t memSize = WasmEdge_MemoryInstanceGetPageSize(memCxt);
        void *mem = WasmEdge_MemoryInstanceGetPointer(memCxt, 0, memSize);
        dest =
            static_cast<void *>(((char *)mem) + WasmEdge_ValueGetI32(in[argI]));
    }
};

template <typename T>
void argFromStack(T &dest, const WasmEdge_Value *in, size_t argI,
                  const WasmEdge_CallingFrameContext *callFrameCxt) {
    arg_stack_helper<T>::argFromStack(dest, in, argI, callFrameCxt);
}

template <typename... Args>
static inline void
getArgsFromStack(const WasmEdge_Value *in,
                 const WasmEdge_CallingFrameContext *callFrameCxt,
                 std::tuple<Args...> &tuple) {
    size_t argI = 0;

    tupleForEach(tuple, [&](auto &item) {
        argFromStack(item, in, argI, callFrameCxt);
        argI++;
    });
}

// return_helper

template <typename Ret, typename Func, typename ArgsTuple> struct return_helper;
template <typename Func, typename ArgsTuple>
struct return_helper<void, Func, ArgsTuple> {
    inline static void callAndReturn(Func func, ArgsTuple args,
                                     WasmEdge_Value *out) {
        call(func, args);
    }
};
template <typename Func, typename ArgsTuple>
struct return_helper<int32_t, Func, ArgsTuple> {
    inline static void callAndReturn(Func func, ArgsTuple args,
                                     WasmEdge_Value *out) {
        out[0] = WasmEdge_ValueGenI32(call(func, args));
    }
};
template <typename Func, typename ArgsTuple>
struct return_helper<uint32_t, Func, ArgsTuple> {
    inline static void callAndReturn(Func func, ArgsTuple args,
                                     WasmEdge_Value *out) {
        out[0] = WasmEdge_ValueGenI32(call(func, args));
    }
};
template <typename Func, typename ArgsTuple>
struct return_helper<int64_t, Func, ArgsTuple> {
    inline static void callAndReturn(Func func, ArgsTuple args,
                                     WasmEdge_Value *out) {
        out[0] = WasmEdge_ValueGenI64(call(func, args));
    }
};
template <typename Func, typename ArgsTuple>
struct return_helper<uint64_t, Func, ArgsTuple> {
    inline static void callAndReturn(Func func, ArgsTuple args,
                                     WasmEdge_Value *out) {
        out[0] = WasmEdge_ValueGenI64(call(func, args));
    }
};
template <typename Func, typename ArgsTuple>
struct return_helper<float, Func, ArgsTuple> {
    inline static void callAndReturn(Func func, ArgsTuple args,
                                     WasmEdge_Value *out) {
        out[0] = WasmEdge_ValueGenF32(call(func, args));
    }
};
template <typename Func, typename ArgsTuple>
struct return_helper<double, Func, ArgsTuple> {
    inline static void callAndReturn(Func func, ArgsTuple args,
                                     WasmEdge_Value *out) {
        out[0] = WasmEdge_ValueGenF64(call(func, args));
    }
};
// wrap helper

template <typename Func, Func func> struct wrap_helper;
template <typename Obj, typename Ret, typename... Args,
          Ret (Obj::*method)(Args...)>
struct wrap_helper<Ret (Obj::*)(Args...), method> {
    static inline WasmEdge_Result
    wrappedFn(void *data, const WasmEdge_CallingFrameContext *callFrameCxt,
              const WasmEdge_Value *in, WasmEdge_Value *out) {
        std::tuple<Args...> args;

        constexpr auto f = func_helper<Ret (Obj::*)(Args...), method>::fn;

        getArgsFromStack(in, callFrameCxt, args);
        std::tuple<void *, Args...> nargs =
            std::tuple_cat(std::make_tuple(data), args);
        ((core::ModsRuntime *)data)->vmContext = callFrameCxt;
        return_helper<Ret, decltype(f),
                      std::tuple<void *, Args...>>::callAndReturn(f, nargs,
                                                                  out);
        return WasmEdge_Result_Success;
    }
};

// link_helper

template <typename Func, Func func> struct link_helper;

template <typename Obj, typename Ret, typename... Args,
          Ret (Obj::*method)(Args...)>
struct link_helper<Ret (Obj::*)(Args...), method> {
    inline static void
    linkWasmedgeFunction(std::string moduleName, std::string functionName,
                         WasmEdge_ModuleInstanceContext *hostMod,
                         core::ModsRuntime *runtime) {
        constexpr auto wrappedFunc =
            &(wrap_helper<Ret (Obj::*)(Args...), method>::wrappedFn);

        size_t paramCount = 0;
        WasmEdge_ValType *paramList =
            ArgsHelper<Args...>::getSignature(paramCount);

        size_t returnCount = 0;
        WasmEdge_ValType *returnList =
            ReturnSignatureHelper<Ret>::getSignature(returnCount);
        ;
        /* Create a function type: {i32, i32} -> {i32}. */
        WasmEdge_FunctionTypeContext *hostFType = WasmEdge_FunctionTypeCreate(
            paramList, paramCount, returnList, returnCount);
        /*
         * Create a function context with the function type and host function
         * body. The `Cost` parameter can be 0 if developers do not need the
         * cost measuring.
         */
        WasmEdge_FunctionInstanceContext *hostFunc =
            WasmEdge_FunctionInstanceCreate(hostFType, wrappedFunc, runtime, 0);
        /*
         * The third parameter is the pointer to the additional data.
         * Developers should guarantee the life cycle of the data, and it can be
         * NULL if the external data is not needed.
         */
        WasmEdge_FunctionTypeDelete(hostFType);

        WasmEdge_String hostName =
            WasmEdge_StringCreateByCString(functionName.c_str());
        WasmEdge_ModuleInstanceAddFunction(hostMod, hostName, hostFunc);
        WasmEdge_StringDelete(hostName);
    }
};
} // namespace wasmedge
} // namespace runtimes
} // namespace webrogue