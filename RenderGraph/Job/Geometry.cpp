#include "Geometry.h"

#include "Dx12lib/Buffer/IndexBuffer.h"
#include "Dx12lib/Buffer/VertexBuffer.h"

namespace rgph {

void Geometry::bind(dx12lib::IGraphicsContext &graphicsCtx, const VertexInputSlot &vertexInputSlot) const {
	graphicsCtx.setPrimitiveTopology(_topology);
	for (size_t slot = 0; slot < dx12lib::kVertexBufferSlotCount; ++slot) {
		if (!vertexInputSlot.test(slot)) 
			continue;

		assert(_pVertexBufferList[slot] != nullptr);
		graphicsCtx.setVertexBuffer(_pVertexBufferList[slot], static_cast<UINT>(slot));
	}
}

void Geometry::setVertexBuffer(size_t idx, std::shared_ptr<dx12lib::VertexBuffer> pVertexBuffer) {
	assert(idx < dx12lib::kVertexBufferSlotCount);
	_pVertexBufferList[idx] = std::move(pVertexBuffer);
}

void Geometry::setIndexBuffer(std::shared_ptr<dx12lib::IndexBuffer> pIndexBuffer) {
	_pIndexBuffer = std::move(pIndexBuffer);
}

void Geometry::setMesh(std::shared_ptr<IMesh> pMesh) {
	_pMesh = std::move(pMesh);
}

std::shared_ptr<dx12lib::VertexBuffer> Geometry::getVertexBuffer(size_t idx) const {
	assert(idx < dx12lib::kVertexBufferSlotCount);
	return _pVertexBufferList[idx];
}

std::shared_ptr<dx12lib::IndexBuffer> Geometry::getIndexBuffer() const {
	return _pIndexBuffer;
}

std::shared_ptr<IMesh> Geometry::getMesh() const {
	return _pMesh;
}

void Geometry::setDrawArgs(const DrawArgs &drawArgs) {
	_drawArgs = drawArgs;
}

void Geometry::setTopology(D3D_PRIMITIVE_TOPOLOGY topology) {
	_topology = topology;
}

const DrawArgs & Geometry::getDrawArgs() const {
	return _drawArgs;
}

D3D_PRIMITIVE_TOPOLOGY Geometry::getTopology() const {
	return _topology;
}

void Geometry::genDrawArgs() {
	size_t vertexCount = 0;
	for (size_t i = 0; i < std::size(_pVertexBufferList); ++i) {
		if (vertexCount == 0 && _pVertexBufferList[i] != nullptr) {
			vertexCount = _pVertexBufferList[i]->getVertexCount();
		}
		else if (vertexCount != 0) {
			assert(vertexCount == _pVertexBufferList[i]->getVertexCount());
		}
	}

	_drawArgs.vertexCount = vertexCount;
	if (_pIndexBuffer != nullptr)
		_drawArgs.indexCount = _pIndexBuffer->getIndexCount();
}

void Geometry::draw(dx12lib::IGraphicsContext &graphicsCtx) const {
	if (_drawArgs.indexCount > 0) {
		graphicsCtx.drawIndexedInstanced(
			_drawArgs.indexCount,
			_drawArgs.instanceCount,
			_drawArgs.startIndexLocation,
			_drawArgs.baseVertexLocation,
			_drawArgs.startInstanceLocation
		);
	} else {
		graphicsCtx.drawInstanced(
			_drawArgs.vertexCount,
			_drawArgs.instanceCount,
			_drawArgs.baseVertexLocation,
			_drawArgs.startInstanceLocation
		);
	}
}

}
