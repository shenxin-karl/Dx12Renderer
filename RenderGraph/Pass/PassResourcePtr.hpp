#pragma once
#include <cassert>
#include <functional>
#include <RenderGraph/Pass/PassResourceBase.h>
#include <Dx12lib/Resource/IResource.h>
#include <Dx12lib/Texture/TextureStd.h>
#include <Dx12lib/Buffer/BufferStd.h>

namespace rgph {


template<typename T>
class PassResourcePtr : public PassResourceBase {
public:
	using PassResourceBase::PassResourceBase;

	template<typename U0, typename U1>
	friend void operator>>(PassResourcePtr<U0> &lhs, PassResourcePtr<U1> &rhs);

	void link(dx12lib::ICommonContext &commonCtx) override {
		assert(_linkResourceFunc != nullptr);
		_linkResourceFunc();
		assert(_pResource != nullptr);
		if (preExecuteState != kNoneType)
			commonCtx.transitionBarrier(_pResource, preExecuteState);
	}

	PassResourcePtr &operator=(std::nullptr_t) {
		_pResource = nullptr;
		return *this;
	}

	~PassResourcePtr() override {
		PassResourcePtr::reset();
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
		return pResource._pResource == nullptr;
	}

	friend bool operator!=(const PassResourcePtr &pResource, std::nullptr_t) noexcept {
		return pResource._pResource != nullptr;
	}

	friend void operator>>(std::function<std::shared_ptr<dx12lib::IResource>()> callback, PassResourcePtr &rhs) {
		assert(rhs.getResourceSource() == nullptr);
		assert(callback != nullptr);
		rhs._linkResourceFunc = [&, cb = std::move(callback)]() {
			rhs._pResource = std::dynamic_pointer_cast<T>(cb());
		};
	}

	friend void operator>>(std::shared_ptr<dx12lib::IResource> pOther, PassResourcePtr &rhs) {
		assert(pOther != nullptr);
		rhs._linkResourceFunc = [&, ptr = std::move(pOther)]() mutable {
			rhs._pResource = std::dynamic_pointer_cast<T>(ptr);
		};
	}

	void reset() override {
		_pResource = nullptr;
	}

	T *get() noexcept {
		return _pResource.get();
	}

	const T *get() const noexcept {
		return _pResource.get();
	}
private:
	bool tryLink() override {
		if (_linkResourceFunc == nullptr)
			return false;
		_linkResourceFunc();
		return _pResource != nullptr;
	}
private:
	std::shared_ptr<T> _pResource;
	std::function<void()> _linkResourceFunc;
};


template<typename U0, typename U1>
void operator>>(PassResourcePtr<U0> &lhs, PassResourcePtr<U1> &rhs) {
	assert(static_cast<PassResourceBase *>(&lhs) != static_cast<PassResourceBase *>(&rhs));
	assert(rhs.getResourceSource() == nullptr);
	rhs.setResourceSource(&lhs);
	rhs._linkResourceFunc = [&]() {
		rhs._pResource = std::dynamic_pointer_cast<U1>(lhs._pResource);
	};
}

}
