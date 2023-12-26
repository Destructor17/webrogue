#include "../include/wrout.hpp"

namespace core {
wrostream::TermBuf::TermBuf(bool pIserr) : iserr(pIserr) {
}

wrostream::wrostream(bool pIserr)
    : buf(pIserr),
#ifdef __WINDOWS__
      std::ostream(&buf, false)
#else
      std::ostream(&buf)
#endif
{
    oldBuf = rdbuf(&buf);
}
wrostream::~wrostream() {
    rdbuf(oldBuf);
}

std::streamsize wrostream::TermBuf::xsputn(const char *s, std::streamsize n) {
    // termFDs[fd]->ifwrite(s, 1, n);
    return n;
}

wrostream wrout{false};
wrostream wrerr{true};

} // namespace core