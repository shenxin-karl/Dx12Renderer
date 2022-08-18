#pragma once
#include <cassert>
#include <functional>

namespace rg {

template<typename T>
class PassResourcePtr {
public:
	void link(dx12lib::ICommonContext &commonCtx) const {
		assert(_refCount == 0);
		assert(_bindFunc != nullptr);
		assert(_linked);
		_bindFunc();
		assert(_pResource == nullptr);
		_linked = true;
		commonCtx.transitionBarrier(_pResource, preExecuteState);
	}

	template<typename T1>
	void moveTo(PassResourcePtr<T1> &to) {
		++_refCount;
		to._bindFunc = [&]() {
			releaseReference();
			to._pResource = std::move(_pResource);
		};
	}

	template<typename T1>
	void copyTo(PassResourcePtr<T1> &to) {
		++_refCount;
		to.bindFunc = [&]() {
			releaseReference();
			to._pResource = _pResource;
		};
	}

	template<typename T1>
	void moveTo(PassResourcePtr<T1> &to, std::function<void(std::shared_ptr<T>)> &&bindCallback) {
		++_refCount;
		to._bindFunc = [&, callback = std::move(bindCallback)]() {
			callback(_pResource);
			assert(_pResource == nullptr);
			releaseReference();
		};
	}

	template<typename T1>
	void copyTo(PassResourcePtr<T1> &to, std::function<void(std::shared_ptr<T>)> &&bindCallback) {
		++_refCount;
		to._bindFunc = [&, callback = std::move(bindCallback)]() {
			callback(_pResource);
			releaseReference();
		};
	}

	template<typename T1>
	void from(std::shared_ptr<T1> &pResource) {
		_bindFunc = [&]() {
			_pResource = pResource;
			assert(_pResource != nullptr);
		};
	}

	void from(std::function<std::shared_ptr<T>()> &&getFunc) {
		_bindFunc = [&, _getFunc = std::move(getFunc)]() {
			_pResource = _getFunc();
			assert(_pResource != nullptr);
		};
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

private:
	friend class BindingPass;
	void releaseReference() {
		assert(_refCount > 0);
		--_refCount;
		if (_refCount == 0)
			_pResource = nullptr;
	}

	void reset() {
		assert(_refCount != 0);
		assert(_pResource != nullptr && !_linked);
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

}
