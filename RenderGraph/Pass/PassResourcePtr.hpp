#pragma once
#include <cassert>
#include <functional>
#include <RenderGraph/Pass/PassResourceBase.h>
#include <Dx12lib/Texture/TextureStd.h>
#include <Dx12lib/Buffer/BufferStd.h>

namespace rgph {


template<typename T>
class PassResourcePtr : public PassResourceBase {
public:
	using PassResourceBase::PassResourceBase;

	template<typename U0, typename U1> requires(std::is_base_of_v<U1, U0> || std::is_same_v<U0, U1>)
	friend void operator>>(PassResourcePtr<U0> &lhs, PassResourcePtr<U1> &rhs);

	void link(dx12lib::ICommonContext &commonCtx) override {
		assert(_linkResourceFunc != nullptr);
		_linkResourceFunc();
		assert(_pResource != nullptr);
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

	friend void operator>>(std::function<std::shared_ptr<T>()> callback, PassResourcePtr &rhs) {
		assert(rhs.getResourceSource() == nullptr);
		assert(callback != nullptr);
		rhs._linkResourceFunc = [&, cb = std::move(callback)]() {
			rhs._pResource = cb();
		};
	}

	friend void operator>>(std::shared_ptr<T> pOther, PassResourcePtr &rhs) {
		assert(pOther != nullptr);
		rhs._linkResourceFunc = [&, ptr = std::move(pOther)]() mutable {
			rhs._pResource = ptr;
		};
	}

	void reset() override {
		_pResource = nullptr;
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
public:
	D3D12_RESOURCE_STATES preExecuteState = D3D12_RESOURCE_STATE_COMMON;
};


template<typename U0, typename U1> requires(std::is_base_of_v<U1, U0> || std::is_same_v<U0, U1>)
void operator>>(PassResourcePtr<U0> &lhs, PassResourcePtr<U1> &rhs) {
	assert(static_cast<PassResourceBase *>(&lhs) != static_cast<PassResourceBase *>(&rhs));
	assert(rhs.getResourceSource() == nullptr);
	rhs.setResourceSource(&lhs);
	rhs._linkResourceFunc = [&]() {
		rhs._pResource = lhs._pResource;
	};
}

}
