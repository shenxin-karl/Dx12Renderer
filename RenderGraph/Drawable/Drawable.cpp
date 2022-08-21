#include "Drawable.h"
#include <RenderGraph/Technique/Technique.h>

#include "Dx12lib/Buffer/IndexBuffer.h"
#include "Dx12lib/Buffer/VertexBuffer.h"

namespace rg {

void Drawable::bind(dx12lib::IGraphicsContext &graphicsCtx) const {
	assert(_topology != D3D_PRIMITIVE_TOPOLOGY_UNDEFINED);
	graphicsCtx.setPrimitiveTopology(_topology);
	for (size_t i = 0; i < std::size(_pVertexBuffers); ++i) {
		if (_pVertexBuffers[i] != nullptr)
			graphicsCtx.setVertexBuffer(_pVertexBuffers[i], static_cast<UINT>(i));
	}

	if (_pIndexBuffer != nullptr)
		graphicsCtx.setIndexBuffer(_pIndexBuffer);
}

void Drawable::submit(const TechniqueFlag &techniqueFlag) const {
	for (auto &pTechnique : _techniques)
		pTechnique->submit(*this, techniqueFlag);
}

bool Drawable::addTechnique(std::shared_ptr<Technique> pTechnique) {
	assert(pTechnique);
	if (hasTechnique(pTechnique->getTechniqueType()))
		return false;

	_techniqueFlag.set(pTechnique->getTechniqueType());
	_techniques.push_back(std::move(pTechnique));
	return true;
}

bool Drawable::hasTechnique(const TechniqueType &techniqueType) const {
	return _techniqueFlag.test(techniqueType);
}

bool Drawable::removeTechnique(const TechniqueType &techniqueType) {
	if (!hasTechnique(techniqueType))
		return false;

	for (auto iter = _techniques.begin(); iter != _techniques.end(); ++iter) {
		if ((*iter)->getTechniqueType() & techniqueType) {
			_techniques.erase(iter);
			return true;
		}
	}

	assert(false);
	return false;
}

void Drawable::setTechniqueActive(const TechniqueType &techniqueType, bool bActivate) {
	if (!hasTechnique(techniqueType))
		return;

	for (auto iter = _techniques.begin(); iter != _techniques.end(); ++iter) {
		if ((*iter)->getTechniqueType() & techniqueType) {
			(*iter)->setActive(bActivate);
			return;
		}
	}
}

void Drawable::setDrawArgs(const DrawArgs &drawArgs) {
	_drawArgs = drawArgs;
}

void Drawable::setTopology(D3D_PRIMITIVE_TOPOLOGY topology) {
	_topology = topology;
}

void Drawable::setIndexBuffer(std::shared_ptr<dx12lib::IndexBuffer> pIndexBuffer) {
	_pIndexBuffer = std::move(pIndexBuffer);
}

void Drawable::setVertexBuffer(std::shared_ptr<dx12lib::VertexBuffer> pVertexBuffer, size_t slot) {
	assert(slot < std::size(_pVertexBuffers));
	if (slot >= std::size(_pVertexBuffers))
		throw std::out_of_range(std::format("Drawable::setVertexBuffer {} out of range", slot));

	_pVertexBuffers[slot] = std::move(pVertexBuffer);
}

const DrawArgs & Drawable::getDrawArgs() const {
	return _drawArgs;
}

D3D_PRIMITIVE_TOPOLOGY Drawable::getTopology() const {
	return _topology;
}

std::shared_ptr<dx12lib::IndexBuffer> Drawable::getIndexBuffer() const {
	return _pIndexBuffer;
}

std::shared_ptr<dx12lib::VertexBuffer> Drawable::getVertexBuffer(size_t slot) const {
	if (slot >= std::size(_pVertexBuffers))
		throw std::out_of_range(std::format("Drawable::getVertexBuffer {} out of range", slot));
	return _pVertexBuffers[slot];
}

void Drawable::genDrawArgs() {
	size_t vertexCount = 0;
	for (size_t i = 0; i < std::size(_pVertexBuffers); ++i) {
		if (vertexCount == 0 && _pVertexBuffers[i] != nullptr) {
			vertexCount = _pVertexBuffers[i]->getVertexCount();
		} else if (vertexCount != 0) {
			assert(vertexCount == _pVertexBuffers[i]->getVertexCount());
		}
	}

	_drawArgs.vertexCount = vertexCount;
	if (_pIndexBuffer != nullptr)
		_drawArgs.indexCount = _pIndexBuffer->getIndexCount();
}

void Drawable::clearTechnique() {
	_techniques.clear();
	_techniqueFlag.reset();
}

}
