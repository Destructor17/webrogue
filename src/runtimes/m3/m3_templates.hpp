#pragma once

#include "../../../external/wasm3/source/wasm3.h"
#include "../../core/ModsRuntime.hpp"
#include <tuple>

namespace webrogue {
using namespace core;
namespace runtimes {
namespace m3 {

template <char c> struct M3GlueSig {
    static const char value = c;
};
template <typename T, typename = void> struct m3_glue_type_to_sig;

template <typename T, typename U>
using is_enum_of_t = typename std::enable_if<
    std::is_enum<T>::value &&
    std::is_same<std::underlying_type_t<T>, U>::value>::type;

template <typename T>
struct m3_glue_type_to_sig<T, is_enum_of_t<T, WASMRawI32>> : M3GlueSig<'i'> {};

template <class T>
struct m3_glue_type_to_sig<T, is_enum_of_t<T, WASMRawI64>> : M3GlueSig<'I'> {};

template <> struct m3_glue_type_to_sig<void> : M3GlueSig<'v'> {};
template <> struct m3_glue_type_to_sig<WASMRawI32> : M3GlueSig<'i'> {};
template <> struct m3_glue_type_to_sig<WASMRawU32> : M3GlueSig<'i'> {};
template <> struct m3_glue_type_to_sig<WASMRawI64> : M3GlueSig<'I'> {};
template <> struct m3_glue_type_to_sig<WASMRawU64> : M3GlueSig<'I'> {};
template <> struct m3_glue_type_to_sig<float> : M3GlueSig<'f'> {};
template <> struct m3_glue_type_to_sig<WASMRawF64> : M3GlueSig<'F'> {};
template <typename T> struct m3_glue_type_to_sig<T *> : M3GlueSig<'*'> {};
template <typename T> struct m3_glue_type_to_sig<const T *> : M3GlueSig<'*'> {};

template <typename Ret, typename... Args> struct SignatureHelper {
    static char *getSignature() {
        static char result[] = {m3_glue_type_to_sig<Ret>::value, '(',
                                m3_glue_type_to_sig<Args>::value..., ')', 0};
        return result;
    }
};

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

template <typename Func, Func func> struct func_helper;
template <typename Obj, typename Ret, typename... Args,
          Ret (Obj::*method)(Args...)>
struct func_helper<Ret (Obj::*)(Args...), method> {
    static inline Ret fn(IM3ImportContext m3Context, Args... args) {
        return (obj_helper<Obj>::get(
                    (webrogue::core::ModsRuntime *)m3Context->userdata)
                    ->*method)(args...);
    }
};

typedef uint64_t *stack_type;
typedef void *mem_type;

template <typename T>
void argFromStack(T &dest, stack_type &_sp, [[maybe_unused]] mem_type mem) {
    m3ApiGetArg(T, tmp);
    dest = tmp;
}

template <typename T>
void argFromStack(T *&dest, stack_type &_sp, [[maybe_unused]] mem_type _mem) {
    m3ApiGetArgMem(T *, tmp);
    dest = tmp;
};

template <typename T>
void argFromStack(const T *&dest, stack_type &_sp,
                  [[maybe_unused]] mem_type _mem) {
    m3ApiGetArgMem(const T *, tmp);
    dest = tmp;
};
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

template <typename... Args>
static inline void getArgsFromStack(stack_type &sp, mem_type mem,
                                    std::tuple<Args...> &tuple) {
    tupleForEach(tuple, [&](auto &item) {
        argFromStack(item, sp, mem);
    });
}

template <typename Ret, typename Func, typename ArgsTuple> struct ReturnHelper {
    inline static const void *getReturnValue(Func func, ArgsTuple args,
                                             void *retptr) {
        Ret ret = call(func, args);
        Ret *raw_return = (Ret *)retptr;
        m3ApiReturn(ret);
    }

    inline static void *getRetptr(stack_type &_sp) {
        m3ApiReturnType(Ret);
        return raw_return;
    }
};

template <typename Func, typename ArgsTuple>
struct ReturnHelper<void, Func, ArgsTuple> {
    inline static const void *getReturnValue(Func func, ArgsTuple args,
                                             void *retptr) {
        call(func, args);
        m3ApiSuccess();
    }

    inline static void *getRetptr(stack_type &sp) {
        return nullptr;
    }
};

template <typename Func, Func func> struct wrap_helper;
template <typename Obj, typename Ret, typename... Args,
          Ret (Obj::*method)(Args...)>
struct wrap_helper<Ret (Obj::*)(Args...), method> {
    inline static const void *wrappedFn([[maybe_unused]] IM3Runtime rt,
                                        IM3ImportContext ctx, stack_type sp,
                                        mem_type mem) {
        std::tuple<Args...> args;

        constexpr auto f = func_helper<Ret (Obj::*)(Args...), method>::fn;

        void *rawReturn =
            ReturnHelper<Ret, Ret (*)(IM3ImportContext, Args...),
                         std::tuple<IM3ImportContext, Args...>>::getRetptr(sp);
        getArgsFromStack(sp, mem, args);
        std::tuple<IM3ImportContext, Args...> nargs =
            std::tuple_cat(std::make_tuple(ctx), args);
        return ReturnHelper<
            Ret, Ret (*)(IM3ImportContext, Args...),
            std::tuple<IM3ImportContext, Args...>>::getReturnValue(f, nargs,
                                                                   rawReturn);
    }
};

template <typename Func, Func func> struct link_helper;

template <typename Obj, typename Ret, typename... Args,
          Ret (Obj::*method)(Args...)>
struct link_helper<Ret (Obj::*)(Args...), method> {
    inline static M3Result linkM3Function(std::string moduleName,
                                          std::string functionName,
                                          IM3Module module,
                                          core::ModsRuntime *runtime) {
        auto wrappedFunc =
            wrap_helper<Ret (Obj::*)(Args...), method>::wrappedFn;
        return m3_LinkRawFunctionEx(
            module, moduleName.c_str(), functionName.c_str(),
            SignatureHelper<Ret, Args...>::getSignature(), wrappedFunc,
            (void *)(runtime));
    }
};

} // namespace m3
} // namespace runtimes
} // namespace webrogue
