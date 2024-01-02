#include "imported_func_wrapper.hpp"
#include "../../core/ApiObject.hpp"
#include "../../core/WASIObject.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

extern "C" {
extern int32_t getArgInt32(uint32_t argNum);
extern uint32_t getArgUInt32(uint32_t argNum);
extern int64_t getArgInt64(uint32_t argNum);
extern uint64_t getArgUInt64(uint32_t argNum);
extern float getArgFloat(uint32_t argNum);
extern double getArgDouble(uint32_t argNum);

extern void writeInt32Result(int32_t result);
extern void writeUInt32Result(uint32_t result);
extern void writeInt64Result(int64_t result);
extern void writeUInt64Result(uint64_t result);
extern void writeFloatResult(float result);
extern void writeDoubleResult(double result);

extern void setImportFuncNames(const char *jsonPtr);
}

namespace webrogue {
namespace runtimes {
namespace web {

// ------------- UTILITY---------------
// tuple_for_each
namespace detail {
template <class Tp, std::size_t... J, class F>
inline F tupleForEachImpl(Tp &&tp, std::integer_sequence<std::size_t, J...>,
                          F &&f) {
    using A = int[sizeof...(J)];
    return (void)A{((void)f(std::get<J>(std::forward<Tp>(tp))), 0)...},
           std::forward<F>(f);
}

template <class Tp, class F>
inline F tupleForEachImpl(Tp && /*tp*/, std::integer_sequence<std::size_t>,
                          F &&f) {
    return std::forward<F>(f);
}
template <typename Function, typename Tuple, size_t... I>
inline auto call(Function f, Tuple t, std::index_sequence<I...>) {
    return f(std::get<I>(t)...);
}

template <typename Function, typename Tuple>
inline auto call(Function f, Tuple t) {
    static constexpr auto size = std::tuple_size<Tuple>::value;
    return call(f, t, std::make_index_sequence<size>{});
}

} // namespace detail

template <class Tp, class F> F inline tupleForEach(Tp &&tp, F &&f) {
    using seq = std::make_index_sequence<
        std::tuple_size<typename std::remove_reference<Tp>::type>::value>;
    return detail::tupleForEachImpl(std::forward<Tp>(tp), seq(),
                                    std::forward<F>(f));
}

void writeResult(int32_t result) {
    writeInt32Result(result);
}

void writeResult(uint32_t result) {
    writeUInt32Result(result);
}

void writeResult(int64_t result) {
    writeInt64Result(result);
}

void writeResult(uint64_t result) {
    writeUInt64Result(result);
}

void writeResult(float result) {
    writeFloatResult(result);
}

void writeResult(double result) {
    writeDoubleResult(result);
}

void argFromStack(int32_t &dest, size_t argNum) {
    dest = getArgInt32(argNum);
}

void argFromStack(uint32_t &dest, size_t argNum) {
    dest = getArgUInt32(argNum);
}

void argFromStack(int64_t &dest, size_t argNum) {
    dest = getArgInt64(argNum);
}

void argFromStack(uint64_t &dest, size_t argNum) {
    dest = getArgUInt64(argNum);
}

void argFromStack(float &dest, size_t argNum) {
    dest = getArgFloat(argNum);
}

void argFromStack(double &dest, size_t argNum) {
    dest = getArgDouble(argNum);
}

typedef void (*wrappedFunc)();

template <typename... Args>
inline void getArgsFromStack(std::tuple<Args...> &tuple) {
    size_t argNum = 0;

    tupleForEach(tuple, [&](auto &item) {
        argFromStack(item, argNum++);
    });
}

webrogue::core::ApiObject *sharedApiObject = nullptr;
webrogue::core::WASIObject *sharedWasiObject = nullptr;

template <typename Obj> struct obj_helper;
template <> struct obj_helper<core::ApiObject> {
    using Obj = core::ApiObject;
    static Obj *get() {
        return sharedApiObject;
    }
};
template <> struct obj_helper<core::WASIObject> {
    using Obj = core::WASIObject;
    static Obj *get() {
        return sharedWasiObject;
    }
};

template <typename Func, Func func> struct func_helper;
template <typename Obj, typename Ret, typename... Args,
          Ret (Obj::*method)(Args...)>
struct func_helper<Ret (Obj::*)(Args...), method> {
    static inline Ret fn(Args... args) {

        return (obj_helper<Obj>::get()->*method)(args...);
    }
};

template <typename Ret, typename Func, typename ArgsTuple>
struct ResultWritingHelper {
    inline static void invoke(Func func, ArgsTuple args) {

        Ret ret = detail::call(func, args);

        writeResult(ret);
    }
};

template <typename Func, typename ArgsTuple>
struct ResultWritingHelper<void, Func, ArgsTuple> {
    inline static void invoke(Func func, ArgsTuple args) {

        detail::call(func, args);
    }
};

template <typename Func, Func func> struct wrap_helper;
template <typename Obj, typename Ret, typename... Args,
          Ret (Obj::*method)(Args...)>
struct wrap_helper<Ret (Obj::*)(Args...), method> {
    inline static void wrappedFn() {

        std::tuple<Args...> args;
        getArgsFromStack(args);

        constexpr auto f = func_helper<Ret (Obj::*)(Args...), method>::fn;

        ResultWritingHelper<Ret, Ret (*)(Args...), std::tuple<Args...>>::invoke(
            f, args);
    }
};
template <typename RetType> struct ret_type_string;
template <> struct ret_type_string<void> {
    static std::string getStr() {
        return "void";
    }
};
template <> struct ret_type_string<int32_t> {
    static std::string getStr() {
        return "int32_t";
    }
};
template <> struct ret_type_string<uint32_t> {
    static std::string getStr() {
        return "uint32_t";
    }
};
template <> struct ret_type_string<int64_t> {
    static std::string getStr() {
        return "int64_t";
    }
};
template <> struct ret_type_string<uint64_t> {
    static std::string getStr() {
        return "uint64_t";
    }
};

template <> struct ret_type_string<float> {
    static std::string getStr() {
        return "float";
    }
};
template <> struct ret_type_string<double> {
    static std::string getStr() {
        return "double";
    }
};

std::vector<wrappedFunc> wrappedFuncs;

void initWrapper(webrogue::core::ApiObject *apiObject,
                 webrogue::core::WASIObject *wasiObject) {
    sharedApiObject = apiObject;
    sharedWasiObject = wasiObject;

    std::stringstream json;
    bool needsComma = false;
#define JSON_ENTRY(NAME, RET_TYPE)                                             \
    json << (needsComma ? "," : "") << "\"" #NAME "\":{\"ret_type\":\""        \
         << ret_type_string<RET_TYPE>::getStr()                                \
         << "\",\"func_id\":" << wrappedFuncs.size() << "}";                   \
    needsComma = true

    needsComma = false;
    json << "{\"webrogue\": {";
#define WR_API_FUNCTION(RET_TYPE, NAME, ARGS)                                  \
    JSON_ENTRY(NAME, RET_TYPE);                                                \
    wrappedFuncs.push_back(wrap_helper<decltype(&core::ApiObject::NAME),       \
                                       &core::ApiObject::NAME>::wrappedFn);

#include "../../../mods/core/include/common/wr_api_functions.def"
#undef WR_API_FUNCTION

    needsComma = false;
    json << "},\"wasi_snapshot_preview1\": {";
#define WASI_FUNCTION(RET_TYPE, NAME, ARGS)                                    \
    JSON_ENTRY(NAME, RET_TYPE);                                                \
    wrappedFuncs.push_back(wrap_helper<decltype(&core::WASIObject::NAME),      \
                                       &core::WASIObject::NAME>::wrappedFn);

#include "../../core/wasi_functions.def"
#undef WASI_FUNCTION
    json << "}}";

#undef JSON_ENTRY

    std::string jsonStr = json.str();

    setImportFuncNames(jsonStr.c_str());
}

void callImportedFunc(int funcId) {
    wrappedFuncs[funcId]();
}
} // namespace web
} // namespace runtimes
} // namespace webrogue
