#ifndef INNOCENT_UTIL_H
#define INNOCENT_UTIL_H

template<typename T>
class Pointer : public Common::SharedPtr<T> {
public:
	inline Pointer<T> &operator=(T *p) {
		Common::SharedPtr<T> csp(p);
		*(static_cast<Common::SharedPtr<T>*>(this)) = csp;
		return *this;
	}
};

#endif
