#include "../../../external/wamr/core/iwasm/include/wasm_export.h"
#include "../../core/ModsRuntime.hpp"
#include <stdint.h>
#include <vector>

namespace webrogue {
namespace runtimes {
namespace wamr {
template <char c> struct WamrGlueSig {
    static const char value = c;
};
template <typename T, typename = void> struct wamr_glue_type_to_sig;

template <typename T, typename U>
using is_enum_of_t = typename std::enable_if<
    std::is_enum<T>::value &&
    std::is_same<std::underlying_type_t<T>, U>::value>::type;

template <typename T>
struct wamr_glue_type_to_sig<T, is_enum_of_t<T, int32_t>> : WamrGlueSig<'i'> {};

template <class T>
struct wamr_glue_type_to_sig<T, is_enum_of_t<T, int64_t>> : WamrGlueSig<'I'> {};

template <> struct wamr_glue_type_to_sig<int32_t> : WamrGlueSig<'i'> {};
template <> struct wamr_glue_type_to_sig<uint32_t> : WamrGlueSig<'i'> {};
template <> struct wamr_glue_type_to_sig<int64_t> : WamrGlueSig<'I'> {};
template <> struct wamr_glue_type_to_sig<uint64_t> : WamrGlueSig<'I'> {};
template <> struct wamr_glue_type_to_sig<float> : WamrGlueSig<'f'> {};
template <> struct wamr_glue_type_to_sig<double> : WamrGlueSig<'F'> {};
template <typename T> struct wamr_glue_type_to_sig<T *> : WamrGlueSig<'*'> {};
template <typename T>
struct wamr_glue_type_to_sig<const T *> : WamrGlueSig<'*'> {};

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
template <typename Ret, typename... Args> struct args_helper;
template <typename... Args> struct args_helper<void, Args...> {
    static char *getSignature() {
        static char result[] = {'(', wamr_glue_type_to_sig<Args>::value..., ')',
                                0, 0};
        return result;
    }
};
template <typename Ret, typename... Args> struct args_helper {
    static char *getSignature() {
        static char result[] = {'(', wamr_glue_type_to_sig<Args>::value..., ')',
                                wamr_glue_type_to_sig<Ret>::value, 0};
        return result;
    }
};

template <typename Func> struct wrap_helper;
template <typename Obj, typename Ret, typename... Args>
struct wrap_helper<Ret (Obj::*)(Args...)> {
    using Func = Ret (Obj::*)(Args...);
    template <Ret (Obj::*method)(Args...)>
    static Ret wrapFn(wasm_exec_env_t execEnv, Args... args) {
        auto *runtime =
            (webrogue::core::ModsRuntime *)wasm_runtime_get_user_data(execEnv);
        return (obj_helper<Obj>::get(runtime)->*method)(args...);
    }
    static char *getSignature() {
        return args_helper<Ret, Args...>::getSignature();
    }
};
} // namespace wamr
} // namespace runtimes
} // namespace webrogue
