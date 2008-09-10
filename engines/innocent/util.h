#ifndef INNOCENT_UTIL_H
#define INNOCENT_UTIL_H

namespace Innocent {
//
#define foreach(T, L) for (Common::List<T>::iterator it = L.begin(); it != L.end(); ++it)
#define unless(x) if (!(x))

template<typename F1, typename F2>
class Composite {
public:
	Composite(const F1 &a, const F2 &b) : _a(a), _b(b) {}
	typename F1::result_type operator()(const typename F2::argument_type &v) { return _a(_b(v)); }
private:
	F1 _a;
	F2 _b;
};

template<typename F1, typename F2>
Composite<F1, F2> compose(const F1 &a, const F2 &b) {
	return Composite<F1, F2>(a, b);
}


}

#endif
