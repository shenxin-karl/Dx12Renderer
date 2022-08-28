#include "TransformCBufferPtr.h"
#include "Dx12lib/Pipeline/ShaderRegister.hpp"

namespace rgph {

const float4x4 & TransformCBufferPtr::getMatWorld() const {
	return getTransformStore().matWorld;
}

const float4x4 & TransformCBufferPtr::getMatNormal() const {
	return getTransformStore().matNormal;
}

const TransformStore & TransformCBufferPtr::getTransformStore() const {
	return *_pTransformCBuf->visit();
}

void TransformCBufferPtr::setTransformStore(const TransformStore &store) {
	auto *ptr = _pTransformCBuf->map();
	std::memcpy(ptr, &store, sizeof(store));
}

void TransformCBufferPtr::setTransformCBuffer(FRConstantBufferPtr<TransformStore> pTransformCBuf) {
	assert(pTransformCBuf != nullptr);
	_pTransformCBuf = std::move(pTransformCBuf);
}

void TransformCBufferPtr::bind(dx12lib::IGraphicsContext &graphicsCtx, const dx12lib::ShaderRegister &shaderRegister) const {
	assert(_pTransformCBuf != nullptr);
	graphicsCtx.setConstantBuffer(shaderRegister, _pTransformCBuf);
}

}
