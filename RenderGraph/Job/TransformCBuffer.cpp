#include "TransformCBuffer.h"
#include "Dx12lib/Pipeline/ShaderRegister.hpp"

namespace rgph {

const float4x4 & TransformCBuffer::getMatWorld() const {
	return getTransformStore().matWorld;
}

const float4x4 & TransformCBuffer::getMatNormal() const {
	return getTransformStore().matNormal;
}

const float4x4 & TransformCBuffer::getMatWVP() const {
	return getTransformStore().matWVP;
}

const AxisAlignedBox & TransformCBuffer::getWorldAABB() const {
	return _worldAABB;
}

const TransformStore & TransformCBuffer::getTransformStore() const {
	return *_pTransformCBuf->visit();
}

void TransformCBuffer::setTransformStore(const TransformStore &store) {
	auto *ptr = _pTransformCBuf->map();
	std::memcpy(ptr, &store, sizeof(store));
}

void TransformCBuffer::setWorldAABB(const AxisAlignedBox &box) {
	_worldAABB = box;
}

void TransformCBuffer::setTransformCBuf(FRConstantBufferPtr<TransformStore> pTransformCBuf) {
	assert(pTransformCBuf != nullptr);
	_pTransformCBuf = std::move(pTransformCBuf);
}

void TransformCBuffer::bind(dx12lib::IGraphicsContext &graphicsCtx, const dx12lib::ShaderRegister &shaderRegister) const {
	assert(_pTransformCBuf != nullptr);
	graphicsCtx.setConstantBuffer(shaderRegister, _pTransformCBuf);
}

}
