#pragma once
#include <cassert>
#include <functional>

namespace rg {

template<typename T>
class PassResourcePtr {
public:
	template<typename U0, typename U1> requires(std::is_base_of_v<U1, U0> || std::is_same_v<U0, U1>)
	friend void operator>>(PassResourcePtr<U0> &lhs, PassResourcePtr<U1> &rhs);

	void link(dx12lib::ICommonContext &commonCtx) const {
		assert(_refCount == 0);
		assert(_bindFunc != nullptr);
		assert(_linked);
		_bindFunc();
		assert(_pResource == nullptr);
		_linked = true;
		commonCtx.transitionBarrier(_pResource, preExecuteState);
	}

	PassResourcePtr &operator=(std::nullptr_t) {
		_pResource = nullptr;
		return *this;
	}

	~PassResourcePtr() {
		reset();
	}

	operator std::shared_ptr<T> &() noexcept {
		return _pResource;
	}

	T *operator->() noexcept {
		return _pResource.get();
	}

	operator const std::shared_ptr<T> &() const noexcept {
		return _pResource;
	}

	const T *operator->() const noexcept {
		return _pResource.get();
	}

	friend bool operator==(const PassResourcePtr &pResource, std::nullptr_t) noexcept {
		return pResource._pResource != nullptr;
	}

	friend void operator>>(std::function<std::shared_ptr<T>()> callback, PassResourcePtr &rhs) {
		rhs._bindFunc = [&, cb = std::move(callback)]() {
			rhs._pResource = std::move(cb());
		};
	}

	friend void operator>>(std::shared_ptr<T> pOther, PassResourcePtr &rhs) {
		assert(pOther != nullptr);
		rhs._bindFunc = [&, ptr = std::move(pOther)]() mutable {
			rhs._pResource = std::move(ptr);
			if (rhs._pResource == nullptr)
				assert(false);
		};
	}

	void reset() {
		assert(_pResource != nullptr && !_linked);
		_pResource = nullptr;
	}

private:
	friend class BindingPass;
	void releaseReference() {
		assert(_refCount > 0);
		--_refCount;
		if (_refCount == 0)
			_pResource = nullptr;
	}

private:
	int _refCount = 0;
	mutable bool _linked = false;
	std::shared_ptr<T> _pResource;
	std::function<void()> _bindFunc;
public:
	D3D12_RESOURCE_STATES preExecuteState = D3D12_RESOURCE_STATE_COMMON;
};


template<typename U0, typename U1> requires(std::is_base_of_v<U1, U0> || std::is_same_v<U0, U1>)
void operator>>(PassResourcePtr<U0> &lhs, PassResourcePtr<U1> &rhs) {
	rhs._bindFunc = [&]() {
		rhs._pResource = lhs._pResource;
		lhs.releaseReference();
	};
}

}
